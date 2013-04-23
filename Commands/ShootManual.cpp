// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "OI.h"
#include "Shooter.h"
#include "ShootManual.h"

ShootManual::ShootManual() :
    Command("ShootManual")
{
    Requires(Robot::shooter());
    m_learn = false;
}

void ShootManual::Initialize()
{
    printf("ShootManual::Initialize\n");

    Robot::shooter()->SetAngle(Shooter::kUnknown);
    Robot::shooter()->SetSpeed(0.0);
    Robot::shooter()->SetInjector(false);
    Robot::shooter()->Start();

    m_learn = false;
}

void ShootManual::Execute()
{
    Shooter::TargetDistance distance;
    double speed;

    switch (Robot::oi()->GetTilt()) {
    case 0:
	distance = Shooter::kShort;
	speed = Robot::theRobot().m_speed_short;
	break;
    case 1:
	distance = Shooter::kMid;
	speed = (Robot::oi()->GetTarget() == 3)
		? Robot::theRobot().m_speed_mid_3
		: Robot::theRobot().m_speed_mid_2;
	break;
    case 2:
	distance = Shooter::kLong;
	speed = (Robot::oi()->GetTarget() == 3)
		? Robot::theRobot().m_speed_long_3
		: Robot::theRobot().m_speed_long_2;
	break;
    default:  // "can't happen"
	distance = Shooter::kUnknown;
	speed = 0.0;
	break;
    }

    // adjust base speed +/- 10%
    speed *= (1.0 + (Robot::oi()->GetSpeedAdjust() - 0.5) / 5.0);

    Robot::shooter()->SetAngle(distance);
    Robot::shooter()->SetSpeed(speed);

    if (Robot::oi()->GetLearn()) {
	if (!m_learn) {
	    printf("Shooter Learn\n");
	    Preferences *pref = Preferences::GetInstance();
	    switch (distance) {
	    case Shooter::kShort:
		printf("saving %s = %g\n", KEY_SPEED_SHORT, speed);
		pref->PutDouble(KEY_SPEED_SHORT, speed);
		Robot::theRobot().m_speed_short = speed;
		break;
	    case Shooter::kMid:
		if (Robot::oi()->GetTarget() == 3) {
		    printf("saving %s = %g\n", KEY_SPEED_MID_3, speed);
		    pref->PutDouble(KEY_SPEED_MID_3,   speed);
		    Robot::theRobot().m_speed_mid_3 = speed;
		} else {
		    printf("saving %s = %g\n", KEY_SPEED_MID_2, speed);
		    pref->PutDouble(KEY_SPEED_MID_2,   speed);
		    Robot::theRobot().m_speed_mid_2 = speed;
		}
		break;
	    case Shooter::kLong:
		if (Robot::oi()->GetTarget() == 3) {
		    printf("saving %s = %g\n", KEY_SPEED_LONG_3, speed);
		    pref->PutDouble(KEY_SPEED_LONG_3,  speed);
		    Robot::theRobot().m_speed_long_3 = speed;
		} else {
		    printf("saving %s = %g\n", KEY_SPEED_LONG_2, speed);
		    pref->PutDouble(KEY_SPEED_LONG_2,  speed);
		    Robot::theRobot().m_speed_long_2 = speed;
		}
		break;
	    default: // "can't happen"
		break;
	    }

	    std::vector<std::string> keys = pref->GetKeys();
	    for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); it++)
	    {
		printf("Shooter Preferences key = %s\n", it->c_str());
	    }

	    pref->Save();
	    m_learn = true;
	}
    } else {
	if (m_learn) {
	    printf("Shooter !Learn\n");
	    m_learn = false;
	}
    }

    if (Robot::shooter()->IsReadyToShoot() || Robot::oi()->GetOverride()) {
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
    printf("ShootManual::End\n");
    Robot::shooter()->Stop();
    Robot::oi()->SetReadyLED(false);
}

void ShootManual::Interrupted()
{
    printf("ShootManual::Interrupted\n");
    Robot::shooter()->Stop();
    Robot::oi()->SetReadyLED(false);
}
