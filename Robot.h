// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

class Robot;

#ifndef _ROBOT_H
#define _ROBOT_H

#include <WPILib.h>

// Subsystems
class BlinkyLight;
class Climber;
class DriveBase;
class RateGyro;
class Shooter;
class TripleSolenoid;

// Commands
class AutoCommand;

// Operator Interface
class OI;

////////////////////////////////////////////////////////////////////
// Digital Inputs:
// 1 - pneumatics pressure switch
//     (polarity handled by Compressor class)

#define	DIGITAL_PRESSURE_SWITCH		1

// 2 - climber left bottom limit switch
// 3 - climber left top limit switch
// 4 - climber right bottom limit switch
// 5 - climber right top limit switch
// 7 - climber left middle limit switch
// 8 - climber right middle limit switch
//     limit switches are normally-open switches to ground
//     so normally true, switch to false = at limit

#define DIGITAL_LIMIT_LEFT_BOTTOM	2	// verified
#define DIGITAL_LIMIT_LEFT_TOP		3	// verified
#define DIGITAL_LIMIT_RIGHT_BOTTOM	4	// verified
#define DIGITAL_LIMIT_RIGHT_TOP		5	// verified

#define DIGITAL_LIMIT_LEFT_MIDDLE	7
#define DIGITAL_LIMIT_RIGHT_MIDDLE	8

// 8 - shooter mid-range position

#define DIGITAL_SHOOTER_CENTER		6

////////////////////////////////////////////////////////////////////
// Analog Inputs:
// 1 - turn rate gyro
//     + output is clockwise rotation

#define ANALOG_GYRO			1

////////////////////////////////////////////////////////////////////
// Relay (Spike) Outputs:
// 1 - compressor

#define	RELAY_COMPRESSOR		1

////////////////////////////////////////////////////////////////////
// Solenoid Outputs:
// 1,2 - climber tilt
//     1 = extend
//     2 = retract
//

#define	SOLENOID_CLIMBER_EXTEND		1
#define	SOLENOID_CLIMBER_RETRACT	2

// 3,4 - shooter position
//     3 = extend (deploy for long range)
//     4 = retract (for shooting at tower goal)

#define SOLENOID_SHOOTER_EXTEND		3
#define SOLENOID_SHOOTER_RETRACT	4

// 5,6 - shooter injector
//     5 = extend (to let a disk drop into firing position)
//     6 = retract (idle)

#define SOLENOID_SHOOTER_LOAD		5
#define SOLENOID_SHOOTER_SHOOT		6

// 7,8 - climber claw
//     7 = extend (close)
//     8 = retract (open)

#define	SOLENOID_CLIMBER_CLAW_CLOSE	7
#define	SOLENOID_CLIMBER_CLAW_OPEN	8

////////////////////////////////////////////////////////////////////
// PWM Outputs:
// 1 - blinky light (not used)

#define PWM_BLINKY			1

// (PWM 2-3 not used)

// 4 - drive left
// 5 - drive right
// 6 - drive rear

#define PWM_DRIVE_LEFT			4
#define PWM_DRIVE_RIGHT			5
#define PWM_DRIVE_REAR			6

// 7 - climber right
// 8 - climber left

#define PWM_CLIMBER_RIGHT		7
#define PWM_CLIMBER_LEFT		8

// CAN (CANJaguar) channels:
// 6 - shooter motor

#define CAN_SHOOTER			6

////////////////////////////////////////////////////////////////////

extern const char *KEY_SPEED_SHORT;
#define	DEFAULT_SPEED_SHORT 1600 // shooting at tower from base of tower

extern const char *KEY_SPEED_MID;
#define	DEFAULT_SPEED_MID   2500 // shooting at high goal from back of tower

extern const char *KEY_SPEED_LONG;
#define	DEFAULT_SPEED_LONG  3200 // shooting at high goal from feeder station

////////////////////////////////////////////////////////////////////

class Robot : public IterativeRobot {
private:
    // Robot is a singleton object, so the constructor is private.
    Robot();

    // Allow construction of the singleton by the WPILib framework.
    friend RobotBase* FRC_userClassFactory();

    // Robot shouldn't be destroyed since references to its member
    // commands and subsystems may still exist in the command scheduler.
    // We can't really remove the (virtual) destructor, but we can hide
    // it by making it private.
    ~Robot();

    // C++ requires that class members must be declared in
    // construction order.  Because of dependencies among these
    // objects, they must be declared and constructed:
    //   sensors and actuators first,
    //   then subsystems based on those sensors and actuators,
    //   then the OI,
    //   then any other commands

    // global sensors
    Compressor* m_compressor;

    // OI
    OI* m_oi;

    // subsystems
    DriveBase* m_driveBase;
    Climber* m_climber;
    Shooter* m_shooter;
    BlinkyLight* m_blinkyLight;
    
public:
    // commands
    AutoCommand* m_autonomousCommand;

    // shared variables
    double m_speed_short;
    double m_speed_mid;
    double m_speed_long;

public:
    // singleton accessor
    static Robot& theRobot() { return (Robot&) RobotBase::getInstance(); };

    // convenience accessors
    static DriveBase* driveBase() { return theRobot().m_driveBase; };
    static Climber* climber() { return theRobot().m_climber; };
    static BlinkyLight* blinkyLight() { return theRobot().m_blinkyLight; };
    static Shooter* shooter() { return theRobot().m_shooter; };
    static OI* oi() { return theRobot().m_oi; };

    void Cancel();
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

#endif
