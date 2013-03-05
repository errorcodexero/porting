// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef BLINKY_OFF_H
#define BLINKY_OFF_H

#include <Commands/Command.h>

class BlinkyOff: public Command {
public:
    BlinkyOff();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
