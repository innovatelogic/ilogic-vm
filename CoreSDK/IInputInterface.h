#ifndef __iinputinterface_h__
#define __iinputinterface_h__

#ifdef _WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"

typedef std::vector<EventKeyInputProxy<class IInputInterface>*>	TVecInputKeyEvents;
typedef TVecInputKeyEvents::iterator		TVecInputKeyEventsIterator;
typedef TVecInputKeyEvents::const_iterator	TVecConstInputKeyEventsIterator;

class CORESDK_API IInputInterface
{
public:
	IInputInterface(const CObjectAbstract *pParent);
	IInputInterface(const IInputInterface &Source);
	virtual ~IInputInterface();

	void RegisterInputInterface(const CActor *Src, int SrcParent = 1);
	void UnregisterInputInterface();

	inline_ class CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }
	void					SetCoreSDK(class CCoreSDK *pCoreSDK) const { m_pCoreSDK = pCoreSDK; }

	virtual void			ProcessInputKey(const EventInput &InputData);

public:
	TVecInputKeyEvents  m_EventsInput;

private:

	// controller event
	EScrObjectEvent		m_ControlEvent;

	// control state
	mutable bool		m_bLocked;
	
	// state
	EActiveState		m_ActiveState;

	//
	unsigned int		m_InputStates;

	TNodeMap<class CActor, class IInputInterface> *m_pNode;
	mutable class CCoreSDK *m_pCoreSDK;
};

#endif//__iinputinterface_h__