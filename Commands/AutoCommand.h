// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef AUTOCOMMAND_H
#define AUTOCOMMAND_H

#include <Commands/Command.h>
#include <Commands/CommandGroup.h>

// This command group is started whenever the robot enters autonomous mode
// and canceled when the robot enters any other mode.

class BlinkyBreathe;
class ShootCommand;

class AutoCommand: public CommandGroup {
public:	
    AutoCommand();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void Stop();
    virtual void Interrupted();

private:
    BlinkyBreathe *m_blinky;
    ShootCommand *m_shoot;
};

#endif
