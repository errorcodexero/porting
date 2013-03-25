// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "DriveBase.h"
#include "Shooter.h"
#include "Climber.h"
#include "ResetRobot.h"

ResetRobot::ResetRobot() :
    Command("ResetRobot")
{
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(chassis);
    Requires(Robot::driveBase());
    Requires(Robot::shooter());
    // The climber is directly controlled by a hard switch;
    //   don't mess with it here.
}

// Called just before this Command runs the first time
void ResetRobot::Initialize()
{
    printf("ResetRobot::Initialize\n");
    Robot::driveBase()->Stop();
    Robot::shooter()->Stop();
    Robot::shooter()->SetAngle(Shooter::kShort);
    Robot::shooter()->SetInjector(true);
    // The climber is directly controlled by a hard switch;
    //   don't mess with it here.
}

// Called repeatedly when this Command is scheduled to run
void ResetRobot::Execute()
{
    ;
}

// Make this return true when this Command no longer needs to run execute()
bool ResetRobot::IsFinished()
{
    bool finished = Robot::shooter()->IsInPosition();
    if (finished) printf("RobotReset::IsFinished\n");
    return finished;
}

// Called once after isFinished returns true
void ResetRobot::End()
{
    printf("ResetRobot::End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ResetRobot::Interrupted()
{
    printf("ResetRobot::Interrupted\n");
    Robot::shooter()->Stop();
}
