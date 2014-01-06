/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.			      */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef __NTTASK_H__
#define __NTTASK_H__

#include "ErrorBase.h"
#include <vxWorks.h>

/**
 * WPI task is a wrapper for the native Task object.
 * All WPILib tasks are managed by a static task manager for simplified cleanup.
 **/
class NTTask : public ErrorBase
{
public:
    static const uint32_t kDefaultPriority = 101;
    static const int32_t kInvalidTaskID = -1;

    NTTask(const char* name, FUNCPTR function, int32_t priority = kDefaultPriority, uint32_t stackSize = 20000);
    virtual ~NTTask();

#ifdef WIN32
    bool Start(void * arg0);
#else
    bool Start(uint32_t arg0 = 0, uint32_t arg1 = 0, uint32_t arg2 = 0, uint32_t arg3 = 0, uint32_t arg4 = 0,
	    uint32_t arg5 = 0, uint32_t arg6 = 0, uint32_t arg7 = 0, uint32_t arg8 = 0, uint32_t arg9 = 0);
#endif

#ifdef _WRS_KERNEL
    bool Restart();
#endif
    bool Stop();

#ifdef _WRS_KERNEL
    bool IsReady();
    bool IsSuspended();
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

#ifdef WIN32
    FUNCPTR m_function;
    void * m_Arg;
#endif

private:
    char* m_taskName;

#ifdef WIN32
    bool StartInternal();
    HANDLE m_Handle;
    DWORD m_ID;
#else
    FUNCPTR m_function;
#ifndef _WRS_KERNEL
    static int Main( class NTTask * );
    uint32_t m_arg0, m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7, m_arg8, m_arg9;
#endif
    int32_t m_taskID;
#endif

    uint32_t m_stackSize;
    int32_t m_priority;
    bool HandleError(STATUS results);
    DISALLOW_COPY_AND_ASSIGN(NTTask);
};

#endif // __NTTASK_H__
