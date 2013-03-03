// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef ROBOTDRIVE3_H_
#define ROBOTDRIVE3_H_

#include "ErrorBase.h"
#include <stdlib.h>
#include <vxWorks.h>
#include "RobotDrive.h"

class SpeedController;

// RobotDrive extension for 3-wheel holonomic drive

class RobotDrive3 : public RobotDrive
{
public:
    RobotDrive3(UINT32 frontMotorChannel, UINT32 rearLeftMotorChannel,
		UINT32 rearRightMotorChannel);
    RobotDrive3(SpeedController *frontMotor, SpeedController *rearLeftMotor,
		SpeedController *rearRightMotor);
    RobotDrive3(SpeedController &frontMotor, SpeedController &rearLeftMotor,
		SpeedController &rearRightMotor);

    void HolonomicDrive_Cartesian(float x, float y, float rotation );

private:
    DISALLOW_COPY_AND_ASSIGN(RobotDrive3);
};

#endif

