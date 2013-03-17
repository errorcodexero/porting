// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include "WPILib.h"
#include "RateGyro.h"
#include "RobotDrive3.h"
#include "DriveCommand.h"

// robot drive base with three-wheeled holonomic drive

class DriveBase : public Subsystem {
private:
	SpeedController * m_left;
	SpeedController * m_right;
	SpeedController * m_rear;
	RobotDrive3 * m_drive3;
	RateGyro * m_gyro;
	INT32 m_gyroZero;
	Command * m_defaultCommand;
	bool m_started;

public:
	DriveBase( int leftMotorChannel,
		   int rightMotorChannel,
		   int rearMotorChannel,
		   int gyroMotorChannel );

	~DriveBase();

	void InitDefaultCommand();

	void Stop();
	void Start();
	void Drive3( float x, float y, float twist );
};

#endif
