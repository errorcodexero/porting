// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Shooter.h"
#include <math.h>

const double Shooter::kPollInterval = 0.034;
const double Shooter::kReportInterval = 0.300;

#define SYNC_GROUP 0x40

// Constructor
Shooter::Shooter( int motorChannel, int positionerChannel, int switchChannel,
			int injectorChannel )
    : Subsystem("Shooter")
{
    LiveWindow *lw = LiveWindow::GetInstance();

    m_motor = new CANJaguar( motorChannel );
    lw->AddActuator("Shooter", "Motor1", m_motor);
    m_motor->SetSafetyEnabled(false);	// motor safety off while configuring

    m_motor2 = new CANJaguar( motorChannel+1 );
    lw->AddActuator("Shooter", "Motor2", m_motor2);
    m_motor2->SetSafetyEnabled(false);	// motor safety off while configuring

    m_P = 1.000;
    SmartDashboard::PutNumber("Shooter P", m_P);

    m_I = 0.005;
    SmartDashboard::PutNumber("Shooter I", m_I);

    m_D = 0.000;
    SmartDashboard::PutNumber("Shooter D", m_D);

    m_speed = 3200.0;
    SmartDashboard::PutNumber("Shooter Speed", m_speed);

    SmartDashboard::PutNumber("Shooter Voltage", 0.0);
    SmartDashboard::PutNumber("Shooter RPM", 0.0);

    m_speedTolerance = 0.25;  // +/- 0.25% speed tolerance
    SmartDashboard::PutNumber("Shooter Tolerance (%)", m_speedTolerance);

    m_speedStable = 0.9; // must remain in tolerance at least this long
    SmartDashboard::PutNumber("Shooter Stable Time", m_speedStable);

    m_timeAtSpeed = 0;
    m_isUpToSpeed = false;
    SmartDashboard::PutBoolean("Shooter UpToSpeed", m_isUpToSpeed);

    // Initialize positioner
    m_distance = kUnknown;
    SmartDashboard::PutString("Shooter Distance", "Unknown");
    m_positioner = new TripleSolenoid( positionerChannel,
				       positionerChannel+1,
				       switchChannel );
    lw->AddActuator("Shooter", "Positioner", m_positioner);
    lw->AddSensor("Shooter", "PositionCenter", m_positioner->m_switch);
    SmartDashboard::PutString("Shooter Position", "Unknown");
    SmartDashboard::PutBoolean("Shooter InPosition", false);

    // Initialize injector
    m_injectCounter = 0;
    m_injectTime = 0.6; // time for full travel
    SmartDashboard::PutNumber("Shooter Injection Time", m_injectTime);
    m_injector = new DoubleSolenoid( injectorChannel, injectorChannel+1 );
    lw->AddActuator("Shooter", "Injector", m_injector);
    SmartDashboard::PutBoolean("Shooter Active", false);

    // Ready/Shooting status
    SmartDashboard::PutBoolean("Shooter Ready", false);

    m_notifier = new Notifier( Shooter::TimerEvent, this );
    m_report = 0;
}

Shooter::~Shooter()
{
//  printf("Shooter::~Shooter\n");
    Stop();
    delete m_notifier;
    delete m_positioner;
    delete m_motor;
    delete m_motor2;
}

Shooter::TargetDistance Shooter::GetAngle()
{
	return m_distance;
}

/*
 * Move the shooter to the desired position
 */
void Shooter::SetAngle(TargetDistance target)
{
    m_distance = target;

    // map TargetDistance to TripleSolenoid::Position
    switch (m_distance) {
    case kShort:
	m_positioner->SetPosition(TripleSolenoid::kRetracted);
	SmartDashboard::PutString("Shooter Distance", "Short");
	break;
    case kMid:
	m_positioner->SetPosition(TripleSolenoid::kCenter);
	SmartDashboard::PutString("Shooter Distance", "Mid");
	break;
    case kLong:
	m_positioner->SetPosition(TripleSolenoid::kExtended);
	SmartDashboard::PutString("Shooter Distance", "Long");
	break;
    default:
	break;
    }
}

/*
 * Set the shooter wheel speed.
 * Start/stop the wheel.
 */
void Shooter::SetSpeed( double speed )
{
    m_speed = speed;
    SmartDashboard::PutNumber("Shooter Speed", m_speed);
}

void Shooter::Start()
{
//  printf("Shooter::Start\n");

    // Set control mode
    m_motor->ChangeControlMode( CANJaguar::kSpeed );
    m_motor2->ChangeControlMode( CANJaguar::kSpeed );

    // Set encoder as reference device for speed controller mode:
    m_motor->SetSpeedReference( CANJaguar::kSpeedRef_Encoder );
    m_motor2->SetSpeedReference( CANJaguar::kSpeedRef_Encoder );

    // Set codes per revolution parameter:
    m_motor->ConfigEncoderCodesPerRev( 1 );
    m_motor2->ConfigEncoderCodesPerRev( 1 );

    // Set Jaguar PID parameters:
    m_P = SmartDashboard::GetNumber("Shooter P");
    m_I = SmartDashboard::GetNumber("Shooter I");
    m_D = SmartDashboard::GetNumber("Shooter D");
    m_motor->SetPID( m_P, m_I, m_D );
    m_motor2->SetPID( m_P, m_I, m_D );

    // Enable Jaguar control:
    m_motor->EnableControl();
    m_motor2->EnableControl();

    // Increase motor safety timer to something suitably long
    // Poke the motor speed to reset the watchdog, then enable the watchdog
    m_speed = SmartDashboard::GetNumber("Shooter Speed");
    m_motor->SetExpiration(2.0);
    m_motor2->SetExpiration(2.0);
    m_motor->Set(m_speed, SYNC_GROUP);
    m_motor2->Set(m_speed, SYNC_GROUP);
    CANJaguar::UpdateSyncGroup(SYNC_GROUP);
    m_motor->SetSafetyEnabled(true);
    m_motor2->SetSafetyEnabled(true);

    // Start run timer
    m_report = 0;
    m_notifier->StartPeriodic(kPollInterval);
}

void Shooter::Stop()
{
//  printf("Shooter::Stop\n");

    // stop timer
    m_notifier->Stop();

    // stop motors
    m_motor->StopMotor();
    m_motor2->StopMotor();
    m_motor->SetSafetyEnabled(false);
    m_motor2->SetSafetyEnabled(false);
    SmartDashboard::PutNumber("Shooter Voltage", 0.0);
    SmartDashboard::PutNumber("Shooter RPM", 0.0);

    // stop positioner
    m_positioner->Stop();

    // stop injector
    m_injector->Set( DoubleSolenoid::kOff );

    // not running any more!
    m_timeAtSpeed = 0;
    m_isUpToSpeed = false;
    SmartDashboard::PutBoolean("Shooter UpToSpeed", m_isUpToSpeed);
}

void Shooter::TimerEvent( void *param )
{
    ((Shooter *)param)->Run();
}

void Shooter::Run()
{
    switch (m_injector->Get()) {
    case DoubleSolenoid::kReverse:
	m_injectTime = SmartDashboard::GetNumber("Shooter Injection Time");
	if (++m_injectCounter * kPollInterval >= m_injectTime) {
	    m_injector->Set(DoubleSolenoid::kForward);
	}
	break;
    case DoubleSolenoid::kForward:
	if (m_injectCounter) {
	    if (--m_injectCounter == 0) {
		m_injector->Set(DoubleSolenoid::kOff);
		SmartDashboard::PutBoolean("Shooter Active", false);
	    }
	}
	break;
    case DoubleSolenoid::kOff:
	break;
    }

    m_speed = SmartDashboard::GetNumber("Shooter Speed");
    m_motor->Set(m_speed, SYNC_GROUP);
    m_motor2->Set(m_speed, SYNC_GROUP);
    CANJaguar::UpdateSyncGroup(SYNC_GROUP);

    if (++m_report * kPollInterval >= kReportInterval) {
	ReportSpeed();

	bool ready = IsReadyToShoot();
	SmartDashboard::PutBoolean("Shooter Ready", ready);
    }
}

void Shooter::ReportSpeed()
{
    // UINT8 jagHWVersion = m_motor->GetHardwareVersion();
    // UINT32 jagFWVersion = m_motor->GetFirmwareVersion();
    // double jagTemp  = m_motor->GetTemperature();
    double jagOutV = m_motor->GetOutputVoltage();
    double jagSpeed = m_motor->GetSpeed(); 

    // Send values to SmartDashboard
    // SmartDashboard::PutNumber("Jaguar Hardware Version", double(jagHWVersion));
    // SmartDashboard::PutNumber("Jaguar Firmware Version", double(jagFWVersion));
    // SmartDashboard::PutNumber("Shooter Temp", jagTemp);
    SmartDashboard::PutNumber("Shooter Voltage", jagOutV);
    SmartDashboard::PutNumber("Shooter RPM", jagSpeed);

    double error = jagSpeed - m_speed;
    m_speedTolerance = SmartDashboard::GetNumber("Shooter Tolerance (%)");
    if (fabs(error) < m_speed * m_speedTolerance / 100.0) {
	// 1000 is arbitrary, just to limit number of digits
	//  displayed on the dashboard.
	if (m_timeAtSpeed < 1000) m_timeAtSpeed++;
	m_speedStable = SmartDashboard::GetNumber("Shooter Stable Time");
	m_isUpToSpeed = (m_timeAtSpeed * kReportInterval >= m_speedStable);
    } else {
	m_timeAtSpeed = 0;
	m_isUpToSpeed = false;
    }
    SmartDashboard::PutBoolean("Shooter UpToSpeed", m_isUpToSpeed);
}

// check positioner at correct angle
bool Shooter::IsInPosition()
{
    TripleSolenoid::Position position = m_positioner->GetPosition();
    const char *where = "";
    bool inPosition = false;

    switch (position) {
    case TripleSolenoid::kUnknown:
	where = "unknown";
	break;
    case TripleSolenoid::kRetracted:
	where = "retracted";
	break;
    case TripleSolenoid::kPartlyRetracted:
	where = "partly retracted";
	break;
    case TripleSolenoid::kCenter:
	where = "center";
	break;
    case TripleSolenoid::kPartlyExtended:
	where = "partly extended";
	break;
    case TripleSolenoid::kExtended:
	where = "extended";
	break;
    }
    SmartDashboard::PutString("Shooter Position", where);

    switch (m_distance) {
    case kUnknown:
	inPosition = true;
	break;
    case kShort:
	inPosition = (position == TripleSolenoid::kRetracted);
	break;
    case kMid:
	inPosition = (position == TripleSolenoid::kCenter);
	break;
    case kLong:
	inPosition = (position == TripleSolenoid::kExtended);
	break;
    }
    SmartDashboard::PutBoolean("Shooter InPosition", inPosition);
    return inPosition;
}

bool Shooter::IsUpToSpeed()
{
    return m_isUpToSpeed;
}

bool Shooter::IsInjectorActive()
{
    bool active = (m_injectCounter != 0);
    return active;
}

bool Shooter::IsReadyToShoot()
{
    // we are ready when in position, wheel up to speed and injector idle
    return (IsInPosition() && IsUpToSpeed() && !IsInjectorActive());
}

/*
 * Launch a disk
 */
void Shooter::Inject()
{
    m_injector->Set( DoubleSolenoid::kReverse );
    SmartDashboard::PutBoolean("Shooter Active", true);
    m_injectCounter++;
    SmartDashboard::PutBoolean("Shooter Ready", false);
}

/*
 * false is the ready (extended) position,
 * true is the inject (retracted) position
 */
void Shooter::SetInjector( bool state )
{
    m_injector->Set(state ? DoubleSolenoid::kReverse : DoubleSolenoid::kForward );
}
