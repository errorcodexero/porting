// sample robot code
// Steve Tarr - team 1425 mentor

#include <WPILib.h>
#include "MyRobot.h"

MyRobot::MyRobot()
{
    m_pDS = DriverStation::GetInstance();
    m_pJoy = new Joystick(1);
    m_pEIO = &m_pDS->GetEnhancedIO();
    m_pLCD = DriverStationLCD::GetInstance();
    // m_compressor = new Compressor( COMPRESSOR_SWITCH, COMPRESSOR_RELAY );
    // m_compressor->Start();
    LiveWindow *lw = LiveWindow::GetInstance();
    m_pSol1 = new Solenoid(1);
    lw->AddActuator("Solenoid", "Sol1", m_pSol1);
    m_pSol2 = new Solenoid(2);
    lw->AddActuator("Solenoid", "Sol2", m_pSol2);
    m_pSol3 = new Solenoid(3);
    lw->AddActuator("Solenoid", "Sol3", m_pSol3);
    m_pSol4 = new Solenoid(4);
    lw->AddActuator("Solenoid", "Sol4", m_pSol4);
    m_pSol5 = new DoubleSolenoid(5,6);
    lw->AddActuator("DoubleSolenoid", "Sol5", m_pSol5);
    m_pSol6 = new DoubleSolenoid(7,8);
    lw->AddActuator("DoubleSolenoid", "Sol6", m_pSol6);
    m_pRelay = new Relay(2);
    lw->AddActuator("Relay", "Relay2", m_pRelay);
}

void MyRobot::RobotInit()
{
    m_pSol1->Set(false);
    SmartDashboard::PutData("Sol1", m_pSol1);
    m_pSol2->Set(false);
    SmartDashboard::PutData("Sol2", m_pSol2);
    m_pSol3->Set(false);
    SmartDashboard::PutData("Sol3", m_pSol3);
    m_pSol4->Set(false);
    SmartDashboard::PutData("Sol4", m_pSol4);
    m_pSol5->Set(DoubleSolenoid::kOff);
    SmartDashboard::PutData("Sol5", m_pSol5);
    m_pSol6->Set(DoubleSolenoid::kOff);
    SmartDashboard::PutData("Sol6", m_pSol6);
    m_pRelay->Set(Relay::kOff);
    SmartDashboard::PutData("Relay", m_pRelay);
}

void MyRobot::DisabledInit()
{
    printf("MyRobot: starting in disabled mode");
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1, "disabled");
    m_pLCD->UpdateLCD();
}

void MyRobot::DisabledPeriodic()
{
    ++disabled_periodic;
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1, "dsbl %d", disabled_periodic);
    m_pLCD->UpdateLCD();
}

void MyRobot::AutonomousInit()
{
    printf("MyRobot: starting autonomous");
    m_pSol5->Set(DoubleSolenoid::kForward);
    m_pSol6->Set(DoubleSolenoid::kForward);
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1, "autonomous");
    m_pLCD->UpdateLCD();
}

void MyRobot::AutonomousPeriodic()
{
    ++autonomous_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1, "auto %d", autonomous_periodic);
    m_pLCD->UpdateLCD();
}

void MyRobot::TeleopInit()
{
    printf("MyRobot: starting teleop");
    m_pSol5->Set(DoubleSolenoid::kReverse);
    m_pSol6->Set(DoubleSolenoid::kReverse);
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1, "teleop");
    m_pLCD->UpdateLCD();
}

void MyRobot::TeleopPeriodic()
{
    ++teleop_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1, "tele %d", teleop_periodic);
    m_pLCD->Printf(DriverStationLCD::kUser_Line3, 1, "x %4.1f y %4.1f",
	m_pJoy->GetX(), m_pJoy->GetY());
    m_pLCD->Printf(DriverStationLCD::kUser_Line4, 1, "z %4.1f t %4.1f",
	m_pJoy->GetZ(), m_pJoy->GetTwist());

    bool sol1 = m_pEIO->GetDigital(13);
    bool sol2 = m_pEIO->GetDigital(14);
    bool sol3 = m_pEIO->GetDigital(15);
    bool sol4 = m_pEIO->GetDigital(16);

    m_pLCD->Printf(DriverStationLCD::kUser_Line5, 1,"%c %c %c %c",
	sol1 ? 't' : 'f',
	sol2 ? 't' : 'f',
	sol3 ? 't' : 'f',
	sol4 ? 't' : 'f');

    m_pLCD->UpdateLCD();

    m_pEIO->SetLED(1, sol1);
    m_pEIO->SetLED(2, sol2);
    m_pEIO->SetLED(3, sol3);
    m_pEIO->SetLED(4, sol4);

    m_pSol1->Set(sol1);
    m_pSol2->Set(sol2);
    m_pSol3->Set(sol3);
    m_pSol4->Set(sol4);
}

void MyRobot::TestInit()
{
    printf("MyRobot: starting test");
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1, "test");
    m_pLCD->UpdateLCD();
}

void MyRobot::TestPeriodic()
{
    ++test_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1, "test %d", test_periodic);
    m_pLCD->UpdateLCD();

    LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(MyRobot);

