// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Robot.h"

ShootCommand::ShootCommand( Shooter::TargetDistance targetDistance )
{
    Requires(Robot::shooter());
    m_targetDistance = targetDistance;
    m_launched = 0;
    SmartDashboard::PutNumber("ShootCommand Launched", (double) m_launched);
}

void ShootCommand::Initialize()
{
//  printf("ShootCommand::Initialize\n");
    m_launched = 0;
    SmartDashboard::PutNumber("ShootCommand Launched", (double) m_launched);
    Robot::shooter()->SetAngle(m_targetDistance);
    switch (m_targetDistance) {
    case Shooter::kShort:
	Robot::shooter()->SetSpeed(SPEED_SHORT);
	break;
    case Shooter::kMid:
	Robot::shooter()->SetSpeed(SPEED_MID);
	break;
    case Shooter::kLong:
	Robot::shooter()->SetSpeed(SPEED_LONG);
	break;
    default:
	break;
    }
    Robot::shooter()->Start();
}

void ShootCommand::Execute()
{
    if (Robot::shooter()->IsReadyToShoot()) {
	Robot::shooter()->Inject();
	m_launched++;
	SmartDashboard::PutNumber("ShootCommand Launched", (double) m_launched);
    }
}

int ShootCommand::GetLaunched()
{
    return m_launched;
}

bool ShootCommand::IsFinished()
{
    // keep running until canceled by caller
    return false;
}

void ShootCommand::End()
{
//  printf("ShootCommand::End\n");
    Robot::shooter()->Stop();
}

void ShootCommand::Interrupted()
{
//  printf("ShootCommand::Interrupted\n");
    Robot::shooter()->Stop();
}
