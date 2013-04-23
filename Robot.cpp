// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"

// Subsystems
#include "BlinkyLight.h"
#include "Climber.h"
#include "DriveBase.h"
#include "RateGyro.h"
#include "Shooter.h"
#include "TripleSolenoid.h"

// Commands
#include "AutoCommand.h"

// Operator Interface
#include "OI.h"

// constant strings used for preferences file entries
const char KEY_SPEED_SHORT[]  = "ShooterSpeedShort";
const char KEY_SPEED_MID_2[]  = "ShooterSpeedMid2";
const char KEY_SPEED_MID_3[]  = "ShooterSpeedMid3";
const char KEY_SPEED_LONG_2[] = "ShooterSpeedLong2";
const char KEY_SPEED_LONG_3[] = "ShooterSpeedLong3";


Robot::Robot()
{
    // printf("Robot::Robot() has been called!\n");
}

Robot::~Robot()
{
    // printf("BWA HA HA HA HA!  The Robot cannot be destroyed!\n");
}

void Robot::RobotInit()
{
    // printf("Robot::RobotInit()\n");

    m_oi = new OI();

    // subsystems

    m_compressor = new Compressor( DIGITAL_PRESSURE_SWITCH, RELAY_COMPRESSOR );

    m_driveBase = new DriveBase( PWM_DRIVE_LEFT, PWM_DRIVE_RIGHT,
    				 PWM_DRIVE_REAR, ANALOG_GYRO );

    m_climber = new Climber( SOLENOID_CLIMBER_EXTEND );

    m_shooter = new Shooter( CAN_SHOOTER,
			     SOLENOID_SHOOTER_EXTEND,
			     DIGITAL_SHOOTER_CENTER,
			     SOLENOID_SHOOTER_LOAD );
    
    m_blinkyLight = new BlinkyLight( PWM_BLINKY );

    // commands

    m_autonomousCommand = new AutoCommand();

    // link operator controls to commands

    m_oi->Initialize();

    // tune system parameters

    Preferences *pref = Preferences::GetInstance();
    m_speed_short  = pref->GetDouble(KEY_SPEED_SHORT,  DEFAULT_SPEED_SHORT);
    m_speed_mid_2  = pref->GetDouble(KEY_SPEED_MID_2,  DEFAULT_SPEED_MID_2);
    m_speed_mid_3  = pref->GetDouble(KEY_SPEED_MID_3,  DEFAULT_SPEED_MID_3);
    m_speed_long_2 = pref->GetDouble(KEY_SPEED_LONG_2, DEFAULT_SPEED_LONG_2);
    m_speed_long_3 = pref->GetDouble(KEY_SPEED_LONG_3, DEFAULT_SPEED_LONG_3);

    // Now that everything else is set up, start the compressor
    m_compressor->Start();

    SmartDashboard::PutData("scheduler", Scheduler::GetInstance());
}

void Robot::Cancel()
{
    printf("Robot::Cancel\n");
    if (m_autonomousCommand->IsRunning()) {
	m_autonomousCommand->Cancel();
    }
    m_driveBase->Stop();
    m_blinkyLight->Set(0.0);
}
	
void Robot::DisabledInit()
{
    printf("Robot::DisabledInit\n");
    Cancel();
}

void Robot::DisabledPeriodic()
{
    Scheduler::GetInstance()->Run();
}

void Robot::AutonomousInit()
{
    printf("Robot::AutonomousInit\n");
    Cancel();
    m_autonomousCommand->Start();
}
    
void Robot::AutonomousPeriodic()
{
    Scheduler::GetInstance()->Run();
}
    
void Robot::TeleopInit()
{
    printf("Robot::TeleopInit\n");
    Cancel();
}
    
void Robot::TeleopPeriodic()
{
    Scheduler::GetInstance()->Run();
}

void Robot::TestInit()
{
    printf("Robot::TestInit\n");
    Cancel();
}

void Robot::TestPeriodic()
{
    LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot);

