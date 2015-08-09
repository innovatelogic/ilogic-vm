#ifndef __threadmanagerserial_h__
#define __threadmanagerserial_h__

#ifdef _WIN32
#pragma once
#endif

#include "ThreadManager.h"

class ThreadManagerSerial: public ThreadManager
{
public:
	// thread-safe utility functions
	static ThreadManagerSerial &getThreadManagerSerial(void);

	// non thread-safe functions (call only from primary thread)
	void doSerialWork();
	bool unassignThread(int id);
	bool startThread(int id);
	bool stopThread(int id);
	bool setThreadSerialized(int id, bool serial);

	// thread-safe functions
	int getAndResetThreadCalls(int id);
	bool isThreadSerialized(int id);

protected:
	// constants for thread status
	enum {
		THREAD_STATUS_SERIAL_STOPPED                        = THREAD_STATUS_MAX << 1,
		THREAD_STATUS_SERIAL_RUNNING                        = THREAD_STATUS_MAX << 2,
		THREAD_STATUS_SERIAL_TO_PARALLEL_STOPPED_REQUEST    = THREAD_STATUS_MAX << 3,
		THREAD_STATUS_SERIAL_TO_PARALLEL_RUNNING_REQUEST    = THREAD_STATUS_MAX << 4,
		THREAD_STATUS_PARALLEL_TO_SERIAL_STOPPED_REQUEST    = THREAD_STATUS_MAX << 5,
		THREAD_STATUS_PARALLEL_TO_SERIAL_RUNNING_REQUEST    = THREAD_STATUS_MAX << 6,
		THREAD_STATUS_SERIAL_MAX                            = THREAD_STATUS_MAX << 6
	};

	ThreadManagerSerial();
	~ThreadManagerSerial();

	void callbackHandleStatus(int id, int status);

	long m_serialCalls;
};

#endif//__threadmanagerserial_h__