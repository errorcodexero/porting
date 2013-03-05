// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef DRIVECOMMAND_H
#define DRIVECOMMAND_H

#include <Commands/Command.h>

// This is the default command for the drive base:
// drive under joystick control.

class DriveCommand: public Command {
public:
    DriveCommand();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
