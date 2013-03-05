// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <SmartDashboard/SmartDashboard.h>
#include "AimTrim.h"

AimTrim::AimTrim(bool left) :
    Command("AimTrim")
{
    m_left = left;
    SmartDashboard::PutNumber("trim",0);
}

// Called just before this Command runs the first time
void AimTrim::Initialize()
{
    double val = SmartDashboard::GetNumber("trim");
    if (m_left)
	val -= 1;
    else
	val += 1;
	
    SmartDashboard::PutNumber("trim",val);
}

// Called repeatedly when this Command is scheduled to run
void AimTrim::Execute()
{
    ;
}

// Make this return true when this Command no longer needs to run execute()
bool AimTrim::IsFinished()
{
    return true;
}

// Called once after isFinished returns true
void AimTrim::End()
{
    ;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void AimTrim::Interrupted()
{
    ;
}
