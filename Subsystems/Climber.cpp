// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Climber.h"
#include "ClimbManual.h"

Climber::Climber( int extendChannel )
    : Subsystem("Climber")
{
    m_pExtender = new Solenoid(extendChannel);
    LiveWindow::GetInstance()->
      AddActuator("Climber", "Extended", m_pExtender);
    m_defaultCommand = NULL;
}

Climber::~Climber()
{
    SetDefaultCommand(NULL);
    Scheduler::GetInstance()->Remove(m_defaultCommand);
    delete m_defaultCommand;
    m_defaultCommand = NULL;
    delete m_pExtender;
    m_pExtender = NULL;
}

void Climber::InitDefaultCommand()
{
    if (!m_defaultCommand) {
	m_defaultCommand = new ClimbManual();
	SetDefaultCommand(m_defaultCommand);
    }
}

void Climber::SetExtended( bool position )
{
    m_pExtender->Set(position);
}

bool Climber::GetExtended()
{
    return m_pExtender->Get();
}

