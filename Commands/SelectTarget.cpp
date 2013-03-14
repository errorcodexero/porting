// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "SelectTarget.h"

SelectTarget::SelectTarget(const char* targetName) :
    Command("SelectTarget")
{
    m_targetName = targetName; 
}

// Called just before this Command runs the first time
void SelectTarget::Initialize()
{
    printf("SelectTarget::Initialize\n");
    SmartDashboard::PutString("targetSelection", m_targetName);
}

// Called repeatedly when this Command is scheduled to run
void SelectTarget::Execute()
{
    ;
}

// Make this return true when this Command no longer needs to run execute()
bool SelectTarget::IsFinished()
{
    printf("SelectTarget::IsFinished\n");
    return true;
}

// Called once after isFinished returns true
void SelectTarget::End()
{
    printf("SelectTarget::End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SelectTarget::Interrupted()
{
    printf("SelectTarget::Interrupted\n");
}
