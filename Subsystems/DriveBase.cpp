// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "DriveBase.h"

DriveBase::DriveBase( int leftMotorChannel,
		      int rightMotorChannel,
		      int rearMotorChannel,
		      int gyroAnalogChannel )
    : Subsystem("DriveBase"),
    m_left(NULL),
    m_right(NULL),
    m_rear(NULL),
    m_drive3(NULL),
    m_gyro(NULL),
    m_defaultCommand(NULL),
    m_started(false)
{
    LiveWindow *lw = LiveWindow::GetInstance();

    // Just to make things interesting (and because we
    // didn't have enough motor controllers of either type),
    // the left and right motors are controlled by Victor 888s
    // and the rear motors are controlled by Talons.
    m_left  = new Victor(leftMotorChannel);
    lw->AddActuator("DriveBase", "Left", dynamic_cast<Victor*>(m_left));
    m_right = new Victor(rightMotorChannel);
    lw->AddActuator("DriveBase", "Right", dynamic_cast<Victor*>(m_right));
    m_rear  = new Talon(rearMotorChannel);
    lw->AddActuator("DriveBase", "Rear", dynamic_cast<Talon*>(m_rear));

    // Our drive base has front-left, front-right and rear wheels
    // but RobotDrive3 (and RobotDrive, from which it is derived)
    // expect (left-)front, left-rear and right-rear motors.
    m_drive3 = new RobotDrive3(m_rear, m_right, m_left);

    m_gyro = new RateGyro(1, gyroAnalogChannel);
    lw->AddSensor("DriveBase", "Gyro", m_gyro);

    Stop();
}

DriveBase::~DriveBase()
{
    SetDefaultCommand(NULL);
    Scheduler::GetInstance()->Remove(m_defaultCommand);
    delete m_defaultCommand;
    m_defaultCommand = NULL;

    Stop();

    delete m_gyro;
    delete m_drive3;
    delete m_rear;
    delete m_right;
    delete m_left;
}
    
void DriveBase::InitDefaultCommand()
{
    if (!m_defaultCommand) {
	m_defaultCommand = new DriveCommand();
	SetDefaultCommand(m_defaultCommand);
    }
}

void DriveBase::Stop()
{
    // printf("DriveBase::Stop\n");

    // stop and disable all motors
    m_drive3->StopMotor();
    // watchdogs not needed while stopped
    m_drive3->SetSafetyEnabled(false);
    dynamic_cast<MotorSafety*>(m_rear)->SetSafetyEnabled(false);
    dynamic_cast<MotorSafety*>(m_right)->SetSafetyEnabled(false);
    dynamic_cast<MotorSafety*>(m_left)->SetSafetyEnabled(false);
    // remember that we're stopped
    m_started = false;
}

void DriveBase::Start()
{
    if (!m_started) {
	// printf("DriveBase::Start\n");
	// set all motors to 0.0 in order to feed their watchdogs
	m_drive3->SetLeftRightMotorOutputs(0.0, 0.0);
	// now enable the watchdogs
	m_drive3->SetSafetyEnabled(true);
	dynamic_cast<MotorSafety*>(m_rear)->SetSafetyEnabled(true);
	dynamic_cast<MotorSafety*>(m_right)->SetSafetyEnabled(true);
	dynamic_cast<MotorSafety*>(m_left)->SetSafetyEnabled(true);
	// remember that we're started
	m_started = true;
    }
}

void DriveBase::Drive3( float x, float y, float twist )
{
    if (!m_started) Start();

    // Reduce the sensitivity to the "twist" control.
    // Add gyro compensation (adjust the "500" for best PID response).
    // Also reverse the direction, since our drive base is a mirror
    // image of what RobotDrive3 expects.
    twist = -( twist / 2. - m_gyro->GetRate() / 500. );

    // limit the twist range to avoid normalization problems
    if (twist < -1.0) twist = -1.0;
    if (twist > 1.0) twist = 1.0;

    m_drive3->HolonomicDrive_Cartesian( x, y, twist );
}

