#include "Robot.h"
#include "BlinkyLight.h"

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
    // SetDefaultCommand(new BlinkyOff());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

void BlinkyLight::Set( float value )
{
    m_pwm->Set(value);
}
