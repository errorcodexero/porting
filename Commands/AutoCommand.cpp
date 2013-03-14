// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "BlinkyLight.h"
#include "Shooter.h"
#include "AutoCommand.h"
#include "BlinkyBreathe.h"
#include "TiltCommand.h"
#include "TargetCommand.h"
#include "ShootCommand.h"

AutoCommand::AutoCommand() :
    CommandGroup("AutoCommand")
{
    m_blinky = new BlinkyBreathe(3.0);
    m_tilt = new TiltCommand(Shooter::kLong);
    m_target = new TargetCommand();
    m_shoot = new ShootCommand(Shooter::kMid, 3);

    AddParallel(m_blinky);
    AddSequential(m_tilt);
    //AddSequential(m_target);
    AddSequential(m_shoot);
}

AutoCommand::~AutoCommand()
{
    // this should remove command instances from scheduler,
    // then delete them.
}

void AutoCommand::Initialize()
{
    printf("AutoCommand::Initialize\n");
}

void AutoCommand::Execute()
{
}

bool AutoCommand::IsFinished()
{
    // this should return true when all commands are done
    return false;
}

void AutoCommand::End()
{
    printf("AutoCommand::End\n");
}

void AutoCommand::Interrupted()
{
    printf("AutoCommand::Interrupted\n");
    Robot::blinkyLight()->Set(0.0);
}

