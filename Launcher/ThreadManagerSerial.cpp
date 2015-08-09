#include "LauncherStdAfx.h"

//------------------------------------------------------------------------
ThreadManagerSerial& ThreadManagerSerial::getThreadManagerSerial(void)
{
    if (sm_threadManager == NULL)
    {
        sm_singletonCS.enter();
        if (sm_threadManager == NULL)
        {
            sm_threadManager = new ThreadManagerSerial();
        }
        sm_singletonCS.leave();
    }
    
    return (ThreadManagerSerial &)*sm_threadManager;
}

//------------------------------------------------------------------------
void ThreadManagerSerial::doSerialWork()
{
    assert(isPrimaryThread());

    // iterate over threadPool and find threads running in serial
    // call their work and finalize functions
    int status;
    for(int i = 0; i < m_threadPoolSize; i++)
    {
        status = getThreadStatus(i);
        switch(status)
        {
            case THREAD_STATUS_SERIAL_RUNNING:
                callWorkFunctionForThread(i);
                break;
        
            case THREAD_STATUS_SERIAL_TO_PARALLEL_STOPPED_REQUEST:
                callFinalizeFunctionForThread(i);
                SetThreadStatus(i, THREAD_STATUS_SERIAL_TO_PARALLEL_STOPPED_REQUEST, THREAD_STATUS_STOPPED);
                break;
        
            case THREAD_STATUS_SERIAL_TO_PARALLEL_RUNNING_REQUEST:
                callFinalizeFunctionForThread(i);
                SetThreadStatus(i, THREAD_STATUS_SERIAL_TO_PARALLEL_RUNNING_REQUEST, THREAD_STATUS_RUNNING_REQUEST);
                break;
        
            default:
                // not in a serial state
                break;
        }
    }
    
    // increment the serial call count
    // Windows has a lightweight way to do an atomic increment
    InterlockedIncrement(&m_serialCalls);
    
    // yield to OS to prevent throttling
    yield();
}

//------------------------------------------------------------------------
bool ThreadManagerSerial::unassignThread(int id)
{
    assert(isPrimaryThread());
   
    bool output = false;
    
    // invalidate the thread
    // check for all states that are no longer handled by thread callback
    // these are states that are fundamentally serialized
    int status = SetThreadStatus(
        id,
        (
            THREAD_STATUS_SERIAL_STOPPED |
            THREAD_STATUS_SERIAL_RUNNING |
            THREAD_STATUS_SERIAL_TO_PARALLEL_STOPPED_REQUEST |
            THREAD_STATUS_SERIAL_TO_PARALLEL_RUNNING_REQUEST
        ),
        THREAD_STATUS_UNASSIGNED
    );
    output = (status == THREAD_STATUS_UNASSIGNED);
    
    if(output)
    {
        // if we unassigned the thread in a serial way, call the finalize function
        callFinalizeFunctionForThread(id);
    }
    else
    {
        // must not be serial, try to unassign it in a parallel way
        output = ThreadManager::unassignThread(id);
    }
    
    return output; 
}

//------------------------------------------------------------------------
bool ThreadManagerSerial::startThread(int id)
{
    assert(isPrimaryThread());

    // make it go, if possible
    bool output = ThreadManager::startThread(id);
    if(!output)
    {
        // deal with extra states introduced to parallel execution
        // NOTE: this will invalidate a pending serialization request
        int status = SetThreadStatus(
            id,
            THREAD_STATUS_PARALLEL_TO_SERIAL_STOPPED_REQUEST,
            THREAD_STATUS_RUNNING_REQUEST
        );
        output = output || (status == THREAD_STATUS_RUNNING_REQUEST);
    
        // also deal with serial states
        // NOTE: this will invalidate a pending parallelization request
        status = SetThreadStatus(
            id,
            (
                THREAD_STATUS_SERIAL_STOPPED |
                THREAD_STATUS_SERIAL_TO_PARALLEL_STOPPED_REQUEST
            ),
            THREAD_STATUS_SERIAL_RUNNING
        );
        output = output || (status == THREAD_STATUS_SERIAL_RUNNING);
    }
    return output;
}

//------------------------------------------------------------------------
bool ThreadManagerSerial::stopThread(int id)
{
    assert(isPrimaryThread());

    // make it stop, if possible
    bool output = ThreadManager::stopThread(id);
    if(!output)
    {
        // deal with extra states introduced to parallel execution
        // NOTE: this will invalidate a pending serialization request
        int status = SetThreadStatus(
            id,
            THREAD_STATUS_PARALLEL_TO_SERIAL_RUNNING_REQUEST,
            THREAD_STATUS_STOPPED
        );
        output = output || (status == THREAD_STATUS_STOPPED);
    
        // also deal with serial states
        // NOTE: this will invalidate a pending parallelization request
        status = SetThreadStatus(
            id,
            (
                THREAD_STATUS_SERIAL_RUNNING |
                THREAD_STATUS_SERIAL_TO_PARALLEL_RUNNING_REQUEST
            ),
            THREAD_STATUS_SERIAL_STOPPED
        );
        output = output || (status == THREAD_STATUS_SERIAL_STOPPED);
    }
    return output;
}

//------------------------------------------------------------------------
bool ThreadManagerSerial::setThreadSerialized(int id, bool serial)
{
    assert(isPrimaryThread());

    bool output = false;
    int status;

    if (serial)
    {
        // make serial
        // don't know if it's running or not, so try both
        // but try stopped state first
        status = SetThreadStatus(
            id,
            THREAD_STATUS_STOPPED,
            THREAD_STATUS_PARALLEL_TO_SERIAL_STOPPED_REQUEST
        );
        output = output || (status == THREAD_STATUS_PARALLEL_TO_SERIAL_STOPPED_REQUEST);
        
        status = SetThreadStatus(
            id,
            THREAD_STATUS_RUNNING | THREAD_STATUS_RUNNING_REQUEST,
            THREAD_STATUS_PARALLEL_TO_SERIAL_RUNNING_REQUEST
        );
        output = output || (status == THREAD_STATUS_PARALLEL_TO_SERIAL_RUNNING_REQUEST);
    }
    else
    {
        // make parallel
        // don't know if it's running or not, so try both
        // but try stopped state first
        status = SetThreadStatus(
            id,
            THREAD_STATUS_SERIAL_STOPPED,
            THREAD_STATUS_SERIAL_TO_PARALLEL_STOPPED_REQUEST
        );
        output = output || (status == THREAD_STATUS_SERIAL_TO_PARALLEL_STOPPED_REQUEST);
        
        status = SetThreadStatus(
            id,
            THREAD_STATUS_SERIAL_RUNNING,
            THREAD_STATUS_SERIAL_TO_PARALLEL_RUNNING_REQUEST
        );
        output = output || (status == THREAD_STATUS_SERIAL_TO_PARALLEL_RUNNING_REQUEST);
    }
    
    return output;
}

//------------------------------------------------------------------------
int ThreadManagerSerial::getAndResetThreadCalls(int id)
{
    int output = 0;
    
    if (id == INDEX_NONE)
    {
        m_internalDataCS.enter();
        output = m_serialCalls;
        m_serialCalls = 0;
        m_internalDataCS.leave();
    }
    else
    {
        output = ThreadManager::getAndResetThreadCalls(id);
    }
    
    return output;
}

//------------------------------------------------------------------------
bool ThreadManagerSerial::isThreadSerialized(int id)
{
    bool output = false;
    
    int status = getThreadStatus(id);
    if(
        (status == THREAD_STATUS_SERIAL_STOPPED) ||
        (status == THREAD_STATUS_SERIAL_RUNNING) ||
        (status == THREAD_STATUS_SERIAL_TO_PARALLEL_RUNNING_REQUEST) ||
        (status == THREAD_STATUS_SERIAL_TO_PARALLEL_STOPPED_REQUEST)
    )
    {
        output = true;
    }
    
    return output;
}

//------------------------------------------------------------------------
ThreadManagerSerial::ThreadManagerSerial()
{
    m_serialCalls = 0;
}
//------------------------------------------------------------------------
ThreadManagerSerial::~ThreadManagerSerial()
{
}
//------------------------------------------------------------------------
void ThreadManagerSerial::callbackHandleStatus(int id, int status)
{
    switch(status)
    {
        case THREAD_STATUS_PARALLEL_TO_SERIAL_RUNNING_REQUEST:
            // change to run serially
            // call the finalize function
            callFinalizeFunctionForThread(id);
            SetThreadStatus(id, THREAD_STATUS_PARALLEL_TO_SERIAL_RUNNING_REQUEST, THREAD_STATUS_SERIAL_RUNNING);
            break;
            
        case THREAD_STATUS_PARALLEL_TO_SERIAL_STOPPED_REQUEST:
            // change to run serially, but stopped
            // call the finalize function
            callFinalizeFunctionForThread(id);
            SetThreadStatus(id, THREAD_STATUS_PARALLEL_TO_SERIAL_STOPPED_REQUEST, THREAD_STATUS_SERIAL_STOPPED);
            break;
            
        default:
            // fall back on ThreadManager's implementation
            ThreadManager::callbackHandleStatus(id, status);
            break;
    }
}
