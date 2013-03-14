// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "BlinkyLight.h"
#include "BlinkyOn.h"

BlinkyOn::BlinkyOn() :
    Command("BlinkyOn")
{
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(chassis);
    Requires(Robot::blinkyLight());
    m_onTimer.Start();
}

// Called just before this Command runs the first time
void BlinkyOn::Initialize()
{
    printf("BlinkyOn::Initialize\n");
    Robot::blinkyLight()->Set(1.0);
    m_onTimer.Reset();
}

// Called repeatedly when this Command is scheduled to run
void BlinkyOn::Execute()
{
    ;
}

// Make this return true when this Command no longer needs to run execute()
bool BlinkyOn::IsFinished()
{
    bool finished = (m_onTimer.Get() >= 1.0);
    if (finished) printf("BlinkyOn::IsFinished\n");
    return finished;
}

// Called once after isFinished returns true
void BlinkyOn::End()
{
    printf("BlinkyOn::End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BlinkyOn::Interrupted()
{
    printf("BlinkyOn::Interrupted\n");
    ;
}
