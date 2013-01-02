// sample robot code
// Steve Tarr - team 1425 mentor

#ifndef _MYROBOT_H_
#define _MYROBOT_H_

#include <WPILib.h>

class MyRobot : public IterativeRobot
{
private:
    DriverStation *m_pDS;
    DriverStationEnhancedIO *m_pIO;
    // Compressor *m_compressor;

    uint32_t disabled_periodic;
    uint32_t disabled_continuous;
    uint32_t autonomous_periodic;
    uint32_t autonomous_continuous;
    uint32_t teleop_periodic;
    uint32_t teleop_continuous;

public:
    MyRobot();

    virtual void RobotInit();

    virtual void DisabledInit();
    virtual void AutonomousInit();
    virtual void TeleopInit();

    virtual void DisabledPeriodic();
    virtual void AutonomousPeriodic();
    virtual void TeleopPeriodic();

    virtual void DisabledContinuous();
    virtual void AutonomousContinuous();
    virtual void TeleopContinuous();
};

#define	COMPRESSOR_SWITCH	1
#define	COMPRESSOR_RELAY	1

#endif // _MYROBOT_H_
