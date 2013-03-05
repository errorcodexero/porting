// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef TIMED_DRIVE_H
#define TIMED_DRIVE_H

#include <Commands/Command.h>

class TimedDrive: public Command {
private:
    double m_x, m_y, m_t, m_time;
    double m_startTime;
public:
    TimedDrive( double x = 0.0, double y = 0.0, double t = 0.0,
    		double seconds = 0 );
    void Set( double x, double y, double t, double seconds );
    virtual void Initialize();
    virtual void Execute();
    void Stop();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
