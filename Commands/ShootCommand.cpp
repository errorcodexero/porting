// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "Shooter.h"
#include "ShootCommand.h"

ShootCommand::ShootCommand( Shooter::TargetDistance targetDistance,
			    int num_disks )
{
    Requires(Robot::shooter());
    m_targetDistance = targetDistance;
    m_numDisks = num_disks;
    m_launched = 0;
    SmartDashboard::PutNumber("ShootCommand Launched", (double) m_launched);
}

void ShootCommand::Initialize()
{
printf("Command ShootCommand Initialize\n");
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
    Robot::shooter()->SetInjector(false);
    Robot::shooter()->Start();
}

void ShootCommand::Execute()
{
    if (Robot::shooter()->IsReadyToShoot()) {
	if (m_launched < m_numDisks) {
	    Robot::shooter()->Inject();
	    m_launched++;
	    SmartDashboard::PutNumber("ShootCommand Launched", (double) m_launched);
	}
    }
}

int ShootCommand::GetLaunched()
{
    return m_launched;
}

bool ShootCommand::IsFinished()
{
    bool finished = (Robot::shooter()->IsReadyToShoot() && m_launched >= m_numDisks);
if (finished) printf("Command ShootCommand IsFinished\n");
    return finished;
}

void ShootCommand::End()
{
printf("Command ShootCommand End\n");
    Robot::shooter()->Stop();
}

void ShootCommand::Interrupted()
{
printf("Command ShootCommand End\n");
    Robot::shooter()->Stop();
}
