// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef SHOOT_MANUAL_H
#define SHOOT_MANUAL_H

#include "Robot.h"
#include <Commands/Command.h>

class ShootManual: public Command {
public:
	ShootManual();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void Interrupted();
	virtual void End();
};

#endif
