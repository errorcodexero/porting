// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "Shooter.h"
#include "ShootCommand.h"

ShootCommand::ShootCommand( Shooter::TargetDistance targetDistance,
			    int targetSelect, int num_disks ) :
    Command("ShootCommand")
{
    Requires(Robot::shooter());
    m_targetDistance = targetDistance;
    m_target = targetSelect;
    m_numDisks = num_disks;
    m_launched = 0;
    SmartDashboard::PutNumber("ShootCommand Launched", (double) m_launched);
}

void ShootCommand::SetDistance( Shooter::TargetDistance distance )
{
    m_targetDistance = distance;
}

void ShootCommand::SetTarget( int target )
{
    m_target = target;
}

void ShootCommand::SetNumDisks( int numDisks )
{
    m_numDisks = numDisks;
}

void ShootCommand::Initialize()
{
    printf("ShootCommand::Initialize\n");
    m_launched = 0;
    SmartDashboard::PutNumber("ShootCommand Launched", (double) m_launched);
    Robot::shooter()->SetAngle(m_targetDistance);
    switch (m_targetDistance) {
    case Shooter::kShort:
	Robot::shooter()->SetSpeed(Robot::theRobot().m_speed_short);
	break;
    case Shooter::kMid:
	Robot::shooter()->SetSpeed( (m_target == 3)
				    ? Robot::theRobot().m_speed_mid_3
				    : Robot::theRobot().m_speed_mid_2 );
	break;
    case Shooter::kLong:
	Robot::shooter()->SetSpeed( (m_target == 3)
				    ? Robot::theRobot().m_speed_long_3
				    : Robot::theRobot().m_speed_long_2 );
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
    bool finished = (Robot::shooter()->IsReadyToShoot() &&
    		     m_launched >= m_numDisks);
    if (finished) printf("ShootCommand::IsFinished\n");
    return finished;
}

void ShootCommand::End()
{
    printf("ShootCommand::End\n");
    Robot::shooter()->Stop();
}

void ShootCommand::Interrupted()
{
    printf("ShootCommand::End\n");
    Robot::shooter()->Stop();
}
