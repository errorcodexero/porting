// sample robot code
// Steve Tarr - team 1425 mentor

#ifndef _MYROBOT_H_
#define _MYROBOT_H_

#include <WPILib.h>

#define INCLUDE_SOLENOID
#define INCLUDE_DOUBLE_SOLENOID
#define INCLUDE_RELAY
#define INCLUDE_SWITCH
#define INCLUDE_COMPRESSOR

class MyRobot : public IterativeRobot
{
private:
#ifdef INCLUDE_COMPRESSOR
    Compressor *m_compressor;
#endif
#ifdef INCLUDE_SOLENOID
    Solenoid *m_pSol1;
    Solenoid *m_pSol2;
#endif
#ifdef INCLUDE_DOUBLE_SOLENOID
    DoubleSolenoid *m_pSol3;
#endif
#ifdef INCLUDE_RELAY
    Relay *m_pRelay;
#endif
#ifdef INCLUDE_SWITCH
    DigitalInput *m_pSw;
#endif

    // OI *m_pOI; // command-based OI
    DriverStation *m_pDS;
    Joystick *m_pJoy;
    DriverStationEnhancedIO *m_pEIO;
    DriverStationLCD *m_pLCD;

    uint32_t disabled_periodic;
    uint32_t autonomous_periodic;
    uint32_t teleop_periodic;
    uint32_t test_periodic;

public:
    virtual void RobotInit();

    virtual void DisabledInit();
    virtual void DisabledPeriodic();

    virtual void AutonomousInit();
    virtual void AutonomousPeriodic();

    virtual void TeleopInit();
    virtual void TeleopPeriodic();

    virtual void TestInit();
    virtual void TestPeriodic();
};

#endif // _MYROBOT_H_
