// sample robot code
// Steve Tarr - team 1425 mentor

#include <WPILib.h>
#include "MyRobot.h"

MyRobot::MyRobot()
{
    RobotInit();
}

void MyRobot::RobotInit()
{
    m_pDS = DriverStation::GetInstance();
    m_pIO = &m_pDS->GetEnhancedIO();
    // m_compressor = new Compressor( COMPRESSOR_SWITCH, COMPRESSOR_RELAY );
    // m_compressor->Start();
}

void MyRobot::DisabledInit()
{
    printf("MyRobot: starting disabled\n");
}

void MyRobot::AutonomousInit()
{
    printf("MyRobot: starting autonomous\n");
}

void MyRobot::TeleopInit()
{
    printf("MyRobot: starting teleop\n");
}

void MyRobot::DisabledPeriodic()
{
    ++disabled_periodic;
}

void MyRobot::AutonomousPeriodic()
{
    ++autonomous_periodic;
}

void MyRobot::TeleopPeriodic()
{
    ++teleop_periodic;
}

void MyRobot::DisabledContinuous()
{
    ++disabled_continuous;
}

void MyRobot::AutonomousContinuous()
{
    ++autonomous_continuous;
}

void MyRobot::TeleopContinuous()
{
    ++teleop_continuous;
}

START_ROBOT_CLASS(MyRobot);

