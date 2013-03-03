// First Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef TARGETCOMMAND_H
#define TARGETCOMMAND_H

#include <Commands/TimedDrive.h>

#define CAMERA_FOV   45.0
#define IMAGE_WIDTH 640.0
#define MAX_AIM_TIME  4.00 // in seconds


struct RobotRotation {
    int    offset; // how far to go
    float  power;
    float  time;
};

// TargetCommand uses a simple time/power vs. offset algorithm to aim the robot at the 
// target.  The aiming process proceeds in three steps:
//   1 - Get a camera frame, determine offset to target (m_tgtOffset)
//   2 - based on pre-measured rotation metrics, compute necessary time/power
//   3 - use TimedDrive to turn drive the robot
class TargetCommand: public TimedDrive {
  private:
    int m_cameraStartFrame; // first frame number  - from camera
    int m_cameraFrame;   // frame number  - from camera
    int m_tgtOffset;     // target offset - from
    // int m_tgtHeight;     // target height - from camera
    // int m_tgtWidth;      // target width  - from camera

    static RobotRotation m_powercurve[]; 

    void findPower(int offset, double *time, double *power);
    bool ReadCamera();   // returns true if there's a new camera frame

  public:
    TargetCommand();
    virtual void Initialize();

    virtual void End();
    virtual void Interrupted();
};

#endif
