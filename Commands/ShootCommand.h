// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef SHOOT_COMMAND_H
#define SHOOT_COMMAND_H

#include <Commands/Command.h>
#include "Shooter.h"

class ShootCommand: public Command {
private:
    Shooter::TargetDistance m_targetDistance;
    int m_target;
    int m_numDisks;
    int m_launched;

public:
    ShootCommand( Shooter::TargetDistance targetDistance = Shooter::kUnknown,
		  int targetSelect = 2,	// 3 pt. goal
    		  int num_disks = 45 ); // all of 'em

    void SetDistance( Shooter::TargetDistance );
    void SetTarget( int );
    void SetNumDisks( int );
    int GetLaunched();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void Interrupted();
    virtual void End();
};

#endif
