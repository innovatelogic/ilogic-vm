#ifndef __iupdateinterface_h__
#define __iupdateinterface_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"
#include "ObjectAbstract.h"

class CORESDK_API IUpdateInterface
{
public:
	IUpdateInterface(const CObjectAbstract *pParent);
	IUpdateInterface(const IUpdateInterface &Source);
	virtual ~IUpdateInterface();

	virtual void RegisterUpdateInterface(const CObjectAbstract *pSrc, const CObjectAbstract *pParent);
	virtual void UnregisterUpdateInterface();

	inline_ class CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }
	void					SetCoreSDK(class CCoreSDK* pCoreSDK) const { m_pCoreSDK = pCoreSDK; }

	virtual void Update(float DeltaTime);

	virtual void PostPhysUpdate(float DeltaTime);

	virtual void PostPhysEvent(EPhysEventCallback EVENT_ID, void *pParamA, void *pParamB);

protected:
	TNodeMap<class CObjectAbstract, class IUpdateInterface> *m_pNode;
	mutable class CCoreSDK *m_pCoreSDK;
};

#endif //__iupdateinterface_h__