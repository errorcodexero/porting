// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Robot.h"

ResetRobot::ResetRobot()
{
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(chassis);
    Requires(Robot::driveBase());
    Requires(Robot::shooter());
}

// Called just before this Command runs the first time
void ResetRobot::Initialize()
{
    Robot::shooter()->SetAngle(Shooter::kShort);
    Robot::shooter()->SetSpeed(0.0);
    Robot::shooter()->Start();
}

// Called repeatedly when this Command is scheduled to run
void ResetRobot::Execute()
{
    ;
}

// Make this return true when this Command no longer needs to run execute()
bool ResetRobot::IsFinished()
{
    return (Robot::shooter()->IsInPosition());
}

// Called once after isFinished returns true
void ResetRobot::End()
{
    Robot::shooter()->Stop();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ResetRobot::Interrupted()
{
    Robot::shooter()->Stop();
}
