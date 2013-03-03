#ifndef CLIMBER_H
#define CLIMBER_H

#include <WPILib.h>

class Climber : public Subsystem
{
private:
	Victor *m_pLeft;
	Victor *m_pRight;
	DigitalInput *m_pLTopLim;
	DigitalInput *m_pRTopLim;
	DigitalInput *m_pLMidLim;
	DigitalInput *m_pRMidLim;
	DigitalInput *m_pLBotLim;
	DigitalInput *m_pRBotLim;
	DoubleSolenoid *m_pExtender;
	DoubleSolenoid *m_pClaw;
	
public:
	Climber( int leftMotor, int rightMotor, int ltLim, int rtLim,
	         int lmLim, int rmLim, int lbLim, int rbLim,
		 int extender, int claw );
	~Climber();
};

#endif
