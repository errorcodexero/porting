// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "OI.h"
#include "Climber.h"
#include "ClimbManual.h"

ClimbManual::ClimbManual() :
    Command("ClimbManual")
{
    Requires(Robot::climber());
}

void ClimbManual::Initialize()
{
    printf("ClimbManual::Initialize\n");
}

void ClimbManual::Execute()
{
    if (Robot::theRobot().IsOperatorControl()) {
	Robot::climber()->SetExtended( Robot::oi()->GetClimber() );
    }
}

bool ClimbManual::IsFinished()
{
    return false;
}

void ClimbManual::End()
{
    printf("ClimbManual::End\n");
}

void ClimbManual::Interrupted()
{
    printf("ClimbManual::Interrupted\n");
}


