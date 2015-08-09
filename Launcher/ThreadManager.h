#ifndef __threadmanager_h__
#define __threadmanager_h__

#ifdef _WIN32
#pragma once
#endif

#include "..\..\Foundation\StdafxFoundation.h"

class ThreadManager
{
public:
	// constants for thread status
	enum {
		THREAD_STATUS_ALL                       = ~0,
		THREAD_STATUS_UNASSIGNED                = 1 << 0,
		THREAD_STATUS_STOPPED                   = 1 << 1,
		THREAD_STATUS_RUNNING                   = 1 << 2,
		THREAD_STATUS_RUNNING_REQUEST           = 1 << 3,
		THREAD_STATUS_UNASSIGN_REQUEST          = 1 << 4,
		THREAD_STATUS_TERMINATE_REQUEST         = 1 << 5,
		THREAD_STATUS_MAX                       = 1 << 5
	};

	typedef void (*WorkFunction)();
	typedef void (*FinalizeFunction)();

	// local data key
	typedef DWORD LocalDataKeyType;
	
	// thread object
	typedef HANDLE ThreadType;

	// thread pool callback
	static unsigned int __stdcall threadCallback(void *param);

	// threadInfo structure keeps track of each thread in the pool
	typedef struct 
	{
		int						id;
		int						status;
		WorkFunction			workFunc;
		FinalizeFunction		finalizeFunc;
		long					calls; // long to take advantage of InterlockedIncrement on WIN32
		ThreadType				thread;
		class CriticalSection	*criticalSection;
		void					*localData;
	} threadInfo;

	static const int INVALID_THREAD_ID = -1;  //< thread ID number returned on error

	static const LocalDataKeyType	sm_invalidLocalDataKey;
	static const ThreadType			sm_invalidThread;
	static class CriticalSection	sm_singletonCS;
	static ThreadManager*			sm_threadManager;

public:
	static ThreadManager &getThreadManager();

	static void yield();
	static void sleep(int milliseconds);

	ThreadManager();
	~ThreadManager();

	bool	init(int threadPoolSize, void *primaryThreadLocalData = NULL);
	bool	isPrimaryThread() const;
	void*	getLocalDataRaw() const;
	int		getThreadPoolSize() const;
	virtual int getAndResetThreadCalls(int id);

	void	shutdown(void);
	int		AssignThreadToWorkFunction(WorkFunction workFunc, FinalizeFunction finalizeFunc = NULL, void *threadLocalData = NULL);
	bool	SetThreadAffinity(int Id, DWORD_PTR core_id = 0L);
	bool	unassignThread(int id);

	virtual bool startThread(int id);
	virtual bool stopThread(int id);

	void		 callWorkFunctionForThread(int id);
	void		 callFinalizeFunctionForThread(int id);
	virtual void callbackHandleStatus(int id, int status);

	void lockThread(int id);
	void unlockThread(int id);

	int getThreadStatus(int id) const;
	int SetThreadStatus(int id, int condition, int value);

	void cleanup(bool waitForThreadTermination);
	void associateLocalDataWithThread(int id);

	threadInfo* GetThreadInfo(int id);

protected:
//private:
	bool				m_initialized;
	threadInfo			*m_threadPool;
	int					m_threadPoolSize;
	void				*m_primaryThreadLocalData;
	LocalDataKeyType	m_localDataKey;
	CriticalSection		m_internalDataCS;
};

#endif//__threadmanager_h__