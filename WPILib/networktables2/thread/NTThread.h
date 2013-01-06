/*
 * NTThread.h
 *
 *  Created on: Sep 21, 2012
 *      Author: Mitchell Wills
 */

#ifndef NTTHREAD_H_
#define NTTHREAD_H_

/**
 * Represents a thread in the network tables system
 * @author mwills
 *
 */
class NTThread
{
public:
	virtual ~NTThread()
	{
	}
	;
	/**
	 * stop the thread
	 */
	virtual void stop() = 0;

	/**
	 * @return true if the thread is running
	 */
#ifdef __VXWORKS__
	virtual bool isRunning() = 0;
#endif
};

#endif /* NTTHREAD_H_ */
