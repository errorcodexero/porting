// sample robot code
// Steve Tarr - team 1425 mentor

#ifndef _MYROBOT_H_
#define _MYROBOT_H_

#include <WPILib.h>

class MyRobot : public IterativeRobot
{
private:
    DriverStation *m_pDS;
    Joystick *m_pJoy;
    DriverStationEnhancedIO *m_pEIO;
    DriverStationLCD *m_pLCD;
    // Compressor *m_compressor;
    Solenoid *m_pSol1;
    Solenoid *m_pSol2;
    Solenoid *m_pSol3;
    Solenoid *m_pSol4;
    DoubleSolenoid *m_pSol5;
    DoubleSolenoid *m_pSol6;

    uint32_t disabled_periodic;
    uint32_t autonomous_periodic;
    uint32_t teleop_periodic;
    uint32_t test_periodic;

public:
    MyRobot();

    virtual void RobotInit();

    virtual void DisabledInit();
    virtual void AutonomousInit();
    virtual void TeleopInit();
    virtual void TestInit();

    virtual void DisabledPeriodic();
    virtual void AutonomousPeriodic();
    virtual void TeleopPeriodic();
    virtual void TestPeriodic();
};

#define	COMPRESSOR_SWITCH	1
#define	COMPRESSOR_RELAY	1

#endif // _MYROBOT_H_
