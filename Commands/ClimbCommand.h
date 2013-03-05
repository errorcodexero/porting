// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef CLIMB_COMMAND_H
#define CLIMB_COMMAND_H

#include <Commands/Command.h>

class ClimbCommand : public Command
{
private:
    double m_startTime;
    int m_state;

public: 
    ClimbCommand();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
