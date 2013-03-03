#ifndef CLIMB_COMMAND_H
#define CLIMB_COMMAND_H
#include <WPILib.h>
#include "Robot.h"

class ClimbCommand : public Command
{
public: 
	ClimbCommand();
	~ClimbCommand();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void Stop();
	virtual void Interrupted();
};
#endif
