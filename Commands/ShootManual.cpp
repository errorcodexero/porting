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
    Shooter::TargetDistance target;
    double speed;

    switch (Robot::oi()->GetTarget()) {
    case 0:
	target = Shooter::kShort;
	speed = Robot::theRobot().m_speed_short;
	break;
    case 1:
	target = Shooter::kMid;
	speed = Robot::theRobot().m_speed_mid;
	break;
    case 2:
	target = Shooter::kLong;
	speed = Robot::theRobot().m_speed_long;
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

    if (Robot::oi()->GetLearn()) {
	if (!m_learn) {
	    printf("Shooter Learn\n");
	    Preferences *pref = Preferences::GetInstance();
	    switch (target) {
	    case Shooter::kShort:
		printf("saving %s = %g\n", KEY_SPEED_SHORT, speed);
		pref->PutDouble(KEY_SPEED_SHORT, speed);
		Robot::theRobot().m_speed_short = speed;
		break;
	    case Shooter::kMid:
		printf("saving %s = %g\n", KEY_SPEED_MID, speed);
		pref->PutDouble(KEY_SPEED_MID,   speed);
		Robot::theRobot().m_speed_mid = speed;
		break;
	    case Shooter::kLong:
		printf("saving %s = %g\n", KEY_SPEED_LONG, speed);
		pref->PutDouble(KEY_SPEED_LONG,  speed);
		Robot::theRobot().m_speed_long = speed;
		break;
	    default: // "can't happen"
		break;
	    }

	    std::vector<std::string> keys = pref->GetKeys();
	    for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); it++)
	    {
		printf("Shooter Preferences key = %s\n", it->c_str());
	    }

	    std::map<std::string, std::string> values = pref->GetValues();
	    for (std::map<std::string,std::string>::iterator it = values.begin(); it != values.end(); it++)
	    {
		printf("Shooter Preferences map = <%s,%s>\n",
			it->first.c_str(), it->second.c_str());
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
    printf("ShootManual::End\n");
    Robot::shooter()->Stop();
}

void ShootManual::Interrupted()
{
    printf("ShootManual::Interrupted\n");
    Robot::shooter()->Stop();
}
