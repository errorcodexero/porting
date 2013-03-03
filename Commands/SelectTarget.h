#ifndef SELECT_TARGET_H
#define SELECT_TARGET_H

#include "../Robot.h"

class SelectTarget: public Command {
public:
	
	char* m_targetName; 
	
    SelectTarget(char* targetName);
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
