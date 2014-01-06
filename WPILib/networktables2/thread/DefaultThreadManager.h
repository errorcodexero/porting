/*
 * DefaultThreadManager.h
 * Desktop
 *
 *  Created on: Sep 21, 2012
 *      Author: Mitchell Wills
 */

#ifndef DEFAULTTHREADMANAGER_H_
#define DEFAULTTHREADMANAGER_H_


class DefaultThreadManager;
class PeriodicNTThread;

#include "networktables2/thread/PeriodicRunnable.h"
#include "networktables2/thread/NTThreadManager.h"
#include "networktables2/thread/NTThread.h"

#include "OSAL/Task.h"

class DefaultThreadManager : public NTThreadManager{
public:
    virtual NTThread* newBlockingPeriodicThread(PeriodicRunnable* r, const char* name);
};

class PeriodicNTThread : public NTThread {
private:
    const char* name;
#if 1 // (defined __vxworks || defined WIN32)
    NTTask* thread;
#else
    pthread_t thread;
#endif
    PeriodicRunnable* r;
    bool run;
#if 1 // (defined __vxworks || defined WIN32)
    int _taskMain();
    static int taskMain(PeriodicNTThread* o);
#else//TODO make return int for pthread as well
    void _taskMain();
    static void* taskMain(PeriodicNTThread* o);
#endif
public:
    PeriodicNTThread(PeriodicRunnable* r, const char* name);
    virtual ~PeriodicNTThread();
    virtual void stop();
#ifdef _WRS_KERNEL
    virtual bool isRunning();
#endif
};

#endif /* DEFAULTTHREADMANAGER_H_ */
