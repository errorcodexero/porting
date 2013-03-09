// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "OI.h"
#include "Shooter.h"
#include "ShootManual.h"

ShootManual::ShootManual()
{
    Requires(Robot::shooter());
}

void ShootManual::Initialize()
{
printf("Command ShootManual Initialize\n");

    Robot::shooter()->SetAngle(Shooter::kUnknown);
    Robot::shooter()->SetSpeed(0.0);
    Robot::shooter()->Start();
}

void ShootManual::Execute()
{
    Shooter::TargetDistance target;
    double speed;

    switch (Robot::oi()->GetTarget()) {
    case 0:
	target = Shooter::kShort;
	speed = SPEED_SHORT;
	break;
    case 1:
	target = Shooter::kMid;
	speed = SPEED_MID;
	break;
    case 2:
	target = Shooter::kLong;
	speed = SPEED_LONG;
	break;
    default:  // "can't happen"
	target = Shooter::kUnknown;
	speed = 0.0;
	break;
    }

    // adjust base speed +/- 16.67%
    speed *= (1.0 + (Robot::oi()->GetSpeedAdjust() - 0.5) / 3.0);

    Robot::shooter()->SetAngle(target);
    Robot::shooter()->SetSpeed(speed);

    if (Robot::shooter()->IsReadyToShoot()) {
	Robot::oi()->SetReadyLED(true);
	if (Robot::oi()->GetLaunch()) {
	    Robot::shooter()->Inject();
	}
    } else {
	Robot::oi()->SetReadyLED(false);
    }
}

bool ShootManual::IsFinished()
{
    // keep running until canceled by caller
    return false;
}

void ShootManual::End()
{
printf("Command ShootManual End\n");
    Robot::shooter()->Stop();
}

void ShootManual::Interrupted()
{
printf("Command ShootManual Interrupted\n");
    Robot::shooter()->Stop();
}
