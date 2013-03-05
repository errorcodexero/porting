// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>
#include "TripleSolenoid.h"

class Shooter : public Subsystem {
public:
    Shooter( int motorChannel, int positionerChannel, int switchChannel,
    		int injectorChannel );
    ~Shooter();

    enum TargetDistance {
	kUnknown,
	kShort,
	kMid,
	kLong,
    };

    void SetAngle(TargetDistance);
    TargetDistance GetAngle();

    void SetSpeed(double speed);
    void Start(void);
    void Stop(void);
    bool IsInPosition(void);
    bool IsUpToSpeed(void);
    bool IsInjectorActive(void);
    bool IsReadyToShoot(void);

    void Inject();
    void SetInjector(bool state);
	
private:
    CANJaguar* m_motor;
    double m_rampRate, m_P, m_I, m_D;
    double m_speed;
    double m_speedTolerance;
    double m_speedStable;
    int m_report;
    int m_timeAtSpeed;
    bool m_isUpToSpeed;
    double m_injectTime;
    int m_injectCounter;
    
    TripleSolenoid *m_positioner;
    TargetDistance m_distance;

    Solenoid *m_injector;

    Notifier* m_notifier;

    static const double kPollInterval;	// not changeable at runtime
    static const double kReportInterval;

    static void TimerEvent( void *param );
    void Run(void);
    void ReportSpeed(void);
};

#endif
