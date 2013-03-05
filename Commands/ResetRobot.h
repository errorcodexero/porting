// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef RESET_ROBOT_H
#define RESET_ROBOT_H

#include <Commands/Command.h>

class ResetRobot: public Command {
public:
    ResetRobot();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
