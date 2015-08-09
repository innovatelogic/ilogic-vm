#ifndef __iuiinputcontroller_h__
#define __iuiinputcontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API IUIInputController
{
public:
	IUIInputController(const CObjectAbstract *pParent);
	IUIInputController(const IUIInputController &Source);
	virtual ~IUIInputController();

	void RegisterUIInputInterface(const class UIScreenObject *pSrc);
	void UnregisterUIInputInterface();

	virtual bool ProcessButtonUI(const EventInput &InputData) = 0;

	virtual bool IsHandleKeyInput() const { return true; }

protected:
private:
	TNodeMap<class UIScreenObject, IUIInputController> *m_pNode;
	mutable class CCoreSDK *m_pCoreSDK_;
};

#endif//__iuiinputcontroller_h__