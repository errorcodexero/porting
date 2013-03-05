// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef CLIMB_MANUAL_H
#define CLIMB_MANUAL_H

#include <Commands/Command.h>

class ClimbManual : public Command
{
private:

public: 
    ClimbManual();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
