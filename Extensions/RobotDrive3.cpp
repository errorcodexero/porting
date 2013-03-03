// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "RobotDrive.h"
#include "CANJaguar.h"
#include "GenericHID.h"
#include "Joystick.h"
#include "Jaguar.h"
#include "NetworkCommunication/UsageReporting.h"
#include "Utility.h"
#include "WPIErrors.h"

#include "RobotDrive3.h"

// Constructor for RobotDrive3 with 3 motors specified by channel numbers.
// This assumes Jaguar motor controllers in PWM mode.
// Hack: use the RobotDrive constructor for a 2-wheel drive system, then
// add the third motor and set all the outputs to zero again.  This is
// inelegant because the rear wheels get set twice, but it allows us to
// implement RobotDrive3 as a derived class rather than doing a
// copy-and-paste of RobotDrive.
// @param frontMotorChannel The front PWM channel used to drive the robot.
// @param leftMotorChannel The rear left PWM channel used to drive the robot.
// @param rightMotorChannel the rear right PWM channel used to drive the robot.

RobotDrive3::RobotDrive3(UINT32 frontMotorChannel,
			 UINT32 rearLeftMotorChannel,
			 UINT32 rearRightMotorChannel)
    : RobotDrive(rearLeftMotorChannel, rearRightMotorChannel)
{
    m_frontLeftMotor = new Jaguar(frontMotorChannel);
    SetLeftRightMotorOutputs(0.0, 0.0);
    m_safetyHelper->SetSafetyEnabled(false);
}

// Constructor for RobotDrive3 with 3 motors specified as SpeedController
// objects.  The SpeedController version of the constructor enables programs
// to use the RobotDrive3 classes with subclasses of the SpeedController
// objects, for example, versions with ramping or reshaping of the curve
// to suit motor bias or deadband elimination.
// @param leftMotor The left SpeedController object used to drive the robot.
// @param rightMotor the right SpeedController object used to drive the robot.

RobotDrive3::RobotDrive3(SpeedController *frontMotor,
			 SpeedController *rearLeftMotor,
			 SpeedController *rearRightMotor)
    : RobotDrive(rearLeftMotor, rearRightMotor)
{
    if (frontMotor == NULL)
    {
	wpi_setWPIError(NullParameter);
	m_rearLeftMotor = m_rearRightMotor = NULL;
	return;
    }
    m_frontLeftMotor = frontMotor;
    SetLeftRightMotorOutputs(0.0, 0.0);
    m_safetyHelper->SetSafetyEnabled(false);
}

RobotDrive3::RobotDrive3(SpeedController &frontMotor,
			 SpeedController &rearLeftMotor,
			 SpeedController &rearRightMotor)
    : RobotDrive(rearLeftMotor, rearRightMotor)
{
    m_frontLeftMotor = &frontMotor;
    SetLeftRightMotorOutputs(0.0, 0.0);
    m_safetyHelper->SetSafetyEnabled(false);
}

// Holonomic Drive method for 3-wheel Omni-wheeled robots.
// @param x   The speed that the robot should drive in the X direction.
// [-1.0..1.0], positive X is to the right.
// @param y   The speed that the robot should drive in the Y direction.
// [-1.0..1.0], negative Y is forward (to match the values provided by
// standard flight sticks.)
// @param rotation The rate of rotation for the robot, completely independent
// of the translation. [-1.0..1.0]

void RobotDrive3::HolonomicDrive_Cartesian( float x, float y, float rotation )
{
    double wheelSpeeds[kMaxNumberOfMotors];
    wheelSpeeds[kFrontLeftMotor] = -x - rotation;
    wheelSpeeds[kFrontRightMotor] = 0.0;
    wheelSpeeds[kRearLeftMotor]  = 0.500*x + 0.866*y - rotation;
    wheelSpeeds[kRearRightMotor] = 0.500*x - 0.866*y - rotation;

    Normalize(wheelSpeeds);

    UINT8 syncGroup = 0x80;

    m_frontLeftMotor->Set(wheelSpeeds[kFrontLeftMotor] * m_invertedMotors[kFrontLeftMotor] * m_maxOutput, syncGroup);
    m_rearLeftMotor->Set(wheelSpeeds[kRearLeftMotor] * m_invertedMotors[kRearLeftMotor] * m_maxOutput, syncGroup);
    m_rearRightMotor->Set(wheelSpeeds[kRearRightMotor] * m_invertedMotors[kRearRightMotor] * m_maxOutput, syncGroup);

    CANJaguar::UpdateSyncGroup(syncGroup);
    
    m_safetyHelper->Feed();
}
