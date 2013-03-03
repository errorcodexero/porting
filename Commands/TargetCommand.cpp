// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "Robot.h"
#include <math.h>

// m_powercurve uses observations to figure out how far we'll rotate with a
// given time/power.  Offsets between any two values are then computed using a
// simple linear approximation, so, an offset 80 adds (80 - 50) * time/power for
// the value for offset 50, etc.
RobotRotation TargetCommand::m_powercurve[] = {
 // offset, power, time
    {  280,  .5, .93 },  // left
    {  200,  .5, .68 },
    {  150,  .5, .52 },
    {  100,  .5, .35 },
    {   50,  .5, .18 },
    {   20,  .5, .08 },
    {    0,  .5, .00 },  // -- you're already aimed  --
    {  -20, -.5, .06 },  // right
    {  -50, -.5, .14 },
    { -100, -.5, .27 },
    { -150, -.5, .39 },
    { -200, -.5, .51 },
    { -280, -.5, .70 },
};

void TargetCommand::findPower(int offset, double *time, double *power) 
{
    int count = sizeof(m_powercurve) / sizeof(RobotRotation);
    RobotRotation *last = &m_powercurve[0];

    *power = 0;
    *time = 0;

    if (offset < last->offset) {  
        for (int i = 1; i < count; i++) {
            RobotRotation *current = &m_powercurve[i];

            if (offset > current->offset) {  // we're between last and current
                int range = last->offset - current->offset;
                int dcur  = offset - current->offset;  
                double mult = ((double) dcur)/((double) range);

                *power = current->power;

                *time = current->time + mult * (last->time - current->time);
                break;
            }

            last = current;
        }
    }
}

// ReadCamera returns true if we've received a new camera frame.
bool TargetCommand::ReadCamera() 
{
    int frame     = (int) SmartDashboard::GetNumber("frameNum");
    bool newframe = frame > m_cameraFrame;

    if (newframe) {
        m_tgtOffset = (int) (SmartDashboard::GetNumber("offset") + 0.5);
        m_cameraFrame = frame;
    }

    return newframe;
}

// Identify resources required by this command.
// Other commands that are using these resources will be Canceled
// when this command is Started.
TargetCommand::TargetCommand() 
{
    Requires(Robot::driveBase());

    SmartDashboard::PutNumber("frameNum", 0.0);
    SmartDashboard::PutNumber("height", 0.0);
    SmartDashboard::PutNumber("width",  0.0);
    SmartDashboard::PutNumber("offset", 0.0);
    SmartDashboard::PutNumber("aimFrames", 0.0);
    SmartDashboard::PutBoolean("aiming", false);
}

// Called just before this Command runs the first time
void TargetCommand::Initialize()
{
    printf("TargetCommand::Initialize\n");

    m_cameraFrame = -1;
    // m_tgtHeight = 0;
    // m_tgtWidth = 0;
    m_tgtOffset = 0;

    ReadCamera();

    m_cameraStartFrame = m_cameraFrame;

    // let the world know that somebody pushed the button 
    SmartDashboard::PutBoolean("aiming", true);

    // figure out time/power
    double power, time;
    findPower(m_tgtOffset, &time, &power);

    printf("frame %d offset %d time %g power %g\n", m_cameraFrame, m_tgtOffset, time, power);

    Set(0.0, 0.0, power, time);
}

// Called once after IsFinished returns true
void TargetCommand::End() 
{
    ReadCamera();  // one last time, just to report 

    int aimFrames = m_cameraFrame - m_cameraStartFrame;

    printf("+++ offset %d frames %d\n", m_tgtOffset, aimFrames);

    SmartDashboard::PutBoolean("aiming", false);
    SmartDashboard::PutNumber("aimFrames", aimFrames);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TargetCommand::Interrupted()
{
    printf("TargetCommand::Interrupted\n");
}
