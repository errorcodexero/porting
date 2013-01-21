// stubs for NI cRIO FPGA interface
#include "stubs.h"
#include "ChipObject.h"
#include "NetworkCommunication/LoadOut.h"
#include "Synchronized.h"

bool nLoadOut::getModulePresence(
	nLoadOut::tModuleType moduleType,
	UINT8 moduleNumber
    )
{
    bool present = (moduleNumber == 0);
#ifdef DEBUG
    printf("%s: module %u is%s type %u\n",
    	   __FUNCTION__, moduleNumber, present ? "" : " not",
	   (UINT8)moduleType);
#endif
    return present;
}

///////////////////////////////////////////////////////////////////////////////
//
// ChipObject tSystemInterface
//
///////////////////////////////////////////////////////////////////////////////

namespace nFPGA {

class stubSystemInterface : public tSystemInterface
{
private:
    uint32_t m_signature[4];

public:
    stubSystemInterface( const uint32_t guid[] ) : tSystemInterface() {
	memcpy(m_signature, guid, sizeof m_signature);
    }
    virtual ~stubSystemInterface() {}
    virtual const uint16_t getExpectedFPGAVersion() {
	return 1;
    }
    virtual const uint32_t getExpectedFPGARevision() {
	return 1;
    }
    virtual const uint32_t * const getExpectedFPGASignature() {
	return m_signature;
    }
    virtual void getHardwareFpgaSignature(
	    uint32_t *guid_ptr,
	    tRioStatusCode *status
	)
    {
	memcpy(guid_ptr, m_signature, sizeof m_signature);
	*status = 0;
    }
    virtual uint32_t getLVHandle(tRioStatusCode *status) {
	*status = 0;
	return 0;
    }
    virtual uint32_t getHandle() {
	return 0;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(stubSystemInterface);
};

}; // namespace nFPGA

///////////////////////////////////////////////////////////////////////////////
//
// ChipObject tGlobal
//
///////////////////////////////////////////////////////////////////////////////

namespace nFPGA {
namespace nFRC_2012_1_6_4 {

class stubGlobal : public tGlobal
{
private:
    tSystemInterface* m_systemInterface;
    bool m_led;

public:
    stubGlobal( const uint32_t guid[] ) : tGlobal() {
	m_systemInterface = new stubSystemInterface(guid);
	m_led = false;
    }
    virtual ~stubGlobal() {
	delete m_systemInterface;
    }
    virtual tSystemInterface* getSystemInterface() {
	return m_systemInterface;
    }
    virtual unsigned short readVersion(tRioStatusCode *status) {
	*status = 0;
	return 1;
    }
    virtual void writeFPGA_LED(bool value, tRioStatusCode *status) {
	printf("%s: cRIO FPGA LED is now %s\n",
	  __FUNCTION__, m_led ? "ON" : "OFF");
	m_led = value;
	*status = 0;
    }
    virtual bool readFPGA_LED(tRioStatusCode *status) {
	*status = 0;
	return m_led;
    }
    virtual unsigned int readLocalTime(tRioStatusCode *status) {
	*status = 0;
	return time(0) * 1000000U;
    }
    virtual unsigned int readRevision(tRioStatusCode *status) {
	*status = 0;
	return 0x01051300;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(stubGlobal);
};


tGlobal* tGlobal::create( tRioStatusCode *status )
{
    uint32_t guid[4];
    snprintf((char *)guid, sizeof guid, "global");
    *status = 0;
    return new stubGlobal(guid);
}

}; // namespace nFRC_2012_1_6_4
}; // namespace nFPGA

///////////////////////////////////////////////////////////////////////////////
//
// ChipObject tAccumulator
//
///////////////////////////////////////////////////////////////////////////////

namespace nFPGA {
namespace nFRC_2012_1_6_4 {

class stubAccumulator : public tAccumulator
{
private:
    unsigned char m_index;
    tSystemInterface *m_systemInterface;
    signed int m_center;
    signed int m_deadband;

public:
    stubAccumulator( unsigned char sys_index, const uint32_t guid[] ) : tAccumulator() {
	m_index = sys_index;
	m_systemInterface = new stubSystemInterface(guid);
	m_center = 0;
	m_deadband = 0;
    }
    virtual ~stubAccumulator() {
	delete m_systemInterface;
    }
    virtual tSystemInterface* getSystemInterface() {
	return m_systemInterface;
    }
    virtual unsigned char getSystemIndex() {
	return m_index;
    }
    virtual tAccumulator::tOutput readOutput(tRioStatusCode *status) {
	tAccumulator::tOutput output;
	memset(&output, 0, sizeof output);
	*status = 0;
	return output;
    }
    virtual signed long long readOutput_Value(tRioStatusCode *status) {
	signed long long value = 0;
	*status = 0;
	return value;
    }
    virtual unsigned int readOutput_Count(tRioStatusCode *status) {
	unsigned int count = 0;
	*status = 0;
	return count;
    }
    virtual void writeCenter(signed int value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA accumulator %u center %d\n", m_index, value);
#endif
	if (m_center != value) {
	    printf("FPGA accumulator %u center was %d changed to %d\n",
		m_index, m_center, value);
	}
	m_center = value;
	*status = 0;
    }
    virtual signed int readCenter(tRioStatusCode *status) {
	*status = 0;
	return m_center;
    }
    virtual void strobeReset(tRioStatusCode *status) {
// #ifdef DEBUG
	printf("FPGA accumulator %u strobe reset\n", m_index);
// #endif
	*status = 0;
    }
    virtual void writeDeadband(signed int value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA accumulator %u deadband %d\n", m_index, value);
#endif
	if (m_deadband != value) {
	    printf("FPGA accumulator %u deadband was %d changed to %d\n",
		m_index, m_deadband, value);
	}
	m_deadband = value;
	*status = 0;
    }
    virtual signed int readDeadband(tRioStatusCode *status) {
	*status = 0;
	return m_deadband;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(stubAccumulator);
};


tAccumulator* tAccumulator::create(
	unsigned char sys_index,
	tRioStatusCode *status
    )
{
    uint32_t guid[4];
    snprintf((char *)guid, sizeof guid, "accumulator %u", sys_index);
    *status = 0;
    return new stubAccumulator(sys_index, guid);
}

}; // namespace nFRC_2012_1_6_4
}; // namespace nFPGA

///////////////////////////////////////////////////////////////////////////////
//
// ChipObject tAI
//
///////////////////////////////////////////////////////////////////////////////

namespace nFPGA {
namespace nFRC_2012_1_6_4 {

class stubAI : public tAI
{
private:
    unsigned char m_index;
    tSystemInterface *m_systemInterface;
    tAI::tConfig m_config;
    unsigned char m_scanList[tAI::kNumScanListElements];
    unsigned int m_loopTiming;
    unsigned char m_averageBits[tAI::kNumAverageBitsElements];
    unsigned char m_oversampleBits[tAI::kNumOversampleBitsElements];
    tAI::tReadSelect m_readSelect;

public:
    stubAI( unsigned char sys_index, const uint32_t guid[] ) : tAI() {
	m_index = sys_index;
	m_systemInterface = new stubSystemInterface(guid);
	m_config.value = 0;
	memset(m_scanList, 0, sizeof m_scanList);
	m_loopTiming = 0;
	memset(m_averageBits, 0, sizeof m_averageBits);
	memset(m_oversampleBits, 0, sizeof m_oversampleBits);
	m_readSelect.value = 0;
    }
    virtual ~stubAI() {
	delete m_systemInterface;
    }
    virtual tSystemInterface* getSystemInterface() {
	return m_systemInterface;
    }
    virtual unsigned char getSystemIndex() {
	return m_index;
    }
    virtual void writeConfig(tAI::tConfig value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA analog in %u config 0x%02x (ScanSize %u ConvertRate %u)\n",
	  m_index, value.value, value.ScanSize, value.ConvertRate);
#endif
	if (m_config.value != value.value) {
	    printf("FPGA analog in %u config was 0x%02x changed to 0x%02x"
	    	   " (ScanSize %u ConvertRate %u)\n",
		m_index, m_config.value, value.value,
		value.ScanSize, value.ConvertRate);
	}
	m_config = value;
	*status = 0;
    }
    virtual void writeConfig_ScanSize( unsigned char value, tRioStatusCode *status) {
	tAI::tConfig new_config = m_config;
	new_config.ScanSize = value;
#ifdef DEBUG
	printf("FPGA analog in %u config 0x%02x (ScanSize %u ConvertRate %u)\n",
	  m_index, new_config.value,
	  new_config.ScanSize, new_config.ConvertRate);
#endif
	if (m_config.value != new_config.value) {
	    printf("FPGA analog in %u config was 0x%02x changed to 0x%02x"
	    	   " (ScanSize %u ConvertRate %u)\n",
		m_index, m_config.value, new_config.value,
		new_config.ScanSize, new_config.ConvertRate);
	}
	m_config = new_config;
	*status = 0;
    }
    virtual void writeConfig_ConvertRate(unsigned int value, tRioStatusCode *status) {
	tAI::tConfig new_config = m_config;
	new_config.ConvertRate = value;
#ifdef DEBUG
	printf("FPGA analog in %u config 0x%02x (ScanSize %u ConvertRate %u)\n",
	  m_index, new_config.value, new_config.ScanSize,
	  new_config.ConvertRate);
#endif
	if (m_config.value != new_config.value) {
	    printf("FPGA analog in %u config was 0x%02x changed to 0x%02x"
	    	   " (ScanSize %u ConvertRate %u)\n",
		m_index, m_config.value, new_config.value,
		new_config.ScanSize, new_config.ConvertRate);
	}
	m_config = new_config;
	*status = 0;
    }
    virtual tAI::tConfig readConfig(tRioStatusCode *status) {
	*status = 0;
	return m_config;
    }
    virtual unsigned char readConfig_ScanSize(tRioStatusCode *status) {
	*status = 0;
	return m_config.ScanSize;
    }
    virtual unsigned int readConfig_ConvertRate(tRioStatusCode *status) {
	*status = 0;
	return m_config.ConvertRate;
    }
    virtual void writeScanList(unsigned char bitfield_index, unsigned char value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA analog in %u channel %u scan list 0x%02x\n",
	  m_index, bitfield_index, value);
#endif
	if (bitfield_index >= tAI::kNumScanListElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
	if (m_scanList[bitfield_index] != value) {
	    printf("FPGA analog in %u channel %u scan list was 0x%02x"
	    	   " changed to 0x%02x\n",
		m_index, bitfield_index, m_scanList[bitfield_index], value);
	}
	m_scanList[bitfield_index] = value;
	*status = 0;
    }
    virtual unsigned char readScanList(unsigned char bitfield_index, tRioStatusCode *status) {
	if (bitfield_index >= tAI::kNumScanListElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_scanList[bitfield_index];
    }
    virtual unsigned int readLoopTiming(tRioStatusCode *status) {
	return m_loopTiming;
    }
    virtual void writeAverageBits(unsigned char bitfield_index, unsigned char value, tRioStatusCode *status) {
	if (bitfield_index >= tAI::kNumAverageBitsElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
#ifdef DEBUG
	printf("FPGA analog in %u channel %u average bits %u\n",
	  m_index, bitfield_index, value);
#endif
	if (m_averageBits[bitfield_index] != value) {
	    printf("FPGA analog in %u channel %u average bits was %u"
	    	   " changed to %u\n",
		m_index, bitfield_index, m_averageBits[bitfield_index], value);
	}
	m_averageBits[bitfield_index] = value;
	*status = 0;
    }
    virtual unsigned char readAverageBits(unsigned char bitfield_index, tRioStatusCode *status) {
	if (bitfield_index >= tAI::kNumAverageBitsElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_averageBits[bitfield_index];
    }
    virtual void writeOversampleBits(unsigned char bitfield_index, unsigned char value, tRioStatusCode *status) {
	if (bitfield_index >= tAI::kNumOversampleBitsElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
#ifdef DEBUG
	printf("FPGA analog in %u channel %u oversample bits %u\n",
	  m_index, bitfield_index, value);
#endif
	if (m_oversampleBits[bitfield_index] != value) {
	    printf("FPGA analog in %u channel %u oversample bits was %u"
	    	   " changed to %u\n",
		m_index, bitfield_index, m_oversampleBits[bitfield_index],
		value);
	}
	m_oversampleBits[bitfield_index] = value;
	*status = 0;
    }
    virtual unsigned char readOversampleBits(unsigned char bitfield_index, tRioStatusCode *status) {
	if (bitfield_index >= tAI::kNumOversampleBitsElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_oversampleBits[bitfield_index];
    }
    virtual void writeReadSelect(tAI::tReadSelect value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA analog in %u readSelect 0x%02x"
	       " (Module %u Channel %u Averaged %c)\n",
	  m_index, m_readSelect.value, m_readSelect.Module,
	  m_readSelect.Channel, m_readSelect.Averaged ? 't' : 'f');
#endif
	if (m_readSelect.value != value.value) {
	    printf("FPGA analog in %u readSelect was 0x%02x changed to 0x%02x"
	    	   " (Module %u Channel %u Averaged %c)\n",
	      m_index, m_readSelect.value, value.value,
	      value.Module, value.Channel, value.Averaged ? 't' : 'f');
	}
	m_readSelect = value;
	*status = 0;
    }
    virtual void writeReadSelect_Module(unsigned char value, tRioStatusCode *status) {
	tAI::tReadSelect new_readSelect = m_readSelect;
	new_readSelect.Module = value;
#ifdef DEBUG
	printf("FPGA analog in %u readSelect 0x%02x"
	       " (Module %u Channel %u Averaged %c)\n",
	  m_index, new_readSelect.value, new_readSelect.Module,
	  new_readSelect.Channel, new_readSelect.Averaged ? 't' : 'f');
#endif
	if (m_readSelect.value != new_readSelect.value) {
	    printf("FPGA analog in %u readSelect was 0x%02x changed to 0x%02x"
	    	   " (Module %u Channel %u Averaged %c)\n",
	  m_index, m_readSelect.value, new_readSelect.value,
	  new_readSelect.Module, new_readSelect.Channel,
	  new_readSelect.Averaged ? 't' : 'f');
	}
	m_readSelect = new_readSelect;
	*status = 0;
    }
    virtual void writeReadSelect_Channel(unsigned char value, tRioStatusCode *status) {
	tAI::tReadSelect new_readSelect = m_readSelect;
	new_readSelect.Channel = value;
#ifdef DEBUG
	printf("FPGA analog in %u readSelect 0x%02x"
	       " (Module %u Channel %u Averaged %c)\n",
	  m_index, new_readSelect.value, new_readSelect.Module,
	  new_readSelect.Channel, new_readSelect.Averaged ? 't' : 'f');
#endif
	if (m_readSelect.value != new_readSelect.value) {
	    printf("FPGA analog in %u readSelect was 0x%02x changed to 0x%02x"
	    	   " (Module %u Channel %u Averaged %c)\n",
	  m_index, m_readSelect.value, new_readSelect.value,
	  new_readSelect.Module, new_readSelect.Channel,
	  new_readSelect.Averaged ? 't' : 'f');
	}
	m_readSelect = new_readSelect;
	*status = 0;
    }
    virtual void writeReadSelect_Averaged(bool value, tRioStatusCode *status) {
	tAI::tReadSelect new_readSelect = m_readSelect;
	new_readSelect.Averaged = value;
#ifdef DEBUG
	printf("FPGA analog in %u readSelect 0x%02x"
	       " (Module %u Channel %u Averaged %c)\n",
	  m_index, new_readSelect.value, new_readSelect.Module,
	  new_readSelect.Channel, new_readSelect.Averaged ? 't' : 'f');
#endif
	if (m_readSelect.value != new_readSelect.value) {
	    printf("FPGA analog in %u readSelect was 0x%02x changed to 0x%02x"
	    	   " (Module %u Channel %u Averaged %c)\n",
	  m_index, m_readSelect.value, new_readSelect.value,
	  new_readSelect.Module, new_readSelect.Channel,
	  new_readSelect.Averaged ? 't' : 'f');
	}
	m_readSelect = new_readSelect;
	*status = 0;
    }
    virtual tAI::tReadSelect readReadSelect(tRioStatusCode *status) {
	*status = 0;
	return m_readSelect;
    }
    virtual unsigned char readReadSelect_Module(tRioStatusCode *status) {
	*status = 0;
	return m_readSelect.Module;
    }
    virtual unsigned char readReadSelect_Channel(tRioStatusCode *status) {
	*status = 0;
	return m_readSelect.Channel;
    }
    virtual bool readReadSelect_Averaged(tRioStatusCode *status) {
	*status = 0;
	return m_readSelect.Averaged;
    }
    virtual signed int readOutput(tRioStatusCode *status) {
	*status = 0;
	if (m_readSelect.Module == 0 && m_readSelect.Channel == 7) {
	    // fake reading the battery voltage
	    return (int)((12.34 * 1000. / 1680.) * 2048 / 10.0);
	}
	return 0;
    }
    virtual void strobeLatchOutput(tRioStatusCode *status) {
	printf("FPGA analog in %u strobe latch output\n", m_index);
	*status = 0;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(stubAI);
};


tAI* tAI::create(
	unsigned char sys_index,
	tRioStatusCode *status
    )
{
    uint32_t guid[4];
    snprintf((char *)guid, sizeof guid, "ai %u", sys_index);
    *status = 0;
    return new stubAI(sys_index, guid);
}

}; // namespace nFRC_2012_1_6_4
}; // namespace nFPGA

///////////////////////////////////////////////////////////////////////////////
//
// ChipObject tDIO
//
///////////////////////////////////////////////////////////////////////////////

namespace nFPGA {
namespace nFRC_2012_1_6_4 {

class stubDIO : public tDIO
{
private:
    unsigned char m_index;
    tSystemInterface *m_systemInterface;
    unsigned int m_i2cDataToSend;
    unsigned short m_loopTiming;
    unsigned int m_DO;
    unsigned char m_filterSelect[kNumFilterSelectElements];
    unsigned char m_filterPeriod[kNumFilterPeriodElements];
    unsigned short m_outputEnable;
    tDIO::tSlowValue m_slowValue;
    unsigned short m_pulse;
    unsigned char m_pwmPeriodScale[tDIO::kNumPWMPeriodScaleElements];
    unsigned char m_pwmDutyCycle[tDIO::kNumDO_PWMDutyCycleElements];
    bool m_bfl;
    tDIO::tDO_PWMConfig m_doPWMConfig;
    unsigned char m_pulseLength;
    tDIO::tI2CConfig m_i2cConfig;
    tDIO::tPWMConfig m_pwmConfig;
    unsigned char m_pwmValueRegisters[kNumPWMValueRegisters];

public:
    stubDIO( unsigned char sys_index, const uint32_t guid[] ) : tDIO() {
	m_index = sys_index;
	m_systemInterface = new stubSystemInterface(guid);
	m_i2cDataToSend = 0;
	m_loopTiming = 0;
	m_DO = 0;
	memset(m_filterSelect, 0, sizeof m_filterSelect);
	memset(m_filterPeriod, 0, sizeof m_filterPeriod);
	m_outputEnable = 0;
	m_slowValue.value = 0;
	m_pulse = 0;
	memset(m_pwmPeriodScale, 0, sizeof m_pwmPeriodScale);
	memset(m_pwmDutyCycle, 0, sizeof m_pwmDutyCycle);
	m_bfl = false;
	m_doPWMConfig.value = 0;
	m_pulseLength = 0;
	m_i2cConfig.value = 0;
	m_pwmConfig.value = 0;
	memset(m_pwmValueRegisters, 0, sizeof m_pwmValueRegisters);
    }
    virtual ~stubDIO() {
	delete m_systemInterface;
    }
    virtual tSystemInterface* getSystemInterface() {
	return m_systemInterface;
    }
    virtual unsigned char getSystemIndex() {
	return m_index;
    }

    virtual void writeI2CDataToSend(unsigned int value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA digital I/O %u write i2c data %04x\n", m_index, value);
#endif
	m_i2cDataToSend = value;
	*status = 0;
    }
    virtual unsigned int readI2CDataToSend(tRioStatusCode *status) {
	*status = 0;
	return m_i2cDataToSend;
    }

    virtual void writeDO(unsigned short value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA digital I/O %u digital out %04x\n", m_index, value);
#endif
	m_DO = value;
	*status = 0;
    }
    virtual unsigned short readDO(tRioStatusCode *status) {
	*status = 0;
	return m_DO;
    }

    virtual void writeFilterSelect(unsigned char bitfield_index, unsigned char value, tRioStatusCode *status) {
	if (bitfield_index >= tDIO::kNumFilterSelectElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
#ifdef DEBUG
	printf("FPGA digital I/O %u channel %u filter select 0x%02x\n",
	  m_index, bitfield_index, value);
#endif
	m_filterSelect[bitfield_index] = value;
	*status = 0;
    }
    virtual unsigned char readFilterSelect(unsigned char bitfield_index, tRioStatusCode *status) {
	if (bitfield_index >= tDIO::kNumFilterSelectElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_filterSelect[bitfield_index];
    }

    virtual void writeFilterPeriod(unsigned char bitfield_index, unsigned char value, tRioStatusCode *status) {
	if (bitfield_index >= tDIO::kNumFilterPeriodElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
#ifdef DEBUG
	printf("FPGA digital I/O %u channel %u filter period 0x%02x\n",
	  m_index, bitfield_index, value);
#endif
	m_filterPeriod[bitfield_index] = value;
	*status = 0;
    }
    virtual unsigned char readFilterPeriod(unsigned char bitfield_index, tRioStatusCode *status) {
	if (bitfield_index >= tDIO::kNumFilterPeriodElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_filterPeriod[bitfield_index];
    }

    virtual void writeOutputEnable(unsigned short value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA digital I/O %u output enable %04x\n", m_index, value);
#endif
	m_outputEnable = value;
	*status = 0;
    }
    virtual unsigned short readOutputEnable(tRioStatusCode *status) {
	*status = 0;
	return m_outputEnable;
    }

    virtual void writeSlowValue(tDIO::tSlowValue value, tRioStatusCode *status) {
	m_slowValue = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u slowValue %u (RelayFwd 0x%02x RelayRev 0x%02x I2CHeader 0x%x)\n",
	  m_index, m_slowValue.value, m_slowValue.RelayFwd,
	  m_slowValue.RelayRev, m_slowValue.I2CHeader);
#endif
	*status = 0;
    }
    virtual void writeSlowValue_RelayFwd(unsigned char value, tRioStatusCode *status) {
	m_slowValue.RelayFwd = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u slowValue %u (RelayFwd 0x%02x RelayRev 0x%02x I2CHeader 0x%x)\n",
	  m_index, m_slowValue.value, m_slowValue.RelayFwd,
	  m_slowValue.RelayRev, m_slowValue.I2CHeader);
#endif
	*status = 0;
    }
    virtual void writeSlowValue_RelayRev(unsigned char value, tRioStatusCode *status) {
	m_slowValue.RelayRev = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u slowValue %u (RelayFwd 0x%02x RelayRev 0x%02x I2CHeader 0x%x)\n",
	  m_index, m_slowValue.value, m_slowValue.RelayFwd,
	  m_slowValue.RelayRev, m_slowValue.I2CHeader);
#endif
	*status = 0;
    }
    virtual void writeSlowValue_I2CHeader(unsigned char value, tRioStatusCode *status) {
	m_slowValue.I2CHeader = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u slowValue %u (RelayFwd 0x%02x RelayRev 0x%02x I2CHeader 0x%x)\n",
	  m_index, m_slowValue.value, m_slowValue.RelayFwd,
	  m_slowValue.RelayRev, m_slowValue.I2CHeader);
#endif
	*status = 0;
    }
    virtual tDIO::tSlowValue readSlowValue(tRioStatusCode *status) {
	*status = 0;
	return m_slowValue;
    }
    virtual unsigned char readSlowValue_RelayFwd(tRioStatusCode *status) {
	*status = 0;
	return m_slowValue.RelayFwd;
    }
    virtual unsigned char readSlowValue_RelayRev(tRioStatusCode *status) {
	*status = 0;
	return m_slowValue.RelayRev;
    }
    virtual unsigned char readSlowValue_I2CHeader(tRioStatusCode *status) {
	*status = 0;
	return m_slowValue.I2CHeader;
    }

    virtual tI2CStatus readI2CStatus(tRioStatusCode *status) {
	tI2CStatus result;
	result.Transaction = 1;
	result.Done = 1;
	result.Aborted = 0;
	result.DataReceivedHigh = 0;
	*status = 0;
	return result;
    }
    virtual unsigned char readI2CStatus_Transaction(tRioStatusCode *status) {
	*status = 0;
	return 1;
    }
    virtual bool readI2CStatus_Done(tRioStatusCode *status) {
	*status = 0;
	return true;
    }
    virtual bool readI2CStatus_Aborted(tRioStatusCode *status) {
	*status = 0;
	return false;
    }
    virtual unsigned int readI2CStatus_DataReceivedHigh(tRioStatusCode *status) {
	*status = 0;
	return 0;
    }

    virtual unsigned int readI2CDataReceived(tRioStatusCode *status) {
	*status = 0;
	return 0;
    }

    virtual unsigned short readDI(tRioStatusCode *status) {
	*status = 0;
	return 0;
    }

    virtual void writePulse(unsigned short value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA digital I/O %u pulse %04x\n", m_index, value);
#endif
	m_pulse = value;
	*status = 0;
    }
    virtual unsigned short readPulse(tRioStatusCode *status) {
	*status = 0;
	return m_pulse;
    }

    virtual void writePWMPeriodScale(unsigned char bitfield_index, unsigned char value, tRioStatusCode *status) {
	if (bitfield_index >= tDIO::kNumPWMPeriodScaleElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
#ifdef DEBUG
	printf("FPGA digital I/O %u PWM %u period scale %u\n",
	  m_index, bitfield_index, value);
#endif
	m_pwmPeriodScale[bitfield_index] = value;
	*status = 0;
    }
    virtual unsigned char readPWMPeriodScale(unsigned char bitfield_index, tRioStatusCode *status) {
	if (bitfield_index >= tDIO::kNumPWMPeriodScaleElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_pwmPeriodScale[bitfield_index];
    }

    virtual void writeDO_PWMDutyCycle(unsigned char bitfield_index, unsigned char value, tRioStatusCode *status) {
	if (bitfield_index >= tDIO::kNumDO_PWMDutyCycleElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
#ifdef DEBUG
	printf("FPGA digital I/O %u PWM %u duty cycle %u\n",
	  m_index, bitfield_index, value);
#endif
	m_pwmDutyCycle[bitfield_index] = value;
	*status = 0;
    }
    virtual unsigned char readDO_PWMDutyCycle(unsigned char bitfield_index, tRioStatusCode *status) {
	if (bitfield_index >= tDIO::kNumDO_PWMDutyCycleElements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_pwmDutyCycle[bitfield_index];
    }

    virtual void writeBFL(bool value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA %u digital I/O BFL %c\n",
	    m_index, value ? 't' : 'f');
#endif
	m_bfl = value;
	*status = 0;
    }
    virtual bool readBFL(tRioStatusCode *status) {
	*status = 0;
	return m_bfl;
    }

    virtual void strobeI2CStart(tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA %u strobe I2C start\n", m_index);
#endif
	*status = 0;
    }

    virtual void writeDO_PWMConfig(tDIO::tDO_PWMConfig value, tRioStatusCode *status) {
	m_doPWMConfig = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u DO_PWM config %u\n"
	       "(PeriodPower 0x%x OutputSelect 0x%x 0x%x 0x%x 0x%x)\n",
	  m_index, m_doPWMConfig.value, m_doPWMConfig.PeriodPower,
	  m_doPWMConfig.OutputSelect_0, m_doPWMConfig.OutputSelect_1,
	  m_doPWMConfig.OutputSelect_2, m_doPWMConfig.OutputSelect_3);
#endif
	*status = 0;
    }
    virtual void writeDO_PWMConfig_PeriodPower(unsigned char value, tRioStatusCode *status) {
	m_doPWMConfig.PeriodPower = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u DO_PWM config %u\n"
	       "(PeriodPower 0x%x OutputSelect 0x%x 0x%x 0x%x 0x%x)\n",
	  m_index, m_doPWMConfig.value, m_doPWMConfig.PeriodPower,
	  m_doPWMConfig.OutputSelect_0, m_doPWMConfig.OutputSelect_1,
	  m_doPWMConfig.OutputSelect_2, m_doPWMConfig.OutputSelect_3);
#endif
	*status = 0;
    }
    virtual void writeDO_PWMConfig_OutputSelect_0(unsigned char value, tRioStatusCode *status) {
	m_doPWMConfig.OutputSelect_0 = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u DO_PWM config %u\n"
	       "(PeriodPower 0x%x OutputSelect 0x%x 0x%x 0x%x 0x%x)\n",
	  m_index, m_doPWMConfig.value, m_doPWMConfig.PeriodPower,
	  m_doPWMConfig.OutputSelect_0, m_doPWMConfig.OutputSelect_1,
	  m_doPWMConfig.OutputSelect_2, m_doPWMConfig.OutputSelect_3);
#endif
	*status = 0;
    }
    virtual void writeDO_PWMConfig_OutputSelect_1(unsigned char value, tRioStatusCode *status) {
	m_doPWMConfig.OutputSelect_1 = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u DO_PWM config %u\n"
	       "(PeriodPower 0x%x OutputSelect 0x%x 0x%x 0x%x 0x%x)\n",
	  m_index, m_doPWMConfig.value, m_doPWMConfig.PeriodPower,
	  m_doPWMConfig.OutputSelect_0, m_doPWMConfig.OutputSelect_1,
	  m_doPWMConfig.OutputSelect_2, m_doPWMConfig.OutputSelect_3);
#endif
	*status = 0;
    }
    virtual void writeDO_PWMConfig_OutputSelect_2(unsigned char value, tRioStatusCode *status) {
	m_doPWMConfig.OutputSelect_2 = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u DO_PWM config %u\n"
	       "(PeriodPower 0x%x OutputSelect 0x%x 0x%x 0x%x 0x%x)\n",
	  m_index, m_doPWMConfig.value, m_doPWMConfig.PeriodPower,
	  m_doPWMConfig.OutputSelect_0, m_doPWMConfig.OutputSelect_1,
	  m_doPWMConfig.OutputSelect_2, m_doPWMConfig.OutputSelect_3);
#endif
	*status = 0;
    }
    virtual void writeDO_PWMConfig_OutputSelect_3(unsigned char value, tRioStatusCode *status) {
	m_doPWMConfig.OutputSelect_3 = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u DO_PWM config %u\n"
	       "(PeriodPower 0x%x OutputSelect 0x%x 0x%x 0x%x 0x%x)\n",
	  m_index, m_doPWMConfig.value, m_doPWMConfig.PeriodPower,
	  m_doPWMConfig.OutputSelect_0, m_doPWMConfig.OutputSelect_1,
	  m_doPWMConfig.OutputSelect_2, m_doPWMConfig.OutputSelect_3);
#endif
	*status = 0;
    }
    virtual tDIO::tDO_PWMConfig readDO_PWMConfig(tRioStatusCode *status) {
	*status = 0;
	return m_doPWMConfig;
    }
    virtual unsigned char readDO_PWMConfig_PeriodPower(tRioStatusCode *status) {
	*status = 0;
	return m_doPWMConfig.PeriodPower;
    }
    virtual unsigned char readDO_PWMConfig_OutputSelect_0(tRioStatusCode *status) {
	*status = 0;
	return m_doPWMConfig.OutputSelect_0;
    }
    virtual unsigned char readDO_PWMConfig_OutputSelect_1(tRioStatusCode *status) {
	*status = 0;
	return m_doPWMConfig.OutputSelect_1;
    }
    virtual unsigned char readDO_PWMConfig_OutputSelect_2(tRioStatusCode *status) {
	*status = 0;
	return m_doPWMConfig.OutputSelect_2;
    }
    virtual unsigned char readDO_PWMConfig_OutputSelect_3(tRioStatusCode *status) {
	*status = 0;
	return m_doPWMConfig.OutputSelect_3;
    }

    virtual void writePulseLength(unsigned char value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA %u pulse length %u\n", m_index, value);
#endif
	*status = 0;
	m_pulseLength = value;
    }
    virtual unsigned char readPulseLength(tRioStatusCode *status) {
	*status = 0;
	return m_pulseLength;
    }

    virtual void writeI2CConfig(tDIO::tI2CConfig value, tRioStatusCode *status) {
	m_i2cConfig = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u I2C config %u\n"
	       "(Address 0x%02x Read %u Write %u Data 0x%04x Bitwise %c)\n",
	  m_index, m_i2cConfig.value, m_i2cConfig.Address,
	  m_i2cConfig.BytesToRead, m_i2cConfig.BytesToWrite,
	  m_i2cConfig.DataToSendHigh,
	  m_i2cConfig.BitwiseHandshake ? 't' : 'f');
#endif
	*status = 0;
    }
    virtual void writeI2CConfig_Address(unsigned char value, tRioStatusCode *status) {
	m_i2cConfig.Address = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u I2C config %u\n"
	       "(Address 0x%02x Read %u Write %u Data 0x%04x Bitwise %c)\n",
	  m_index, m_i2cConfig.value, m_i2cConfig.Address,
	  m_i2cConfig.BytesToRead, m_i2cConfig.BytesToWrite,
	  m_i2cConfig.DataToSendHigh,
	  m_i2cConfig.BitwiseHandshake ? 't' : 'f');
#endif
	*status = 0;
    }
    virtual void writeI2CConfig_BytesToRead(unsigned char value, tRioStatusCode *status) {
	m_i2cConfig.BytesToRead = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u I2C config %u\n"
	       "(Address 0x%02x Read %u Write %u Data 0x%04x Bitwise %c)\n",
	  m_index, m_i2cConfig.value, m_i2cConfig.Address,
	  m_i2cConfig.BytesToRead, m_i2cConfig.BytesToWrite,
	  m_i2cConfig.DataToSendHigh,
	  m_i2cConfig.BitwiseHandshake ? 't' : 'f');
#endif
	*status = 0;
    }
    virtual void writeI2CConfig_BytesToWrite(unsigned char value, tRioStatusCode *status) {
	m_i2cConfig.BytesToWrite = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u I2C config %u\n"
	       "(Address 0x%02x Read %u Write %u Data 0x%04x Bitwise %c)\n",
	  m_index, m_i2cConfig.value, m_i2cConfig.Address,
	  m_i2cConfig.BytesToRead, m_i2cConfig.BytesToWrite,
	  m_i2cConfig.DataToSendHigh,
	  m_i2cConfig.BitwiseHandshake ? 't' : 'f');
#endif
	*status = 0;
    }
    virtual void writeI2CConfig_DataToSendHigh(unsigned short value, tRioStatusCode *status) {
	m_i2cConfig.DataToSendHigh = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u I2C config %u\n"
	       "(Address 0x%02x Read %u Write %u Data 0x%04x Bitwise %c)\n",
	  m_index, m_i2cConfig.value, m_i2cConfig.Address,
	  m_i2cConfig.BytesToRead, m_i2cConfig.BytesToWrite,
	  m_i2cConfig.DataToSendHigh,
	  m_i2cConfig.BitwiseHandshake ? 't' : 'f');
#endif
	*status = 0;
    }
    virtual void writeI2CConfig_BitwiseHandshake(bool value, tRioStatusCode *status) {
	m_i2cConfig.BitwiseHandshake = value;
#ifdef DEBUG
	printf("FPGA digital I/O %u I2C config %u\n"
	       "(Address 0x%02x Read %u Write %u Data 0x%04x Bitwise %c)\n",
	  m_index, m_i2cConfig.value, m_i2cConfig.Address,
	  m_i2cConfig.BytesToRead, m_i2cConfig.BytesToWrite,
	  m_i2cConfig.DataToSendHigh,
	  m_i2cConfig.BitwiseHandshake ? 't' : 'f');
#endif
	*status = 0;
    }
    virtual tDIO::tI2CConfig readI2CConfig(tRioStatusCode *status) {
	*status = 0;
	return m_i2cConfig;
    }
    virtual unsigned char readI2CConfig_Address(tRioStatusCode *status) {
	*status = 0;
	return m_i2cConfig.Address;
    }
    virtual unsigned char readI2CConfig_BytesToRead(tRioStatusCode *status) {
	*status = 0;
	return m_i2cConfig.BytesToRead;
    }
    virtual unsigned char readI2CConfig_BytesToWrite(tRioStatusCode *status) {
	*status = 0;
	return m_i2cConfig.BytesToWrite;
    }
    virtual unsigned short readI2CConfig_DataToSendHigh(tRioStatusCode *status) {
	*status = 0;
	return m_i2cConfig.DataToSendHigh;
    }
    virtual bool readI2CConfig_BitwiseHandshake(tRioStatusCode *status) {
	*status = 0;
	return m_i2cConfig.BitwiseHandshake;
    }

    virtual unsigned short readLoopTiming(tRioStatusCode *status) {
	return m_loopTiming;
    }

    virtual void writePWMConfig(tPWMConfig value, tRioStatusCode *status) {
	m_pwmConfig = value;
#ifdef DEBUG
	printf("FPGA %u PWM config 0x%08x (Period %u MinHigh %u)\n",
	    m_index, m_pwmConfig.value, m_pwmConfig.Period,
	    m_pwmConfig.MinHigh);
#endif
	*status = 0;
    }
    virtual void writePWMConfig_Period(unsigned short value, tRioStatusCode *status) {
	m_pwmConfig.Period = value;
#ifdef DEBUG
	printf("FPGA %u PWM config 0x%08x (Period %u MinHigh %u)\n",
	    m_index, m_pwmConfig.value, m_pwmConfig.Period,
	    m_pwmConfig.MinHigh);
#endif
	*status = 0;
    }
    virtual void writePWMConfig_MinHigh(unsigned short value, tRioStatusCode *status) {
	m_pwmConfig.MinHigh = value;
#ifdef DEBUG
	printf("FPGA %u PWM config 0x%08x (Period %u MinHigh %u)\n",
	    m_index, m_pwmConfig.value, m_pwmConfig.Period,
	    m_pwmConfig.MinHigh);
#endif
	*status = 0;
    }
    virtual tPWMConfig readPWMConfig(tRioStatusCode *status) {
	*status = 0;
	return m_pwmConfig;
    }
    virtual unsigned short readPWMConfig_Period(tRioStatusCode *status) {
	*status = 0;
	return m_pwmConfig.Period;
    }
    virtual unsigned short readPWMConfig_MinHigh(tRioStatusCode *status) {
	*status = 0;
	return m_pwmConfig.MinHigh;
    }

    virtual void writePWMValue(unsigned char reg_index, unsigned char value, tRioStatusCode *status) {
	if (reg_index >= tDIO::kNumPWMValueRegisters) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
#ifdef DEBUG
	printf("FPGA %u PWM %u value %u\n", m_index, reg_index, value);
#endif
	m_pwmValueRegisters[reg_index] = value;
	*status = 0;
    }
    virtual unsigned char readPWMValue(unsigned char reg_index, tRioStatusCode *status) {
	if (reg_index >= tDIO::kNumPWMValueRegisters) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_pwmValueRegisters[reg_index];
    }

private:
    DISALLOW_COPY_AND_ASSIGN(stubDIO);
};

tDIO* tDIO::create(
	unsigned char sys_index,
	tRioStatusCode *status
    )
{
    uint32_t guid[4];
    snprintf((char *)guid, sizeof guid, "dio %u", sys_index);
    *status = 0;
    return new stubDIO(sys_index, guid);
}

}; // namespace nFRC_2012_1_6_4
}; // namespace nFPGA

///////////////////////////////////////////////////////////////////////////////
//
// ChipObject tSolenoid
//
///////////////////////////////////////////////////////////////////////////////

namespace nFPGA {
namespace nFRC_2012_1_6_4 {

class stubSolenoid : public tSolenoid
{
private:
    unsigned char m_index;
    tSystemInterface *m_systemInterface;
    unsigned char m_value[tSolenoid::kNumDO7_0Elements];

public:
    stubSolenoid( unsigned char sys_index, const uint32_t guid[] ) : tSolenoid() {
	m_index = sys_index;
	m_systemInterface = new stubSystemInterface(guid);
    }
    virtual ~stubSolenoid() {
	delete m_systemInterface;
    }
    virtual tSystemInterface* getSystemInterface() {
	return m_systemInterface;
    }
    virtual unsigned char getSystemIndex() {
	return m_index;
    }

    virtual void writeDO7_0(unsigned char bitfield_index, unsigned char value, tRioStatusCode *status) {
	if (bitfield_index >= tSolenoid::kNumDO7_0Elements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return;
	}
#ifdef DEBUG
	printf("FPGA solenoid %u write DO7_0 %x\n", m_index, value);
#endif
	if (m_value[m_index] != value) {
	    printf("FPGA solenoid %u DO7_0 was %x changed to %x\n",
		m_index, m_value[m_index], value);
	}
	m_value[m_index] = value;
	*status = 0;
    }

    virtual unsigned char readDO7_0(unsigned char bitfield_index, tRioStatusCode *status) {
	if (bitfield_index >= tSolenoid::kNumDO7_0Elements) {
	    *status = NiFpga_Status_InvalidParameter;
	    return 0;
	}
	*status = 0;
	return m_value[m_index];
    }

private:
    DISALLOW_COPY_AND_ASSIGN(stubSolenoid);
};

tSolenoid* tSolenoid::create(
	tRioStatusCode *status
    )
{
    uint32_t guid[4];
    unsigned char sys_index = 0;
    snprintf((char *)guid, sizeof guid, "sol %u", sys_index);
    *status = 0;
    return new stubSolenoid(sys_index, guid);
}

}; // namespace nFRC_2012_1_6_4
}; // namespace nFPGA

///////////////////////////////////////////////////////////////////////////////
//
// ChipObject tWatchdog
//
///////////////////////////////////////////////////////////////////////////////

namespace nFPGA {
namespace nFRC_2012_1_6_4 {

class stubWatchdog : public tWatchdog
{
private:
    tSystemInterface *m_systemInterface;
    tWatchdog::tStatus m_status;
    bool m_immortal;
    unsigned int m_expiration;
    unsigned int m_timer;

public:
    stubWatchdog( const uint32_t guid[] ) : tWatchdog() {
	m_systemInterface = new stubSystemInterface(guid);
	m_status.value = 0;
	m_immortal = false;
	m_expiration = 0;
	m_timer = 0;
    }
    virtual ~stubWatchdog() {
	delete m_systemInterface;
    }
    virtual tSystemInterface* getSystemInterface() {
	return m_systemInterface;
    }

    virtual void writeStatus(tWatchdog::tStatus value, tRioStatusCode *status) {
	m_status = value;
#if 1
	printf("FPGA watchdog 0x%08x (SystemActive %c Alive %c\n"
	       "  SysDisableCount %u DisableCount %u)\n",
	  m_status.value, m_status.SystemActive ? 't' : 'f',
	  m_status.Alive ? 't' : 'f',
	  m_status.SysDisableCount,
	  m_status.DisableCount);
#endif
	*status = 0;
    }
    virtual void writeStatus_SystemActive(bool value, tRioStatusCode *status) {
	m_status.SystemActive = value;
#if 1
	printf("FPGA watchdog 0x%08x (SystemActive %c Alive %c\n"
	       "  SysDisableCount %u DisableCount %u)\n",
	  m_status.value, m_status.SystemActive ? 't' : 'f',
	  m_status.Alive ? 't' : 'f',
	  m_status.SysDisableCount,
	  m_status.DisableCount);
#endif
	*status = 0;
    }
    virtual void writeStatus_Alive(bool value, tRioStatusCode *status) {
	m_status.Alive = value;
#if 1
	printf("FPGA watchdog 0x%08x (SystemActive %c Alive %c\n"
	       "  SysDisableCount %u DisableCount %u)\n",
	  m_status.value, m_status.SystemActive ? 't' : 'f',
	  m_status.Alive ? 't' : 'f',
	  m_status.SysDisableCount,
	  m_status.DisableCount);
#endif
	*status = 0;
    }
    virtual void writeStatus_SysDisableCount(unsigned short value, tRioStatusCode *status) {
	m_status.SysDisableCount = value;
#if 1
	printf("FPGA watchdog 0x%08x (SystemActive %c Alive %c\n"
	       "  SysDisableCount %u DisableCount %u)\n",
	  m_status.value, m_status.SystemActive ? 't' : 'f',
	  m_status.Alive ? 't' : 'f',
	  m_status.SysDisableCount,
	  m_status.DisableCount);
#endif
	*status = 0;
    }
    virtual void writeStatus_DisableCount(unsigned short value, tRioStatusCode *status) {
	m_status.DisableCount = value;
#if 1
	printf("FPGA watchdog 0x%08x (SystemActive %c Alive %c\n"
	       "  SysDisableCount %u DisableCount %u)\n",
	  m_status.value, m_status.SystemActive ? 't' : 'f',
	  m_status.Alive ? 't' : 'f', m_status.SysDisableCount, m_status.DisableCount);
#endif
	*status = 0;
    }
    virtual tWatchdog::tStatus readStatus(tRioStatusCode *status) {
	*status = 0;
	return m_status;
    }
    virtual bool readStatus_SystemActive(tRioStatusCode *status) {
	*status = 0;
	return m_status.SystemActive;
    }
    virtual bool readStatus_Alive(tRioStatusCode *status) {
	*status = 0;
	return m_status.Alive;
    }
    virtual unsigned short readStatus_SysDisableCount(tRioStatusCode *status) {
	*status = 0;
	return m_status.SysDisableCount;
    }
    virtual unsigned short readStatus_DisableCount(tRioStatusCode *status) {
	*status = 0;
	return m_status.DisableCount;
    }

    virtual void writeImmortal(bool value, tRioStatusCode *status) {
#if 1
	printf("FPGA watchdog immortal %c\n", value ? 't' : 'f');
#endif
	m_immortal = value;
	*status = 0;
    }
    virtual bool readImmortal(tRioStatusCode *status) {
	*status = 0;
	return m_immortal;
    }

    virtual void strobeKill(tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA watchdog strobe kill\n");
#endif
	*status = 0;
    }

    virtual void strobeFeed(tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA watchdog strobe feed\n");
#endif
	*status = 0;
    }

    virtual void writeExpiration(unsigned int value, tRioStatusCode *status) {
#ifdef DEBUG
	printf("FPGA watchdog expiration %u\n", value);
#endif
	m_expiration = value;
	*status = 0;
    }
    virtual unsigned int readExpiration(tRioStatusCode *status) {
	*status = 0;
	return m_expiration;
    }

    virtual unsigned int readTimer(tRioStatusCode *status) {
	*status = 0;
	return m_timer;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(stubWatchdog);
};

tWatchdog* tWatchdog::create( tRioStatusCode *status )
{
    uint32_t guid[4];
    snprintf((char *)guid, sizeof guid, "watchdog");
    *status = 0;
    return new stubWatchdog(guid);
}

}; // namespace nFRC_2012_1_6_4
}; // namespace nFPGA

///////////////////////////////////////////////////////////////////////////////
//
// system clock
// The cRIO's PPC clock is 33 MHz (30ns).
//
///////////////////////////////////////////////////////////////////////////////

extern "C" {

UINT64 niTimestamp64(void)
{
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    UINT64 now = (ts.tv_sec * 1000000000LLU) + ts.tv_nsec;
    return (now / 30LLU);
}

}; // extern "C"

///////////////////////////////////////////////////////////////////////////////
//
// cRIO hardware
//
///////////////////////////////////////////////////////////////////////////////

extern "C" {

INT32 UserSwitchInput( INT32 nSwitch )
{
    assert(nSwitch == 0);
    return 0;
}

static int cRIO_User1_LED = 0;

INT32 LedInput(INT32 led)
{
    assert(led == 0);
    return cRIO_User1_LED;
}

INT32 LedOutput(INT32 led, INT32 value)
{
    assert(led == 0);
    cRIO_User1_LED = (value != 0);
    printf("%s: cRIO USER1 LED is now %s\n",
      __FUNCTION__, cRIO_User1_LED ? "ON" : "OFF");
    return cRIO_User1_LED;
}

}; // extern "C"

///////////////////////////////////////////////////////////////////////////////
//
// cRIO analog input calibration
//
// WPILib/AnalogModule.cpp says:
//   float voltage = LSBWeight * 1.0e-9 * value - offset * 1.0e-9;
// so:
//   LSBWeight = ((voltage * 1.0e9) + offset) / value;
//
// For the cRIO, full-scale voltage is -10.0 to +10.0 and the A/D
// resolution is 12 bits, so (with perfect scaling and no offset):
//   LSBWeight = ((10.0 * 1.0e9) + 0.0) / ((1 << 11) - 1) = 4885198
//
///////////////////////////////////////////////////////////////////////////////

extern "C" {

UINT32 FRC_NetworkCommunication_nAICalibration_getLSBWeight( const UINT32 aiSystemIndex, const UINT32 channel, INT32 *status )
{
    *status = OK;
    return 4885198;
}

INT32 FRC_NetworkCommunication_nAICalibration_getOffset( const UINT32 aiSystemIndex, const UINT32 channel, INT32 *status )
{
    *status = OK;
    return 0;
}

}; // extern "C"

