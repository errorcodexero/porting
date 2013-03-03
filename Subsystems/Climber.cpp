#include "Climber.h"

Climber::Climber( int leftMotor, int rightMotor, int ltLim, int rtLim,
	          int lmLim, int rmLim, int lbLim, int rbLim,
		  int extender, int claw )
    : Subsystem("Climber")
{
    LiveWindow *lw = LiveWindow::GetInstance();

    m_pLeft = new Victor(leftMotor);
    lw->AddActuator("Climber", "Left", m_pLeft);

    m_pRight = new Victor(rightMotor);
    lw->AddActuator("Climber", "Right", m_pRight);

    m_pLTopLim = new DigitalInput(ltLim);
    lw->AddSensor("Climber", "LeftTopLimit", m_pLTopLim);

    m_pRTopLim = new DigitalInput(rtLim);
    lw->AddSensor("Climber", "RightTopLimit", m_pRTopLim);

    m_pLMidLim = new DigitalInput(lmLim);
    lw->AddSensor("Climber", "LeftMidLimit", m_pLMidLim);

    m_pRMidLim = new DigitalInput(rmLim);
    lw->AddSensor("Climber", "RightMidLimit", m_pRMidLim);

    m_pLBotLim = new DigitalInput(lbLim);
    lw->AddSensor("Climber", "LeftBotLimit", m_pLBotLim);

    m_pRBotLim = new DigitalInput(rbLim);
    lw->AddSensor("Climber", "RightBotLimit", m_pRBotLim);

    m_pExtender = new DoubleSolenoid(extender, extender+1);
    lw->AddActuator("Climber", "Extender", m_pExtender);

    m_pClaw = new DoubleSolenoid(claw, claw+1);
    lw->AddActuator("Climber", "Claw", m_pClaw);
}

Climber::~Climber()
{
    delete m_pLeft;
    delete m_pRight;
    delete m_pLTopLim;
    delete m_pRTopLim;
    delete m_pLMidLim;
    delete m_pRMidLim;
    delete m_pLBotLim;
    delete m_pRBotLim;
    delete m_pExtender;
    delete m_pClaw;
}
