// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef BLINKY_LIGHT_H
#define BLINKY_LIGHT_H

#include <WPILib.h>

class BlinkyLight: public Subsystem
{
private:
    Victor* m_pwm;

public:
    BlinkyLight( int pwmChannel );
    void InitDefaultCommand();
    void Set( float value );
};

#endif
