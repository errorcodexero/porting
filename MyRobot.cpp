// sample robot code
// Steve Tarr - team 1425 mentor

#include <WPILib.h>
#include "MyRobot.h"

void MyRobot::RobotInit()
{
    ///////////////////////////////////////////////////////
    // Subsystems
    ///////////////////////////////////////////////////////

#if 0
    m_compressor = new Compressor( 1, 1 );
    m_compressor->Start();
#endif
    m_pSol1 = new Solenoid(1);
    m_pSol2 = new Solenoid(2);
    m_pSol3 = new DoubleSolenoid(3,4);
    m_pRelay = new Relay(2, Relay::kBothDirections);
    m_pSw = new DigitalInput(6);

    ///////////////////////////////////////////////////////
    // OI
    ///////////////////////////////////////////////////////

    // m_pOI = new OI();
    m_pDS = DriverStation::GetInstance();
    m_pJoy = new Joystick(1);
    m_pEIO = &m_pDS->GetEnhancedIO();
    m_pLCD = DriverStationLCD::GetInstance();

    ///////////////////////////////////////////////////////
    // autonomous command
    ///////////////////////////////////////////////////////

    // m_pAutonomousCommand = new AutonomousCommand();
}

void MyRobot::DisabledInit()
{
    printf("MyRobot: starting disabled mode\n");
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1, "disabled");
    m_pLCD->UpdateLCD();

 // m_pAutonomousCommand->Cancel();
}

void MyRobot::DisabledPeriodic()
{
    ++disabled_periodic;
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1, "dsbl %d", disabled_periodic);
    m_pLCD->Printf(DriverStationLCD::kUser_Line3, 1, "sw %s",
	m_pSw->Get() ? "true " : "false");
    m_pLCD->UpdateLCD();
}

void MyRobot::AutonomousInit()
{
    printf("MyRobot: starting autonomous mode\n");
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1, "autonomous");
    m_pLCD->UpdateLCD();

 // m_pAutonomousCommand->Start();
}

void MyRobot::AutonomousPeriodic()
{
    ++autonomous_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1, "auto %d", autonomous_periodic);
    m_pLCD->Printf(DriverStationLCD::kUser_Line3, 1, "sw %s",
	m_pSw->Get() ? "true " : "false");
    m_pLCD->UpdateLCD();

    Scheduler::GetInstance()->Run();
}

void MyRobot::TeleopInit()
{
    printf("MyRobot: starting teleop mode\n");
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1, "teleop");
    m_pLCD->UpdateLCD();

 // m_pAutonomousCommand->Cancel();
}

void MyRobot::TeleopPeriodic()
{
    ++teleop_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1, "tele %d", teleop_periodic);
    m_pLCD->Printf(DriverStationLCD::kUser_Line3, 1, "sw %s",
	m_pSw->Get() ? "true " : "false");
    m_pLCD->Printf(DriverStationLCD::kUser_Line4, 1,
	"x %4.1f y %4.1f z %4.1f t %4.1f",
	m_pJoy->GetX(), m_pJoy->GetY(), m_pJoy->GetZ(), m_pJoy->GetTwist());

    bool din1 = ! m_pEIO->GetDigital(1);
    bool din2 = ! m_pEIO->GetDigital(2);
    bool din3 = ! m_pEIO->GetDigital(3);
    bool din4 = ! m_pEIO->GetDigital(4);
    bool din5 = ! m_pEIO->GetDigital(5);
    bool din6 = ! m_pEIO->GetDigital(6);

    m_pLCD->Printf(DriverStationLCD::kUser_Line5, 1,"%c %c %c %c %c %c",
    din1 ? 't' : 'f',
    din2 ? 't' : 'f',
    din3 ? 't' : 'f',
    din4 ? 't' : 'f',
    din5 ? 't' : 'f',
    din6 ? 't' : 'f');

    m_pEIO->SetLED(1, din1);
    m_pEIO->SetLED(2, din2);
    m_pEIO->SetLED(3, din3);
    m_pEIO->SetLED(4, din4);
    m_pEIO->SetLED(5, din5);
    m_pEIO->SetLED(6, din6);

    m_pSol1->Set(din1);
    m_pSol2->Set(din2);
    m_pSol3->Set(din3 ? DoubleSolenoid::kForward :
		 din4 ? DoubleSolenoid::kReverse :
		 DoubleSolenoid::kOff);
    m_pRelay->Set(din5 ? Relay::kForward :
		  din6 ? Relay::kReverse :
		  Relay::kOff);
    m_pLCD->UpdateLCD();

    Scheduler::GetInstance()->Run();
}

void MyRobot::TestInit()
{
    printf("MyRobot: starting test mode\n");
    m_pLCD->Clear();
    m_pLCD->Printf(DriverStationLCD::kUser_Line1, 1, "test");
    m_pLCD->UpdateLCD();

 // m_pAutonomousCommand->Cancel();
}

void MyRobot::TestPeriodic()
{
    ++test_periodic;
    m_pLCD->Printf(DriverStationLCD::kUser_Line2, 1, "test %d", test_periodic);
    m_pLCD->UpdateLCD();

    LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(MyRobot);

