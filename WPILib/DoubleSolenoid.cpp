/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.			      */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "DoubleSolenoid.h"
#include "NetworkCommunication/UsageReporting.h"
#include "WPIErrors.h"
#include <string.h>
#include "LiveWindow/LiveWindow.h"

/**
 * Common function to implement constructor behavior.
 */
void DoubleSolenoid::InitSolenoid()
{
    char buf[64];
    m_table = NULL;
    if (!CheckSolenoidModule(m_moduleNumber))
    {
	snprintf(buf, 64, "Solenoid Module %u", (u_int)m_moduleNumber);
	wpi_setWPIErrorWithContext(ModuleIndexOutOfRange, buf);
	return;
    }
    if (!CheckSolenoidChannel(m_forwardChannel))
    {
	snprintf(buf, 64, "Solenoid Channel %u", (u_int)m_forwardChannel);
	wpi_setWPIErrorWithContext(ChannelIndexOutOfRange, buf);
	return;
    }
    if (!CheckSolenoidChannel(m_reverseChannel))
    {
	snprintf(buf, 64, "Solenoid Channel %u", (u_int)m_reverseChannel);
	wpi_setWPIErrorWithContext(ChannelIndexOutOfRange, buf);
	return;
    }
    Resource::CreateResourceObject(&m_allocated, tSolenoid::kNumDO7_0Elements * kSolenoidChannels);

    snprintf(buf, 64, "Solenoid %u (Module %u)", (u_int)m_forwardChannel, (u_int)m_moduleNumber);
    if (m_allocated->Allocate((m_moduleNumber - 1) * kSolenoidChannels + m_forwardChannel - 1, buf) == ~0ul)
    {
	CloneError(m_allocated);
	return;
    }
    snprintf(buf, 64, "Solenoid %u (Module %u)", (u_int)m_reverseChannel, (u_int)m_moduleNumber);
    if (m_allocated->Allocate((m_moduleNumber - 1) * kSolenoidChannels + m_reverseChannel - 1, buf) == ~0ul)
    {
	CloneError(m_allocated);
	return;
    }
    m_forwardMask = 1 << (m_forwardChannel - 1);
    m_reverseMask = 1 << (m_reverseChannel - 1);

    nUsageReporting::report(nUsageReporting::kResourceType_Solenoid, m_forwardChannel, m_moduleNumber - 1);
    nUsageReporting::report(nUsageReporting::kResourceType_Solenoid, m_reverseChannel, m_moduleNumber - 1);

    LiveWindow::GetInstance()->AddActuator("Double Solenoid", m_moduleNumber, m_forwardChannel, this);
}

/**
 * Constructor.
 *
 * @param forwardChannel The forward channel on the module to control.
 * @param reverseChannel The reverse channel on the module to control.
 */
DoubleSolenoid::DoubleSolenoid(UINT32 forwardChannel, UINT32 reverseChannel)
    : SolenoidBase (GetDefaultSolenoidModule())
    , m_forwardChannel (forwardChannel)
    , m_reverseChannel (reverseChannel)
{
    InitSolenoid();
}

/**
 * Constructor.
 *
 * @param moduleNumber The solenoid module (1 or 2).
 * @param forwardChannel The forward channel on the module to control.
 * @param reverseChannel The reverse channel on the module to control.
 */
DoubleSolenoid::DoubleSolenoid(UINT8 moduleNumber, UINT32 forwardChannel, UINT32 reverseChannel)
    : SolenoidBase (moduleNumber)
    , m_forwardChannel (forwardChannel)
    , m_reverseChannel (reverseChannel)
{
    InitSolenoid();
}

/**
 * Destructor.
 */
DoubleSolenoid::~DoubleSolenoid()
{
    if (CheckSolenoidModule(m_moduleNumber))
    {
	m_allocated->Free((m_moduleNumber - 1) * kSolenoidChannels + m_forwardChannel - 1);
	m_allocated->Free((m_moduleNumber - 1) * kSolenoidChannels + m_reverseChannel - 1);
    }
}

/**
 * Set the value of a solenoid.
 *
 * @param value Move the solenoid to forward, reverse, or don't move it.
 */
void DoubleSolenoid::Set(Value value)
{
    if (StatusIsFatal()) return;

    Value oldValue = Get();

    UINT8 rawValue = 0x00;
    switch(value)
    {
    case kOff:
	rawValue = 0x00;
	break;
    case kForward:
	rawValue = m_forwardMask;
	break;
    case kReverse:
	rawValue = m_reverseMask;
	break;
    }

    if (value != oldValue) {
 	SolenoidBase::Set(rawValue, m_forwardMask | m_reverseMask);
    }
}

/**
 * Read the current value of the solenoid.
 *
 * @return The current value of the solenoid.
 */
DoubleSolenoid::Value DoubleSolenoid::Get()
{
    if (StatusIsFatal()) {
	printf("DoubleSolenoid[%d][%d-%d]::Get"
		" status is FATAL, returning kOff\n",
		(int)m_moduleNumber, (int)m_forwardChannel,
		(int)m_reverseChannel);
	return kOff;
    }

    UINT8 value = GetAll();
    if (value & m_forwardMask) return kForward;
    if (value & m_reverseMask) return kReverse;
    return kOff;
}

void DoubleSolenoid::ValueChanged(ITable* source, const std::string& key, EntryValue value, bool isNew)
{
    std::string *val = (std::string *) value.ptr;
    if (*val == "Off") Set(DoubleSolenoid::kOff);
    else if (*val == "Forward") Set(DoubleSolenoid::kForward);
    else if (*val == "Reverse") Set(DoubleSolenoid::kReverse);
    else printf("DoubleSolenoid[%d][%d-%d]::ValueChanged"
    		" value [%s] not valid\n", (int)m_moduleNumber,
		(int)m_forwardChannel, (int)m_reverseChannel, val->c_str());
}

void DoubleSolenoid::UpdateTable()
{
    if (m_table != NULL) {
	Value lvalue = Get();
	m_table->PutString("Value", (lvalue == kForward ? "Forward" : (lvalue == kReverse ? "Reverse" : "Off")));
    }
}

void DoubleSolenoid::StartLiveWindowMode()
{
    Set(kOff);
    if (m_table != NULL) {
	m_table->AddTableListener("Value", this, true);
    }
}

void DoubleSolenoid::StopLiveWindowMode() {
    Set(kOff);
    if (m_table != NULL) {
	m_table->RemoveTableListener(this);
    }
}

std::string DoubleSolenoid::GetSmartDashboardType() {
    return "Double Solenoid";
}

void DoubleSolenoid::InitTable(ITable *subTable) {
    m_table = subTable;
    UpdateTable();
}

ITable * DoubleSolenoid::GetTable() {
    return m_table;
}

