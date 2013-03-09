// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "BlinkyLight.h"
#include "BlinkyBreathe.h"
#include <math.h>

#ifndef M_PI
#define	M_PI	3.1415926535
#endif

BlinkyBreathe::BlinkyBreathe( float seconds )
{
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(chassis);
    Requires(Robot::blinkyLight());

    m_cycleTime = seconds;
    m_startTime = 0.0;
}

// Called just before this Command runs the first time
void BlinkyBreathe::Initialize()
{
printf("Command BlinkyBreathe Initialize\n");
    m_startTime = Timer::GetFPGATimestamp();
    Robot::blinkyLight()->Set(0.0);
}

// Called repeatedly when this Command is scheduled to run
void BlinkyBreathe::Execute()
{
    double elapsed = Timer::GetFPGATimestamp() - m_startTime;
    // sin ranges from -1.0 to +1.0, we need 0.0 to +1.0
    float breathe = (sin(2.0 * M_PI * elapsed / m_cycleTime) + 1.0) / 2.0;
    Robot::blinkyLight()->Set(breathe);
}

// Make this return true when this Command no longer needs to run execute()
bool BlinkyBreathe::IsFinished()
{
    return false;
}

// Called once after isFinished returns true
void BlinkyBreathe::End()
{
printf("Command BlinkyBreathe End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BlinkyBreathe::Interrupted()
{
printf("Command BlinkyBreathe Interrupted\n");
    Robot::blinkyLight()->Set(0.0);
}
