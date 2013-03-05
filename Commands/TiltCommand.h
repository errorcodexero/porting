// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef TILT_COMMAND_H
#define TILT_COMMAND_H

#include <Commands/Command.h>

class TiltCommand: public Command {
private:
    Shooter::TargetDistance m_targetDistance;
    Shooter::TargetDistance m_currentDistance;
    bool m_needToGoToMid;

public:
    TiltCommand( Shooter::TargetDistance targetDistance );
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void Interrupted();
    virtual void End();
};

#endif
