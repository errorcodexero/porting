// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef SHOOT_COMMAND_H
#define SHOOT_COMMAND_H

#include "Robot.h"
#include <Commands/Command.h>

class ShootCommand: public Command {
private:
	Shooter::TargetDistance m_targetDistance;
	int m_launched;

public:
	ShootCommand( Shooter::TargetDistance targetDistance
			= Shooter::kUnknown );
	int GetLaunched();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void Interrupted();
	virtual void End();
};

#endif