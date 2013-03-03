#include "SelectTarget.h"

SelectTarget::SelectTarget(char* targetName) :
    Command("SelectTarget")
{
    m_targetName = targetName; 
}

// Called just before this Command runs the first time
void SelectTarget::Initialize()
{
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
    return true;
}

// Called once after isFinished returns true
void SelectTarget::End()
{
    ;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SelectTarget::Interrupted()
{
    ;
}
