// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef SHOOT_MANUAL_H
#define SHOOT_MANUAL_H

#include <Commands/Command.h>

class ShootManual: public Command {
private:
    bool m_learn;	// for debouncing

public:
    ShootManual();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void Interrupted();
    virtual void End();
};

#endif
