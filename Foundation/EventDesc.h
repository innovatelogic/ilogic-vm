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
		: m_receiverObject(nullptr)
		, EventId(Event_None)
	{
	}

	virtual ~EventAbstract()
	{
	}

	virtual void		CallEvent(const T * Sender) = 0;	

	const T				*m_receiverObject; // pointer to invoking object
	ESystemEventID		EventId;
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<class T>
class EXPORT EventDesc final : public EventAbstract<T>
{	
	typedef boost::signals2::signal<void(const T*, ESystemEventID)> TSignalEvent;

public:
	EventDesc()
	{
	}

	virtual ~EventDesc()
	{
		Event.disconnect(m_receiverObject);
		m_receiverObject = nullptr;
	
		EventId = Event_None;
	}

	void CallEvent(const T * Sender) override
	{
		Event(Sender, EventId);
	}

	TSignalEvent    Event;
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<class T>
class EXPORT EventKeyInputProxy final : public EventAbstract<T>
{	
	typedef boost::signals2::signal<bool(const T*, const EventInput &InputData)> TSignalEvent;

public:
	EventKeyInputProxy()
	{}

	virtual ~EventKeyInputProxy()
	{}

	void Execute(const T * Sender, const EventInput &InputData)
	{
		Event(Sender, InputData);
	}

	void CallEvent(const T * Sender) override 
	{}

	TSignalEvent Event;
};

#endif//__eventdesc_h__