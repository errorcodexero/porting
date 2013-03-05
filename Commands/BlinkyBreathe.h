// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef BLINKY_BREATHE_H
#define BLINKY_BREATHE_H

#include <Commands/Command.h>

class BlinkyBreathe: public Command {
private:
    double m_cycleTime;
    double m_startTime;
public:
    BlinkyBreathe( float seconds );
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
