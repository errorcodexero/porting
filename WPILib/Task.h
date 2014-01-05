/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.			      */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef __TASK_H__
#define __TASK_H__

#include "ErrorBase.h"
#include <vxWorks.h>

/**
 * WPI task is a wrapper for the native Task object.
 * All WPILib tasks are managed by a static task manager for simplified cleanup.
 **/
class Task : public ErrorBase
{
public:
    static const uint32_t kDefaultPriority = 101;
    static const int32_t kInvalidTaskID = -1;

    Task(const char* name, FUNCPTR function, int32_t priority = kDefaultPriority, uint32_t stackSize = 20000);
    virtual ~Task();

    bool Start(uint32_t arg0 = 0, uint32_t arg1 = 0, uint32_t arg2 = 0, uint32_t arg3 = 0, uint32_t arg4 = 0,
	    uint32_t arg5 = 0, uint32_t arg6 = 0, uint32_t arg7 = 0, uint32_t arg8 = 0, uint32_t arg9 = 0);
#ifndef _WRS_KERNEL
private:
    static int Main( class Task * );
    uint32_t m_arg0, m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7, m_arg8, m_arg9;
#endif

public:
#ifdef _WRS_KERNEL
    bool Restart();
#endif
    bool Stop();

#ifdef _WRS_KERNEL
    bool IsReady();
    bool IsSuspended();
#endif

#ifdef _WRS_KERNEL
    bool Suspend();
    bool Resume();
#endif

    bool Verify();

#ifdef _WRS_KERNEL
    int32_t GetPriority();
    bool SetPriority(int32_t priority);
#endif
    const char* GetName();
    int32_t GetID();

private:
    FUNCPTR m_function;
    char* m_taskName;
    int32_t m_taskID;
    uint32_t m_stackSize;
    int32_t m_priority;
    bool HandleError(STATUS results);
    DISALLOW_COPY_AND_ASSIGN(Task);
};

#endif // __TASK_H__
