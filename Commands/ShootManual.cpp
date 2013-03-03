// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Robot.h"

ShootManual::ShootManual()
{
    Requires(Robot::shooter());
}

void ShootManual::Initialize()
{
//  printf("ShootManual::Initialize\n");

    Robot::shooter()->SetAngle(Shooter::kUnknown);
    Robot::shooter()->SetSpeed(0.0);
    Robot::shooter()->Start();
}

void ShootManual::Execute()
{
    Shooter::TargetDistance target;
    double speed;

    switch (Robot::oi()->getTarget()) {
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
    speed *= (1.0 + (Robot::oi()->getSpeedAdjust() - 0.5) / 3.0);

    Robot::shooter()->SetAngle(target);
    Robot::shooter()->SetSpeed(speed);

    if (Robot::shooter()->IsReadyToShoot()) {
	Robot::oi()->setReadyLED(true);
	if (Robot::oi()->getLaunch()) {
	    Robot::shooter()->Inject();
	}
    } else {
	Robot::oi()->setReadyLED(false);
    }
}

bool ShootManual::IsFinished()
{
    // keep running until canceled by caller
    return false;
}

void ShootManual::End()
{
//  printf("ShootManual::End\n");
    Robot::shooter()->Stop();
}

void ShootManual::Interrupted()
{
//  printf("ShootManual::Interrupted\n");
    Robot::shooter()->Stop();
}
