// stubs for network communications
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <zlib.h>
#include "stubs.h"
#include "Synchronized.h"
#include "Task.h"
#include "DriverStation.h"
#include "NetworkCommunication/FRCComm.h"
#include "NetworkCommunication/UsageReporting.h"
#include "CAN/can_proto.h"

uint8_t fakeMAC[] = { 0x00, 0x80, 0x2f, 0x11, 0xd8, 0x2a };
char commVersion[] = "06300800";

///////////////////////////////////////////////////////////////////////////////
//
// libpcap replacement for logging traffic to pcap file
//
///////////////////////////////////////////////////////////////////////////////

class pcap {
private:
    FILE * m_file;
    SEM_ID m_mutex;

public:
    pcap( const char *name );
    ~pcap();

    void write_record( const sockaddr_in *src, const sockaddr_in *dst,
    			const char *data, int len );
};

pcap::pcap( const char *name )
{
    m_file = fopen(name, "wb");
    if (!m_file) {
	perror("pcap::fopen");
	abort();
    }

    m_mutex = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);

#pragma pack(push,1)
    struct pcap_hdr {
	uint32_t magic_number;
	uint16_t version_major;
	uint16_t version_minor;
	int32_t  thiszone;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t network;
    } hdr;
#pragma pack(pop)

    hdr.magic_number = 0xa1b2c3d4;
    hdr.version_major = 2;
    hdr.version_minor = 4;
    hdr.thiszone = 0;
    hdr.sigfigs = 0;
    hdr.snaplen = 1500;
    hdr.network = 1; // Ethernet

    fwrite((char *)&hdr, sizeof hdr, 1, m_file);
}

void pcap::write_record( const sockaddr_in *src, const sockaddr_in *dst,
			 const char *packet, int len )
{
    Synchronized sync(m_mutex);

#pragma pack(push,1)
    struct pcaprec_hdr {
	uint32_t ts_sec;
	uint32_t ts_usec;
	uint32_t incl_len;
	uint32_t orig_len;
    } hdr;
#pragma pack(pop)

#pragma pack(push,1)
    struct ether_hdr {
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t type;
    } eth;
#pragma pack(pop)

#pragma pack(push,1)
    struct ip_hdr {
	uint8_t ver;
	uint8_t dscp;
	uint16_t iplen;
	uint16_t id;
	uint16_t flags;
	uint8_t ttl;
	uint8_t proto;
	uint16_t cksum;
	uint32_t srcip;
	uint32_t dstip;
    } ip;
#pragma pack(pop)

#pragma pack(push,1)
    struct udp_hdr {
	uint16_t srcport;
	uint16_t dstport;
	uint16_t udplen;
	uint16_t cksum;
    } udp;
#pragma pack(pop)

    int udplen = len + sizeof (udp);
    int iplen = udplen + sizeof (ip);
    int ethlen = iplen + sizeof (eth);

    gettimeofday((struct timeval *)&hdr.ts_sec, NULL);
    hdr.incl_len = ethlen;
    hdr.orig_len = ethlen;

    fwrite((char *)&hdr, sizeof hdr, 1, m_file);

    memset(eth.dst, 0, sizeof eth.dst);
    memset(eth.src, 0, sizeof eth.src);
    eth.type = htons(0x0800);

    fwrite((char *)&eth, sizeof eth, 1, m_file);

    ip.ver = 0x45;
    ip.dscp = 0;
    ip.iplen = htons(iplen);
    ip.id = 0;
    ip.flags = 0;
    ip.ttl = 7;
    ip.proto = IPPROTO_UDP;
    ip.cksum = 0;
    ip.srcip = src->sin_addr.s_addr;
    ip.dstip = dst->sin_addr.s_addr;

    fwrite((char *)&ip, sizeof ip, 1, m_file);

    udp.srcport = src->sin_port;
    udp.dstport = dst->sin_port;
    udp.udplen = htons(udplen);
    udp.cksum = 0;

    fwrite((char *)&udp, sizeof udp, 1, m_file);

    fwrite((char *)packet, len, 1, m_file);
    fflush(m_file);
}

pcap::~pcap()
{
    // can't use Synchronized here since we need to destroy the semaphore
    //   without releasing it
    semTake(m_mutex, WAIT_FOREVER);
    fclose(m_file);
    semDelete(m_mutex);
}

///////////////////////////////////////////////////////////////////////////////
//
// network communication with the driver station and/or FMS
//
///////////////////////////////////////////////////////////////////////////////

#define	ROBOT_ADDR	0x0a0e1902
#define	ROBOT_RECV_PORT	1110
#define	ROBOT_SEND_PORT	1025
#define	DS_REPLY_PORT	1150

extern "C" void FRC_NetworkCommunicationInitialize();

class FNC // FRC_NetworkCommunication
{
private:
    friend void FRC_NetworkCommunicationInitialize();
    friend void setNewDataSem( SEM_ID );
    friend STATUS waitForDS();

    FNC();
    void SetNewDataSem( SEM_ID );

public:
    ~FNC();

private:
    pcap *m_pcap;

    int m_recvSocket;
    sockaddr_in m_recvAddr;

    int m_sendSocket;
    sockaddr_in m_sendAddr;

    Task *m_commTask;
    static void FRCCommTask( FNC * );

    SEM_ID m_dataAvailable;
    SEM_ID m_dataMutex;

public:
#pragma pack(push,1)
    // BEGIN: Definitions from DriverStationEnhancedIO
    // BEGIN: Definitions from the Cypress firmware
    typedef struct
    {
	uint16_t digital;
	uint16_t digital_oe;
	uint16_t digital_pe;
	uint16_t pwm_compare[4];
	uint16_t pwm_period[2];
	uint8_t dac[2];
	uint8_t leds;
	union
	{
	    struct
	    {
// This uses __BYTE_ORDER__ as a stand-in for "__BITFIELD_PACKING_ORDER__"
// (which is not defined).  It gives the intended result for gcc/g++ on
// PPC and x86 architectures but may not be correct for other compilers
// or even gcc with other processors.

#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
		// Bits are inverted from cypress fw because of big-endian!
		uint8_t pwm_enable : 4;
		uint8_t comparator_enable : 2;
		uint8_t quad_index_enable : 2;
#elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
		uint8_t quad_index_enable : 2;
		uint8_t comparator_enable : 2;
		uint8_t pwm_enable : 4;
#else
#error __BYTE_ORDER__ must be __ORDER_BIG_ENDIAN__ or __ORDER_LITTLE_ENDIAN__
#endif // __BYTE_ORDER__
	    };
	    uint8_t enables;
	};
	uint8_t fixed_digital_out;
    } output_t;  //data to IO (23 bytes)

    static void SwapDSEIOOutput( output_t *pOutput );

    typedef struct
    {
	uint8_t api_version;
	uint8_t fw_version;
	int16_t analog[8];
	uint16_t digital;
	int16_t accel[3];
	int16_t quad[2];
	uint8_t buttons;
	uint8_t capsense_slider;
	uint8_t capsense_proximity;
    } input_t;	//data from IO (33 bytes)
    // END: Definitions from the Cypress firmware

    static void SwapDSEIOInput( input_t *pInput );

    // Dynamic block definitions
    typedef struct
    {
	uint8_t size; // Must be 25 (size remaining in the block not counting the size variable)
	uint8_t id; // Must be 18
	output_t data;
	uint8_t flags;
    } status_block_t;

    typedef struct
    {
	uint8_t size; // Must be 34
	uint8_t id; // Must be 17
	input_t data;
    } control_block_t;
#pragma pack(pop)

    // END: Definitions from DriverStationEnhancedIO

private:
    STATUS Recv();

#define FRCCCDSIZE sizeof (FRCCommonControlData) // 80
#define	FRCEXTDSIZE 940

#pragma pack(push,1)
    struct FRCRecvPkt {			// received data in network order...
	FRCCommonControlData ctrl;	// common control data: 80 bytes
	char extData[FRCEXTDSIZE];	// extension data: up to 940 bytes
	uint32_t crc;			// 32-bit CRC
    } m_recvPkt;
#pragma pack(pop)

    sockaddr_in m_fromAddr;		// sending driver station's address
    FRCCommonControlData m_recvData;	// control data in host byte order
    uint8_t m_extData[FRCEXTDSIZE];	// dynamic data in unknown byte order

public:
    int GetCommonControlData( FRCCommonControlData *data );
    int GetDynamicControlData( uint8_t type, char *dynamicData, int32_t maxLength );

public:
    // keep a local copy of user data to be sent to the DS
    void ObserveUserProgramStarting();
    void ObserveUserProgramDisabled();
    void ObserveUserProgramAutonomous();
    void ObserveUserProgramTeleop();
    void ObserveUserProgramTest();
    int SetStatusData( float battery, uint8_t dsDigitalOut, uint8_t updateNumber,
		        const char *userDataHigh, int userDataHighLength,
		        const char *userDataLow, int userDataLowLength,
			int wait_ms );

private:
    uint16_t m_mode;

    float m_battery;
    uint8_t m_dsDigitalOut;
    uint8_t m_updateNumber;

    char *m_userDataHigh;
    uint32_t m_userDataHighLength;

    void FreeUserDataHigh();

    char *m_userDataLow;
    uint32_t m_userDataLowLength;

    void FreeUserDataLow();

public:
    int SetErrorData( const char *errors, int errorsLength, int wait_ms );

private:
    char *m_userErrMsg;
    uint32_t m_userErrMsgLength;

    void FreeErrorData();

public:
    // keep a local copy of enhanced I/O data
    // until it can be sent back to the DS
    int OverrideIOConfig( const char *ioConfig, int wait_ms );

private:
    char *m_eioConfig;
    uint32_t m_eioConfigLength;

    void FreeEIOConfig();

public:
    int SetLCDData( const char *lcdData, int lcdDataLength,
			   int wait_ms );
    char *m_lcdData;
    uint32_t m_lcdDataLength;

    void FreeLCDData();

private:
    int Send();

#pragma pack(push,1)
    struct FRCStatusPkt {
/*000*/	union {
		uint8_t control;

// This uses __BYTE_ORDER as a stand-in for "__BITFIELD_PACKING_ORDER"
// (which is not defined).  It gives the intended result for gcc/g++ on
// PPC and x86 architectures but may not be correct for other compilers
// or even gcc with other processors.

#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
		struct {
			uint8_t reset : 1;
			uint8_t notEStop : 1;
			uint8_t enabled : 1;
			uint8_t autonomous : 1;
			uint8_t fmsAttached : 1;
			uint8_t resync : 1;
			uint8_t test : 1;
			uint8_t checkVersions : 1;
		};
#elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
		struct {
			uint8_t checkVersions : 1;
			uint8_t test : 1;
			uint8_t resync : 1;
			uint8_t fmsAttached : 1;
			uint8_t autonomous : 1;
			uint8_t enabled : 1;
			uint8_t notEStop : 1;
			uint8_t reset : 1;
		};
#else
#error __BYTE_ORDER__ must be __ORDER_BIG_ENDIAN__ or __ORDER_LITTLE_ENDIAN__
#endif
	};

/*001*/	uint8_t battery[2];		// 2 bytes, scaled integer
/*003*/	uint8_t dsDigitalOut;		// DS digital outputs
/*004*/	uint8_t unknown1[4];
/*008*/	uint16_t teamID;		// copy of team ID from DS
/*00A*/	uint8_t macAddr[6];		// cRIO MAC address
/*010*/	char versionData[8];		// FRC comm protocol version
/*018*/	uint8_t unknown2[4];
/*01C*/ uint16_t mode;			// robot operating mode
/*01E*/	uint16_t packetIndex;		// copied from DS control packet
/*020*/	uint8_t updateNumber;		// from setStatusData()
/*021*/	uint8_t userData[911];		// contains:
					// user data high len (4 bytes)
					// user data high (n bytes)
					// error string len (4 bytes)
					// error string (n bytes)
					// user data low len (4 bytes)
					// user data low (n bytes)
					// ...or as much as will fit
/*3B0*/	uint8_t sysstat[40];		// system status data
/*3D8*/	uint8_t eioConfig[36];		// DSEIO status_block_t
/*3FC*/	uint32_t crc;
/*400*/	uint8_t lcdData[USER_DS_LCD_DATA_SIZE];	// optional
    } m_sendPkt;
#pragma pack(pop)

    sockaddr_in m_toAddr;
    uint32_t m_sendPktLength;

private:
    DISALLOW_COPY_AND_ASSIGN(FNC);
};


static FNC *netCommObj = NULL;

extern "C" void FRC_NetworkCommunicationInitialize()
{
    if (!netCommObj) {
	netCommObj = new FNC();
    }
}

FNC::FNC()
{
    m_pcap = new pcap("robot.pcap");

    m_recvSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if (m_recvSocket == -1) {
	perror("socket");
	abort();
    }

    m_recvAddr.sin_family = AF_INET;
    m_recvAddr.sin_addr.s_addr = htonl(ROBOT_ADDR);
    m_recvAddr.sin_port = htons(ROBOT_RECV_PORT);

    if (bind(m_recvSocket, (struct sockaddr *)&m_recvAddr, sizeof m_recvAddr) == -1) {
	perror("bind");
	abort();
    }

    m_sendSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if (m_sendSocket == -1) {
	perror("socket");
	abort();
    }

    m_sendAddr.sin_family = AF_INET;
    m_sendAddr.sin_addr.s_addr = htonl(ROBOT_ADDR);
    m_sendAddr.sin_port = htons(ROBOT_SEND_PORT);

    if (bind(m_sendSocket, (struct sockaddr *)&m_sendAddr, sizeof m_sendAddr) == -1) {
	perror("bind");
	abort();
    }

    m_mode = 0;

    m_battery = 37.372;	// must round to 0x3737 when converted to BCD

    m_dataAvailable = NULL;
    m_dataMutex = semMCreate(SEM_Q_PRIORITY|SEM_DELETE_SAFE|SEM_INVERSION_SAFE);
    m_commTask = new Task( "FRC_NetRecv", (FUNCPTR) FNC::FRCCommTask );
    m_commTask->Start((uint32_t)this);
}

FNC::~FNC()
{
    // can't use Synchronized here since we need to destroy the semaphore
    //   without releasing it
    semTake(m_dataMutex, WAIT_FOREVER);
    delete m_commTask;
    close(m_recvSocket);
    delete m_pcap;
    semDelete(m_dataMutex);
}

void setNewDataSem( SEM_ID dataAvailable )
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }
    netCommObj->SetNewDataSem( dataAvailable );
}

void FNC::SetNewDataSem( SEM_ID sem )
{
    m_dataAvailable = sem;
}

void FNC::FRCCommTask( FNC *pObj )
{
    while (1) {
	if (pObj->Recv() == ERROR) {
	    break;
	}
	if (pObj->Send() == ERROR) {
	    break;
	}
    }
}

STATUS FNC::Recv()
{
    struct sockaddr_in fromAddr;
    socklen_t fromAddrLen = sizeof fromAddr;

    int n = recvfrom(m_recvSocket, (char *)&m_recvPkt, sizeof m_recvPkt, 0,
		     (struct sockaddr *)&fromAddr, &fromAddrLen);
    if (n == -1) {
	perror("FRC_NetworkCommunication::Recv::recvfrom");
	return ERROR;
    }

    if (m_recvAddr.sin_addr.s_addr == INADDR_ANY) {
	// fill in my own address based on:
	// DS at 10.TE.AM.xx
	// robot at 10.TE.AM.2
	in_addr_t ipaddr = ntohl(fromAddr.sin_addr.s_addr);
	ipaddr = (ipaddr & 0xFFFFFF00) | 0x02;
	m_recvAddr.sin_addr.s_addr = htonl(ipaddr);
	m_sendAddr.sin_addr.s_addr = htonl(ipaddr);
    }

    m_pcap->write_record(&fromAddr, &m_recvAddr, (char *)&m_recvPkt, n);

    if (n != sizeof m_recvPkt) {
	printf("FRC_NetworkCommunication::Recv: wrong size (%d)\n", n);
	return OK;
    }

    uint32_t recvCRC = ntohl(m_recvPkt.crc);
    m_recvPkt.crc = 0;
    uint32_t calcCRC = crc32(0, Z_NULL, 0);
    calcCRC = crc32(calcCRC, (uint8_t *)&m_recvPkt, sizeof m_recvPkt);

    if (calcCRC != recvCRC) {
	printf("FRC_NetworkCommunication::Recv: bad checksum "
	       "(received 0x%08x, calculated 0x%08x)\n", recvCRC, calcCRC);
	return OK;
    }

    // unpack and copy data to m_recvData
    {
	// begin critical section
	Synchronized sync(m_dataMutex);

	// clear the work area for safety
	memset(&m_recvData, 0, sizeof m_recvData);

	// unpack common header elements
	m_recvData.packetIndex		= ntohs(m_recvPkt.ctrl.packetIndex);

	m_recvData.reset		= m_recvPkt.ctrl.reset;
	m_recvData.notEStop		= m_recvPkt.ctrl.notEStop;
	m_recvData.enabled		= m_recvPkt.ctrl.enabled;
	m_recvData.autonomous		= m_recvPkt.ctrl.autonomous;
	m_recvData.fmsAttached		= m_recvPkt.ctrl.fmsAttached;
	m_recvData.resync		= m_recvPkt.ctrl.resync;
	m_recvData.test			= m_recvPkt.ctrl.test;
	m_recvData.checkVersions	= m_recvPkt.ctrl.checkVersions;

	m_recvData.dsDigitalIn		= m_recvPkt.ctrl.dsDigitalIn;
	m_recvData.teamID		= ntohs(m_recvPkt.ctrl.teamID);
	m_recvData.dsID_Alliance	= m_recvPkt.ctrl.dsID_Alliance;
	m_recvData.dsID_Position	= m_recvPkt.ctrl.dsID_Position;
#if 0
	printf("pkt %u ctrl 0x%02x dig in 0x%02x team %u position %c%c\n",
	       m_recvData.packetIndex, m_recvData.control,
	       m_recvData.dsDigitalIn, m_recvData.teamID,
	       m_recvData.dsID_Alliance, m_recvData.dsID_Position);
#endif

	// unpack data for each of the 4 possible joysticks
	m_recvData.stick0Axis1		= m_recvPkt.ctrl.stick0Axis1;
	m_recvData.stick0Axis2		= m_recvPkt.ctrl.stick0Axis2;
	m_recvData.stick0Axis3		= m_recvPkt.ctrl.stick0Axis3;
	m_recvData.stick0Axis4		= m_recvPkt.ctrl.stick0Axis4;
	m_recvData.stick0Axis5		= m_recvPkt.ctrl.stick0Axis5;
	m_recvData.stick0Axis6		= m_recvPkt.ctrl.stick0Axis6;
	m_recvData.stick0Buttons	= ntohs(m_recvPkt.ctrl.stick0Buttons);
#if 0
	printf("stick0: %02x %02x %02x %02x %02x %02x %04x\n",
		m_recvData.stick0Axis1, m_recvData.stick0Axis2,
		m_recvData.stick0Axis3, m_recvData.stick0Axis4,
		m_recvData.stick0Axis5, m_recvData.stick0Axis6,
		m_recvData.stick0Buttons);
#endif
	m_recvData.stick1Axis1		= m_recvPkt.ctrl.stick1Axis1;
	m_recvData.stick1Axis2		= m_recvPkt.ctrl.stick1Axis2;
	m_recvData.stick1Axis3		= m_recvPkt.ctrl.stick1Axis3;
	m_recvData.stick1Axis4		= m_recvPkt.ctrl.stick1Axis4;
	m_recvData.stick1Axis5		= m_recvPkt.ctrl.stick1Axis5;
	m_recvData.stick1Axis6		= m_recvPkt.ctrl.stick1Axis6;
	m_recvData.stick1Buttons	= ntohs(m_recvPkt.ctrl.stick1Buttons);
#if 0
	printf("stick1: %02x %02x %02x %02x %02x %02x %04x\n",
		m_recvData.stick1Axis1, m_recvData.stick1Axis2,
		m_recvData.stick1Axis3, m_recvData.stick1Axis4,
		m_recvData.stick1Axis5, m_recvData.stick1Axis6,
		m_recvData.stick1Buttons);
#endif
	m_recvData.stick2Axis1		= m_recvPkt.ctrl.stick2Axis1;
	m_recvData.stick2Axis2		= m_recvPkt.ctrl.stick2Axis2;
	m_recvData.stick2Axis3		= m_recvPkt.ctrl.stick2Axis3;
	m_recvData.stick2Axis4		= m_recvPkt.ctrl.stick2Axis4;
	m_recvData.stick2Axis5		= m_recvPkt.ctrl.stick2Axis5;
	m_recvData.stick2Axis6		= m_recvPkt.ctrl.stick2Axis6;
	m_recvData.stick2Buttons	= ntohs(m_recvPkt.ctrl.stick2Buttons);
#if 0
	printf("stick2: %02x %02x %02x %02x %02x %02x %04x\n",
		m_recvData.stick2Axis1, m_recvData.stick2Axis2,
		m_recvData.stick2Axis3, m_recvData.stick2Axis4,
		m_recvData.stick2Axis5, m_recvData.stick2Axis6,
		m_recvData.stick2Buttons);
#endif
	m_recvData.stick3Axis1		= m_recvPkt.ctrl.stick3Axis1;
	m_recvData.stick3Axis2		= m_recvPkt.ctrl.stick3Axis2;
	m_recvData.stick3Axis3		= m_recvPkt.ctrl.stick3Axis3;
	m_recvData.stick3Axis4		= m_recvPkt.ctrl.stick3Axis4;
	m_recvData.stick3Axis5		= m_recvPkt.ctrl.stick3Axis5;
	m_recvData.stick3Axis6		= m_recvPkt.ctrl.stick3Axis6;
	m_recvData.stick3Buttons	= ntohs(m_recvPkt.ctrl.stick3Buttons);
#if 0
	printf("stick3: %02x %02x %02x %02x %02x %02x %04x\n",
		m_recvData.stick3Axis1, m_recvData.stick3Axis2,
		m_recvData.stick3Axis3, m_recvData.stick3Axis4,
		m_recvData.stick3Axis5, m_recvData.stick3Axis6,
		m_recvData.stick3Buttons);
#endif

	// unpack the four analog input channels
	m_recvData.analog1		= ntohs(m_recvPkt.ctrl.analog1);
	m_recvData.analog2		= ntohs(m_recvPkt.ctrl.analog2);
	m_recvData.analog3		= ntohs(m_recvPkt.ctrl.analog3);
	m_recvData.analog4		= ntohs(m_recvPkt.ctrl.analog4);
#if 0
	printf("analog: %04x %04x %04x %04x\n",
		m_recvData.analog1, m_recvData.analog2,
		m_recvData.analog3, m_recvData.analog4);
#endif

	// unpack the cRIO and FPGA checksums (?)
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
# define ntohll(x) ((uint64_t)(x))
#elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
# define ntohll(x) \
	((uint64_t)((((uint64_t)(x) & 0x00000000000000ffLLU) << 56) | \
		    (((uint64_t)(x) & 0x000000000000ff00LLU) << 40) | \
		    (((uint64_t)(x) & 0x0000000000ff0000LLU) << 24) | \
		    (((uint64_t)(x) & 0x00000000ff000000LLU) <<  8) | \
		    (((uint64_t)(x) & 0x000000ff00000000LLU) >>  8) | \
		    (((uint64_t)(x) & 0x0000ff0000000000LLU) >> 24) | \
		    (((uint64_t)(x) & 0x00ff000000000000LLU) >> 40) | \
		    (((uint64_t)(x) & 0xff00000000000000LLU) >> 56)))
#else
#error __BYTE_ORDER__ must be __ORDER_BIG_ENDIAN__ or __ORDER_LITTLE_ENDIAN__
#endif

	m_recvData.cRIOChecksum		= ntohll(m_recvPkt.ctrl.cRIOChecksum);
#if 0
	printf("cRIO checksum: %08lx%08lx\n",
	    (unsigned long)(m_recvData.cRIOChecksum >> 32),
	    (unsigned long)(m_recvData.cRIOChecksum));
#endif
	m_recvData.FPGAChecksum0	= ntohl(m_recvPkt.ctrl.FPGAChecksum0);
	m_recvData.FPGAChecksum1	= ntohl(m_recvPkt.ctrl.FPGAChecksum1);
	m_recvData.FPGAChecksum2	= ntohl(m_recvPkt.ctrl.FPGAChecksum2);
	m_recvData.FPGAChecksum3	= ntohl(m_recvPkt.ctrl.FPGAChecksum3);
#if 0
	printf("FPGA checksums: %08x %08x %08x %08x\n",
		m_recvData.FPGAChecksum0, m_recvData.FPGAChecksum1,
		m_recvData.FPGAChecksum2, m_recvData.FPGAChecksum3);
#endif

	// unpack the DS version string
	// ASCII string, mmddyyvv ("vv"="version" aka "build number")
	memcpy(m_recvData.versionData, m_recvPkt.ctrl.versionData, 8);
#if 0
	printf("DS version %.2s.%.2s.%.2s.%.2s\n",
		&m_recvData.versionData[0], &m_recvData.versionData[2],
		&m_recvData.versionData[4], &m_recvData.versionData[6]);
#endif

	// copy the dynamic extensions
	// we don't know the internal structure of these, so clients are
	// responsible for unpacking their own data
	memcpy(m_extData, m_recvPkt.extData, sizeof m_extData);

	// save sender's address for reply
	memcpy(&m_fromAddr, &fromAddr, sizeof m_fromAddr);

	// tell DriverStation object that new data is available
	if (m_dataAvailable) semFlush(m_dataAvailable);

	// end critical section
    }

    return OK;
}

int getCommonControlData( FRCCommonControlData *data, int wait_ms )
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    // This assumes that wait_ms is always WAIT_FOREVER
    // so we can safely ignore the parameter.
    return netCommObj->GetCommonControlData( data );
}

int FNC::GetCommonControlData( FRCCommonControlData *data )
{
    Synchronized sync(m_dataMutex);

    // copy received data to caller
    memcpy(data, &m_recvData, sizeof (FRCCommonControlData));

    return OK;
}

int getDynamicControlData( uint8_t type, char *dynamicData, int32_t maxLength,
			   int wait_ms )
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    // This assumes that wait_ms is always WAIT_FOREVER
    // so we can safely ignore the parameter.
    return netCommObj->GetDynamicControlData( type, dynamicData, maxLength );
}

// byte-swap 16-bit fields from network to host order
void FNC::SwapDSEIOInput( input_t *pInput )
{
    int i;
    for (i = 0; i < 7; i++) {
	pInput->analog[i] = ntohs(pInput->analog[i]);
    }
    pInput->digital = ntohs(pInput->digital);
    for (i = 0; i < 3; i++) {
	pInput->accel[i] = ntohs(pInput->accel[i]);
    }
    for (i = 0; i < 2; i++) {
	pInput->quad[i] = ntohs(pInput->quad[i]);
    }
}

// byte-swap 16-bit fields from network to host order (or vice versa)
void FNC::SwapDSEIOOutput( output_t *pOutput )
{
    int i;
    pOutput->digital = ntohs(pOutput->digital);
    pOutput->digital_oe = ntohs(pOutput->digital_oe);
    pOutput->digital_pe = ntohs(pOutput->digital_pe);
    for (i = 0; i < 4; i++) {
	pOutput->pwm_compare[i] = ntohs(pOutput->pwm_compare[i]);
    }
    for (i = 0; i < 2; i++) {
	pOutput->pwm_period[i] = ntohs(pOutput->pwm_period[i]);
    }
    // "enables" bitfield order set by #defines in struct definition
}

int FNC::GetDynamicControlData( uint8_t type, char *dynamicData, int32_t maxLength )
{
    Synchronized sync(m_dataMutex);
    
    uint8_t *pData = m_extData;
    uint8_t len;
    uint8_t tag;

    while (pData < &m_extData[FRCEXTDSIZE - 2]) {
	len = pData[0];
	tag = pData[1];
	if (len == 0 || tag == 0) {
	    break;
	}
	if (tag == type) {
	    if (1+len > maxLength) {
		fprintf(stderr, "%s: buffer too small, was %d bytes,"
			" need %u bytes\n", __FUNCTION__, maxLength, len+1);
		return ERROR;
	    }
	    memcpy(dynamicData, pData, 1+len);
	    switch (tag) {
		case kFRC_NetworkCommunication_DynamicType_DSEnhancedIO_Input:
		    SwapDSEIOInput((input_t *)(dynamicData + 2));
		    break;
		case kFRC_NetworkCommunication_DynamicType_DSEnhancedIO_Output:
		    SwapDSEIOOutput((output_t *)(dynamicData + 2));
		    break;
		default:
		    // ignore Kinect data for now
		    break;
	    }
	    return OK;
	}
	pData += 1 + len;
    }

    return ERROR;
}

// These functions set up data to be transmitted back to the driver station
// but don't actually send anything.  The reply is sent when the FRCComm
// task calls Send().

void FRC_NetworkCommunication_observeUserProgramStarting()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    netCommObj->ObserveUserProgramStarting();
}

void FNC::ObserveUserProgramStarting()
{
    Synchronized sync(m_dataMutex);
    printf("FRC_NetworkCommunication: user program is Starting\n");
    m_mode = 0x00;
}

void FRC_NetworkCommunication_observeUserProgramDisabled()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    netCommObj->ObserveUserProgramDisabled();
}

void FNC::ObserveUserProgramDisabled()
{
    Synchronized sync(m_dataMutex);
    if (m_mode != 0x01) {
	printf("FRC_NetworkCommunication: user program is in Disabled mode\n");
	m_mode = 0x01;
    }
}

void FRC_NetworkCommunication_observeUserProgramAutonomous()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    netCommObj->ObserveUserProgramAutonomous();
}

void FNC::ObserveUserProgramAutonomous()
{
    Synchronized sync(m_dataMutex);
    if (m_mode != 0x02) {
	printf("FRC_NetworkCommunication: user program is in Autonomous mode\n");
	m_mode = 0x02;
    }
}

void FRC_NetworkCommunication_observeUserProgramTeleop()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    netCommObj->ObserveUserProgramTeleop();
}

void FNC::ObserveUserProgramTeleop()
{
    Synchronized sync(m_dataMutex);
    if (m_mode != 0x04) {
	printf("FRC_NetworkCommunication: user program is in Teleop mode\n");
	m_mode = 0x04;
    }
}

void FRC_NetworkCommunication_observeUserProgramTest()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    netCommObj->ObserveUserProgramTest();
}

void FNC::ObserveUserProgramTest()
{
    Synchronized sync(m_dataMutex);
    if (m_mode != 0x08) {
	printf("FRC_NetworkCommunication: user program is in Test mode\n");
	m_mode = 0x08;
    }
}


int setStatusData( float battery, uint8_t dsDigitalOut, uint8_t updateNumber,
		   const char *userDataHigh, int userDataHighLength,
		   const char *userDataLow, int userDataLowLength,
		   int wait_ms )
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    return netCommObj->SetStatusData( battery, dsDigitalOut, updateNumber,
				      userDataHigh, userDataHighLength,
				      userDataLow, userDataLowLength,
				      wait_ms );
}

int FNC::SetStatusData( float battery, uint8_t dsDigitalOut, uint8_t updateNumber,
		        const char *userDataHigh, int userDataHighLength,
		        const char *userDataLow, int userDataLowLength,
			int wait_ms )
{
    Synchronized sync(m_dataMutex);

    m_battery = battery;
    m_dsDigitalOut = dsDigitalOut;
    m_updateNumber = updateNumber;

    if (userDataHigh && userDataHighLength) {
	// discard any unsent data
	FreeUserDataHigh();
	// copy new data to local storage until we can send it to DS
	m_userDataHigh = new char[userDataHighLength];
	memcpy(m_userDataHigh, userDataHigh, userDataHighLength);
	m_userDataHighLength = userDataHighLength;
    }

    if (userDataLow && userDataLowLength) {
	// discard any unsent data
	FreeUserDataLow();
	// copy new data to local storage until we can send it to DS
	m_userDataLow = new char[userDataLowLength];
	memcpy(m_userDataLow, userDataLow, userDataLowLength);
	m_userDataLowLength = userDataLowLength;
    }

    // ignore wait_ms - assume it's always WAIT_FOREVER

    return OK;
}

void FNC::FreeUserDataHigh()
{
    if (m_userDataHigh) {
	delete m_userDataHigh;
	m_userDataHigh = NULL;
	m_userDataHighLength = 0;
    }
}

void FNC::FreeUserDataLow()
{
    if (m_userDataLow) {
	delete m_userDataLow;
	m_userDataLow = NULL;
	m_userDataLowLength = 0;
    }
}

int setErrorData(const char *errors, int errorsLength, int wait_ms)
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    return netCommObj->SetErrorData( errors, errorsLength, wait_ms );
}

int FNC::SetErrorData( const char *errors, int errorsLength, int wait_ms )
{
    Synchronized sync(m_dataMutex);
    
    if (errors && errorsLength) {
	// discard any unsent data
	FreeErrorData();
	// copy new data to local storage until we can send it to DS
	m_userErrMsg = new char[errorsLength];
	memcpy(m_userErrMsg, errors, errorsLength);
	m_userErrMsgLength = errorsLength;
    }

    return OK;
}

void FNC::FreeErrorData()
{
    if (m_userErrMsg) {
	delete m_userErrMsg;
	m_userErrMsg = NULL;
	m_userErrMsgLength = 0;
    }
}

int overrideIOConfig( const char *ioConfig, int wait_ms )
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    // Ignore wait_ms - we don't know what it means in this context.
    return netCommObj->OverrideIOConfig( ioConfig, wait_ms );
}

int FNC::OverrideIOConfig( const char *eioConfig, int wait_ms )
{
    Synchronized sync(m_dataMutex);

    if (eioConfig) {
	// discard any unsent data
	FreeEIOConfig();
	// copy new data to local storage until we can send it to DS
	int ecl = 1 + (uint8_t)eioConfig[0];
	m_eioConfig = new char[ecl];
	memcpy(m_eioConfig, eioConfig, ecl);
	m_eioConfigLength = ecl;
    }

    return OK;
}

void FNC::FreeEIOConfig()
{
    if (m_eioConfigLength) {
	delete m_eioConfig;
	m_eioConfig = NULL;
	m_eioConfigLength = 0;
    }
}

int setUserDsLcdData( const char *lcdData, int lcdDataLength, int wait_ms )
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }

    // Ignore wait_ms - we don't know what it means in this context.
    return netCommObj->SetLCDData( lcdData, lcdDataLength, wait_ms );
}

int FNC::SetLCDData( const char *lcdData, int lcdDataLength, int wait_ms )
{
    Synchronized sync(m_dataMutex);

    if (lcdData) {
	// discard any unsent data
	FreeLCDData();

	if (lcdDataLength == USER_DS_LCD_DATA_SIZE) {
	    // copy new data to local storage until we can send it to DS
	    m_lcdData = new char[lcdDataLength];
	    memcpy(m_lcdData, lcdData, lcdDataLength);
	    *(uint16_t *)m_lcdData = htons(*(uint16_t *)lcdData);
	    m_lcdDataLength = lcdDataLength;
	} else {
	    fprintf(stderr, "%s: wrong length (%d), expected (%d)\n",
		__FUNCTION__, lcdDataLength, USER_DS_LCD_DATA_SIZE);
	}
    }

    return OK;
}

void FNC::FreeLCDData()
{
    if (m_lcdData) {
	delete m_lcdData;
	m_lcdData = NULL;
	m_lcdDataLength = 0;
    }
}


int FNC::Send()
{
    Synchronized sync(m_dataMutex);

    memset(&m_sendPkt, 0, sizeof m_sendPkt);

    // send our current control mode back to DS
    // keep the same bit ordering sent from the DS
    m_sendPkt.control = m_recvPkt.ctrl.control;

    // battery voltage - BCD scaled integer
    int vbat = (int)(m_battery * 100.0);
    m_sendPkt.battery[0] = (((vbat / 1000) % 10) << 4) | ((vbat / 100) % 10);
    m_sendPkt.battery[1] = (((vbat / 10) % 10) << 4) | (vbat % 10);

    // 8 bits of "digital outputs" displayed on DS
    m_sendPkt.dsDigitalOut = m_dsDigitalOut;

    // copy team ID from DS control packet
    m_sendPkt.teamID = htons(m_recvData.teamID);

    // fake the cRIO MAC address - does the DS care about this?
    memcpy(m_sendPkt.macAddr, fakeMAC, 6);

    // fake the FRC_NetworkCommunication version
    strncpy(m_sendPkt.versionData, commVersion, sizeof m_sendPkt.versionData);

    // robot operating mode
    m_sendPkt.mode = htons(m_mode);

    // copy last received packet number from DS control packet
    m_sendPkt.packetIndex = htons(m_recvData.packetIndex);

    // dashboard status update number
    m_sendPkt.updateNumber = m_updateNumber;

    // dashboard data...include as much as will fit in the allowed space
    //   high-priority data length (4 bytes)
    //   high-priority data bytes  (variable-length)
    //   error message length (4 bytes)
    //   error message string (variable-length)
    //   low-priority data length (4 bytes)
    //   low-priority data bytes (variable-length)
    unsigned int udlen = 0;
    const unsigned int udMaxLen = sizeof m_sendPkt.userData;
    if (udlen + sizeof (uint32_t) + m_userDataHighLength <= udMaxLen) {
	uint32_t udhl = htonl(m_userDataHighLength);
	memcpy(&m_sendPkt.userData[udlen], &udhl, sizeof udhl);
	udlen += sizeof udhl;
	if (m_userDataHigh) {
	    memcpy(&m_sendPkt.userData[udlen], m_userDataHigh,
		    m_userDataHighLength);
	    udlen += m_userDataHighLength;
	    // now that the message has been sent, free the local copy
	    FreeUserDataHigh();
	}
    }

    if (udlen + sizeof (uint32_t) + m_userErrMsgLength <= udMaxLen) {
	uint32_t erl = htonl(m_userErrMsgLength);
	memcpy(&m_sendPkt.userData[udlen], &erl, sizeof erl);
	udlen += sizeof erl;
	if (m_userErrMsg) {
	    memcpy(&m_sendPkt.userData[udlen], m_userErrMsg,
		    m_userErrMsgLength);
	    udlen += m_userErrMsgLength;
	    // now that the message has been sent, free the local copy
	    FreeErrorData();
	}
    }

    if (udlen + sizeof (uint32_t) + m_userDataLowLength <= udMaxLen) {
	uint32_t udll = htonl(m_userDataLowLength);
	memcpy(&m_sendPkt.userData[udlen], &udll, sizeof udll);
	udlen += sizeof udll;
	if (m_userDataLow) {
	    memcpy(&m_sendPkt.userData[udlen], m_userDataLow,
		    m_userDataLowLength);
	    udlen += m_userDataLowLength;
	    // now that the message has been sent, free the local copy
	    FreeUserDataLow();
	}
    }

#if 0
    // this is a hack
    // the real robot code updates this data once/second
    // and it's not clear how the numbers are generated
    // (or why it needs to be 40 bytes long!)
    clock_gettime(CLOCK_REALTIME, (timespec *)m_sendPkt.sysstat);
#endif

    // DS Enhanced I/O configuration
    if (m_eioConfig) {
	memcpy(m_sendPkt.eioConfig, m_eioConfig, m_eioConfigLength);
	if (m_eioConfigLength >= 26 && m_sendPkt.eioConfig[1] ==
	  kFRC_NetworkCommunication_DynamicType_DSEnhancedIO_Output) {
	    SwapDSEIOOutput((output_t *)&m_sendPkt.eioConfig[2]);
	}
	FreeEIOConfig();
    }

    // calculate CRC on the first 1024 bytes
    uint32_t crc = crc32(0, Z_NULL, 0);
    crc = crc32(crc, (uint8_t *)&m_sendPkt, 1024);
    m_sendPkt.crc = htonl(crc);

    m_sendPktLength = 1024;
    if (m_lcdData) {
	if (m_lcdDataLength <= sizeof m_sendPkt.lcdData) {
	    memcpy(m_sendPkt.lcdData, m_lcdData, m_lcdDataLength);
	    m_sendPktLength += m_lcdDataLength;
	}
	FreeLCDData();
    }

    m_toAddr.sin_family = AF_INET;
    m_toAddr.sin_addr.s_addr = m_fromAddr.sin_addr.s_addr;
    m_toAddr.sin_port = htons(DS_REPLY_PORT);

    int n = sendto(m_sendSocket, (char *)&m_sendPkt, m_sendPktLength, 0,
		    (struct sockaddr *)&m_toAddr, sizeof m_toAddr);
    if (n == -1) {
	perror("FRC_NetworkCommunication::Recv::sendto");
	return ERROR;
    }

    m_pcap->write_record(&m_sendAddr, &m_toAddr, (char *)&m_sendPkt, n);

    return OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// usage reporting
//
///////////////////////////////////////////////////////////////////////////////

namespace nUsageReporting {

uint32_t report(
	tResourceType resource,
	uint8_t instanceNumber,
	uint8_t context,
	const char *feature
    )
{
    const char *resourceName = NULL;
    const char *instanceName = NULL;

    switch (resource) {
    case kResourceType_Controller:
	resourceName = "Controller";
	break;
    case kResourceType_Module:
	resourceName = "Module";
	break;
    case kResourceType_Language:
	resourceName = "Language";
	switch (instanceNumber) {
	case kLanguage_LabVIEW:
	    instanceName = "LabVIEW";
	    break;
	case kLanguage_CPlusPlus:
	    instanceName = "C++";
	    break;
	case kLanguage_Java:
	    instanceName = "Java";
	    break;
	case kLanguage_Python:
	    instanceName = "Python";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    case kResourceType_CANPlugin:
	resourceName = "CANPlugin";
	switch (instanceNumber) {
	case kCANPlugin_BlackJagBridge:
	    instanceName = "BlackJagBridge";
	    break;
	case kCANPlugin_2CAN:
	    instanceName = "2CAN";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    case kResourceType_Accelerometer:
	resourceName = "Accelerometer";
	break;
    case kResourceType_ADXL345:
	resourceName = "ADXL345";
	switch (instanceNumber) {
	case kADXL345_SPI:
	    instanceName = "SPI";
	    break;
	case kADXL345_I2C:
	    instanceName = "I2C";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    case kResourceType_AnalogChannel:
	resourceName = "AnalogChannel";
	break;
    case kResourceType_AnalogTrigger:
	resourceName = "AnalogTrigger";
	break;
    case kResourceType_AnalogTriggerOutput:
	resourceName = "AnalogTriggerOutput";
	break;
    case kResourceType_CANJaguar:
	resourceName = "CANJaguar";
	break;
    case kResourceType_Compressor:
	resourceName = "Compressor";
	break;
    case kResourceType_Counter:
	resourceName = "Counter";
	break;
    case kResourceType_Dashboard:
	resourceName = "Dashboard";
	break;
    case kResourceType_DigitalInput:
	resourceName = "DigitalInput";
	break;
    case kResourceType_DigitalOutput:
	resourceName = "DigitalOutput";
	break;
    case kResourceType_DriverStationCIO:
	resourceName = "DriverStationCIO";
	switch (instanceNumber) {
	case kDriverStationCIO_Analog:
	    instanceName = "Analog";
	    break;
	case kDriverStationCIO_DigitalIn:
	    instanceName = "DigitalIn";
	    break;
	case kDriverStationCIO_DigitalOut:
	    instanceName = "DigitalOut";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    case kResourceType_DriverStationEIO:
	resourceName = "DriverStationEIO";
	switch (instanceNumber) {
	case kDriverStationEIO_Acceleration:
	    instanceName = "Acceleration";
	    break;
	case kDriverStationEIO_AnalogIn:
	    instanceName = "AnalogIn";
	    break;
	case kDriverStationEIO_AnalogOut:
	    instanceName = "AnalogOut";
	    break;
	case kDriverStationEIO_Button:
	    instanceName = "Button";
	    break;
	case kDriverStationEIO_LED:
	    instanceName = "LED";
	    break;
	case kDriverStationEIO_DigitalIn:
	    instanceName = "DigitalIn";
	    break;
	case kDriverStationEIO_DigitalOut:
	    instanceName = "DigitalOut";
	    break;
	case kDriverStationEIO_FixedDigitalOut:
	    instanceName = "FixedDigitalOut";
	    break;
	case kDriverStationEIO_PWM:
	    instanceName = "PWM";
	    break;
	case kDriverStationEIO_Encoder:
	    instanceName = "Encoder";
	    break;
	case kDriverStationEIO_TouchSlider:
	    instanceName = "TouchSlider";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    case kResourceType_DriverStationLCD:
	resourceName = "DriverStationLCD";
	break;
    case kResourceType_Encoder:
	resourceName = "Encoder";
	break;
    case kResourceType_GearTooth:
	resourceName = "GearTooth";
	break;
    case kResourceType_Gyro:
	resourceName = "Gyro";
	break;
    case kResourceType_I2C:
	resourceName = "I2C";
	break;
    case kResourceType_Framework:
	resourceName = "Framework";
	switch (instanceNumber) {
	case kFramework_Iterative:
	    instanceName = "IterativeRobot";
	    break;
	case kFramework_Simple:
	    instanceName = "SimpleRobot";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    case kResourceType_Jaguar:
	resourceName = "Jaguar";
	break;
    case kResourceType_Joystick:
	resourceName = "Joystick";
	break;
    case kResourceType_Kinect:
	resourceName = "Kinect";
	break;
    case kResourceType_KinectStick:
	resourceName = "KinectStick";
	break;
    case kResourceType_PIDController:
	resourceName = "PIDController";
	break;
    case kResourceType_Preferences:
	resourceName = "Preferences";
	break;
    case kResourceType_PWM:
	resourceName = "PWM";
	break;
    case kResourceType_Relay:
	resourceName = "Relay";
	break;
    case kResourceType_RobotDrive:
	resourceName = "RobotDrive";
	switch (instanceNumber) {
	case kRobotDrive_ArcadeStandard:
	    instanceName = "ArcadeStandard";
	    break;
	case kRobotDrive_ArcadeButtonSpin:
	    instanceName = "ArcadeButtonSpin";
	    break;
	case kRobotDrive_ArcadeRatioCurve:
	    instanceName = "ArcadeRatioCurve";
	    break;
	case kRobotDrive_Tank:
	    instanceName = "Tank";
	    break;
	case kRobotDrive_MecanumPolar:
	    instanceName = "MecanumPolar";
	    break;
	case kRobotDrive_MecanumCartesian:
	    instanceName = "MecanumCartesian";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    case kResourceType_SerialPort:
	resourceName = "SerialPort";
	break;
    case kResourceType_Servo:
	resourceName = "Servo";
	break;
    case kResourceType_Solenoid:
	resourceName = "Solenoid";
	break;
    case kResourceType_SPI:
	resourceName = "SPI";
	break;
    case kResourceType_Task:
	resourceName = "Task";
	break;
    case kResourceType_Ultrasonic:
	resourceName = "Ultrasonic";
	break;
    case kResourceType_Victor:
	resourceName = "Victor";
	break;
    case kResourceType_Button:
	resourceName = "Button";
	break;
    case kResourceType_Command:
	resourceName = "Command";
	switch (instanceNumber) {
	case kCommand_Scheduler:
	    instanceName = "Scheduler";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    case kResourceType_AxisCamera:
	resourceName = "AxisCamera";
	break;
    case kResourceType_PCVideoServer:
	resourceName = "PCVideoServer";
	break;
    case kResourceType_SmartDashboard:
	resourceName = "SmartDashboard";
	switch (instanceNumber) {
	case kSmartDashboard_Instance:
	    instanceName = "Instance";
	    break;
	default:
	    instanceName = "unknown";
	    break;
	}
	break;
    default:
	resourceName = "unknown";
	break;
    }

    // ugly hack to concatenate strings into a single printf

    char r[40], i[40], c[40], f[40];

    r[0] = '\0';
    if (resourceName) {
	snprintf(r, sizeof r, " resource %u (%s)",
		 (unsigned) resource, resourceName);
    }

    i[0] = '\0';
    if (instanceName) {
	snprintf(i, sizeof i, " instance %u (%s)",
		 instanceNumber, instanceName);
    }

    c[0] = '\0';
    {
	snprintf(c, sizeof c, " context %u", context);
    }

    f[0] = '\0';
    if (feature) {
        snprintf(f, sizeof f, " feature %s", feature);
    }

#ifdef DEBUG
    printf("usageReporting:%s%s%s%s\n", r, i, c, f);
#endif

    return OK;
}

}; // namespace UsageReporting

static uint32_t replyMessageID = 0;
static uint8_t replyData[8];
static uint8_t replyDataSize = 0;

extern "C" void FRC_NetworkCommunication_JaguarCANDriver_sendMessage(
    uint32_t messageID,
    const uint8_t *data,
    uint8_t dataSize,
    int32_t *status )
{
    messageID &= ~0x80000000;
    switch (messageID & ~0x3f) {
    case CAN_MSGID_API_SYNC:
	// ignore this message
	break;

    case CAN_MSGID_API_FIRMVER:
	replyMessageID = messageID;
	*(uint32_t *)replyData = htole32(101);
	replyDataSize = sizeof (uint32_t);
	break;

    case LM_API_SPD_PC:
    case LM_API_SPD_IC:
    case LM_API_SPD_DC:
    case LM_API_POS_PC:
    case LM_API_POS_IC:
    case LM_API_POS_DC:
    case LM_API_ICTRL_PC:
    case LM_API_ICTRL_IC:
    case LM_API_ICTRL_DC:
    case LM_API_VOLT_T_EN:
    case LM_API_SPD_T_EN:
    case LM_API_POS_T_EN:
    case LM_API_ICTRL_T_EN:
    case LM_API_VCOMP_T_EN:
    case LM_API_VOLT_DIS:
    case LM_API_SPD_DIS:
    case LM_API_POS_DIS:
    case LM_API_ICTRL_DIS:
    case LM_API_VCOMP_DIS:
    case LM_API_STATUS_POWER:
    case LM_API_VOLT_SET_RAMP:
    case LM_API_VCOMP_IN_RAMP:
    case LM_API_CFG_BRAKE_COAST:
    case LM_API_CFG_ENC_LINES:
    case LM_API_CFG_POT_TURNS:
    case LM_API_CFG_LIMIT_FWD:
    case LM_API_CFG_LIMIT_REV:
    case LM_API_CFG_LIMIT_MODE:
    case LM_API_CFG_MAX_VOUT:
    case LM_API_CFG_FAULT_TIME:
    case LM_API_VOLT_T_SET:
    case LM_API_SPD_T_SET:
    case LM_API_POS_T_SET:
    case LM_API_ICTRL_T_SET:
    case LM_API_VCOMP_T_SET:
    case LM_API_SPD_REF:
    case LM_API_POS_REF:
	replyMessageID = LM_API_ACK | (messageID & 0x3f);
	memcpy(replyData, data, dataSize);
	replyDataSize = dataSize;
	break;

    case LM_API_VOLT_SET:
    case LM_API_ICTRL_SET:
    case LM_API_VCOMP_SET:
	replyMessageID = messageID;
	memset(replyData, 0, sizeof replyData);
	replyDataSize = sizeof(int16_t);
	break;

    case LM_API_SPD_SET:
    case LM_API_POS_SET:
	replyMessageID = messageID;
	memset(replyData, 0, sizeof replyData);
	replyDataSize = sizeof(int32_t);
	break;

    default:
	replyMessageID = 0;
	replyDataSize = 0;
	break;
    }
    *status = 0;
}

extern "C" void FRC_NetworkCommunication_JaguarCANDriver_receiveMessage(
    uint32_t *messageID,
    uint8_t *data,
    uint8_t *dataSize,
    uint32_t timeoutMs,
    int32_t *status )
{
    if (messageID && replyMessageID == (*messageID & ~0x80000000)) {
	if (dataSize) *dataSize = replyDataSize;
	if (data) memcpy(data, replyData, replyDataSize);
	*status = 0;
    } else {
	*status = -1;
    }
}
