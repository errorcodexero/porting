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
    m_pLCD = DriverStationLCD::GetInstance();
    // m_compressor = new Compressor( COMPRESSOR_SWITCH, COMPRESSOR_RELAY );
    // m_compressor->Start();
}

void MyRobot::DisabledInit()
{
    printf("MyRobot: starting disabled\n");
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1,
	"MyRobot: starting disabled\n");
    m_pLCD->UpdateLCD();
}

void MyRobot::AutonomousInit()
{
    printf("MyRobot: starting autonomous\n");
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1,
	"MyRobot: starting autonomous\n");
    m_pLCD->UpdateLCD();
}

void MyRobot::TeleopInit()
{
    printf("MyRobot: starting teleop\n");
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1,
	"MyRobot: starting teleop\n");
    m_pLCD->UpdateLCD();
}

void MyRobot::DisabledPeriodic()
{
    ++disabled_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1,
	"MyRobot: disabled periodic %d\n", disabled_periodic);
    m_pLCD->UpdateLCD();
}

void MyRobot::AutonomousPeriodic()
{
    ++autonomous_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1,
	"MyRobot: autonomous periodic %d\n", autonomous_periodic);
    m_pLCD->UpdateLCD();
}

void MyRobot::TeleopPeriodic()
{
    ++teleop_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1,
	"MyRobot: teleop periodic %d\n", teleop_periodic);
    m_pLCD->UpdateLCD();
}

void MyRobot::DisabledContinuous()
{
    ++disabled_continuous;
    taskDelay(1);
}

void MyRobot::AutonomousContinuous()
{
    ++autonomous_continuous;
    taskDelay(1);
}

void MyRobot::TeleopContinuous()
{
    ++teleop_continuous;
    taskDelay(1);
}

START_ROBOT_CLASS(MyRobot);

