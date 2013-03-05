// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef _TRIPLE_SOLENOID_H_
#define _TRIPLE_SOLENOID_H_

#include "DoubleSolenoid.h"

class TripleSolenoid : public DoubleSolenoid {
public:
    enum Position {
	kUnknown,
	kRetracted,
	kPartlyRetracted,
	kCenter,
	kPartlyExtended,
	kExtended,
    };

    TripleSolenoid( int forwardChannel, int reverseChannel,
    			   int switchChannel );
    virtual ~TripleSolenoid();
    DigitalInput *m_switch;
    void SetPosition( Position );
    Position GetPosition(void);
    void Start();
    void Stop();
    std::string GetSmartDashboardType() { return "3PS"; };

private:
    Position m_goal;
    Position m_position;
    DoubleSolenoid::Value m_direction;
    int m_howLong;
    Notifier *m_pNotifier;
    static void TimerEvent( void *param );
    void Run(void);
    void Update(void);
    bool Move(void);

    static const double kPollInterval;
    static const double kTravelTime;
};

#endif
