// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Robot.h"
#include "BlinkyLight.h"
#include "BlinkyOn.h"

BlinkyLight::BlinkyLight( int pwmChannel )
    : Subsystem("BlinkyLight")
{
    m_pwm = new Victor(pwmChannel);
    m_pwm->Set(0.0);
    // blinky lights don't need watchdogs
    m_pwm->SetSafetyEnabled(false);
    LiveWindow::GetInstance()->AddActuator("BlinkyLight", "PWM", m_pwm);
}
    
void BlinkyLight::InitDefaultCommand()
{
    // SetDefaultCommand(new BlinkyOn());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

void BlinkyLight::Set( float value )
{
    m_pwm->Set(value);
}

