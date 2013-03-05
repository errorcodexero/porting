// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Climber.h"
#include "ClimbManual.h"

const double Climber::kHookSpeed = 0.995;
const double Climber::kExtendTime = 1.2;
const double Climber::kGrabTime = 1.2;

Climber::Climber( int leftMotor, int rightMotor,
		  int ltLim, int rtLim,
		  int lmLim, int rmLim,
		  int lbLim, int rbLim,
		  int extendChannel, int retractChannel,
		  int clawOpen, int clawClose )
    : Subsystem("Climber")
{
    LiveWindow *lw = LiveWindow::GetInstance();

    m_pLeftMotor = new Victor( leftMotor );
    lw->AddActuator("Climber", "Left", m_pLeftMotor);

    m_pRightMotor = new Victor( rightMotor );
    lw->AddActuator("Climber", "Right", m_pRightMotor);

    m_pLTopLim = new DigitalInput( ltLim );
    lw->AddSensor("Climber", "LeftTopLimit", m_pLTopLim);

    m_pRTopLim = new DigitalInput( rtLim );
    lw->AddSensor("Climber", "RightTopLimit", m_pRTopLim);

    m_pLMidLim = new DigitalInput( lmLim );
    lw->AddSensor("Climber", "LeftMidLimit", m_pLMidLim);

    m_pRMidLim = new DigitalInput( rmLim );
    lw->AddSensor("Climber", "RightMidLimit", m_pRMidLim);

    m_pLBotLim = new DigitalInput( lbLim );
    lw->AddSensor("Climber", "LeftBotLimit", m_pLBotLim);

    m_pRBotLim = new DigitalInput( rbLim );
    lw->AddSensor("Climber", "RightBotLimit", m_pRBotLim);

    m_hookDirection = kStop;
    m_leftPosition = kHooksUnknown;
    m_rightPosition = kHooksUnknown;

    m_pExtender = new DoubleSolenoid(extendChannel, retractChannel);
    lw->AddActuator("Climber", "Extender", m_pExtender);

    m_extenderPosition = kExtenderUnknown;
    m_extenderStartTime = 0.0;
    m_extenderMoving = false;

    m_pClaw = new DoubleSolenoid(clawOpen, clawClose);
    lw->AddActuator("Climber", "Claw", m_pClaw);

    m_clawPosition = kClawUnknown;
    m_clawStartTime = 0.0;
    m_clawMoving = false;

    m_defaultCommand = NULL;
}

Climber::~Climber()
{
    SetDefaultCommand(NULL);
    Scheduler::GetInstance()->Remove(m_defaultCommand);
    delete m_defaultCommand;
    m_defaultCommand = NULL;

    delete m_pLeftMotor;
    delete m_pRightMotor;
    delete m_pLTopLim;
    delete m_pRTopLim;
    delete m_pLMidLim;
    delete m_pRMidLim;
    delete m_pLBotLim;
    delete m_pRBotLim;
    delete m_pExtender;
    delete m_pClaw;
}

void Climber::InitDefaultCommand()
{
    if (!m_defaultCommand) {
	m_defaultCommand = new ClimbManual();
	SetDefaultCommand(m_defaultCommand);
    }
}

void Climber::UpdateHookPosition()
{
    HookPosition was = m_leftPosition;
    if (!m_pLBotLim->Get()) {
	m_leftPosition = kBottom;
    } else if (!m_pLTopLim->Get()) {
	m_leftPosition = kTop;
    } else if (!m_pLMidLim->Get()) {
	if (m_hookDirection == kUp) {
	    m_leftPosition = kMidLow;
	} else if (m_hookDirection == kDown) {
	    m_leftPosition = kMidHigh;
	}
    } else if (m_hookDirection == kUp && m_leftPosition == kMidLow) {
	m_leftPosition = kMidHigh;
    } else if (m_hookDirection == kDown && m_leftPosition == kMidHigh) {
	m_leftPosition = kMidLow;
    }
    if (m_leftPosition != was) {
	printf("left hook position was %u now %u\n",
	    (unsigned) was, (unsigned) m_leftPosition);
    }

    if (!m_pRBotLim->Get()) {
	m_rightPosition = kBottom;
    } else if (!m_pRTopLim->Get()) {
	m_rightPosition = kTop;
    } else if (!m_pRMidLim->Get()) {
	if (m_hookDirection == kUp) {
	    m_rightPosition = kMidLow;
	} else if (m_hookDirection == kDown) {
	    m_rightPosition = kMidHigh;
	}
    } else if (m_hookDirection == kUp && m_rightPosition == kMidLow) {
	m_rightPosition = kMidHigh;
    } else if (m_hookDirection == kDown && m_rightPosition == kMidHigh) {
	m_rightPosition = kMidLow;
    }
    // printf("right position %u\n", (unsigned) m_rightPosition);
}

bool Climber::SetHooks( HookDirection direction, HookPosition stopAt )
{
    int left, right;

    // figure out where we are now based on last driving directions

    UpdateHookPosition();

    // set motor driving direction, deal with top/bottom limit switches

    m_hookDirection = direction;
    switch (m_hookDirection) {
    case kUp:
	// Set motor directions for upward motion.
	left = 1;
	right = 1;
	// If we're already at the top limit, stop.
	if (!m_pLTopLim->Get())
	    left = 0;
	if (!m_pRTopLim->Get())
	    right = 0;
	// If we're already at or above the desired position, stop.
	if ((m_leftPosition != kHooksUnknown)
	 && (stopAt != kHooksUnknown)
	 && (m_leftPosition >= stopAt))
	    left = 0;
	if ((m_rightPosition != kHooksUnknown)
	 && (stopAt != kHooksUnknown)
	 && (m_rightPosition >= stopAt))
	    right = 0;
	break;
    case kDown:
	// Set motor directions for downward motion.
	left = -1;
	right = -1;
	// If we're already at the bottom limit, stop.
	if (!m_pLBotLim->Get())
	    left = 0;
	if (!m_pRBotLim->Get())
	    right = 0;
	// If we're already at or below the desired position, stop.
	if ((m_leftPosition != kHooksUnknown)
	 && (stopAt != kHooksUnknown)
	 && (m_leftPosition <= stopAt))
	    left = 0;
	if ((m_rightPosition != kHooksUnknown)
	 && (stopAt != kHooksUnknown)
	 && (m_rightPosition <= stopAt))
	    right = 0;
	break;
    case kStop:
	left = 0;
	right = 0;
    }

    m_pLeftMotor->Set(left * kHookSpeed);
    m_pRightMotor->Set(right * kHookSpeed);

    // return true when both hooks are in position (not moving)
    return (left == 0) || (right == 0);
}

bool Climber::HooksAtTop()
{
    return (m_leftPosition == kTop) && (m_rightPosition == kTop);
}

bool Climber::HooksAtMidHigh()
{
    return (m_leftPosition == kMidHigh) && (m_rightPosition == kMidHigh);
}

bool Climber::HooksAtMidLow()
{
    return (m_leftPosition == kMidLow) && (m_rightPosition == kMidLow);
}

bool Climber::HooksAtBottom()
{
    return (m_leftPosition == kBottom) && (m_rightPosition == kBottom);
}

void Climber::SetExtender( ExtenderPosition position )
{
    switch (position) {
    case kExtended:
	m_pExtender->Set(DoubleSolenoid::kForward);
	break;
    case kRetracted:
	m_pExtender->Set(DoubleSolenoid::kReverse);
	break;
    default:
	m_pExtender->Set(DoubleSolenoid::kOff);
	break;
    }
    if (position != m_extenderPosition) {
	m_extenderStartTime = Timer::GetFPGATimestamp();
	m_extenderMoving = true;
	m_extenderPosition = position;
    }
}

Climber::ExtenderPosition Climber::GetExtender()
{
    if (ExtenderIsMoving())
	return kExtenderUnknown;
    else
	return m_extenderPosition;
}

bool Climber::ExtenderIsMoving()
{
    if (m_extenderMoving &&
        ((Timer::GetFPGATimestamp() - m_extenderStartTime) >= kExtendTime))
    {
	m_extenderMoving = false;
    }
    return m_extenderMoving;
}

void Climber::SetClaw( ClawPosition position )
{
    switch (position) {
    case kOpen:
	m_pClaw->Set(DoubleSolenoid::kForward);
	break;
    case kClosed:
	m_pClaw->Set(DoubleSolenoid::kReverse);
	break;
    default:
	m_pClaw->Set(DoubleSolenoid::kOff);
	break;
    }
    if (position != m_clawPosition) {
	m_clawStartTime = Timer::GetFPGATimestamp();
	m_clawMoving = true;
	m_clawPosition = position;
    }
}

Climber::ClawPosition Climber::GetClaw()
{
    if (ClawIsMoving())
	return kClawUnknown;
    else
	return m_clawPosition;
}

bool Climber::ClawIsMoving()
{
    if (m_clawMoving &&
        ((Timer::GetFPGATimestamp() - m_clawStartTime) >= kGrabTime))
    {
	m_clawMoving = false;
    }
    return m_clawMoving;
}

// for debugging

UINT8 Climber::GetLimits()
{
    UINT8 limits = 0;
    // printf("limit:");
    if (!m_pLTopLim->Get()) {
	// printf(" left top,");
	limits |= 0x01;
    }
    if (m_pRTopLim->Get()) {
	// printf(" right top,");
	limits |= 0x02;
    }
    if (m_pLMidLim->Get()) {
	// printf(" left mid,");
	limits |= 0x04;
    }
    if (m_pRMidLim->Get()) {
	// printf(" right mid,");
	limits |= 0x08;
    }
    if (m_pLBotLim->Get()) {
	// printf(" left bot,");
	limits |= 0x10;
    }
    if (m_pRBotLim->Get()) {
	// printf(" right bot,");
	limits |= 0x20;
    }
    // printf("\n");
    return limits;
}

