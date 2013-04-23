// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "OI.h"
#include "BlinkyLight.h"
#include "Shooter.h"
#include "AutoCommand.h"
#include "BlinkyBreathe.h"
#include "TiltCommand.h"
#include "TargetCommand.h"
#include "ShootCommand.h"
#include "TimedDrive.h"

// constant strings used for preferences file entries
static const char AUTO0STEP0X[] = "AUTO0STEP0X";
static const char AUTO0STEP0Y[] = "AUTO0STEP0Y";
static const char AUTO0STEP0T[] = "AUTO0STEP0T";
static const char AUTO0STEP0S[] = "AUTO0STEP0S";
static const char AUTO0STEP1X[] = "AUTO0STEP1X";
static const char AUTO0STEP1Y[] = "AUTO0STEP1Y";
static const char AUTO0STEP1T[] = "AUTO0STEP1T";
static const char AUTO0STEP1S[] = "AUTO0STEP1S";
static const char AUTO0STEP2X[] = "AUTO0STEP2X";
static const char AUTO0STEP2Y[] = "AUTO0STEP2Y";
static const char AUTO0STEP2T[] = "AUTO0STEP2T";
static const char AUTO0STEP2S[] = "AUTO0STEP2S";
static const char AUTO0STEP3X[] = "AUTO0STEP3X";
static const char AUTO0STEP3Y[] = "AUTO0STEP3Y";
static const char AUTO0STEP3T[] = "AUTO0STEP3T";
static const char AUTO0STEP3S[] = "AUTO0STEP3S";
static const char AUTO0STEP4X[] = "AUTO0STEP4X";
static const char AUTO0STEP4Y[] = "AUTO0STEP4Y";
static const char AUTO0STEP4T[] = "AUTO0STEP4T";
static const char AUTO0STEP4S[] = "AUTO0STEP4S";
static const char AUTO0STEP5X[] = "AUTO0STEP5X";
static const char AUTO0STEP5Y[] = "AUTO0STEP5Y";
static const char AUTO0STEP5T[] = "AUTO0STEP5T";
static const char AUTO0STEP5S[] = "AUTO0STEP5S";
static const char AUTO1STEP0X[] = "AUTO1STEP0X";
static const char AUTO1STEP0Y[] = "AUTO1STEP0Y";
static const char AUTO1STEP0T[] = "AUTO1STEP0T";
static const char AUTO1STEP0S[] = "AUTO1STEP0S";
static const char AUTO1STEP1X[] = "AUTO1STEP1X";
static const char AUTO1STEP1Y[] = "AUTO1STEP1Y";
static const char AUTO1STEP1T[] = "AUTO1STEP1T";
static const char AUTO1STEP1S[] = "AUTO1STEP1S";
static const char AUTO1STEP2X[] = "AUTO1STEP2X";
static const char AUTO1STEP2Y[] = "AUTO1STEP2Y";
static const char AUTO1STEP2T[] = "AUTO1STEP2T";
static const char AUTO1STEP2S[] = "AUTO1STEP2S";
static const char AUTO1STEP3X[] = "AUTO1STEP3X";
static const char AUTO1STEP3Y[] = "AUTO1STEP3Y";
static const char AUTO1STEP3T[] = "AUTO1STEP3T";
static const char AUTO1STEP3S[] = "AUTO1STEP3S";
static const char AUTO1STEP4X[] = "AUTO1STEP4X";
static const char AUTO1STEP4Y[] = "AUTO1STEP4Y";
static const char AUTO1STEP4T[] = "AUTO1STEP4T";
static const char AUTO1STEP4S[] = "AUTO1STEP4S";
static const char AUTO1STEP5X[] = "AUTO1STEP5X";
static const char AUTO1STEP5Y[] = "AUTO1STEP5Y";
static const char AUTO1STEP5T[] = "AUTO1STEP5T";
static const char AUTO1STEP5S[] = "AUTO1STEP5S";
static const char AUTO2STEP0X[] = "AUTO2STEP0X";
static const char AUTO2STEP0Y[] = "AUTO2STEP0Y";
static const char AUTO2STEP0T[] = "AUTO2STEP0T";
static const char AUTO2STEP0S[] = "AUTO2STEP0S";
static const char AUTO2STEP1X[] = "AUTO2STEP1X";
static const char AUTO2STEP1Y[] = "AUTO2STEP1Y";
static const char AUTO2STEP1T[] = "AUTO2STEP1T";
static const char AUTO2STEP1S[] = "AUTO2STEP1S";
static const char AUTO2STEP2X[] = "AUTO2STEP2X";
static const char AUTO2STEP2Y[] = "AUTO2STEP2Y";
static const char AUTO2STEP2T[] = "AUTO2STEP2T";
static const char AUTO2STEP2S[] = "AUTO2STEP2S";
static const char AUTO2STEP3X[] = "AUTO2STEP3X";
static const char AUTO2STEP3Y[] = "AUTO2STEP3Y";
static const char AUTO2STEP3T[] = "AUTO2STEP3T";
static const char AUTO2STEP3S[] = "AUTO2STEP3S";
static const char AUTO2STEP4X[] = "AUTO2STEP4X";
static const char AUTO2STEP4Y[] = "AUTO2STEP4Y";
static const char AUTO2STEP4T[] = "AUTO2STEP4T";
static const char AUTO2STEP4S[] = "AUTO2STEP4S";
static const char AUTO2STEP5X[] = "AUTO2STEP5X";
static const char AUTO2STEP5Y[] = "AUTO2STEP5Y";
static const char AUTO2STEP5T[] = "AUTO2STEP5T";
static const char AUTO2STEP5S[] = "AUTO2STEP5S";
static const char AUTO3STEP0X[] = "AUTO3STEP0X";
static const char AUTO3STEP0Y[] = "AUTO3STEP0Y";
static const char AUTO3STEP0T[] = "AUTO3STEP0T";
static const char AUTO3STEP0S[] = "AUTO3STEP0S";
static const char AUTO3STEP1X[] = "AUTO3STEP1X";
static const char AUTO3STEP1Y[] = "AUTO3STEP1Y";
static const char AUTO3STEP1T[] = "AUTO3STEP1T";
static const char AUTO3STEP1S[] = "AUTO3STEP1S";
static const char AUTO3STEP2X[] = "AUTO3STEP2X";
static const char AUTO3STEP2Y[] = "AUTO3STEP2Y";
static const char AUTO3STEP2T[] = "AUTO3STEP2T";
static const char AUTO3STEP2S[] = "AUTO3STEP2S";
static const char AUTO3STEP3X[] = "AUTO3STEP3X";
static const char AUTO3STEP3Y[] = "AUTO3STEP3Y";
static const char AUTO3STEP3T[] = "AUTO3STEP3T";
static const char AUTO3STEP3S[] = "AUTO3STEP3S";
static const char AUTO3STEP4X[] = "AUTO3STEP4X";
static const char AUTO3STEP4Y[] = "AUTO3STEP4Y";
static const char AUTO3STEP4T[] = "AUTO3STEP4T";
static const char AUTO3STEP4S[] = "AUTO3STEP4S";
static const char AUTO3STEP5X[] = "AUTO3STEP5X";
static const char AUTO3STEP5Y[] = "AUTO3STEP5Y";
static const char AUTO3STEP5T[] = "AUTO3STEP5T";
static const char AUTO3STEP5S[] = "AUTO3STEP5S";
static const char AUTO4STEP0X[] = "AUTO4STEP0X";
static const char AUTO4STEP0Y[] = "AUTO4STEP0Y";
static const char AUTO4STEP0T[] = "AUTO4STEP0T";
static const char AUTO4STEP0S[] = "AUTO4STEP0S";
static const char AUTO4STEP1X[] = "AUTO4STEP1X";
static const char AUTO4STEP1Y[] = "AUTO4STEP1Y";
static const char AUTO4STEP1T[] = "AUTO4STEP1T";
static const char AUTO4STEP1S[] = "AUTO4STEP1S";
static const char AUTO4STEP2X[] = "AUTO4STEP2X";
static const char AUTO4STEP2Y[] = "AUTO4STEP2Y";
static const char AUTO4STEP2T[] = "AUTO4STEP2T";
static const char AUTO4STEP2S[] = "AUTO4STEP2S";
static const char AUTO4STEP3X[] = "AUTO4STEP3X";
static const char AUTO4STEP3Y[] = "AUTO4STEP3Y";
static const char AUTO4STEP3T[] = "AUTO4STEP3T";
static const char AUTO4STEP3S[] = "AUTO4STEP3S";
static const char AUTO4STEP4X[] = "AUTO4STEP4X";
static const char AUTO4STEP4Y[] = "AUTO4STEP4Y";
static const char AUTO4STEP4T[] = "AUTO4STEP4T";
static const char AUTO4STEP4S[] = "AUTO4STEP4S";
static const char AUTO4STEP5X[] = "AUTO4STEP5X";
static const char AUTO4STEP5Y[] = "AUTO4STEP5Y";
static const char AUTO4STEP5T[] = "AUTO4STEP5T";
static const char AUTO4STEP5S[] = "AUTO4STEP5S";
static const char AUTO5STEP0X[] = "AUTO5STEP0X";
static const char AUTO5STEP0Y[] = "AUTO5STEP0Y";
static const char AUTO5STEP0T[] = "AUTO5STEP0T";
static const char AUTO5STEP0S[] = "AUTO5STEP0S";
static const char AUTO5STEP1X[] = "AUTO5STEP1X";
static const char AUTO5STEP1Y[] = "AUTO5STEP1Y";
static const char AUTO5STEP1T[] = "AUTO5STEP1T";
static const char AUTO5STEP1S[] = "AUTO5STEP1S";
static const char AUTO5STEP2X[] = "AUTO5STEP2X";
static const char AUTO5STEP2Y[] = "AUTO5STEP2Y";
static const char AUTO5STEP2T[] = "AUTO5STEP2T";
static const char AUTO5STEP2S[] = "AUTO5STEP2S";
static const char AUTO5STEP3X[] = "AUTO5STEP3X";
static const char AUTO5STEP3Y[] = "AUTO5STEP3Y";
static const char AUTO5STEP3T[] = "AUTO5STEP3T";
static const char AUTO5STEP3S[] = "AUTO5STEP3S";
static const char AUTO5STEP4X[] = "AUTO5STEP4X";
static const char AUTO5STEP4Y[] = "AUTO5STEP4Y";
static const char AUTO5STEP4T[] = "AUTO5STEP4T";
static const char AUTO5STEP4S[] = "AUTO5STEP4S";
static const char AUTO5STEP5X[] = "AUTO5STEP5X";
static const char AUTO5STEP5Y[] = "AUTO5STEP5Y";
static const char AUTO5STEP5T[] = "AUTO5STEP5T";
static const char AUTO5STEP5S[] = "AUTO5STEP5S";
static const char AUTO6STEP0X[] = "AUTO6STEP0X";
static const char AUTO6STEP0Y[] = "AUTO6STEP0Y";
static const char AUTO6STEP0T[] = "AUTO6STEP0T";
static const char AUTO6STEP0S[] = "AUTO6STEP0S";
static const char AUTO6STEP1X[] = "AUTO6STEP1X";
static const char AUTO6STEP1Y[] = "AUTO6STEP1Y";
static const char AUTO6STEP1T[] = "AUTO6STEP1T";
static const char AUTO6STEP1S[] = "AUTO6STEP1S";
static const char AUTO6STEP2X[] = "AUTO6STEP2X";
static const char AUTO6STEP2Y[] = "AUTO6STEP2Y";
static const char AUTO6STEP2T[] = "AUTO6STEP2T";
static const char AUTO6STEP2S[] = "AUTO6STEP2S";
static const char AUTO6STEP3X[] = "AUTO6STEP3X";
static const char AUTO6STEP3Y[] = "AUTO6STEP3Y";
static const char AUTO6STEP3T[] = "AUTO6STEP3T";
static const char AUTO6STEP3S[] = "AUTO6STEP3S";
static const char AUTO6STEP4X[] = "AUTO6STEP4X";
static const char AUTO6STEP4Y[] = "AUTO6STEP4Y";
static const char AUTO6STEP4T[] = "AUTO6STEP4T";
static const char AUTO6STEP4S[] = "AUTO6STEP4S";
static const char AUTO6STEP5X[] = "AUTO6STEP5X";
static const char AUTO6STEP5Y[] = "AUTO6STEP5Y";
static const char AUTO6STEP5T[] = "AUTO6STEP5T";
static const char AUTO6STEP5S[] = "AUTO6STEP5S";
static const char AUTO7STEP0X[] = "AUTO7STEP0X";
static const char AUTO7STEP0Y[] = "AUTO7STEP0Y";
static const char AUTO7STEP0T[] = "AUTO7STEP0T";
static const char AUTO7STEP0S[] = "AUTO7STEP0S";
static const char AUTO7STEP1X[] = "AUTO7STEP1X";
static const char AUTO7STEP1Y[] = "AUTO7STEP1Y";
static const char AUTO7STEP1T[] = "AUTO7STEP1T";
static const char AUTO7STEP1S[] = "AUTO7STEP1S";
static const char AUTO7STEP2X[] = "AUTO7STEP2X";
static const char AUTO7STEP2Y[] = "AUTO7STEP2Y";
static const char AUTO7STEP2T[] = "AUTO7STEP2T";
static const char AUTO7STEP2S[] = "AUTO7STEP2S";
static const char AUTO7STEP3X[] = "AUTO7STEP3X";
static const char AUTO7STEP3Y[] = "AUTO7STEP3Y";
static const char AUTO7STEP3T[] = "AUTO7STEP3T";
static const char AUTO7STEP3S[] = "AUTO7STEP3S";
static const char AUTO7STEP4X[] = "AUTO7STEP4X";
static const char AUTO7STEP4Y[] = "AUTO7STEP4Y";
static const char AUTO7STEP4T[] = "AUTO7STEP4T";
static const char AUTO7STEP4S[] = "AUTO7STEP4S";
static const char AUTO7STEP5X[] = "AUTO7STEP5X";
static const char AUTO7STEP5Y[] = "AUTO7STEP5Y";
static const char AUTO7STEP5T[] = "AUTO7STEP5T";
static const char AUTO7STEP5S[] = "AUTO7STEP5S";
static const char AUTO8STEP0X[] = "AUTO8STEP0X";
static const char AUTO8STEP0Y[] = "AUTO8STEP0Y";
static const char AUTO8STEP0T[] = "AUTO8STEP0T";
static const char AUTO8STEP0S[] = "AUTO8STEP0S";
static const char AUTO8STEP1X[] = "AUTO8STEP1X";
static const char AUTO8STEP1Y[] = "AUTO8STEP1Y";
static const char AUTO8STEP1T[] = "AUTO8STEP1T";
static const char AUTO8STEP1S[] = "AUTO8STEP1S";
static const char AUTO8STEP2X[] = "AUTO8STEP2X";
static const char AUTO8STEP2Y[] = "AUTO8STEP2Y";
static const char AUTO8STEP2T[] = "AUTO8STEP2T";
static const char AUTO8STEP2S[] = "AUTO8STEP2S";
static const char AUTO8STEP3X[] = "AUTO8STEP3X";
static const char AUTO8STEP3Y[] = "AUTO8STEP3Y";
static const char AUTO8STEP3T[] = "AUTO8STEP3T";
static const char AUTO8STEP3S[] = "AUTO8STEP3S";
static const char AUTO8STEP4X[] = "AUTO8STEP4X";
static const char AUTO8STEP4Y[] = "AUTO8STEP4Y";
static const char AUTO8STEP4T[] = "AUTO8STEP4T";
static const char AUTO8STEP4S[] = "AUTO8STEP4S";
static const char AUTO8STEP5X[] = "AUTO8STEP5X";
static const char AUTO8STEP5Y[] = "AUTO8STEP5Y";
static const char AUTO8STEP5T[] = "AUTO8STEP5T";
static const char AUTO8STEP5S[] = "AUTO8STEP5S";
static const char AUTO9STEP0X[] = "AUTO9STEP0X";
static const char AUTO9STEP0Y[] = "AUTO9STEP0Y";
static const char AUTO9STEP0T[] = "AUTO9STEP0T";
static const char AUTO9STEP0S[] = "AUTO9STEP0S";
static const char AUTO9STEP1X[] = "AUTO9STEP1X";
static const char AUTO9STEP1Y[] = "AUTO9STEP1Y";
static const char AUTO9STEP1T[] = "AUTO9STEP1T";
static const char AUTO9STEP1S[] = "AUTO9STEP1S";
static const char AUTO9STEP2X[] = "AUTO9STEP2X";
static const char AUTO9STEP2Y[] = "AUTO9STEP2Y";
static const char AUTO9STEP2T[] = "AUTO9STEP2T";
static const char AUTO9STEP2S[] = "AUTO9STEP2S";
static const char AUTO9STEP3X[] = "AUTO9STEP3X";
static const char AUTO9STEP3Y[] = "AUTO9STEP3Y";
static const char AUTO9STEP3T[] = "AUTO9STEP3T";
static const char AUTO9STEP3S[] = "AUTO9STEP3S";
static const char AUTO9STEP4X[] = "AUTO9STEP4X";
static const char AUTO9STEP4Y[] = "AUTO9STEP4Y";
static const char AUTO9STEP4T[] = "AUTO9STEP4T";
static const char AUTO9STEP4S[] = "AUTO9STEP4S";
static const char AUTO9STEP5X[] = "AUTO9STEP5X";
static const char AUTO9STEP5Y[] = "AUTO9STEP5Y";
static const char AUTO9STEP5T[] = "AUTO9STEP5T";
static const char AUTO9STEP5S[] = "AUTO9STEP5S";

// constant strings used for dashboard entries

static const string PATTERN = "AUTO PATTERN";

static const char STEP0X[] = "STEP 0 X";
static const char STEP0Y[] = "STEP 0 Y";
static const char STEP0T[] = "STEP 0 T";
static const char STEP0S[] = "STEP 0 S";

static const char STEP1X[] = "STEP 1 X";
static const char STEP1Y[] = "STEP 1 Y";
static const char STEP1T[] = "STEP 1 T";
static const char STEP1S[] = "STEP 1 S";

static const char STEP2X[] = "STEP 2 X";
static const char STEP2Y[] = "STEP 2 Y";
static const char STEP2T[] = "STEP 2 T";
static const char STEP2S[] = "STEP 2 S";

static const char STEP3X[] = "STEP 3 X";
static const char STEP3Y[] = "STEP 3 Y";
static const char STEP3T[] = "STEP 3 T";
static const char STEP3S[] = "STEP 3 S";

static const char STEP4X[] = "STEP 4 X";
static const char STEP4Y[] = "STEP 4 Y";
static const char STEP4T[] = "STEP 4 T";
static const char STEP4S[] = "STEP 4 S";

static const char STEP5X[] = "STEP 5 X";
static const char STEP5Y[] = "STEP 5 Y";
static const char STEP5T[] = "STEP 5 T";
static const char STEP5S[] = "STEP 5 S";


AutoCommand::AutoCommand() :
    CommandGroup("AutoCommand")
{
    // construct subcommands, establish command sequence
    m_blinky = new BlinkyBreathe(3.0);
    AddParallel(m_blinky);

    m_tilt   = new TiltCommand(Shooter::kLong);
    AddSequential(m_tilt);

#ifdef AUTO_TURN_TO_TARGET
    m_target = new TargetCommand();
    AddSequential(m_target);
#endif

    // default setting: shoot 3 disks at 3-point goal from mid-field
    m_shoot  = new ShootCommand(Shooter::kMid, 3, 3);
    AddSequential(m_shoot);

    // default movement commands - we'll set driving pattern later
    for (int i = 0; i < MAX_AUTO_STEPS; i++) {
	m_step[i] = new TimedDrive(0.0, 0.0, 0.0, 0.0);
	AddSequential(m_step[i]);
    }

    // get initial driving parameters
    GetAutoPreferences();
    m_pattern = Robot::oi()->GetAuto();

    // set dashboard to configure driving sequence
    PutDashboardSettings();

    m_autoSelectCmd = new AutoSelect(this);
    m_autoSelectCmd->SetRunWhenDisabled(true);
    m_autoSelectCmd->Start();

    m_autoSaveCmd = new AutoSave(this);;
    m_autoSaveCmd->SetRunWhenDisabled(true);
    SmartDashboard::PutData("AUTO SAVE", m_autoSaveCmd);
}

AutoCommand::~AutoCommand()
{
    // TBD: This should remove command instances from scheduler
    // then delete them.  But WPILib doesn't support that.
}

void AutoCommand::Initialize()
{
    printf("AutoCommand::Initialize\n");
    
    // shooter configuration:
    // hard-coded for shooting at 3-point goal from mid-field
    m_shoot->SetDistance(Shooter::kMid);
    m_shoot->SetTarget(3);
    m_shoot->SetNumDisks(3);
    
    // configure TimedDrive steps to run the selected pattern
    // as currently displayed on the SmartDashboard
    GetDashboardSettings();
    DriveStep *pStep = &m_drivePattern[m_pattern].step[0];
    for (int i = 0; i < MAX_AUTO_STEPS; i++) {
	m_step[i]->Set( pStep->x, pStep->y, pStep->s, pStep->t );
	++pStep;
    }
}

void AutoCommand::GetAutoPreferences()
{
    printf("AutoCommand::GetAutoPreferences\n");

    Preferences *pref = Preferences::GetInstance();

    // autonomous mode sequence 0 step 0
    m_drivePattern[0].step[0].x = pref->GetDouble(AUTO0STEP0X, 0.0);
    m_drivePattern[0].step[0].y = pref->GetDouble(AUTO0STEP0Y, 0.0);
    m_drivePattern[0].step[0].t = pref->GetDouble(AUTO0STEP0T, 0.0);
    m_drivePattern[0].step[0].s = pref->GetDouble(AUTO0STEP0S, 0.0);
    // autonomous mode sequence 0 step 1
    m_drivePattern[0].step[1].x = pref->GetDouble(AUTO0STEP1X, 0.0);
    m_drivePattern[0].step[1].y = pref->GetDouble(AUTO0STEP1Y, 0.0);
    m_drivePattern[0].step[1].t = pref->GetDouble(AUTO0STEP1T, 0.0);
    m_drivePattern[0].step[1].s = pref->GetDouble(AUTO0STEP1S, 0.0);
    // autonomous mode sequence 0 step 2
    m_drivePattern[0].step[2].x = pref->GetDouble(AUTO0STEP2X, 0.0);
    m_drivePattern[0].step[2].y = pref->GetDouble(AUTO0STEP2Y, 0.0);
    m_drivePattern[0].step[2].t = pref->GetDouble(AUTO0STEP2T, 0.0);
    m_drivePattern[0].step[2].s = pref->GetDouble(AUTO0STEP2S, 0.0);
    // autonomous mode sequence 0 step 3
    m_drivePattern[0].step[3].x = pref->GetDouble(AUTO0STEP3X, 0.0);
    m_drivePattern[0].step[3].y = pref->GetDouble(AUTO0STEP3Y, 0.0);
    m_drivePattern[0].step[3].t = pref->GetDouble(AUTO0STEP3T, 0.0);
    m_drivePattern[0].step[3].s = pref->GetDouble(AUTO0STEP3S, 0.0);
    // autonomous mode sequence 0 step 0
    m_drivePattern[0].step[4].x = pref->GetDouble(AUTO0STEP4X, 0.0);
    m_drivePattern[0].step[4].y = pref->GetDouble(AUTO0STEP4Y, 0.0);
    m_drivePattern[0].step[4].t = pref->GetDouble(AUTO0STEP4T, 0.0);
    m_drivePattern[0].step[4].s = pref->GetDouble(AUTO0STEP4S, 0.0);
    // autonomous mode sequence 0 step 5
    m_drivePattern[0].step[5].x = pref->GetDouble(AUTO0STEP5X, 0.0);
    m_drivePattern[0].step[5].y = pref->GetDouble(AUTO0STEP5Y, 0.0);
    m_drivePattern[0].step[5].t = pref->GetDouble(AUTO0STEP5T, 0.0);
    m_drivePattern[0].step[5].s = pref->GetDouble(AUTO0STEP5S, 0.0);

    // autonomous mode sequence 1 step 0
    m_drivePattern[1].step[0].x = pref->GetDouble(AUTO1STEP0X, 0.0);
    m_drivePattern[1].step[0].y = pref->GetDouble(AUTO1STEP0Y, 0.0);
    m_drivePattern[1].step[0].t = pref->GetDouble(AUTO1STEP0T, 0.0);
    m_drivePattern[1].step[0].s = pref->GetDouble(AUTO1STEP0S, 0.0);
    // autonomous mode sequence 1 step 1
    m_drivePattern[1].step[1].x = pref->GetDouble(AUTO1STEP1X, 0.0);
    m_drivePattern[1].step[1].y = pref->GetDouble(AUTO1STEP1Y, 0.0);
    m_drivePattern[1].step[1].t = pref->GetDouble(AUTO1STEP1T, 0.0);
    m_drivePattern[1].step[1].s = pref->GetDouble(AUTO1STEP1S, 0.0);
    // autonomous mode sequence 1 step 2
    m_drivePattern[1].step[2].x = pref->GetDouble(AUTO1STEP2X, 0.0);
    m_drivePattern[1].step[2].y = pref->GetDouble(AUTO1STEP2Y, 0.0);
    m_drivePattern[1].step[2].t = pref->GetDouble(AUTO1STEP2T, 0.0);
    m_drivePattern[1].step[2].s = pref->GetDouble(AUTO1STEP2S, 0.0);
    // autonomous mode sequence 1 step 3
    m_drivePattern[1].step[3].x = pref->GetDouble(AUTO1STEP3X, 0.0);
    m_drivePattern[1].step[3].y = pref->GetDouble(AUTO1STEP3Y, 0.0);
    m_drivePattern[1].step[3].t = pref->GetDouble(AUTO1STEP3T, 0.0);
    m_drivePattern[1].step[3].s = pref->GetDouble(AUTO1STEP3S, 0.0);
    // autonomous mode sequence 1 step 4
    m_drivePattern[1].step[4].x = pref->GetDouble(AUTO1STEP4X, 0.0);
    m_drivePattern[1].step[4].y = pref->GetDouble(AUTO1STEP4Y, 0.0);
    m_drivePattern[1].step[4].t = pref->GetDouble(AUTO1STEP4T, 0.0);
    m_drivePattern[1].step[4].s = pref->GetDouble(AUTO1STEP4S, 0.0);
    // autonomous mode sequence 1 step 5
    m_drivePattern[1].step[5].x = pref->GetDouble(AUTO1STEP5X, 0.0);
    m_drivePattern[1].step[5].y = pref->GetDouble(AUTO1STEP5Y, 0.0);
    m_drivePattern[1].step[5].t = pref->GetDouble(AUTO1STEP5T, 0.0);
    m_drivePattern[1].step[5].s = pref->GetDouble(AUTO1STEP5S, 0.0);

    // autonomous mode sequence 2 step 0
    m_drivePattern[2].step[0].x = pref->GetDouble(AUTO2STEP0X, 0.0);
    m_drivePattern[2].step[0].y = pref->GetDouble(AUTO2STEP0Y, 0.0);
    m_drivePattern[2].step[0].t = pref->GetDouble(AUTO2STEP0T, 0.0);
    m_drivePattern[2].step[0].s = pref->GetDouble(AUTO2STEP0S, 0.0);
    // autonomous mode sequence 2 step 1
    m_drivePattern[2].step[1].x = pref->GetDouble(AUTO2STEP1X, 0.0);
    m_drivePattern[2].step[1].y = pref->GetDouble(AUTO2STEP1Y, 0.0);
    m_drivePattern[2].step[1].t = pref->GetDouble(AUTO2STEP1T, 0.0);
    m_drivePattern[2].step[1].s = pref->GetDouble(AUTO2STEP1S, 0.0);
    // autonomous mode sequence 2 step 2
    m_drivePattern[2].step[2].x = pref->GetDouble(AUTO2STEP2X, 0.0);
    m_drivePattern[2].step[2].y = pref->GetDouble(AUTO2STEP2Y, 0.0);
    m_drivePattern[2].step[2].t = pref->GetDouble(AUTO2STEP2T, 0.0);
    m_drivePattern[2].step[2].s = pref->GetDouble(AUTO2STEP2S, 0.0);
    // autonomous mode sequence 2 step 3
    m_drivePattern[2].step[3].x = pref->GetDouble(AUTO2STEP3X, 0.0);
    m_drivePattern[2].step[3].y = pref->GetDouble(AUTO2STEP3Y, 0.0);
    m_drivePattern[2].step[3].t = pref->GetDouble(AUTO2STEP3T, 0.0);
    m_drivePattern[2].step[3].s = pref->GetDouble(AUTO2STEP3S, 0.0);
    // autonomous mode sequence 2 step 4
    m_drivePattern[2].step[4].x = pref->GetDouble(AUTO2STEP4X, 0.0);
    m_drivePattern[2].step[4].y = pref->GetDouble(AUTO2STEP4Y, 0.0);
    m_drivePattern[2].step[4].t = pref->GetDouble(AUTO2STEP4T, 0.0);
    m_drivePattern[2].step[4].s = pref->GetDouble(AUTO2STEP4S, 0.0);
    // autonomous mode sequence 2 step 5
    m_drivePattern[2].step[5].x = pref->GetDouble(AUTO2STEP5X, 0.0);
    m_drivePattern[2].step[5].y = pref->GetDouble(AUTO2STEP5Y, 0.0);
    m_drivePattern[2].step[5].t = pref->GetDouble(AUTO2STEP5T, 0.0);
    m_drivePattern[2].step[5].s = pref->GetDouble(AUTO2STEP5S, 0.0);

    // autonomous mode sequence 3 step 0
    m_drivePattern[3].step[0].x = pref->GetDouble(AUTO3STEP0X, 0.0);
    m_drivePattern[3].step[0].y = pref->GetDouble(AUTO3STEP0Y, 0.0);
    m_drivePattern[3].step[0].t = pref->GetDouble(AUTO3STEP0T, 0.0);
    m_drivePattern[3].step[0].s = pref->GetDouble(AUTO3STEP0S, 0.0);
    // autonomous mode sequence 3 step 1
    m_drivePattern[3].step[1].x = pref->GetDouble(AUTO3STEP1X, 0.0);
    m_drivePattern[3].step[1].y = pref->GetDouble(AUTO3STEP1Y, 0.0);
    m_drivePattern[3].step[1].t = pref->GetDouble(AUTO3STEP1T, 0.0);
    m_drivePattern[3].step[1].s = pref->GetDouble(AUTO3STEP1S, 0.0);
    // autonomous mode sequence 3 step 2
    m_drivePattern[3].step[2].x = pref->GetDouble(AUTO3STEP2X, 0.0);
    m_drivePattern[3].step[2].y = pref->GetDouble(AUTO3STEP2Y, 0.0);
    m_drivePattern[3].step[2].t = pref->GetDouble(AUTO3STEP2T, 0.0);
    m_drivePattern[3].step[2].s = pref->GetDouble(AUTO3STEP2S, 0.0);
    // autonomous mode sequence 3 step 3
    m_drivePattern[3].step[3].x = pref->GetDouble(AUTO3STEP3X, 0.0);
    m_drivePattern[3].step[3].y = pref->GetDouble(AUTO3STEP3Y, 0.0);
    m_drivePattern[3].step[3].t = pref->GetDouble(AUTO3STEP3T, 0.0);
    m_drivePattern[3].step[3].s = pref->GetDouble(AUTO3STEP3S, 0.0);
    // autonomous mode sequence 3 step 4
    m_drivePattern[3].step[4].x = pref->GetDouble(AUTO3STEP4X, 0.0);
    m_drivePattern[3].step[4].y = pref->GetDouble(AUTO3STEP4Y, 0.0);
    m_drivePattern[3].step[4].t = pref->GetDouble(AUTO3STEP4T, 0.0);
    m_drivePattern[3].step[4].s = pref->GetDouble(AUTO3STEP4S, 0.0);
    // autonomous mode sequence 3 step 5
    m_drivePattern[3].step[5].x = pref->GetDouble(AUTO3STEP5X, 0.0);
    m_drivePattern[3].step[5].y = pref->GetDouble(AUTO3STEP5Y, 0.0);
    m_drivePattern[3].step[5].t = pref->GetDouble(AUTO3STEP5T, 0.0);
    m_drivePattern[3].step[5].s = pref->GetDouble(AUTO3STEP5S, 0.0);

    // autonomous mode sequence 4 step 0
    m_drivePattern[4].step[0].x = pref->GetDouble(AUTO4STEP0X, 0.0);
    m_drivePattern[4].step[0].y = pref->GetDouble(AUTO4STEP0Y, 0.0);
    m_drivePattern[4].step[0].t = pref->GetDouble(AUTO4STEP0T, 0.0);
    m_drivePattern[4].step[0].s = pref->GetDouble(AUTO4STEP0S, 0.0);
    // autonomous mode sequence 4 step 1
    m_drivePattern[4].step[1].x = pref->GetDouble(AUTO4STEP1X, 0.0);
    m_drivePattern[4].step[1].y = pref->GetDouble(AUTO4STEP1Y, 0.0);
    m_drivePattern[4].step[1].t = pref->GetDouble(AUTO4STEP1T, 0.0);
    m_drivePattern[4].step[1].s = pref->GetDouble(AUTO4STEP1S, 0.0);
    // autonomous mode sequence 4 step 2
    m_drivePattern[4].step[2].x = pref->GetDouble(AUTO4STEP2X, 0.0);
    m_drivePattern[4].step[2].y = pref->GetDouble(AUTO4STEP2Y, 0.0);
    m_drivePattern[4].step[2].t = pref->GetDouble(AUTO4STEP2T, 0.0);
    m_drivePattern[4].step[2].s = pref->GetDouble(AUTO4STEP2S, 0.0);
    // autonomous mode sequence 4 step 3
    m_drivePattern[4].step[3].x = pref->GetDouble(AUTO4STEP3X, 0.0);
    m_drivePattern[4].step[3].y = pref->GetDouble(AUTO4STEP3Y, 0.0);
    m_drivePattern[4].step[3].t = pref->GetDouble(AUTO4STEP3T, 0.0);
    m_drivePattern[4].step[3].s = pref->GetDouble(AUTO4STEP3S, 0.0);
    // autonomous mode sequence 4 step 4
    m_drivePattern[4].step[4].x = pref->GetDouble(AUTO4STEP4X, 0.0);
    m_drivePattern[4].step[4].y = pref->GetDouble(AUTO4STEP4Y, 0.0);
    m_drivePattern[4].step[4].t = pref->GetDouble(AUTO4STEP4T, 0.0);
    m_drivePattern[4].step[4].s = pref->GetDouble(AUTO4STEP4S, 0.0);
    // autonomous mode sequence 4 step 5
    m_drivePattern[4].step[5].x = pref->GetDouble(AUTO4STEP5X, 0.0);
    m_drivePattern[4].step[5].y = pref->GetDouble(AUTO4STEP5Y, 0.0);
    m_drivePattern[4].step[5].t = pref->GetDouble(AUTO4STEP5T, 0.0);
    m_drivePattern[4].step[5].s = pref->GetDouble(AUTO4STEP5S, 0.0);

    // autonomous mode sequence 5 step 0
    m_drivePattern[5].step[0].x = pref->GetDouble(AUTO5STEP0X, 0.0);
    m_drivePattern[5].step[0].y = pref->GetDouble(AUTO5STEP0Y, 0.0);
    m_drivePattern[5].step[0].t = pref->GetDouble(AUTO5STEP0T, 0.0);
    m_drivePattern[5].step[0].s = pref->GetDouble(AUTO5STEP0S, 0.0);
    // autonomous mode sequence 5 step 1
    m_drivePattern[5].step[1].x = pref->GetDouble(AUTO5STEP1X, 0.0);
    m_drivePattern[5].step[1].y = pref->GetDouble(AUTO5STEP1Y, 0.0);
    m_drivePattern[5].step[1].t = pref->GetDouble(AUTO5STEP1T, 0.0);
    m_drivePattern[5].step[1].s = pref->GetDouble(AUTO5STEP1S, 0.0);
    // autonomous mode sequence 5 step 2
    m_drivePattern[5].step[2].x = pref->GetDouble(AUTO5STEP2X, 0.0);
    m_drivePattern[5].step[2].y = pref->GetDouble(AUTO5STEP2Y, 0.0);
    m_drivePattern[5].step[2].t = pref->GetDouble(AUTO5STEP2T, 0.0);
    m_drivePattern[5].step[2].s = pref->GetDouble(AUTO5STEP2S, 0.0);
    // autonomous mode sequence 5 step 3
    m_drivePattern[5].step[3].x = pref->GetDouble(AUTO5STEP3X, 0.0);
    m_drivePattern[5].step[3].y = pref->GetDouble(AUTO5STEP3Y, 0.0);
    m_drivePattern[5].step[3].t = pref->GetDouble(AUTO5STEP3T, 0.0);
    m_drivePattern[5].step[3].s = pref->GetDouble(AUTO5STEP3S, 0.0);
    // autonomous mode sequence 5 step 4
    m_drivePattern[5].step[4].x = pref->GetDouble(AUTO5STEP4X, 0.0);
    m_drivePattern[5].step[4].y = pref->GetDouble(AUTO5STEP4Y, 0.0);
    m_drivePattern[5].step[4].t = pref->GetDouble(AUTO5STEP4T, 0.0);
    m_drivePattern[5].step[4].s = pref->GetDouble(AUTO5STEP4S, 0.0);
    // autonomous mode sequence 5 step 5
    m_drivePattern[5].step[5].x = pref->GetDouble(AUTO5STEP5X, 0.0);
    m_drivePattern[5].step[5].y = pref->GetDouble(AUTO5STEP5Y, 0.0);
    m_drivePattern[5].step[5].t = pref->GetDouble(AUTO5STEP5T, 0.0);
    m_drivePattern[5].step[5].s = pref->GetDouble(AUTO5STEP5S, 0.0);

    // autonomous mode sequence 6 step 0
    m_drivePattern[6].step[0].x = pref->GetDouble(AUTO6STEP0X, 0.0);
    m_drivePattern[6].step[0].y = pref->GetDouble(AUTO6STEP0Y, 0.0);
    m_drivePattern[6].step[0].t = pref->GetDouble(AUTO6STEP0T, 0.0);
    m_drivePattern[6].step[0].s = pref->GetDouble(AUTO6STEP0S, 0.0);
    // autonomous mode sequence 6 step 1
    m_drivePattern[6].step[1].x = pref->GetDouble(AUTO6STEP1X, 0.0);
    m_drivePattern[6].step[1].y = pref->GetDouble(AUTO6STEP1Y, 0.0);
    m_drivePattern[6].step[1].t = pref->GetDouble(AUTO6STEP1T, 0.0);
    m_drivePattern[6].step[1].s = pref->GetDouble(AUTO6STEP1S, 0.0);
    // autonomous mode sequence 6 step 2
    m_drivePattern[6].step[2].x = pref->GetDouble(AUTO6STEP2X, 0.0);
    m_drivePattern[6].step[2].y = pref->GetDouble(AUTO6STEP2Y, 0.0);
    m_drivePattern[6].step[2].t = pref->GetDouble(AUTO6STEP2T, 0.0);
    m_drivePattern[6].step[2].s = pref->GetDouble(AUTO6STEP2S, 0.0);
    // autonomous mode sequence 6 step 3
    m_drivePattern[6].step[3].x = pref->GetDouble(AUTO6STEP3X, 0.0);
    m_drivePattern[6].step[3].y = pref->GetDouble(AUTO6STEP3Y, 0.0);
    m_drivePattern[6].step[3].t = pref->GetDouble(AUTO6STEP3T, 0.0);
    m_drivePattern[6].step[3].s = pref->GetDouble(AUTO6STEP3S, 0.0);
    // autonomous mode sequence 6 step 4
    m_drivePattern[6].step[4].x = pref->GetDouble(AUTO6STEP4X, 0.0);
    m_drivePattern[6].step[4].y = pref->GetDouble(AUTO6STEP4Y, 0.0);
    m_drivePattern[6].step[4].t = pref->GetDouble(AUTO6STEP4T, 0.0);
    m_drivePattern[6].step[4].s = pref->GetDouble(AUTO6STEP4S, 0.0);
    // autonomous mode sequence 6 step 5
    m_drivePattern[6].step[5].x = pref->GetDouble(AUTO6STEP5X, 0.0);
    m_drivePattern[6].step[5].y = pref->GetDouble(AUTO6STEP5Y, 0.0);
    m_drivePattern[6].step[5].t = pref->GetDouble(AUTO6STEP5T, 0.0);
    m_drivePattern[6].step[5].s = pref->GetDouble(AUTO6STEP5S, 0.0);

    // autonomous mode sequence 7 step 0
    m_drivePattern[7].step[0].x = pref->GetDouble(AUTO7STEP0X, 0.0);
    m_drivePattern[7].step[0].y = pref->GetDouble(AUTO7STEP0Y, 0.0);
    m_drivePattern[7].step[0].t = pref->GetDouble(AUTO7STEP0T, 0.0);
    m_drivePattern[7].step[0].s = pref->GetDouble(AUTO7STEP0S, 0.0);
    // autonomous mode sequence 7 step 1
    m_drivePattern[7].step[1].x = pref->GetDouble(AUTO7STEP1X, 0.0);
    m_drivePattern[7].step[1].y = pref->GetDouble(AUTO7STEP1Y, 0.0);
    m_drivePattern[7].step[1].t = pref->GetDouble(AUTO7STEP1T, 0.0);
    m_drivePattern[7].step[1].s = pref->GetDouble(AUTO7STEP1S, 0.0);
    // autonomous mode sequence 7 step 2
    m_drivePattern[7].step[2].x = pref->GetDouble(AUTO7STEP2X, 0.0);
    m_drivePattern[7].step[2].y = pref->GetDouble(AUTO7STEP2Y, 0.0);
    m_drivePattern[7].step[2].t = pref->GetDouble(AUTO7STEP2T, 0.0);
    m_drivePattern[7].step[2].s = pref->GetDouble(AUTO7STEP2S, 0.0);
    // autonomous mode sequence 7 step 3
    m_drivePattern[7].step[3].x = pref->GetDouble(AUTO7STEP3X, 0.0);
    m_drivePattern[7].step[3].y = pref->GetDouble(AUTO7STEP3Y, 0.0);
    m_drivePattern[7].step[3].t = pref->GetDouble(AUTO7STEP3T, 0.0);
    m_drivePattern[7].step[3].s = pref->GetDouble(AUTO7STEP3S, 0.0);
    // autonomous mode sequence 7 step 4
    m_drivePattern[7].step[4].x = pref->GetDouble(AUTO7STEP4X, 0.0);
    m_drivePattern[7].step[4].y = pref->GetDouble(AUTO7STEP4Y, 0.0);
    m_drivePattern[7].step[4].t = pref->GetDouble(AUTO7STEP4T, 0.0);
    m_drivePattern[7].step[4].s = pref->GetDouble(AUTO7STEP4S, 0.0);
    // autonomous mode sequence 7 step 5
    m_drivePattern[7].step[5].x = pref->GetDouble(AUTO7STEP5X, 0.0);
    m_drivePattern[7].step[5].y = pref->GetDouble(AUTO7STEP5Y, 0.0);
    m_drivePattern[7].step[5].t = pref->GetDouble(AUTO7STEP5T, 0.0);
    m_drivePattern[7].step[5].s = pref->GetDouble(AUTO7STEP5S, 0.0);

    // autonomous mode sequence 8 step 0
    m_drivePattern[8].step[0].x = pref->GetDouble(AUTO8STEP0X, 0.0);
    m_drivePattern[8].step[0].y = pref->GetDouble(AUTO8STEP0Y, 0.0);
    m_drivePattern[8].step[0].t = pref->GetDouble(AUTO8STEP0T, 0.0);
    m_drivePattern[8].step[0].s = pref->GetDouble(AUTO8STEP0S, 0.0);
    // autonomous mode sequence 8 step 1
    m_drivePattern[8].step[1].x = pref->GetDouble(AUTO8STEP1X, 0.0);
    m_drivePattern[8].step[1].y = pref->GetDouble(AUTO8STEP1Y, 0.0);
    m_drivePattern[8].step[1].t = pref->GetDouble(AUTO8STEP1T, 0.0);
    m_drivePattern[8].step[1].s = pref->GetDouble(AUTO8STEP1S, 0.0);
    // autonomous mode sequence 8 step 2
    m_drivePattern[8].step[2].x = pref->GetDouble(AUTO8STEP2X, 0.0);
    m_drivePattern[8].step[2].y = pref->GetDouble(AUTO8STEP2Y, 0.0);
    m_drivePattern[8].step[2].t = pref->GetDouble(AUTO8STEP2T, 0.0);
    m_drivePattern[8].step[2].s = pref->GetDouble(AUTO8STEP2S, 0.0);
    // autonomous mode sequence 8 step 3
    m_drivePattern[8].step[3].x = pref->GetDouble(AUTO8STEP3X, 0.0);
    m_drivePattern[8].step[3].y = pref->GetDouble(AUTO8STEP3Y, 0.0);
    m_drivePattern[8].step[3].t = pref->GetDouble(AUTO8STEP3T, 0.0);
    m_drivePattern[8].step[3].s = pref->GetDouble(AUTO8STEP3S, 0.0);
    // autonomous mode sequence 8 step 4
    m_drivePattern[8].step[4].x = pref->GetDouble(AUTO8STEP4X, 0.0);
    m_drivePattern[8].step[4].y = pref->GetDouble(AUTO8STEP4Y, 0.0);
    m_drivePattern[8].step[4].t = pref->GetDouble(AUTO8STEP4T, 0.0);
    m_drivePattern[8].step[4].s = pref->GetDouble(AUTO8STEP4S, 0.0);
    // autonomous mode sequence 8 step 5
    m_drivePattern[8].step[5].x = pref->GetDouble(AUTO8STEP5X, 0.0);
    m_drivePattern[8].step[5].y = pref->GetDouble(AUTO8STEP5Y, 0.0);
    m_drivePattern[8].step[5].t = pref->GetDouble(AUTO8STEP5T, 0.0);
    m_drivePattern[8].step[5].s = pref->GetDouble(AUTO8STEP5S, 0.0);

    // autonomous mode sequence 9 step 0
    m_drivePattern[9].step[0].x = pref->GetDouble(AUTO9STEP0X, 0.0);
    m_drivePattern[9].step[0].y = pref->GetDouble(AUTO9STEP0Y, 0.0);
    m_drivePattern[9].step[0].t = pref->GetDouble(AUTO9STEP0T, 0.0);
    m_drivePattern[9].step[0].s = pref->GetDouble(AUTO9STEP0S, 0.0);
    // autonomous mode sequence 9 step 1
    m_drivePattern[9].step[1].x = pref->GetDouble(AUTO9STEP1X, 0.0);
    m_drivePattern[9].step[1].y = pref->GetDouble(AUTO9STEP1Y, 0.0);
    m_drivePattern[9].step[1].t = pref->GetDouble(AUTO9STEP1T, 0.0);
    m_drivePattern[9].step[1].s = pref->GetDouble(AUTO9STEP1S, 0.0);
    // autonomous mode sequence 9 step 2
    m_drivePattern[9].step[2].x = pref->GetDouble(AUTO9STEP2X, 0.0);
    m_drivePattern[9].step[2].y = pref->GetDouble(AUTO9STEP2Y, 0.0);
    m_drivePattern[9].step[2].t = pref->GetDouble(AUTO9STEP2T, 0.0);
    m_drivePattern[9].step[2].s = pref->GetDouble(AUTO9STEP2S, 0.0);
    // autonomous mode sequence 9 step 3
    m_drivePattern[9].step[3].x = pref->GetDouble(AUTO9STEP3X, 0.0);
    m_drivePattern[9].step[3].y = pref->GetDouble(AUTO9STEP3Y, 0.0);
    m_drivePattern[9].step[3].t = pref->GetDouble(AUTO9STEP3T, 0.0);
    m_drivePattern[9].step[3].s = pref->GetDouble(AUTO9STEP3S, 0.0);
    // autonomous mode sequence 9 step 4
    m_drivePattern[9].step[4].x = pref->GetDouble(AUTO9STEP4X, 0.0);
    m_drivePattern[9].step[4].y = pref->GetDouble(AUTO9STEP4Y, 0.0);
    m_drivePattern[9].step[4].t = pref->GetDouble(AUTO9STEP4T, 0.0);
    m_drivePattern[9].step[4].s = pref->GetDouble(AUTO9STEP4S, 0.0);
    // autonomous mode sequence 9 step 5
    m_drivePattern[9].step[5].x = pref->GetDouble(AUTO9STEP5X, 0.0);
    m_drivePattern[9].step[5].y = pref->GetDouble(AUTO9STEP5Y, 0.0);
    m_drivePattern[9].step[5].t = pref->GetDouble(AUTO9STEP5T, 0.0);
    m_drivePattern[9].step[5].s = pref->GetDouble(AUTO9STEP5S, 0.0);
}

void AutoCommand::SaveAutoPreferences()
{
    printf("AutoCommand::SaveAutoPreferences\n");

    Preferences *pref = Preferences::GetInstance();

    // autonomous mode sequence 0 step 0
    pref->PutDouble(AUTO0STEP0X, m_drivePattern[0].step[0].x);
    pref->PutDouble(AUTO0STEP0Y, m_drivePattern[0].step[0].y);
    pref->PutDouble(AUTO0STEP0T, m_drivePattern[0].step[0].t);
    pref->PutDouble(AUTO0STEP0S, m_drivePattern[0].step[0].s);
    // autonomous mode sequence 0 step 1
    pref->PutDouble(AUTO0STEP1X, m_drivePattern[0].step[1].x);
    pref->PutDouble(AUTO0STEP1Y, m_drivePattern[0].step[1].y);
    pref->PutDouble(AUTO0STEP1T, m_drivePattern[0].step[1].t);
    pref->PutDouble(AUTO0STEP1S, m_drivePattern[0].step[1].s);
    // autonomous mode sequence 0 step 2
    pref->PutDouble(AUTO0STEP2X, m_drivePattern[0].step[2].x);
    pref->PutDouble(AUTO0STEP2Y, m_drivePattern[0].step[2].y);
    pref->PutDouble(AUTO0STEP2T, m_drivePattern[0].step[2].t);
    pref->PutDouble(AUTO0STEP2S, m_drivePattern[0].step[2].s);
    // autonomous mode sequence 0 step 3
    pref->PutDouble(AUTO0STEP3X, m_drivePattern[0].step[3].x);
    pref->PutDouble(AUTO0STEP3Y, m_drivePattern[0].step[3].y);
    pref->PutDouble(AUTO0STEP3T, m_drivePattern[0].step[3].t);
    pref->PutDouble(AUTO0STEP3S, m_drivePattern[0].step[3].s);
    // autonomous mode sequence 0 step 0
    pref->PutDouble(AUTO0STEP4X, m_drivePattern[0].step[4].x);
    pref->PutDouble(AUTO0STEP4Y, m_drivePattern[0].step[4].y);
    pref->PutDouble(AUTO0STEP4T, m_drivePattern[0].step[4].t);
    pref->PutDouble(AUTO0STEP4S, m_drivePattern[0].step[4].s);
    // autonomous mode sequence 0 step 5
    pref->PutDouble(AUTO0STEP5X, m_drivePattern[0].step[5].x);
    pref->PutDouble(AUTO0STEP5Y, m_drivePattern[0].step[5].y);
    pref->PutDouble(AUTO0STEP5T, m_drivePattern[0].step[5].t);
    pref->PutDouble(AUTO0STEP5S, m_drivePattern[0].step[5].s);

    // autonomous mode sequence 1 step 0
    pref->PutDouble(AUTO1STEP0X, m_drivePattern[1].step[0].x);
    pref->PutDouble(AUTO1STEP0Y, m_drivePattern[1].step[0].y);
    pref->PutDouble(AUTO1STEP0T, m_drivePattern[1].step[0].t);
    pref->PutDouble(AUTO1STEP0S, m_drivePattern[1].step[0].s);
    // autonomous mode sequence 1 step 1
    pref->PutDouble(AUTO1STEP1X, m_drivePattern[1].step[1].x);
    pref->PutDouble(AUTO1STEP1Y, m_drivePattern[1].step[1].y);
    pref->PutDouble(AUTO1STEP1T, m_drivePattern[1].step[1].t);
    pref->PutDouble(AUTO1STEP1S, m_drivePattern[1].step[1].s);
    // autonomous mode sequence 1 step 2
    pref->PutDouble(AUTO1STEP2X, m_drivePattern[1].step[2].x);
    pref->PutDouble(AUTO1STEP2Y, m_drivePattern[1].step[2].y);
    pref->PutDouble(AUTO1STEP2T, m_drivePattern[1].step[2].t);
    pref->PutDouble(AUTO1STEP2S, m_drivePattern[1].step[2].s);
    // autonomous mode sequence 1 step 3
    pref->PutDouble(AUTO1STEP3X, m_drivePattern[1].step[3].x);
    pref->PutDouble(AUTO1STEP3Y, m_drivePattern[1].step[3].y);
    pref->PutDouble(AUTO1STEP3T, m_drivePattern[1].step[3].t);
    pref->PutDouble(AUTO1STEP3S, m_drivePattern[1].step[3].s);
    // autonomous mode sequence 1 step 4
    pref->PutDouble(AUTO1STEP4X, m_drivePattern[1].step[4].x);
    pref->PutDouble(AUTO1STEP4Y, m_drivePattern[1].step[4].y);
    pref->PutDouble(AUTO1STEP4T, m_drivePattern[1].step[4].t);
    pref->PutDouble(AUTO1STEP4S, m_drivePattern[1].step[4].s);
    // autonomous mode sequence 1 step 5
    pref->PutDouble(AUTO1STEP5X, m_drivePattern[1].step[5].x);
    pref->PutDouble(AUTO1STEP5Y, m_drivePattern[1].step[5].y);
    pref->PutDouble(AUTO1STEP5T, m_drivePattern[1].step[5].t);
    pref->PutDouble(AUTO1STEP5S, m_drivePattern[1].step[5].s);

    // autonomous mode sequence 2 step 0
    pref->PutDouble(AUTO2STEP0X, m_drivePattern[2].step[0].x);
    pref->PutDouble(AUTO2STEP0Y, m_drivePattern[2].step[0].y);
    pref->PutDouble(AUTO2STEP0T, m_drivePattern[2].step[0].t);
    pref->PutDouble(AUTO2STEP0S, m_drivePattern[2].step[0].s);
    // autonomous mode sequence 2 step 1
    pref->PutDouble(AUTO2STEP1X, m_drivePattern[2].step[1].x);
    pref->PutDouble(AUTO2STEP1Y, m_drivePattern[2].step[1].y);
    pref->PutDouble(AUTO2STEP1T, m_drivePattern[2].step[1].t);
    pref->PutDouble(AUTO2STEP1S, m_drivePattern[2].step[1].s);
    // autonomous mode sequence 2 step 2
    pref->PutDouble(AUTO2STEP2X, m_drivePattern[2].step[2].x);
    pref->PutDouble(AUTO2STEP2Y, m_drivePattern[2].step[2].y);
    pref->PutDouble(AUTO2STEP2T, m_drivePattern[2].step[2].t);
    pref->PutDouble(AUTO2STEP2S, m_drivePattern[2].step[2].s);
    // autonomous mode sequence 2 step 3
    pref->PutDouble(AUTO2STEP3X, m_drivePattern[2].step[3].x);
    pref->PutDouble(AUTO2STEP3Y, m_drivePattern[2].step[3].y);
    pref->PutDouble(AUTO2STEP3T, m_drivePattern[2].step[3].t);
    pref->PutDouble(AUTO2STEP3S, m_drivePattern[2].step[3].s);
    // autonomous mode sequence 2 step 4
    pref->PutDouble(AUTO2STEP4X, m_drivePattern[2].step[4].x);
    pref->PutDouble(AUTO2STEP4Y, m_drivePattern[2].step[4].y);
    pref->PutDouble(AUTO2STEP4T, m_drivePattern[2].step[4].t);
    pref->PutDouble(AUTO2STEP4S, m_drivePattern[2].step[4].s);
    // autonomous mode sequence 2 step 5
    pref->PutDouble(AUTO2STEP5X, m_drivePattern[2].step[5].x);
    pref->PutDouble(AUTO2STEP5Y, m_drivePattern[2].step[5].y);
    pref->PutDouble(AUTO2STEP5T, m_drivePattern[2].step[5].t);
    pref->PutDouble(AUTO2STEP5S, m_drivePattern[2].step[5].s);

    // autonomous mode sequence 3 step 0
    pref->PutDouble(AUTO3STEP0X, m_drivePattern[3].step[0].x);
    pref->PutDouble(AUTO3STEP0Y, m_drivePattern[3].step[0].y);
    pref->PutDouble(AUTO3STEP0T, m_drivePattern[3].step[0].t);
    pref->PutDouble(AUTO3STEP0S, m_drivePattern[3].step[0].s);
    // autonomous mode sequence 3 step 1
    pref->PutDouble(AUTO3STEP1X, m_drivePattern[3].step[1].x);
    pref->PutDouble(AUTO3STEP1Y, m_drivePattern[3].step[1].y);
    pref->PutDouble(AUTO3STEP1T, m_drivePattern[3].step[1].t);
    pref->PutDouble(AUTO3STEP1S, m_drivePattern[3].step[1].s);
    // autonomous mode sequence 3 step 2
    pref->PutDouble(AUTO3STEP2X, m_drivePattern[3].step[2].x);
    pref->PutDouble(AUTO3STEP2Y, m_drivePattern[3].step[2].y);
    pref->PutDouble(AUTO3STEP2T, m_drivePattern[3].step[2].t);
    pref->PutDouble(AUTO3STEP2S, m_drivePattern[3].step[2].s);
    // autonomous mode sequence 3 step 3
    pref->PutDouble(AUTO3STEP3X, m_drivePattern[3].step[3].x);
    pref->PutDouble(AUTO3STEP3Y, m_drivePattern[3].step[3].y);
    pref->PutDouble(AUTO3STEP3T, m_drivePattern[3].step[3].t);
    pref->PutDouble(AUTO3STEP3S, m_drivePattern[3].step[3].s);
    // autonomous mode sequence 3 step 4
    pref->PutDouble(AUTO3STEP4X, m_drivePattern[3].step[4].x);
    pref->PutDouble(AUTO3STEP4Y, m_drivePattern[3].step[4].y);
    pref->PutDouble(AUTO3STEP4T, m_drivePattern[3].step[4].t);
    pref->PutDouble(AUTO3STEP4S, m_drivePattern[3].step[4].s);
    // autonomous mode sequence 3 step 5
    pref->PutDouble(AUTO3STEP5X, m_drivePattern[3].step[5].x);
    pref->PutDouble(AUTO3STEP5Y, m_drivePattern[3].step[5].y);
    pref->PutDouble(AUTO3STEP5T, m_drivePattern[3].step[5].t);
    pref->PutDouble(AUTO3STEP5S, m_drivePattern[3].step[5].s);

    // autonomous mode sequence 4 step 0
    pref->PutDouble(AUTO4STEP0X, m_drivePattern[4].step[0].x);
    pref->PutDouble(AUTO4STEP0Y, m_drivePattern[4].step[0].y);
    pref->PutDouble(AUTO4STEP0T, m_drivePattern[4].step[0].t);
    pref->PutDouble(AUTO4STEP0S, m_drivePattern[4].step[0].s);
    // autonomous mode sequence 4 step 1
    pref->PutDouble(AUTO4STEP1X, m_drivePattern[4].step[1].x);
    pref->PutDouble(AUTO4STEP1Y, m_drivePattern[4].step[1].y);
    pref->PutDouble(AUTO4STEP1T, m_drivePattern[4].step[1].t);
    pref->PutDouble(AUTO4STEP1S, m_drivePattern[4].step[1].s);
    // autonomous mode sequence 4 step 2
    pref->PutDouble(AUTO4STEP2X, m_drivePattern[4].step[2].x);
    pref->PutDouble(AUTO4STEP2Y, m_drivePattern[4].step[2].y);
    pref->PutDouble(AUTO4STEP2T, m_drivePattern[4].step[2].t);
    pref->PutDouble(AUTO4STEP2S, m_drivePattern[4].step[2].s);
    // autonomous mode sequence 4 step 3
    pref->PutDouble(AUTO4STEP3X, m_drivePattern[4].step[3].x);
    pref->PutDouble(AUTO4STEP3Y, m_drivePattern[4].step[3].y);
    pref->PutDouble(AUTO4STEP3T, m_drivePattern[4].step[3].t);
    pref->PutDouble(AUTO4STEP3S, m_drivePattern[4].step[3].s);
    // autonomous mode sequence 4 step 4
    pref->PutDouble(AUTO4STEP4X, m_drivePattern[4].step[4].x);
    pref->PutDouble(AUTO4STEP4Y, m_drivePattern[4].step[4].y);
    pref->PutDouble(AUTO4STEP4T, m_drivePattern[4].step[4].t);
    pref->PutDouble(AUTO4STEP4S, m_drivePattern[4].step[4].s);
    // autonomous mode sequence 4 step 5
    pref->PutDouble(AUTO4STEP5X, m_drivePattern[4].step[5].x);
    pref->PutDouble(AUTO4STEP5Y, m_drivePattern[4].step[5].y);
    pref->PutDouble(AUTO4STEP5T, m_drivePattern[4].step[5].t);
    pref->PutDouble(AUTO4STEP5S, m_drivePattern[4].step[5].s);

    // autonomous mode sequence 5 step 0
    pref->PutDouble(AUTO5STEP0X, m_drivePattern[5].step[0].x);
    pref->PutDouble(AUTO5STEP0Y, m_drivePattern[5].step[0].y);
    pref->PutDouble(AUTO5STEP0T, m_drivePattern[5].step[0].t);
    pref->PutDouble(AUTO5STEP0S, m_drivePattern[5].step[0].s);
    // autonomous mode sequence 5 step 1
    pref->PutDouble(AUTO5STEP1X, m_drivePattern[5].step[1].x);
    pref->PutDouble(AUTO5STEP1Y, m_drivePattern[5].step[1].y);
    pref->PutDouble(AUTO5STEP1T, m_drivePattern[5].step[1].t);
    pref->PutDouble(AUTO5STEP1S, m_drivePattern[5].step[1].s);
    // autonomous mode sequence 5 step 2
    pref->PutDouble(AUTO5STEP2X, m_drivePattern[5].step[2].x);
    pref->PutDouble(AUTO5STEP2Y, m_drivePattern[5].step[2].y);
    pref->PutDouble(AUTO5STEP2T, m_drivePattern[5].step[2].t);
    pref->PutDouble(AUTO5STEP2S, m_drivePattern[5].step[2].s);
    // autonomous mode sequence 5 step 3
    pref->PutDouble(AUTO5STEP3X, m_drivePattern[5].step[3].x);
    pref->PutDouble(AUTO5STEP3Y, m_drivePattern[5].step[3].y);
    pref->PutDouble(AUTO5STEP3T, m_drivePattern[5].step[3].t);
    pref->PutDouble(AUTO5STEP3S, m_drivePattern[5].step[3].s);
    // autonomous mode sequence 5 step 4
    pref->PutDouble(AUTO5STEP4X, m_drivePattern[5].step[4].x);
    pref->PutDouble(AUTO5STEP4Y, m_drivePattern[5].step[4].y);
    pref->PutDouble(AUTO5STEP4T, m_drivePattern[5].step[4].t);
    pref->PutDouble(AUTO5STEP4S, m_drivePattern[5].step[4].s);
    // autonomous mode sequence 5 step 5
    pref->PutDouble(AUTO5STEP5X, m_drivePattern[5].step[5].x);
    pref->PutDouble(AUTO5STEP5Y, m_drivePattern[5].step[5].y);
    pref->PutDouble(AUTO5STEP5T, m_drivePattern[5].step[5].t);
    pref->PutDouble(AUTO5STEP5S, m_drivePattern[5].step[5].s);

    // autonomous mode sequence 6 step 0
    pref->PutDouble(AUTO6STEP0X, m_drivePattern[6].step[0].x);
    pref->PutDouble(AUTO6STEP0Y, m_drivePattern[6].step[0].y);
    pref->PutDouble(AUTO6STEP0T, m_drivePattern[6].step[0].t);
    pref->PutDouble(AUTO6STEP0S, m_drivePattern[6].step[0].s);
    // autonomous mode sequence 6 step 1
    pref->PutDouble(AUTO6STEP1X, m_drivePattern[6].step[1].x);
    pref->PutDouble(AUTO6STEP1Y, m_drivePattern[6].step[1].y);
    pref->PutDouble(AUTO6STEP1T, m_drivePattern[6].step[1].t);
    pref->PutDouble(AUTO6STEP1S, m_drivePattern[6].step[1].s);
    // autonomous mode sequence 6 step 2
    pref->PutDouble(AUTO6STEP2X, m_drivePattern[6].step[2].x);
    pref->PutDouble(AUTO6STEP2Y, m_drivePattern[6].step[2].y);
    pref->PutDouble(AUTO6STEP2T, m_drivePattern[6].step[2].t);
    pref->PutDouble(AUTO6STEP2S, m_drivePattern[6].step[2].s);
    // autonomous mode sequence 6 step 3
    pref->PutDouble(AUTO6STEP3X, m_drivePattern[6].step[3].x);
    pref->PutDouble(AUTO6STEP3Y, m_drivePattern[6].step[3].y);
    pref->PutDouble(AUTO6STEP3T, m_drivePattern[6].step[3].t);
    pref->PutDouble(AUTO6STEP3S, m_drivePattern[6].step[3].s);
    // autonomous mode sequence 6 step 4
    pref->PutDouble(AUTO6STEP4X, m_drivePattern[6].step[4].x);
    pref->PutDouble(AUTO6STEP4Y, m_drivePattern[6].step[4].y);
    pref->PutDouble(AUTO6STEP4T, m_drivePattern[6].step[4].t);
    pref->PutDouble(AUTO6STEP4S, m_drivePattern[6].step[4].s);
    // autonomous mode sequence 6 step 5
    pref->PutDouble(AUTO6STEP5X, m_drivePattern[6].step[5].x);
    pref->PutDouble(AUTO6STEP5Y, m_drivePattern[6].step[5].y);
    pref->PutDouble(AUTO6STEP5T, m_drivePattern[6].step[5].t);
    pref->PutDouble(AUTO6STEP5S, m_drivePattern[6].step[5].s);

    // autonomous mode sequence 7 step 0
    pref->PutDouble(AUTO7STEP0X, m_drivePattern[7].step[0].x);
    pref->PutDouble(AUTO7STEP0Y, m_drivePattern[7].step[0].y);
    pref->PutDouble(AUTO7STEP0T, m_drivePattern[7].step[0].t);
    pref->PutDouble(AUTO7STEP0S, m_drivePattern[7].step[0].s);
    // autonomous mode sequence 7 step 1
    pref->PutDouble(AUTO7STEP1X, m_drivePattern[7].step[1].x);
    pref->PutDouble(AUTO7STEP1Y, m_drivePattern[7].step[1].y);
    pref->PutDouble(AUTO7STEP1T, m_drivePattern[7].step[1].t);
    pref->PutDouble(AUTO7STEP1S, m_drivePattern[7].step[1].s);
    // autonomous mode sequence 7 step 2
    pref->PutDouble(AUTO7STEP2X, m_drivePattern[7].step[2].x);
    pref->PutDouble(AUTO7STEP2Y, m_drivePattern[7].step[2].y);
    pref->PutDouble(AUTO7STEP2T, m_drivePattern[7].step[2].t);
    pref->PutDouble(AUTO7STEP2S, m_drivePattern[7].step[2].s);
    // autonomous mode sequence 7 step 3
    pref->PutDouble(AUTO7STEP3X, m_drivePattern[7].step[3].x);
    pref->PutDouble(AUTO7STEP3Y, m_drivePattern[7].step[3].y);
    pref->PutDouble(AUTO7STEP3T, m_drivePattern[7].step[3].t);
    pref->PutDouble(AUTO7STEP3S, m_drivePattern[7].step[3].s);
    // autonomous mode sequence 7 step 4
    pref->PutDouble(AUTO7STEP4X, m_drivePattern[7].step[4].x);
    pref->PutDouble(AUTO7STEP4Y, m_drivePattern[7].step[4].y);
    pref->PutDouble(AUTO7STEP4T, m_drivePattern[7].step[4].t);
    pref->PutDouble(AUTO7STEP4S, m_drivePattern[7].step[4].s);
    // autonomous mode sequence 7 step 5
    pref->PutDouble(AUTO7STEP5X, m_drivePattern[7].step[5].x);
    pref->PutDouble(AUTO7STEP5Y, m_drivePattern[7].step[5].y);
    pref->PutDouble(AUTO7STEP5T, m_drivePattern[7].step[5].t);
    pref->PutDouble(AUTO7STEP5S, m_drivePattern[7].step[5].s);

    // autonomous mode sequence 8 step 0
    pref->PutDouble(AUTO8STEP0X, m_drivePattern[8].step[0].x);
    pref->PutDouble(AUTO8STEP0Y, m_drivePattern[8].step[0].y);
    pref->PutDouble(AUTO8STEP0T, m_drivePattern[8].step[0].t);
    pref->PutDouble(AUTO8STEP0S, m_drivePattern[8].step[0].s);
    // autonomous mode sequence 8 step 1
    pref->PutDouble(AUTO8STEP1X, m_drivePattern[8].step[1].x);
    pref->PutDouble(AUTO8STEP1Y, m_drivePattern[8].step[1].y);
    pref->PutDouble(AUTO8STEP1T, m_drivePattern[8].step[1].t);
    pref->PutDouble(AUTO8STEP1S, m_drivePattern[8].step[1].s);
    // autonomous mode sequence 8 step 2
    pref->PutDouble(AUTO8STEP2X, m_drivePattern[8].step[2].x);
    pref->PutDouble(AUTO8STEP2Y, m_drivePattern[8].step[2].y);
    pref->PutDouble(AUTO8STEP2T, m_drivePattern[8].step[2].t);
    pref->PutDouble(AUTO8STEP2S, m_drivePattern[8].step[2].s);
    // autonomous mode sequence 8 step 3
    pref->PutDouble(AUTO8STEP3X, m_drivePattern[8].step[3].x);
    pref->PutDouble(AUTO8STEP3Y, m_drivePattern[8].step[3].y);
    pref->PutDouble(AUTO8STEP3T, m_drivePattern[8].step[3].t);
    pref->PutDouble(AUTO8STEP3S, m_drivePattern[8].step[3].s);
    // autonomous mode sequence 8 step 4
    pref->PutDouble(AUTO8STEP4X, m_drivePattern[8].step[4].x);
    pref->PutDouble(AUTO8STEP4Y, m_drivePattern[8].step[4].y);
    pref->PutDouble(AUTO8STEP4T, m_drivePattern[8].step[4].t);
    pref->PutDouble(AUTO8STEP4S, m_drivePattern[8].step[4].s);
    // autonomous mode sequence 8 step 5
    pref->PutDouble(AUTO8STEP5X, m_drivePattern[8].step[5].x);
    pref->PutDouble(AUTO8STEP5Y, m_drivePattern[8].step[5].y);
    pref->PutDouble(AUTO8STEP5T, m_drivePattern[8].step[5].t);
    pref->PutDouble(AUTO8STEP5S, m_drivePattern[8].step[5].s);

    // autonomous mode sequence 9 step 0
    pref->PutDouble(AUTO9STEP0X, m_drivePattern[9].step[0].x);
    pref->PutDouble(AUTO9STEP0Y, m_drivePattern[9].step[0].y);
    pref->PutDouble(AUTO9STEP0T, m_drivePattern[9].step[0].t);
    pref->PutDouble(AUTO9STEP0S, m_drivePattern[9].step[0].s);
    // autonomous mode sequence 9 step 1
    pref->PutDouble(AUTO9STEP1X, m_drivePattern[9].step[1].x);
    pref->PutDouble(AUTO9STEP1Y, m_drivePattern[9].step[1].y);
    pref->PutDouble(AUTO9STEP1T, m_drivePattern[9].step[1].t);
    pref->PutDouble(AUTO9STEP1S, m_drivePattern[9].step[1].s);
    // autonomous mode sequence 9 step 2
    pref->PutDouble(AUTO9STEP2X, m_drivePattern[9].step[2].x);
    pref->PutDouble(AUTO9STEP2Y, m_drivePattern[9].step[2].y);
    pref->PutDouble(AUTO9STEP2T, m_drivePattern[9].step[2].t);
    pref->PutDouble(AUTO9STEP2S, m_drivePattern[9].step[2].s);
    // autonomous mode sequence 9 step 3
    pref->PutDouble(AUTO9STEP3X, m_drivePattern[9].step[3].x);
    pref->PutDouble(AUTO9STEP3Y, m_drivePattern[9].step[3].y);
    pref->PutDouble(AUTO9STEP3T, m_drivePattern[9].step[3].t);
    pref->PutDouble(AUTO9STEP3S, m_drivePattern[9].step[3].s);
    // autonomous mode sequence 9 step 4
    pref->PutDouble(AUTO9STEP4X, m_drivePattern[9].step[4].x);
    pref->PutDouble(AUTO9STEP4Y, m_drivePattern[9].step[4].y);
    pref->PutDouble(AUTO9STEP4T, m_drivePattern[9].step[4].t);
    pref->PutDouble(AUTO9STEP4S, m_drivePattern[9].step[4].s);
    // autonomous mode sequence 9 step 5
    pref->PutDouble(AUTO9STEP5X, m_drivePattern[9].step[5].x);
    pref->PutDouble(AUTO9STEP5Y, m_drivePattern[9].step[5].y);
    pref->PutDouble(AUTO9STEP5T, m_drivePattern[9].step[5].t);
    pref->PutDouble(AUTO9STEP5S, m_drivePattern[9].step[5].s);

    pref->Save();
}

void AutoCommand::SetDrivePattern( int newPattern )
{
    if (newPattern != m_pattern) {
	printf("AutoCommand::SetDrivePattern: pattern was %d changed to %d\n",
	       m_pattern, newPattern);
	GetDashboardSettings();
	m_pattern = newPattern;
	PutDashboardSettings();
    }
}

void AutoCommand::GetDashboardSettings()
{
    printf("AutoCommand::GetDashboardSettings\n");

    DrivePattern *dp = &m_drivePattern[ m_pattern ];

    dp->step[0].x = SmartDashboard::GetNumber(STEP0X);
    dp->step[0].y = SmartDashboard::GetNumber(STEP0Y);
    dp->step[0].t = SmartDashboard::GetNumber(STEP0T);
    dp->step[0].s = SmartDashboard::GetNumber(STEP0S);

    dp->step[1].x = SmartDashboard::GetNumber(STEP1X);
    dp->step[1].y = SmartDashboard::GetNumber(STEP1Y);
    dp->step[1].t = SmartDashboard::GetNumber(STEP1T);
    dp->step[1].s = SmartDashboard::GetNumber(STEP1S);

    dp->step[2].x = SmartDashboard::GetNumber(STEP2X);
    dp->step[2].y = SmartDashboard::GetNumber(STEP2Y);
    dp->step[2].t = SmartDashboard::GetNumber(STEP2T);
    dp->step[2].s = SmartDashboard::GetNumber(STEP2S);

    dp->step[3].x = SmartDashboard::GetNumber(STEP3X);
    dp->step[3].y = SmartDashboard::GetNumber(STEP3Y);
    dp->step[3].t = SmartDashboard::GetNumber(STEP3T);
    dp->step[3].s = SmartDashboard::GetNumber(STEP3S);

    dp->step[4].x = SmartDashboard::GetNumber(STEP4X);
    dp->step[4].y = SmartDashboard::GetNumber(STEP4Y);
    dp->step[4].t = SmartDashboard::GetNumber(STEP4T);
    dp->step[4].s = SmartDashboard::GetNumber(STEP4S);

    dp->step[5].x = SmartDashboard::GetNumber(STEP5X);
    dp->step[5].y = SmartDashboard::GetNumber(STEP5Y);
    dp->step[5].t = SmartDashboard::GetNumber(STEP5T);
    dp->step[5].s = SmartDashboard::GetNumber(STEP5S);
}

void AutoCommand::PutDashboardSettings()
{
    printf("AutoCommand::PutDashboardSettings\n");

    SmartDashboard::PutNumber(PATTERN, m_pattern);

    DrivePattern *dp = &m_drivePattern[ m_pattern ];

    SmartDashboard::PutNumber(STEP0X, dp->step[0].x);
    SmartDashboard::PutNumber(STEP0Y, dp->step[0].y);
    SmartDashboard::PutNumber(STEP0T, dp->step[0].t);
    SmartDashboard::PutNumber(STEP0S, dp->step[0].s);

    SmartDashboard::PutNumber(STEP1X, dp->step[1].x);
    SmartDashboard::PutNumber(STEP1Y, dp->step[1].y);
    SmartDashboard::PutNumber(STEP1T, dp->step[1].t);
    SmartDashboard::PutNumber(STEP1S, dp->step[1].s);

    SmartDashboard::PutNumber(STEP2X, dp->step[2].x);
    SmartDashboard::PutNumber(STEP2Y, dp->step[2].y);
    SmartDashboard::PutNumber(STEP2T, dp->step[2].t);
    SmartDashboard::PutNumber(STEP2S, dp->step[2].s);

    SmartDashboard::PutNumber(STEP3X, dp->step[3].x);
    SmartDashboard::PutNumber(STEP3Y, dp->step[3].y);
    SmartDashboard::PutNumber(STEP3T, dp->step[3].t);
    SmartDashboard::PutNumber(STEP3S, dp->step[3].s);

    SmartDashboard::PutNumber(STEP4X, dp->step[4].x);
    SmartDashboard::PutNumber(STEP4Y, dp->step[4].y);
    SmartDashboard::PutNumber(STEP4T, dp->step[4].t);
    SmartDashboard::PutNumber(STEP4S, dp->step[4].s);

    SmartDashboard::PutNumber(STEP5X, dp->step[5].x);
    SmartDashboard::PutNumber(STEP5Y, dp->step[5].y);
    SmartDashboard::PutNumber(STEP5T, dp->step[5].t);
    SmartDashboard::PutNumber(STEP5S, dp->step[5].s);
}

void AutoCommand::Execute()
{
}

bool AutoCommand::IsFinished()
{
    // this should return true when all commands are done
    return false;
}

void AutoCommand::End()
{
    printf("AutoCommand::End\n");
}

void AutoCommand::Interrupted()
{
    printf("AutoCommand::Interrupted\n");
    Robot::blinkyLight()->Set(0.0);
}

AutoSelect::AutoSelect( AutoCommand *cmd ) : Command("AutoSelect")
{
    m_auto = cmd;
}

AutoSelect:: ~AutoSelect() {}

void AutoSelect::Initialize()
{
    printf("AutoSelect::Initialize\n");
}

void AutoSelect::Execute()
{
    m_auto->SetDrivePattern(Robot::oi()->GetAuto());
}

bool AutoSelect::IsFinished() { return false; }

void AutoSelect::End() {}

void AutoSelect::Interrupted() {}

AutoSave::AutoSave( AutoCommand *cmd ) : Command("AutoSave")
{
    m_auto = cmd;
}

AutoSave::~AutoSave() {}

void AutoSave::Initialize()
{
    printf("AutoSave::Initialize\n");
    m_auto->GetDashboardSettings();
    m_auto->SaveAutoPreferences();
}

void AutoSave::Execute() {}

bool AutoSave::IsFinished() { return true; }

void AutoSave::End() {}

void AutoSave::Interrupted() {}

