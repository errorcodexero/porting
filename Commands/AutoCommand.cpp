// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "BlinkyLight.h"
#include "Shooter.h"
#include "AutoCommand.h"
#include "BlinkyBreathe.h"
#include "ShootCommand.h"

AutoCommand::AutoCommand()
{
    m_blinky = new BlinkyBreathe(3.0);
    m_shoot = new ShootCommand(Shooter::kMid);

    AddParallel(m_blinky);
    AddSequential(m_shoot);
}

void AutoCommand::Initialize()
{
}

void AutoCommand::Execute()
{
}

bool AutoCommand::IsFinished()
{
    return (m_shoot->IsRunning() && (m_shoot->GetLaunched() >= 3) &&
            !Robot::shooter()->IsInjectorActive());
}

void AutoCommand::End()
{
}

void AutoCommand::Interrupted()
{
    Robot::blinkyLight()->Set(0.0);
}

