// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "Shooter.h"
#include "TiltCommand.h"

TiltCommand::TiltCommand( Shooter::TargetDistance targetDistance ) :
    Command("TiltCommand")
{
    Requires(Robot::shooter());
    m_targetDistance = targetDistance;
    m_currentDistance = Shooter::kUnknown;
    m_needToGoToMid = false;
}

void TiltCommand::Initialize()
{
printf("Command TiltCommand Initialize\n");
    m_currentDistance = Robot::shooter()->GetAngle();
    
    switch(m_targetDistance) {
    case Shooter::kShort:
    case Shooter::kLong:
	m_needToGoToMid = false;
	Robot::shooter()->SetAngle(m_targetDistance);
	break;

    case Shooter::kMid:
	if (m_currentDistance != Shooter::kMid) {
	    if (m_currentDistance == Shooter::kLong) {
		m_needToGoToMid = false;
		Robot::shooter()->SetAngle(m_targetDistance);
	    } else {
		m_needToGoToMid = true;
		Robot::shooter()->SetAngle(Shooter::kLong);
	    }
	} else {
	    m_needToGoToMid = false;
	}
	break;

    default:
	break;
    }
    Robot::shooter()->SetSpeed( 0.0 );
    Robot::shooter()->Start();
}

void TiltCommand::Execute()
{
    ;
}

bool TiltCommand::IsFinished()
{
    // keep running until canceled by caller
    bool isInPosition = Robot::shooter()->IsInPosition();
    if (isInPosition && m_needToGoToMid) {
	m_needToGoToMid = false;
	Robot::shooter()->SetAngle(m_targetDistance);
	isInPosition = false;
    }
if (isInPosition) printf("Command TiltCommand IsFinished\n");
    return isInPosition;
}

void TiltCommand::End()
{
printf("Command TiltCommand End\n");
    Robot::shooter()->Stop();
}

void TiltCommand::Interrupted()
{
printf("Command TiltCommand Interrupted\n");
    Robot::shooter()->Stop();
}
