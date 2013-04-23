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

// 2-5 - not used

// 6 - shooter mid-range position

#define DIGITAL_SHOOTER_CENTER		6

// 7-8 - not used

////////////////////////////////////////////////////////////////////
// Analog Inputs:
// 1 - turn rate gyro
//     + output is clockwise rotation

#define ANALOG_GYRO			1

// 2-7 - not used

// 8 - battery (from analog bumper)

////////////////////////////////////////////////////////////////////
// Relay (Spike) Outputs:
// 1 - compressor

#define	RELAY_COMPRESSOR		1

////////////////////////////////////////////////////////////////////
// Solenoid Outputs:
//

// 1,2 - not used

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

// 7   - climber
//     true = extend
//     false = retract
//     retracts when robot is disabled

#define	SOLENOID_CLIMBER_EXTEND		7

// 8   - not used

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

// 7,8 - not used

////////////////////////////////////////////////////////////////////
// CAN (CANJaguar) channels:
// 6 - shooter motor

#define CAN_SHOOTER			6

////////////////////////////////////////////////////////////////////

extern const char KEY_SPEED_SHORT[];
#define	DEFAULT_SPEED_SHORT  1600 // shooting at tower from base of tower

extern const char KEY_SPEED_MID_2[];
#define	DEFAULT_SPEED_MID_2  2300 // shooting at 2 pt. goal from back of tower

extern const char KEY_SPEED_MID_3[];
#define	DEFAULT_SPEED_MID_3  2500 // shooting at 3 pt. goal from back of tower

extern const char KEY_SPEED_LONG_2[];
#define	DEFAULT_SPEED_LONG_2 3000 // shooting at 2 pt. goal from feeder station

extern const char KEY_SPEED_LONG_3[];
#define	DEFAULT_SPEED_LONG_3 3200 // shooting at 3 pt. goal from feeder station

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

    // shooter speed
    double m_speed_short;
    double m_speed_mid_2;
    double m_speed_mid_3;
    double m_speed_long_2;
    double m_speed_long_3;

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
