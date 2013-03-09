// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "OI.h"
#include "Climber.h"
#include "ClimbManual.h"

#define	EXTEND_TIME	1.2
#define	GRAB_TIME	1.2

ClimbManual::ClimbManual()
{
    Requires(Robot::climber());
}

void ClimbManual::Initialize()
{
}

void ClimbManual::Execute()
{
    // The OI "tip" switch controls the extender.
    switch (Robot::oi()->GetTip()) {
    case 0:
	Robot::climber()->SetExtender(Climber::kRetracted);
	break;
    case 1:
	break;
    case 2:
	Robot::climber()->SetExtender(Climber::kExtended);
	break;
    }

    // The OI "dump" switch controls the claw.
    Robot::climber()->SetClaw(
	Robot::oi()->GetDump() ? Climber::kOpen : Climber::kClosed
    );

    // The OI "climb" switch controls the hooks.
    switch ( Robot::oi()->GetClimber() ) {
    case 0:
	Robot::climber()->SetHooks( Climber::kDown );
	break;
    case 1:
	Robot::climber()->SetHooks( Climber::kStop );
	break;
    case 2:
	Robot::climber()->SetHooks( Climber::kUp );
	break;
    }

    // Hook position sensors control the DS LEDs
    Robot::oi()->GetEIO()->SetLEDs( Robot::climber()->GetLimits() );
}

bool ClimbManual::IsFinished()
{
    return false;
}

void ClimbManual::End()
{
    ;
}

void ClimbManual::Interrupted()
{
    ;
}


