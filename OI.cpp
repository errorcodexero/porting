// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "OI.h"
#include "TimedDrive.h"
#include "Rotate.h"
#include "AimTrim.h"
#include "ClimbCommand.h"
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
#if 1	// using gamepad rather than joystick
    // left stick L/R 1  -> X
    // left stick U/D 2  -> 
    // Z buttons L/R 3   -> throttle
    // right stick L/R 4 -> twist
    // right stick U/D 5 -> Y
    m_pStick->SetAxisChannel( Joystick::kXAxis, 1 );
    m_pStick->SetAxisChannel( Joystick::kThrottleAxis, 3 );
    m_pStick->SetAxisChannel( Joystick::kTwistAxis, 4 );
    m_pStick->SetAxisChannel( Joystick::kYAxis, 5 );
#endif
    m_pGamepadButtonA     = new JoystickButton(m_pStick, 1);
    m_pGamepadButtonB     = new JoystickButton(m_pStick, 2);
    m_pGamepadButtonX     = new JoystickButton(m_pStick, 3);
    m_pGamepadButtonY     = new JoystickButton(m_pStick, 4);
    m_pGamepadLeftBumper  = new JoystickButton(m_pStick, 5);
    m_pGamepadRightBumper = new JoystickButton(m_pStick, 6);
    m_pGamepadBack        = new JoystickButton(m_pStick, 7);
    m_pGamepadStart       = new JoystickButton(m_pStick, 8);

    m_pClimber            = new DSAnalogInput(m_pEIO, 1);
    m_pTip                = new DSAnalogInput(m_pEIO, 2);
    m_pSpeedAdjust        = new DSAnalogInput(m_pEIO, 3);
    m_pShooterTarget      = new DSAnalogInput(m_pEIO, 4);

    m_pDump               = new DSDigitalInput(m_pEIO, 1,
				    DriverStationEnhancedIO::kInputPullUp,
				    true);
    m_pCameraLight        = new DSDigitalInput(m_pEIO, 2,
				    DriverStationEnhancedIO::kInputPullUp,
				    true);
    m_pCameraPosition     = new DSDigitalInput(m_pEIO, 3,
				    DriverStationEnhancedIO::kInputPullUp,
				    true);
    m_pLearnButton        = new DSDigitalInput(m_pEIO, 4,
				    DriverStationEnhancedIO::kInputPullUp,
				    false);	// active-low pushbutton
    m_pManualOverride     = new DSDigitalInput(m_pEIO, 5,
				    DriverStationEnhancedIO::kInputPullUp,
				    true);
    m_pLaunch             = new DSDigitalInput(m_pEIO, 6,
				    DriverStationEnhancedIO::kInputPullDown,
				    true);	// active-high pushbutton

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
    delete m_pClimber;
    delete m_pTip;
    delete m_pSpeedAdjust;
    delete m_pShooterTarget;
    delete m_pDump;
    delete m_pCameraLight;
    delete m_pCameraPosition;
    delete m_pLearnButton;
    delete m_pManualOverride;
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

    m_pNudgeLeft = new TimedDrive( 0.0, 0.0, -.35, 0.20 );
    m_pGamepadLeftBumper->WhenPressed(m_pNudgeLeft);

    m_pNudgeRight = new TimedDrive( 0.0, 0.0, .35, 0.20 );
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
    m_pManualOverride->WhileHeld(m_pShootManual);

    m_pTiltShort = new TiltCommand( Shooter::kShort );
    SmartDashboard::PutData("Tilt Short", m_pTiltShort);

    m_pTiltMid = new TiltCommand( Shooter::kMid );
    SmartDashboard::PutData("Tilt Mid", m_pTiltMid);

    m_pTiltLong = new TiltCommand( Shooter::kLong );
    SmartDashboard::PutData("Tilt Long", m_pTiltLong);

    m_pClimbCommand = new ClimbCommand();
    SmartDashboard::PutData("Climb", m_pClimbCommand);

    // m_pBlinkyOn = new BlinkyOn();
    // SmartDashboard::PutData("Blinky On", m_pBlinkyOn);
    // m_pCameraLight->WhileHeld(m_pBlinkyOn);

    m_pResetRobot = new ResetRobot();
    SmartDashboard::PutData("Reset Robot", m_pResetRobot);
}

