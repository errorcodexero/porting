// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "OI.h"
#include "TimedDrive.h"
#include "Rotate.h"
#include "AimTrim.h"
#include "TargetCommand.h"
#include "SelectTarget.h"
#include "ShootCommand.h"
#include "ShootManual.h"
#include "TiltCommand.h"
#include "ResetRobot.h"

OI::OI() 
{
    // Instantiate operator controls
    m_pDS = DriverStation::GetInstance();
    m_pEIO = &m_pDS->GetEnhancedIO();
    m_pLCD = DriverStationLCD::GetInstance();

    m_pStick = new Joystick(1);
    m_pGamepadButtonA     = new JoystickButton(m_pStick, 1);
    m_pGamepadButtonB     = new JoystickButton(m_pStick, 2);
    m_pGamepadButtonX     = new JoystickButton(m_pStick, 3);
    m_pGamepadButtonY     = new JoystickButton(m_pStick, 4);
    m_pGamepadLeftBumper  = new JoystickButton(m_pStick, 5);
    m_pGamepadRightBumper = new JoystickButton(m_pStick, 6);
    m_pGamepadBack        = new JoystickButton(m_pStick, 7);
    m_pGamepadStart       = new JoystickButton(m_pStick, 8);

    // analog input 1 was 3-position momentary climber motor control
    // analog input 2 was 3-position momentary climber tilt control

    m_pSpeedAdjust        = new DSAnalogInput(m_pEIO, 3);
    m_pShooterTilt        = new DSAnalogInput(m_pEIO, 4);
    m_pAutoSelect         = new DSAnalogInput(m_pEIO, 5);
    m_pShooterTarget      = new DSAnalogInput(m_pEIO, 6);

    // digital inputs
    m_pClimber            = new DSDigitalInput(m_pEIO, 1,
				    DriverStationEnhancedIO::kInputPullUp,
				    true);

    // digital input 2 not connected/not used
    m_pLightsOn           = new DSDigitalInput(m_pEIO, 2,
				    DriverStationEnhancedIO::kInputPullUp,
				    true);	// active-high toggle

    m_pDriverSelect       = new DSDigitalInput(m_pEIO, 3,
				    DriverStationEnhancedIO::kInputPullUp,
				    true);	// active-high toggle

    m_pShooterOverride    = new DSDigitalInput(m_pEIO, 4,
				    DriverStationEnhancedIO::kInputPullUp,
				    false);	// active-low pushbutton

    m_pShooterOn          = new DSDigitalInput(m_pEIO, 5,
				    DriverStationEnhancedIO::kInputPullUp,
				    true);	// active-high toggle

    m_pLaunch             = new DSDigitalInput(m_pEIO, 6,
				    DriverStationEnhancedIO::kInputPullDown,
				    true);	// active-high pushbutton

    m_pLearn              = new DSDigitalInput(m_pEIO, 7,
				    DriverStationEnhancedIO::kInputPullUp,
				    false);	// active-low pushbutton

    m_pReadyLED           = new DSDigitalOutput(m_pEIO, 16);
}

OI::~OI()
{
    // initialized in constructor
    delete m_pStick;
    delete m_pGamepadButtonA;
    delete m_pGamepadButtonB;
    delete m_pGamepadButtonX;
    delete m_pGamepadButtonY;
    delete m_pGamepadLeftBumper;
    delete m_pGamepadRightBumper;
    delete m_pGamepadBack;
    delete m_pGamepadStart;
    delete m_pSpeedAdjust;
    delete m_pShooterTarget;
    delete m_pShooterTilt;
    delete m_pAutoSelect;
    delete m_pClimber;
    delete m_pLightsOn;
    delete m_pDriverSelect;
    delete m_pLearn;
    delete m_pShooterOverride;
    delete m_pShooterOn;
    delete m_pLaunch;
    delete m_pReadyLED;

    // initialized in Initialize()
    if (m_pNudgeLeft) delete m_pNudgeLeft;
    if (m_pNudgeRight) delete m_pNudgeRight;
    if (m_pRotateFwd) delete m_pRotateFwd;
    if (m_pRotateRev) delete m_pRotateRev;
    if (m_pTrimLeft) delete m_pTrimLeft;
    if (m_pTrimRight) delete m_pTrimRight;
    if (m_pTargetCommand) delete m_pTargetCommand;
    if (m_pSelectTargetLeft) delete m_pSelectTargetLeft;
    if (m_pSelectTargetRight) delete m_pSelectTargetRight;
    if (m_pSelectTargetMid) delete m_pSelectTargetMid;
    if (m_pShootShort) delete m_pShootShort;
    if (m_pShootMid) delete m_pShootMid;
    if (m_pShootLong) delete m_pShootLong;
    if (m_pShootManual) delete m_pShootManual;
    if (m_pTiltShort) delete m_pTiltShort;
    if (m_pTiltMid) delete m_pTiltMid;
    if (m_pTiltLong) delete m_pTiltLong;
    if (m_pResetRobot) delete m_pResetRobot;
}


void OI::Initialize()
{
    // Link controls to commands
    m_pTargetCommand = new TargetCommand();
    m_pGamepadButtonA->WhenPressed(m_pTargetCommand);

    m_pSelectTargetRight = new SelectTarget("right");
    m_pGamepadButtonB->WhenPressed(m_pSelectTargetRight);

    m_pSelectTargetLeft = new SelectTarget("left");
    m_pGamepadButtonX->WhenPressed(m_pSelectTargetLeft);

    m_pSelectTargetMid = new SelectTarget("mid");
    m_pGamepadButtonY->WhenPressed(m_pSelectTargetMid);

    m_pNudgeLeft = new TimedDrive( 0.0, 0.0, -.20, 0.20 );
    m_pGamepadLeftBumper->WhenPressed(m_pNudgeLeft);

    m_pNudgeRight = new TimedDrive( 0.0, 0.0, .20, 0.20 );
    m_pGamepadRightBumper->WhenPressed(m_pNudgeRight);

    m_pTrimLeft = new AimTrim(true);
    m_pGamepadBack->WhenPressed(m_pTrimLeft);

    m_pTrimRight = new AimTrim(false);
    m_pGamepadStart->WhenPressed(m_pTrimRight);

    m_pRotateFwd = new Rotate(1);
    SmartDashboard::PutData("Rotate Fwd", m_pRotateFwd);

    m_pRotateRev = new Rotate(-1);
    SmartDashboard::PutData("Rotate Rev", m_pRotateRev);

    m_pShootShort = new ShootCommand( Shooter::kShort );
    SmartDashboard::PutData("Shoot Short", m_pShootShort);

    m_pShootMid = new ShootCommand( Shooter::kMid );
    SmartDashboard::PutData("Shoot Mid", m_pShootMid);

    m_pShootLong = new ShootCommand( Shooter::kLong );
    SmartDashboard::PutData("Shoot Long", m_pShootLong);

    m_pShootManual = new ShootManual();
    m_pShooterOn->WhileHeld(m_pShootManual);

    m_pTiltShort = new TiltCommand( Shooter::kShort );
    SmartDashboard::PutData("Tilt Short", m_pTiltShort);

    m_pTiltMid = new TiltCommand( Shooter::kMid );
    SmartDashboard::PutData("Tilt Mid", m_pTiltMid);

    m_pTiltLong = new TiltCommand( Shooter::kLong );
    SmartDashboard::PutData("Tilt Long", m_pTiltLong);

    // m_pBlinkyOn = new BlinkyOn();
    // SmartDashboard::PutData("Blinky On", m_pBlinkyOn);
    // m_pLightsOn->WhileHeld(m_pBlinkyOn);

    m_pResetRobot = new ResetRobot();
    SmartDashboard::PutData("Reset Robot", m_pResetRobot);
}

