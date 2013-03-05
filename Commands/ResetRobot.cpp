// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "DriveBase.h"
#include "Shooter.h"
#include "Climber.h"
#include "ResetRobot.h"

ResetRobot::ResetRobot()
{
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(chassis);
    Requires(Robot::driveBase());
    Requires(Robot::shooter());
    Requires(Robot::climber());
}

// Called just before this Command runs the first time
void ResetRobot::Initialize()
{
    Robot::driveBase()->Stop();
    Robot::climber()->SetClaw(Climber::kOpen);
    Robot::climber()->SetExtender(Climber::kRetracted);
    Robot::climber()->SetHooks(Climber::kDown);
    Robot::shooter()->SetAngle(Shooter::kShort);
    Robot::shooter()->SetSpeed(0.0);
    Robot::shooter()->Start();
}

// Called repeatedly when this Command is scheduled to run
void ResetRobot::Execute()
{
    Robot::climber()->SetHooks(Climber::kDown);
}

// Make this return true when this Command no longer needs to run execute()
bool ResetRobot::IsFinished()
{
    return (Robot::shooter()->IsInPosition() && Robot::climber()->HooksAtBottom());
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
    Robot::climber()->SetHooks(Climber::kStop);
}
