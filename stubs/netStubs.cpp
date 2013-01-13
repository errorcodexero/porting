// stubs for network communications
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <zlib.h>
#include "stubs.h"
#include "Synchronized.h"
#include "DriverStation.h"
#include "NetworkCommunication/FRCComm.h"
#include "NetworkCommunication/UsageReporting.h"

uint8_t fakeMAC[] = { 0x00, 0x80, 0x2f, 0x11, 0xd8, 0x2a };
char fakeVersion[] = "06300800";

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

class FNC // FRC_NetworkCommunication
{
private:
    friend void setNewDataSem( SEM_ID );

    FNC( SEM_ID );
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

private:
    STATUS Recv();

    static const size_t FRCCCDSize = sizeof (FRCCommonControlData); // 80

#pragma pack(push,1)
    struct FRCRecvPkt {			// received data in network order...
	FRCCommonControlData ctrl;	// common control data: 80 bytes
	char ext[940];			// extension data: up to 940 bytes
	uint32_t crc;			// 32-bit CRC
    } m_recvPkt;
#pragma pack(pop)
    sockaddr_in m_fromAddr;		// sending driver station's address
    FRCCommonControlData m_recvData;	// control data in host byte order

public:
    int GetCommonControlData( FRCCommonControlData *data );

public:
    // keep a local copy of user data to be sent to the DS
    void ObserveUserProgramStarting();
    void ObserveUserProgramDisabled();
    void ObserveUserProgramAutonomous();
    void ObserveUserProgramTeleop();
    void ObserveUserProgramTest();
    int SetStatusData( float battery, UINT8 dsDigitalOut, UINT8 updateNumber,
		        const char *userDataHigh, int userDataHighLength,
		        const char *userDataLow, int userDataLowLength,
			int wait_ms );

private:
    bool m_reset;
    bool m_enabled;
    bool m_autonomous;
    bool m_test;

    float m_battery;
    UINT8 m_dsDigitalOut;
    UINT8 m_updateNumber;

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
	union {
		UINT8 control;
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
		struct {
			UINT8 checkVersions : 1;
			UINT8 test : 1;
			UINT8 resync : 1;
			UINT8 fmsAttached : 1;
			UINT8 autonomous : 1;
			UINT8 enabled : 1;
			UINT8 notEStop : 1;
			UINT8 reset : 1;
		};
#else
		struct {
			UINT8 reset : 1;
			UINT8 notEStop : 1;
			UINT8 enabled : 1;
			UINT8 autonomous : 1;
			UINT8 fmsAttached : 1;
			UINT8 resync : 1;
			UINT8 test : 1;
			UINT8 checkVersions : 1;
		};
#endif
	};
	uint8_t battery[2];		// 2 bytes, scaled integer
	uint8_t dsDigitalOut;		// DS digital outputs
	uint8_t unknown1[4];
	uint16_t teamID;		// copy of team ID from DS
	uint8_t macAddr[6];		// cRIO MAC address
	char versionData[8];		// DS version?
	uint8_t unknown2[6];
	uint16_t packetIndex;		// copied from DS control packet
	uint8_t updateNumber;		// from setStatusData()
	uint8_t userData[951];		// contains:
					// user data high len (4 bytes)
					// user data high (n bytes)
					// error string len (4 bytes)
					// error string (n bytes)
					// user data low len (4 bytes)
					// user data low (n bytes)
					// ...or as much as will fit
	uint8_t eioConfig[32];		// DSEIO status_block_t + padding
	uint8_t unknown3[4];
	uint32_t crc;
	uint8_t lcdData[448];		// optional
    } m_sendPkt;
#pragma pack(pop)
    sockaddr_in m_toAddr;
    uint32_t m_sendPktLength;

private:
    DISALLOW_COPY_AND_ASSIGN(FNC);
};


static FNC *netCommObj = NULL;

void setNewDataSem( SEM_ID dataAvailable )
{
    if (dataAvailable) {
	if (!netCommObj) {
	    netCommObj = new FNC( dataAvailable );
	}
	// This probably isn't necessary. setNewDataSem is only supposed
	// to be called with a non-NULL SEM_ID when the DriverStation
	// singleton object is created and with a NULL SEM_ID when that
	// object is destroyed.  But just in case, we also allow a new
	// semaphore to replace the old semaphore.
	netCommObj->SetNewDataSem( dataAvailable );
    } else {
	delete netCommObj;
	netCommObj = NULL;
    }
}

FNC::FNC( SEM_ID dataAvailable )
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

    m_reset = true;

    m_dataAvailable = dataAvailable;
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
#if (BYTE_ORDER == BIG_ENDIAN)
# define ntohll(x) ((uint64_t)(x))
#else
# define ntohll(x) \
	((uint64_t)((((uint64_t)(x) & 0x00000000000000ffLLU) << 56) | \
		    (((uint64_t)(x) & 0x000000000000ff00LLU) << 40) | \
		    (((uint64_t)(x) & 0x0000000000ff0000LLU) << 24) | \
		    (((uint64_t)(x) & 0x00000000ff000000LLU) <<  8) | \
		    (((uint64_t)(x) & 0x000000ff00000000LLU) >>  8) | \
		    (((uint64_t)(x) & 0x0000ff0000000000LLU) >> 24) | \
		    (((uint64_t)(x) & 0x00ff000000000000LLU) >> 40) | \
		    (((uint64_t)(x) & 0xff00000000000000LLU) >> 56)))
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
	printf("FPGA checksums: %08lx %08lx %08lx %08lx\n",
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

	// save sender's address for reply
	memcpy(&m_fromAddr, &fromAddr, sizeof m_fromAddr);

	// tell DriverStation object that new data is available
	semFlush(m_dataAvailable);

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

int getDynamicControlData( UINT8 type, char *dynamicData, INT32 maxLength,
			   int wait_ms )
{
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
	// abort();
	return;
    }

    netCommObj->ObserveUserProgramStarting();
}

void FNC::ObserveUserProgramStarting()
{
    Synchronized sync(m_dataMutex);
    if (!m_reset) {
	printf("FRC_NetworkCommunication: user program is Starting\n");
	// this should make the driver station switch to disabled mode!
	m_reset = true;
	m_enabled = false;
	m_autonomous = false;
	m_test = false;
    }
}

void FRC_NetworkCommunication_observeUserProgramDisabled()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	// abort();
	return;
    }

    netCommObj->ObserveUserProgramDisabled();
}

void FNC::ObserveUserProgramDisabled()
{
    Synchronized sync(m_dataMutex);
    if (m_enabled) {
	printf("FRC_NetworkCommunication: user program is in Disabled mode\n");
	m_enabled = false;
    }
}

void FRC_NetworkCommunication_observeUserProgramAutonomous()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	// abort();
	return;
    }

    netCommObj->ObserveUserProgramAutonomous();
}

void FNC::ObserveUserProgramAutonomous()
{
    Synchronized sync(m_dataMutex);
    if (!m_autonomous) {
	printf("FRC_NetworkCommunication: user program is in Autonomous mode\n");
	m_autonomous = true;
	m_test = false;
    }
}

void FRC_NetworkCommunication_observeUserProgramTeleop()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	// abort();
	return;
    }

    netCommObj->ObserveUserProgramTeleop();
}

void FNC::ObserveUserProgramTeleop()
{
    Synchronized sync(m_dataMutex);
    if (m_autonomous || m_test) {
	printf("FRC_NetworkCommunication: user program is in Teleop mode\n");
	m_autonomous = false;
	m_test = false;
    }
}

void FRC_NetworkCommunication_observeUserProgramTest()
{
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	// abort();
	return;
    }

    netCommObj->ObserveUserProgramTest();
}

void FNC::ObserveUserProgramTest()
{
    Synchronized sync(m_dataMutex);
    if (!m_test) {
	printf("FRC_NetworkCommunication: user program is in Test mode\n");
	m_autonomous = false;
	m_test = true;
    }
}


int setStatusData( float battery, UINT8 dsDigitalOut, UINT8 updateNumber,
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

int FNC::SetStatusData( float battery, UINT8 dsDigitalOut, UINT8 updateNumber,
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
	// copy new data to local storage until we can send it to DS
	int lcdlen = 1 + (uint8_t)lcdData[0];
	m_lcdData = new char[lcdlen];
	memcpy(m_lcdData, lcdData, lcdlen);
	m_lcdDataLength = lcdlen;
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
    m_sendPkt.reset = m_reset; m_reset = false;
    m_sendPkt.notEStop = true;
    m_sendPkt.enabled = m_enabled;
    m_sendPkt.autonomous = m_autonomous;
    m_sendPkt.test = m_test;

    // copy resync flag from DS control packet
    m_sendPkt.resync = m_recvData.resync;

    // battery voltage - BCD scaled integer
    int vbat = (int)(m_battery * 100.0);
    m_sendPkt.battery[0] = (((vbat / 1000) % 10) << 4) | ((vbat / 100) % 10);
    m_sendPkt.battery[1] = (((vbat / 10) % 10) << 4) | (vbat % 10);

    // 8 bits of "digital outputs" displayed on DS
    m_sendPkt.dsDigitalOut = m_dsDigitalOut;

    m_sendPkt.unknown1[0] = 0;
    m_sendPkt.unknown1[1] = 0;
    m_sendPkt.unknown1[2] = 0;
    m_sendPkt.unknown1[3] = 0;

    // copy team ID from DS control packet
    m_sendPkt.teamID = htons(m_recvData.teamID);

    // fake the cRIO MAC address - does the DS care about this?
    memcpy(m_sendPkt.macAddr, fakeMAC, 6);

    // fake the FRC_NetworkCommunications version
    strncpy(m_sendPkt.versionData, fakeVersion, sizeof m_sendPkt.versionData);

    m_sendPkt.unknown2[0] = 0;
    m_sendPkt.unknown2[1] = 0;
    m_sendPkt.unknown2[2] = 0;
    m_sendPkt.unknown2[3] = 0;
    m_sendPkt.unknown2[4] = 0;
    m_sendPkt.unknown2[5] = 0;

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

    // DS Enhanced I/O configuration
    if (m_eioConfig) {
	memcpy(m_sendPkt.eioConfig, m_eioConfig, m_eioConfigLength);
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

UINT32 report(
	tResourceType resource,
	UINT8 instanceNumber,
	UINT8 context,
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

    printf("usageReporting:%s%s%s%s\n", r, i, c, f);

    return OK;
}

}; // namespace UsageReporting

