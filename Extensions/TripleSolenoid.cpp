// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "DoubleSolenoid.h"
#include "TripleSolenoid.h"

// update period chosen to avoid collisions
// with other scheduled tasks
const double TripleSolenoid::kPollInterval = 0.053;

// end-to-end travel time
const double TripleSolenoid::kTravelTime = 4.00;

TripleSolenoid::TripleSolenoid( int forwardChannel,
			        int reverseChannel,
			        int switchChannel )
    : DoubleSolenoid( forwardChannel, reverseChannel )
{
//  printf("TripleSolenoid::TripleSolenoid\n");
    m_switch = new DigitalInput( switchChannel );
    m_pNotifier = new Notifier( TripleSolenoid::TimerEvent, this );
    m_goal = kUnknown;
    m_position = kUnknown;
    m_direction = DoubleSolenoid::kOff;
    m_howLong = 0;
}

TripleSolenoid::~TripleSolenoid()
{
//  printf("TripleSolenoid::~TripleSolenoid\n");
    Stop();
    delete m_pNotifier;
    delete m_switch;
}

TripleSolenoid::Position TripleSolenoid::GetPosition()
{
//  printf("TripleSolenoid::GetPosition = %d\n", (int)m_position);
    return m_position;
}

void TripleSolenoid::SetPosition( Position position )
{
    if (m_goal != position) {
	// printf("TripleSolenoid::SetPosition %d\n", (int)m_goal);
	m_pNotifier->Stop();
	m_goal = position;
	Start();
    }
}

void TripleSolenoid::Start()
{
//  printf("TripleSolenoid::Start\n");
    m_howLong = 0;
    if (Move()) {
        // printf("TripleSolenoid::Start: we are moving\n");
	m_pNotifier->StartPeriodic( kPollInterval );
    }
}

void TripleSolenoid::Stop()
{
    // printf("TripleSolenoid::Stop\n");
    m_pNotifier->Stop();
    Set(DoubleSolenoid::kOff);
}

void TripleSolenoid::TimerEvent( void *param )
{
//  printf("TripleSolenoid::TimerEvent\n");
    ((TripleSolenoid*)param)->Run();
}

void TripleSolenoid::Run()
{
//  printf("TripleSolenoid::Run\n");
    m_howLong++;
    SmartDashboard::PutNumber("position time", (double)m_howLong);
    Update();
    if (!Move()) {
        // printf("TripleSolenoid::Run done!\n");
	Stop();
    }
}

// update current position
void TripleSolenoid::Update()
{
    bool sw = ! m_switch->Get();
    SmartDashboard::PutBoolean("position center", sw);
    if (sw) {
	m_position = kCenter;
    } else if (m_direction == DoubleSolenoid::kForward) {
	if (m_position == kRetracted)
	    m_position = kPartlyRetracted;
	else if (m_position == kCenter)
	    m_position = kPartlyExtended;
	// use timer instead of limit switch to detect
	// PartlyExtended => Extended
	else if (m_howLong * kPollInterval >= kTravelTime)
	    m_position = kExtended;
    } else if (m_direction == DoubleSolenoid::kReverse) {
	if (m_position == kExtended)
	    m_position = kPartlyExtended;
	else if (m_position == kCenter)
	    m_position = kPartlyRetracted;
	// use timer instead of limit switch to detect
	// PartlyRetracted => Retracted
	else if (m_howLong * kPollInterval >= kTravelTime)
	    m_position = kRetracted;
    }
//  printf("TripleSolenoid::Update sw=%d pos=%d\n", (int)sw, (int)m_position);
}

// determine how to move toward goal
bool TripleSolenoid::Move()
{
    if (m_position == m_goal || m_goal == kUnknown) {
        // printf("TripleSolenoid::Move stopping\n");
	m_direction = DoubleSolenoid::kOff;
    } else {
	switch (m_goal) {
	case kRetracted:
	    m_direction = DoubleSolenoid::kReverse;
	    break;
	case kPartlyRetracted:
	    if (m_position == kRetracted)
		m_direction = DoubleSolenoid::kForward;
	    else
		m_direction = DoubleSolenoid::kReverse;
	    break;
	case kCenter:
	    if (m_position == kRetracted || m_position == kPartlyRetracted)
		m_direction = DoubleSolenoid::kForward;
	    else
		m_direction = DoubleSolenoid::kReverse;
	    break;
	case kPartlyExtended:
	    if (m_position == kExtended)
		m_direction = DoubleSolenoid::kReverse;
	    else
		m_direction = DoubleSolenoid::kForward;
	    break;
	case kExtended:
	    m_direction = DoubleSolenoid::kForward;
	    break;
	default: // can't happen
	    m_direction = DoubleSolenoid::kOff;
	    break;
	}
    }
    Set(m_direction);
    SmartDashboard::PutNumber("position direction", (double)m_direction);
    return (m_direction != DoubleSolenoid::kOff);
}

