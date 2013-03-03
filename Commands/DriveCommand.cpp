// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Robot.h"
#include "Utility.h"

static UINT32 then;

// Identify resources required by this command.
// Other commands that are using these resources will be Canceled
// when this command is Started.
DriveCommand::DriveCommand()
{
//  printf("DriveCommand::DriveCommand\n");
    Requires(Robot::driveBase());
}

// Called just before this Command runs the first time
void DriveCommand::Initialize()
{
//  printf("DriveCommand::Initialize\n");
    then = GetFPGATime();
}

// Called repeatedly when this Command is scheduled to run
void DriveCommand::Execute()
{
//  UINT32 now = GetFPGATime();
//  if (now - then > 30000) {
//      printf("DriveCommand::Execute %u\n", now - then);
//  }
//  then = now;

    Joystick *joy = Robot::oi()->getStick();
    float throttle = (1.0 - joy->GetThrottle())/2.0;
    float x = joy->GetX() * throttle;
    float y = joy->GetY() * throttle;
    float t = joy->GetTwist();
//  printf("%g %g %g\n", x, y, t);

    Robot::driveBase()->Drive3(x, y, t);
}

// Make this return true when this Command no longer needs to run execute()
bool DriveCommand::IsFinished()
{
    return false;
}

// Called once after isFinished returns true
void DriveCommand::End()
{
//  printf("DriveCommand::End (can't happen)\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveCommand::Interrupted()
{
//  printf("DriveCommand::Interrupted\n");
    Robot::driveBase()->Stop();
}