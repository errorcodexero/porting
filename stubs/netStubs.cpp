// stubs for network communications
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <zlib.h>
#include "stubs.h"
#include "Synchronized.h"
#include "DriverStation.h"
#include "NetworkCommunication/FRCComm.h"
#include "NetworkCommunication/UsageReporting.h"

///////////////////////////////////////////////////////////////////////////////
//
// libpcap replacement for logging traffic to pcap file
//
///////////////////////////////////////////////////////////////////////////////

class pcap {
private:
    FILE * m_file;
    sockaddr_in m_myaddr;

public:
    pcap( char *name, sockaddr_in *addr );
    ~pcap();

    void write_record( sockaddr_in *from, char *data, int len );
};

pcap::pcap( char *name, sockaddr_in *addr )
{
    memcpy(&m_myaddr, addr, sizeof (sockaddr_in));
    m_file = fopen(name, "wb");
    if (!m_file) {
	perror("pcap::fopen");
	abort();
    }

    struct pcap_hdr {
	uint32_t magic_number;
	uint16_t version_major;
	uint16_t version_minor;
	int32_t  thiszone;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t network;
    } hdr;

    hdr.magic_number = 0xa1b2c3d4;
    hdr.version_major = 2;
    hdr.version_minor = 4;
    hdr.thiszone = 0;
    hdr.sigfigs = 0;
    hdr.snaplen = 1500;
    hdr.network = 1; // Ethernet

    fwrite((char *)&hdr, sizeof hdr, 1, m_file);
}

void pcap::write_record( sockaddr_in *from, char *packet, int len )
{
    struct pcaprec_hdr {
	uint32_t ts_sec;
	uint32_t ts_usec;
	uint32_t incl_len;
	uint32_t orig_len;
    } hdr;

    struct ether_hdr {
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t type;
    } eth;

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

    struct udp_hdr {
	uint16_t srcport;
	uint16_t dstport;
	uint16_t udplen;
	uint16_t cksum;
    } udp;

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
    ip.srcip = from->sin_addr.s_addr;
    ip.dstip = m_myaddr.sin_addr.s_addr;

    fwrite((char *)&ip, sizeof ip, 1, m_file);

    udp.srcport = from->sin_port;
    udp.dstport = m_myaddr.sin_port;
    udp.udplen = htons(udplen);
    udp.cksum = 0;

    fwrite((char *)&udp, sizeof udp, 1, m_file);

    fwrite((char *)packet, len, 1, m_file);
    fflush(m_file);
}

pcap::~pcap()
{
    fclose(m_file);
}

///////////////////////////////////////////////////////////////////////////////
//
// network communication with the driver station and/or FMS
//
///////////////////////////////////////////////////////////////////////////////

#define	ROBOT_PORT	1110
#define	DS_PACKET_MIN	1024
#define	DS_PACKET_MAX	1472	// 1500 - 20 (IP header) - 8 (UDP header)

extern "C" void setNewDataSem( SEM_ID );
extern "C" int getCommonControlData( FRCCommonControlData *, int );

class FNC // FRC_NetworkCommunication
{
private:
    friend void setNewDataSem( SEM_ID );
    friend int getCommonControlData( FRCCommonControlData *, int );

    FNC( SEM_ID );
    void SetNewDataSem( SEM_ID );
    static void RecvTask( FNC * );
    STATUS Recv();

    int m_socket;
    pcap *m_pcap;
    SEM_ID m_dataAvailable;
    SEM_ID m_dataMutex;
    FRCCommonControlData m_recvData;
    static const size_t FRCCCDSize = sizeof (FRCCommonControlData);
    sockaddr_in m_dsaddr;

    Task *m_recvTask;
    Task *m_sendTask;

public:
    ~FNC();

private:
    FNC(const FNC&);
    void operator=(const FNC&);
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
    m_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if (m_socket == -1) {
	perror("socket");
	abort();
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(ROBOT_PORT);

    if (bind(m_socket, (struct sockaddr *)&addr, sizeof addr) == -1) {
	perror("bind");
	abort();
    }

    m_pcap = new pcap("robot.pcap", &addr);

    m_dataAvailable = dataAvailable;
    m_dataMutex = semMCreate(SEM_Q_PRIORITY|SEM_DELETE_SAFE|SEM_INVERSION_SAFE);
    m_recvTask = new Task( "NetRecv", (FUNCPTR) FNC::RecvTask );
    m_recvTask->Start((UINT32)this);
}

FNC::~FNC()
{
    // can't use Synchronized here since we need to destroy the semaphore
    //   without releasing it
    semTake(m_dataMutex, WAIT_FOREVER);
    delete m_sendTask;
    delete m_recvTask;
    close(m_socket);
    delete m_pcap;
    semDelete(m_dataMutex);
}

void FNC::SetNewDataSem( SEM_ID sem )
{
    m_dataAvailable = sem;
}

void FNC::RecvTask( FNC *pObj )
{
    while (1) {
	if (pObj->Recv() == ERROR) {
	    break;
	}
    }
}

STATUS FNC::Recv()
{
    char packet[DS_PACKET_MAX];
    struct sockaddr_in sender;
    socklen_t senderLen = sizeof sender;

    int n = recvfrom(m_socket, packet, sizeof packet, 0,
		     (struct sockaddr *)&sender, &senderLen);
    if (n == -1) {
	perror("FRC_NetworkCommunication::Recv::recvfrom");
	return ERROR;
    }

    m_pcap->write_record(&sender, packet, n);

    if (n < DS_PACKET_MIN) {
	printf("FRC_NetworkCommunication::Recv:"
	       " packet too small (%d)\n", n);
	return OK;
    }

    UINT32 recvCRC = ntohl(*(UINT32 *)(&packet[DS_PACKET_MIN - 4]));
    UINT32 calcCRC = crc32(0, Z_NULL, 0);
    *(UINT32 *)(&packet[DS_PACKET_MIN - 4]) = 0;
    calcCRC = crc32(calcCRC, (uint8_t *)packet, DS_PACKET_MIN);

    if (calcCRC != recvCRC) {
	printf("FRC_NetworkCommunication::Recv: bad checksum "
	       "(received 0x%08lx, calculated 0x%08lx)\n", recvCRC, calcCRC);
	return OK;
    }

    // unpack and copy data to m_recvData
    FRCCommonControlData *pkt = (FRCCommonControlData *) packet;
    FRCCommonControlData unpack;

    memset(&unpack, 0, sizeof unpack);
    unpack.packetIndex		= ntohs(pkt->packetIndex);
    unpack.control		= pkt->control;
    unpack.dsDigitalIn		= pkt->dsDigitalIn;
    unpack.teamID		= ntohs(pkt->teamID);
    unpack.dsID_Alliance	= pkt->dsID_Alliance;
    unpack.dsID_Position	= pkt->dsID_Position;
#if 0
    printf("pkt %u ctrl 0x%02x dig in 0x%02x team %u position %c%c\n",
	   unpack.packetIndex, unpack.control,
	   unpack.dsDigitalIn, unpack.teamID,
	   unpack.dsID_Alliance, unpack.dsID_Position);
#endif
    unpack.stick0Axis1		= pkt->stick0Axis1;
    unpack.stick0Axis2		= pkt->stick0Axis2;
    unpack.stick0Axis3		= pkt->stick0Axis3;
    unpack.stick0Axis4		= pkt->stick0Axis4;
    unpack.stick0Axis5		= pkt->stick0Axis5;
    unpack.stick0Axis6		= pkt->stick0Axis6;
    unpack.stick0Buttons	= ntohs(pkt->stick0Buttons);
#if 0
    printf("stick0: %02x %02x %02x %02x %02x %02x %04x\n",
	    unpack.stick0Axis1, unpack.stick0Axis2, unpack.stick0Axis3,
	    unpack.stick0Axis4, unpack.stick0Axis5, unpack.stick0Axis6,
	    unpack.stick0Buttons);
#endif
    unpack.stick1Axis1		= pkt->stick1Axis1;
    unpack.stick1Axis2		= pkt->stick1Axis2;
    unpack.stick1Axis3		= pkt->stick1Axis3;
    unpack.stick1Axis4		= pkt->stick1Axis4;
    unpack.stick1Axis5		= pkt->stick1Axis5;
    unpack.stick1Axis6		= pkt->stick1Axis6;
    unpack.stick1Buttons	= ntohs(pkt->stick1Buttons);
#if 0
    printf("stick1: %02x %02x %02x %02x %02x %02x %04x\n",
	    unpack.stick1Axis1, unpack.stick1Axis2, unpack.stick1Axis3,
	    unpack.stick1Axis4, unpack.stick1Axis5, unpack.stick1Axis6,
	    unpack.stick1Buttons);
#endif
    unpack.stick2Axis1		= pkt->stick2Axis1;
    unpack.stick2Axis2		= pkt->stick2Axis2;
    unpack.stick2Axis3		= pkt->stick2Axis3;
    unpack.stick2Axis4		= pkt->stick2Axis4;
    unpack.stick2Axis5		= pkt->stick2Axis5;
    unpack.stick2Axis6		= pkt->stick2Axis6;
    unpack.stick2Buttons	= ntohs(pkt->stick2Buttons);
#if 0
    printf("stick2: %02x %02x %02x %02x %02x %02x %04x\n",
	    unpack.stick2Axis1, unpack.stick2Axis2, unpack.stick2Axis3,
	    unpack.stick2Axis4, unpack.stick2Axis5, unpack.stick2Axis6,
	    unpack.stick2Buttons);
#endif
    unpack.stick3Axis1		= pkt->stick3Axis1;
    unpack.stick3Axis2		= pkt->stick3Axis2;
    unpack.stick3Axis3		= pkt->stick3Axis3;
    unpack.stick3Axis4		= pkt->stick3Axis4;
    unpack.stick3Axis5		= pkt->stick3Axis5;
    unpack.stick3Axis6		= pkt->stick3Axis6;
    unpack.stick3Buttons	= ntohs(pkt->stick3Buttons);
#if 0
    printf("stick3: %02x %02x %02x %02x %02x %02x %04x\n",
	    unpack.stick3Axis1, unpack.stick3Axis2, unpack.stick3Axis3,
	    unpack.stick3Axis4, unpack.stick3Axis5, unpack.stick3Axis6,
	    unpack.stick3Buttons);
#endif
    unpack.analog1		= ntohs(pkt->analog1);
    unpack.analog2		= ntohs(pkt->analog2);
    unpack.analog3		= ntohs(pkt->analog3);
    unpack.analog4		= ntohs(pkt->analog4);
#if 0
    printf("analog: %04x %04x %04x %04x\n",
	    unpack.analog1, unpack.analog2, unpack.analog3, unpack.analog4);
#endif

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

    unpack.cRIOChecksum		= ntohll(pkt->cRIOChecksum);
#if 0
    printf("cRIO checksum: %08lx%08lx\n",
	(unsigned long)(unpack.cRIOChecksum >> 32),
	(unsigned long)(unpack.cRIOChecksum));
#endif
    unpack.FPGAChecksum0	= ntohl(pkt->FPGAChecksum0);
    unpack.FPGAChecksum1	= ntohl(pkt->FPGAChecksum1);
    unpack.FPGAChecksum2	= ntohl(pkt->FPGAChecksum2);
    unpack.FPGAChecksum3	= ntohl(pkt->FPGAChecksum3);
#if 0
    printf("FPGA checksums: %08lx %08lx %08lx %08lx\n",
	    unpack.FPGAChecksum0, unpack.FPGAChecksum1,
	    unpack.FPGAChecksum2, unpack.FPGAChecksum3);
#endif
    memcpy(unpack.versionData, pkt->versionData, 8);

    {
	// begin critical section
	Synchronized sync(m_dataMutex);

	// copy common control data
	memcpy(&m_recvData, &unpack, sizeof m_recvData);

	// save sender's address for reply
	memcpy(&m_dsaddr, &sender, sizeof m_dsaddr);

	// tell DriverStation object that new data is available
	// semFlush will awaken any task that's waiting to take the semaphore
	// without actually giving them the semaphore (so their semTake will
	// fail, but they'll ignore that).
	semFlush(m_dataAvailable);

	// end critical section
    }

    return OK;
}

int getCommonControlData( FRCCommonControlData *data, int wait_ms )
{
    // This assumes that wait_ms is always WAIT_FOREVER so we can
    // use a Synchronized object and ignore the timeout.
    if (!netCommObj) {
	fprintf(stderr, "%s: network communications task not initialized\n",
		__FUNCTION__);
	abort();
    }
    Synchronized sync(netCommObj->m_dataMutex);
    memcpy(data, &netCommObj->m_recvData, sizeof (FRCCommonControlData));
    return OK;
}

extern "C" {

void FRC_NetworkCommunication_observeUserProgramStarting()
{
    // printf("FRC_NetworkCommunication: user program is Starting\n");
    // this should make the driver station switch to disabled mode!
}

void FRC_NetworkCommunication_observeUserProgramDisabled()
{
    // printf("FRC_NetworkCommunication: user program is in Disabled mode\n");
}

void FRC_NetworkCommunication_observeUserProgramAutonomous()
{
    // printf("FRC_NetworkCommunication: user program is in Autonomous mode\n");
}

void FRC_NetworkCommunication_observeUserProgramTeleop()
{
    // printf("FRC_NetworkCommunication: user program is in Teleop mode\n");
}

int getDynamicControlData( UINT8 type, char *dynamicData, INT32 maxLength,
			   int wait_ms )
{
    return ERROR;
}

int overrideIOConfig( const char *ioConfig, int wait_ms )
{
    return OK;
}

int setStatusData(float battery, UINT8 dsDigitalOut, UINT8 updateNumber,
		  const char *userDataHigh, int userDataHighLength,
		  const char *userDataLow, int userDataLowLength, int wait_ms)
{
    return OK;
}

int setErrorData(const char *errors, int errorsLength, int wait_ms)
{
    return OK;
}

}; // extern "C"

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

