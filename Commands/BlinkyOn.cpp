// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Robot.h"

BlinkyOn::BlinkyOn()
{
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(chassis);
    Requires(Robot::blinkyLight());
    m_onTimer.Start();
}

// Called just before this Command runs the first time
void BlinkyOn::Initialize()
{
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
    return (m_onTimer.Get() >= 1.0);
}

// Called once after isFinished returns true
void BlinkyOn::End()
{
    ;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BlinkyOn::Interrupted()
{
    ;
}
