#ifndef __eventdesc_h__
#define __eventdesc_h__

#ifdef _WIN32
#pragma once
#endif

#include "StdAfxFoundation.h"

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T>
class EXPORT EventAbstract
{	
public:
	EventAbstract()
		: ReceiverObject(nullptr)
		, EventId(Event_None)
	{
	}

	virtual ~EventAbstract()
	{
	}

	virtual void		CallEvent(const T * Sender) = 0;	

	const T				*ReceiverObject; // pointer to invoking object
	ESystemEventID		EventId;
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<class T>
class EXPORT EventDesc : public EventAbstract<T>
{	
	typedef boost::signal<void(const T*, ESystemEventID)> TSignalEvent;

public:
	EventDesc()
	{
	}

	virtual ~EventDesc()
	{
		Event.disconnect(ReceiverObject);
		ReceiverObject = nullptr;
	
		EventId = Event_None;
	}

	virtual void CallEvent(const T * Sender)
	{
		Event(Sender, EventId);
	}

	TSignalEvent    Event;
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<class T>
class EXPORT EventKeyInputProxy : public EventAbstract<T>
{	
	typedef boost::signal<bool(const T*, const EventInput &InputData)> TSignalEvent;

public:
	EventKeyInputProxy()
	{}

	virtual ~EventKeyInputProxy()
	{}

	virtual void Execute(const T * Sender, const EventInput &InputData)
	{
		Event(Sender, InputData);
	}
	virtual void CallEvent(const T * Sender) {}

	TSignalEvent Event;
};

#endif//__eventdesc_h__