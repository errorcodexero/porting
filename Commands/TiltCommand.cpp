// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Robot.h"
#include "TiltCommand.h"

TiltCommand::TiltCommand( Shooter::TargetDistance targetDistance )
{
    Requires(Robot::shooter());
    m_targetDistance = targetDistance;
}

void TiltCommand::Initialize()
{
//  printf("TiltCommand::Initialize\n");
    Robot::shooter()->SetAngle(m_targetDistance);
    Robot::shooter()->Start();
}

void TiltCommand::Execute()
{
	;
}

bool TiltCommand::IsFinished()
{
    // keep running until canceled by caller
    return Robot::shooter()->IsInPosition();
}

void TiltCommand::End()
{
//  printf("ShootCommand::End\n");
    Robot::shooter()->Stop();
}

void TiltCommand::Interrupted()
{
//  printf("ShootCommand::Interrupted\n");
    Robot::shooter()->Stop();
}
