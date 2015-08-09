#ifndef __updatemanager_h__
#define __updatemanager_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/Stdafxfoundation.h"

class EXPORT UpdateManager
{
	
public:
	typedef TTreeMap<class CObjectAbstract, class IUpdateInterface>	TTreeMapArr;
	typedef TNodeMap<CObjectAbstract, IUpdateInterface> TTNodeMap;

	UpdateManager(class CCoreSDK* pSDK);
	virtual ~UpdateManager();

	virtual bool RegisterActor(class CActor * Object);
	virtual bool UnRegisterActor(class CActor * Object);

	TTNodeMap* RegisterObject(const class IUpdateInterface* Src, 
								const class CObjectAbstract* Key,
								const class CObjectAbstract* KeyParent);

	void	UnregisterObject(TTNodeMap* Src);

	void	Update(float DeltaTime);

	void	PostPhysUpdate(float DeltaTime);

	void	PostPhysEvent(class CActor *pObject, EPhysEventCallback EVENT_ID, void *pParamA, void *pParamB);

private:
	TVecActorChild VUpdateList;

	TTreeMapArr m_List;

	class CCoreSDK* m_pCoreSDK;
};

#endif//__updatemanager_h__