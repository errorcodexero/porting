// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef CLIMBER_H
#define CLIMBER_H

#include <WPILib.h>

class Climber : public Subsystem
{
	
public:
    Climber( int leftMotor, int rightMotor,
    	     int ltLim, int rtLim,
	     int lmLim, int rmLim,
	     int lbLim, int rbLim,
	     int extendChannel, int retractChannel,
	     int clawOpen, int clawClose );
    ~Climber();

    virtual void InitDefaultCommand();

    typedef enum { kExtenderUnknown, kRetracted, kExtended } ExtenderPosition;

    void SetExtender(ExtenderPosition);
    ExtenderPosition GetExtender();
    bool ExtenderIsMoving();

    typedef enum { kUp, kDown, kStop } HookDirection;
    typedef enum { kHooksUnknown, kBottom, kMidLow, kMidHigh, kTop } HookPosition;
    // SetHooks returns true when both motors are stopped
    bool SetHooks( HookDirection direction, HookPosition stopAt = kHooksUnknown );

    // These functions return the values from the sensor inputs
    bool HooksAtTop();
    bool HooksAtMidHigh();
    bool HooksAtMidLow();
    bool HooksAtBottom();

    typedef enum { kClawUnknown, kOpen, kClosed } ClawPosition;
    void SetClaw(ClawPosition);
    ClawPosition GetClaw();
    bool ClawIsMoving();

    // for debugging
    UINT8 GetLimits();

private:
    Victor *m_pLeftMotor;
    Victor *m_pRightMotor;
    DigitalInput *m_pLTopLim;
    DigitalInput *m_pRTopLim;
    DigitalInput *m_pLMidLim;
    DigitalInput *m_pRMidLim;
    DigitalInput *m_pLBotLim;
    DigitalInput *m_pRBotLim;
    DoubleSolenoid *m_pExtender;
    DoubleSolenoid *m_pClaw;

    ExtenderPosition m_extenderPosition;
    bool m_extenderMoving;
    double m_extenderStartTime;

    static const double kExtendTime;

    HookDirection m_hookDirection;
    HookPosition m_leftPosition;
    HookPosition m_rightPosition;

    static const double kHookSpeed;

    void UpdateHookPosition();

    ClawPosition m_clawPosition;
    bool m_clawMoving;
    double m_clawStartTime;

    static const double kGrabTime;

    Command *m_defaultCommand;
};

#endif
