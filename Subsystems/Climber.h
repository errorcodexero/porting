// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef CLIMBER_H
#define CLIMBER_H

#include <WPILib.h>

class Climber : public Subsystem
{
	
public:
    Climber( int extendChannel );
    ~Climber();

    virtual void InitDefaultCommand();

    void SetExtended(bool);
    bool GetExtended();

private:
    Solenoid *m_pExtender;
    Command *m_defaultCommand;
};

#endif
