// sample robot code
// Steve Tarr - team 1425 mentor

#ifndef _MYROBOT_H_
#define _MYROBOT_H_

#include <WPILib.h>

class MyRobot : public IterativeRobot
{
private:
#if 0
    Compressor *m_compressor;
#endif
    Solenoid *m_pSol1;
    Solenoid *m_pSol2;
    DoubleSolenoid *m_pSol3;
    Relay *m_pRelay;
    DigitalInput *m_pSw;

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
