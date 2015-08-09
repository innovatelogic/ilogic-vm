#ifndef __remotecontrolmanager_h__
#define __remotecontrolmanager_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"

struct SPlayerEntityInfo
{
	class IEntityController *pEntity;
	bool	bIsPlayer;
	unsigned int PlayerID;

	SPlayerEntityInfo(class IEntityController *pEnt, bool bPlayer)
		: pEntity(pEnt)
		, bIsPlayer(bPlayer)
		, PlayerID(0)
	{}
};

class CORESDK_API CRemoteControlManager
{
	typedef TTreeMap<class CActor, SPlayerEntityInfo>					TTreeMapController;
	typedef TTreeMap<class UIScreenObject, class IUIInputController>	TTreeMapUIController;

public:
	CRemoteControlManager(class CCoreSDK* pSDK);
	virtual ~CRemoteControlManager();

	inline_ class CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }

	TNodeMap<class CActor, SPlayerEntityInfo>* RegisterObject(IEntityController *pSrc,
																const CActor *pKey, 
																const CActor *pKeyParent,
																bool bPlayer,
																unsigned int PlayerID = 0);
	void UnregisterObject(TNodeMap<CActor, SPlayerEntityInfo> *pNode);

	TNodeMap<class UIScreenObject, IUIInputController>*  RegisterUIObject(IUIInputController *pSrc, const UIScreenObject *pKey);
	void UnregisterUIObject(TNodeMap<UIScreenObject, IUIInputController> *pNode);

	// user controller input
	bool ProcessInputMouse(const MouseInputData &InputData, unsigned int PlayerID = 0);
	bool ProcessInputMouse(const MouseMoveInputData &InputData, unsigned int PlayerID = 0);
	bool ProcessInputKey(const EventInput& InputData, unsigned int PlayerID = 0);

protected:
private:
	TTreeMapController		m_VecControllers;
	TTreeMapUIController	m_VecUIControllers;

	class CCoreSDK			*m_pCoreSDK;
};

#endif//__remotecontrolmanager_h__