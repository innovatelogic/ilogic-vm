#include "LauncherStdAfx.h"

const int THREAD_SLEEP_DURATION = 50;  //< milliseconds of sleep between status checks

const ThreadManager::LocalDataKeyType ThreadManager::sm_invalidLocalDataKey = TLS_OUT_OF_INDEXES;
const ThreadManager::ThreadType ThreadManager::sm_invalidThread = NULL;

CriticalSection ThreadManager::sm_singletonCS;
ThreadManager *ThreadManager::sm_threadManager = NULL;

//------------------------------------------------------------------------
ThreadManager &ThreadManager::getThreadManager(void)
{
	if (sm_threadManager == NULL)
	{
		sm_singletonCS.enter();
		if(sm_threadManager == NULL)
		{
			sm_threadManager = new ThreadManager();
		}
		sm_singletonCS.leave();
	}
	return *sm_threadManager;
}

//------------------------------------------------------------------------
ThreadManager::ThreadManager()
: m_initialized(false)
, m_threadPool(NULL)
, m_threadPoolSize(0)
, m_primaryThreadLocalData(NULL)
{
	m_localDataKey = sm_invalidLocalDataKey;
}

//------------------------------------------------------------------------
ThreadManager::~ThreadManager()
{

}

//------------------------------------------------------------------------
void ThreadManager::yield()
{
	Sleep(0);
}

//------------------------------------------------------------------------
void ThreadManager::sleep(int milliseconds)
{
	assert(milliseconds > 0);
	Sleep(milliseconds);
}

//------------------------------------------------------------------------
void *ThreadManager::getLocalDataRaw() const
{
	assert(m_initialized);

	void *output = NULL;

	output = TlsGetValue(m_localDataKey);

	return output;
}

//------------------------------------------------------------------------
bool ThreadManager::isPrimaryThread() const
{
	return(getLocalDataRaw() == this);
}

//------------------------------------------------------------------------
int ThreadManager::getThreadPoolSize(void) const
{
	return m_threadPoolSize;
}

//------------------------------------------------------------------------
int ThreadManager::getAndResetThreadCalls(int id)
{
	assert(m_initialized);
	assert((id >= 0) && (id < m_threadPoolSize));

	int output = 0;

	lockThread(id);
	output = m_threadPool[id].calls;
	m_threadPool[id].calls = 0;
	unlockThread(id);

	return output;
}
//------------------------------------------------------------------------
bool ThreadManager::init(int threadPoolSize, void *primaryThreadLocalData)
{
	bool output = false;

	// make sure init isn't called on a succesfully initialized ThreadManager
	m_internalDataCS.enter();

	if (!m_initialized)
	{
		bool successful = true;

		// allocate a new thread pool
		if (threadPoolSize > 0)
		{
			// create the thread pool
			m_threadPool = new threadInfo[threadPoolSize];
			if (m_threadPool == NULL)
			{
				successful = false;
			}
			else
			{
				m_threadPoolSize = threadPoolSize;

				for (int i = 0; i < threadPoolSize; i++)
				{
					// populate the threadInfo structure
					m_threadPool[i].id = i;
					m_threadPool[i].status = THREAD_STATUS_UNASSIGNED;
					m_threadPool[i].workFunc = NULL;
					m_threadPool[i].calls = 0;
					m_threadPool[i].thread = sm_invalidThread;
					m_threadPool[i].criticalSection = NULL;
					m_threadPool[i].localData = NULL;

					// initialize the critical section
					m_threadPool[i].criticalSection = new CriticalSection();
					
					if ((m_threadPool[i].criticalSection == NULL) || (m_threadPool[i].criticalSection->isValid() == false))
					{
						// remember this thread pool size so we can deallocate threads properly later
						m_threadPoolSize = i;
						break;
					}

					// spawn the thread

					// we use _beginthreadex to spawn threads because CreateThread and _beginthread
					// have inconsistencies in corner cases
					m_threadPool[i].thread = (ThreadType)_beginthreadex(
						NULL,
						0,
						ThreadManager::threadCallback,
						&(m_threadPool[i].id),
						0,
						NULL
						);

					if (m_threadPool[i].thread == sm_invalidThread)
					{
						successful = false;
						// remember this thread pool size so we can deallocate threads properly later
						m_threadPoolSize = i;
						break;
					}
				}
			}
		}

		if (successful)
		{
			// create the local data key and seed it with our primaryThreadLocalData

			m_localDataKey = TlsAlloc();

			if(m_localDataKey == sm_invalidLocalDataKey)
			{
				successful = false;
			}
			else
			{
				// okay, instead of storing primaryThreadLocalData, we store a pointer to this
				// which will let us know when we are executing from the primary thread
				successful = TlsSetValue(m_localDataKey, this) ? true : false; // avoid warning
			}

			// store the primaryThreadLocalData for later
			m_primaryThreadLocalData = primaryThreadLocalData;  
		}

		if (!successful)
		{
			// deallocate everything we made
			// don't wait for threads to terminate -- they were never assigned work functions
			cleanup(false);
		}
		else
		{        
			// everything worked, make sure we aren't initialized again
			m_initialized = true;   
			output = true;
		}
	}

	m_internalDataCS.leave();
	return output;
}

//------------------------------------------------------------------------
void ThreadManager::shutdown()
{
	m_internalDataCS.enter();

	if (m_initialized)
	{
		assert(isPrimaryThread());
		// call cleanup and ensure that threads get terminated before returning
		cleanup(true);
		m_initialized = false;
	}

	m_internalDataCS.leave();
}

//------------------------------------------------------------------------
int ThreadManager::AssignThreadToWorkFunction(WorkFunction workFunc, FinalizeFunction finalizeFunc, void *localData /*= NULL*/)
{
	assert(m_initialized);
	assert(isPrimaryThread());

	int id = INDEX_NONE;

	// enter major critical section
	// we can now access thread pool data directly
	m_internalDataCS.enter();

	// find the next available thread id
	for (int i = 0; i < m_threadPoolSize; i++)
	{
		if (m_threadPool[i].status == THREAD_STATUS_UNASSIGNED)
		{
			id = i;
			break;
		}
	}

	if (id != INDEX_NONE)
	{
		m_threadPool[id].status = THREAD_STATUS_STOPPED;

		// populate the threadInfo record
		m_threadPool[id].workFunc = workFunc;
		m_threadPool[id].finalizeFunc = finalizeFunc;
		m_threadPool[id].calls = 0;
		m_threadPool[id].localData = localData;
	}

	m_internalDataCS.leave();

	return id;
}

//------------------------------------------------------------------------
bool ThreadManager::SetThreadAffinity(int Id, DWORD_PTR core_id /*= 0L*/)
{
	assert(m_initialized);
	
	bool bResult = false;
	
	m_internalDataCS.enter();

	for (int i = 0; i < m_threadPoolSize; i++)
	{
		if (m_threadPool[i].id == Id)
		{
			if (SetThreadAffinityMask(m_threadPool[i].thread, core_id)){
				bResult = true;
			}
			break;
		}
	}

	m_internalDataCS.leave();

	return bResult; 
}
//------------------------------------------------------------------------
bool ThreadManager::unassignThread(int id)
{
	assert(isPrimaryThread());

	// invalidate the thread
	// don't unassign already unassigned threads
	int status = SetThreadStatus(id,	~THREAD_STATUS_UNASSIGNED, THREAD_STATUS_UNASSIGN_REQUEST);
	return ((status == THREAD_STATUS_UNASSIGN_REQUEST) || (status == THREAD_STATUS_UNASSIGNED));
}

//------------------------------------------------------------------------
bool ThreadManager::startThread(int id)
{
    assert(isPrimaryThread());

    // make it go, if possible
    int status = SetThreadStatus(
        id,
        THREAD_STATUS_STOPPED,
        THREAD_STATUS_RUNNING_REQUEST
    );
    return (status == THREAD_STATUS_RUNNING_REQUEST) || (status == THREAD_STATUS_RUNNING);
}

//------------------------------------------------------------------------
bool ThreadManager::stopThread(int id)
{
    assert(isPrimaryThread());

    // make it stop, if possible
    int status = SetThreadStatus(
        id,
        THREAD_STATUS_RUNNING | THREAD_STATUS_RUNNING_REQUEST,
        THREAD_STATUS_STOPPED
    );
    return (status == THREAD_STATUS_STOPPED);
}
//------------------------------------------------------------------------
void ThreadManager::callWorkFunctionForThread(int id)
{
	assert(m_initialized);
	assert((id >= 0) && (id < m_threadPoolSize));

	// no need to lock thread here since workFunc is guaranteed to be valid
	// for the duration of the function
	WorkFunction func = m_threadPool[id].workFunc;
	if(func != NULL)
	{
		(*func)();

		// Windows has a lightweight way to do an atomic increment
		InterlockedIncrement(&m_threadPool[id].calls);
	}
}

//------------------------------------------------------------------------
void ThreadManager::callFinalizeFunctionForThread(int id)
{
	assert(m_initialized);
	assert((id >= 0) && (id < m_threadPoolSize));

	// no need to lock thread here since finalizeFunc is guaranteed to be valid
	// for the duration of the function
	FinalizeFunction func = m_threadPool[id].finalizeFunc;
	if((func != NULL) && (m_threadPool[id].calls > 0))
	{
		(*func)();
	}
}
//------------------------------------------------------------------------
void ThreadManager::callbackHandleStatus(int id, int status)
{
	int statusResult = THREAD_STATUS_UNASSIGNED;

	switch(status)
	{
	case THREAD_STATUS_RUNNING_REQUEST:
		statusResult = SetThreadStatus(id, THREAD_STATUS_RUNNING_REQUEST, THREAD_STATUS_RUNNING);
		if(statusResult != THREAD_STATUS_RUNNING)
		{
			// sleep it off and try again later
			sleep(THREAD_SLEEP_DURATION);
			break;
		}
		// it worked, so prepare for running
		associateLocalDataWithThread(id);
		// intentional fall-through
	case THREAD_STATUS_RUNNING:
		callWorkFunctionForThread(id);
		yield();
		break;

	case THREAD_STATUS_UNASSIGN_REQUEST:
		callFinalizeFunctionForThread(id);
		SetThreadStatus(id, THREAD_STATUS_UNASSIGN_REQUEST, THREAD_STATUS_UNASSIGNED);
		// whether successful or not, fall through to default case
	default:
		// sleep for a bit
		sleep(THREAD_SLEEP_DURATION);
		break;
	}
}
//------------------------------------------------------------------------
unsigned int __stdcall ThreadManager::threadCallback(void *param)
{
	assert(param != NULL);

	// param is just the thread id number
	int id = *(int *)param;

	// get the ThreadManager
	ThreadManager &tm = ThreadManager::getThreadManager();

	int status = tm.getThreadStatus(id);
	while (status != THREAD_STATUS_TERMINATE_REQUEST)
	{
		// take whatever actions are appropriate for this status
		tm.callbackHandleStatus(id, status);
		// update status
		status = tm.getThreadStatus(id);
	}

	_endthreadex(0);

	return 0; 
}
//------------------------------------------------------------------------
void ThreadManager::lockThread(int id)
{
    // potentially called before initialized is true
    // so we don't assert on it
    assert((id >= 0) && (id < m_threadPoolSize));
    assert(m_threadPool[id].criticalSection != NULL);
    
    m_threadPool[id].criticalSection->enter();
}
//------------------------------------------------------------------------
void ThreadManager::unlockThread(int id)
{
    // potentially called before initialized is true
    // so we don't assert on it
    assert((id >= 0) && (id < m_threadPoolSize));
    assert(m_threadPool[id].criticalSection != NULL);
    
    m_threadPool[id].criticalSection->leave();
}

//------------------------------------------------------------------------
int ThreadManager::getThreadStatus(int id) const
{
    // potentially, this function is called before initialized is true
    // so we don't assert on it
    assert((id >= 0) && (id < m_threadPoolSize));
    
    return m_threadPool[id].status;
}

//------------------------------------------------------------------------
int ThreadManager::SetThreadStatus(int id, int condition, int value)
{
    // potentially, this function is called before initialized is true
    // so we don't assert on it
    assert((id >= 0) && (id < m_threadPoolSize));
    
    // atomic operation to test thread status for a mask of states
    // if status matches mask, set status to value
    lockThread(id);
    
    int output = m_threadPool[id].status;
    if((output & condition) != 0)
    {
        m_threadPool[id].status = value;
        output = value;
    }
 
    unlockThread(id); 
    return output;
}

//------------------------------------------------------------------------
void ThreadManager::cleanup(bool waitForThreadTermination)
{
	// we don't enter the internalDataCS, because this function is only called from init and shutdown
	// where the critical section is already entered

	// signal termination to all the threads in the pool
	for(int i = 0; i < m_threadPoolSize; i++)
	{
		// signal termination to thread
		SetThreadStatus(i, THREAD_STATUS_ALL, THREAD_STATUS_TERMINATE_REQUEST);
	}

	// wait on all threads in turn, to terminate
	for(int i = 0; i < m_threadPoolSize; i++)
	{
		// wait until this thread is done, then destroy it

		// NOTE: we don't use WaitForMultipleObjects here, in order to make the cross-platform
		// code have more parity.  If we wanted to do so, we would move this code outside the loop,
		// allocate an array of thread objects, populate it from the thread pool, and destroy it
		// when finished.  We would still need to close each handle individually within the loop.

		if (waitForThreadTermination)
		{
			// wait forever
			WaitForSingleObject(m_threadPool[i].thread, INFINITE);
		}
		else
		{
			// wait at most one second
			WaitForSingleObject(m_threadPool[i].thread, 1000);
		}
		CloseHandle(m_threadPool[i].thread);

		// also cleanup the critical section
		delete m_threadPool[i].criticalSection;
		m_threadPool[i].criticalSection = NULL;
	}

	delete [] m_threadPool;
	m_threadPool = NULL;
	m_threadPoolSize = 0;

	// destroy the local data key
	if(m_localDataKey != sm_invalidLocalDataKey)
	{
		TlsFree(m_localDataKey);
	}
	m_localDataKey = sm_invalidLocalDataKey;
}

//------------------------------------------------------------------------
void ThreadManager::associateLocalDataWithThread(int id)
{
	assert(m_initialized);
	assert((id >= 0) && (id < m_threadPoolSize));

	TlsSetValue(m_localDataKey, m_threadPool[id].localData);
}

//------------------------------------------------------------------------
ThreadManager::threadInfo* ThreadManager::GetThreadInfo(int id)
{
	assert((id >= 0) && (id < m_threadPoolSize));
	return &m_threadPool[id];
}