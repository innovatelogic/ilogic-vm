#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
CRemoteControlManager::CRemoteControlManager(CCoreSDK* pSDK)
: m_pCoreSDK(pSDK)
{

}

//----------------------------------------------------------------------------------------------
CRemoteControlManager::~CRemoteControlManager()
{

}

//----------------------------------------------------------------------------------------------
TNodeMap<class CActor, SPlayerEntityInfo>* 
CRemoteControlManager::RegisterObject(IEntityController *pSrc, const CActor *pKey, const CActor *pKeyParent,
										bool bPlayer, unsigned int PlayerID /*= 0*/)
{
	TNodeMap<class CActor, SPlayerEntityInfo> *pOut = 0;

	SPlayerEntityInfo *pInfo = new SPlayerEntityInfo(pSrc, bPlayer);
	
	if (pInfo)
	{
		pOut = m_VecControllers.PushBack(pKey, pInfo, pKeyParent);
		if (!pOut)
		{
			delete pInfo;
			return 0;
		}
	}
	return pOut;
}

//----------------------------------------------------------------------------------------------
void CRemoteControlManager::UnregisterObject(TNodeMap<CActor, SPlayerEntityInfo> *pNode)
{
	delete pNode->m_pValue;
	
	m_VecControllers.Erase(pNode);
}

//----------------------------------------------------------------------------------------------
TNodeMap<UIScreenObject, IUIInputController>* 
	CRemoteControlManager::RegisterUIObject(IUIInputController *pSrc, const UIScreenObject *pKey)
{
	return m_VecUIControllers.PushBack(pKey, pSrc, 0);
}

//----------------------------------------------------------------------------------------------
void CRemoteControlManager::UnregisterUIObject(TNodeMap<UIScreenObject, IUIInputController> *pNode)
{
	m_VecUIControllers.Erase(pNode);
}

//----------------------------------------------------------------------------------------------
bool CRemoteControlManager::ProcessInputMouse(const MouseInputData &InputData, unsigned int PlayerID /*= 0*/)
{
	bool bResult = false;

	if (GetCoreSDK()->bPaused){
		return bResult;
	}

	TNodeMap<CActor, SPlayerEntityInfo> *pNode = m_VecControllers.m_pFirstElement;

	if (pNode)
	{
		do
		{
			if (pNode->m_pValue->pEntity && pNode->m_pValue->bIsPlayer)
			{
				const_cast<IEntityController*>(pNode->m_pValue->pEntity)->ProcessInputMouseEnt(InputData);
				bResult = true;
				break;
			}

			pNode = m_VecControllers.GetNext(pNode);
		}while (pNode);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CRemoteControlManager::ProcessInputMouse(const MouseMoveInputData &InputData, unsigned int PlayerID /*= 0*/)
{
	bool bResult = false;

	if (GetCoreSDK()->bPaused){
		return bResult;
	}

	TNodeMap<CActor, SPlayerEntityInfo> *pNode = m_VecControllers.m_pFirstElement;

	if (pNode)
	{
		do
		{
			if (pNode->m_pValue->pEntity && pNode->m_pValue->bIsPlayer)
			{
				const_cast<IEntityController*>(pNode->m_pValue->pEntity)->ProcessInputMouseEnt(InputData);
				bResult = true;
				break;
			}

			pNode = m_VecControllers.GetNext(pNode);
		}while (pNode);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CRemoteControlManager::ProcessInputKey(const EventInput &InputData, unsigned int PlayerID /*= 0*/)
{
	//process ui
	// TEMP fix/ add reverse iteration
	std::vector<TNodeMap<UIScreenObject, IUIInputController>*> VReverse;

	if (TNodeMap<UIScreenObject, IUIInputController> *pNodeUI = m_VecUIControllers.m_pLastElement)
	{
		while (pNodeUI)
		{
			IUIInputController *pInt = const_cast<IUIInputController*>(pNodeUI->m_pValue);
			if (pInt->IsHandleKeyInput() &&	pInt->ProcessButtonUI(InputData)){
				return true;
			}
			pNodeUI = m_VecUIControllers.GetPrev(pNodeUI);
		} 
	}

	// common objects
	if (TNodeMap<CActor, SPlayerEntityInfo> *pNode = m_VecControllers.m_pFirstElement)
	{
		do
		{
			if (pNode->m_pValue->pEntity && pNode->m_pValue->bIsPlayer)
			{
				if (const_cast<IEntityController*>(pNode->m_pValue->pEntity)->ProcessButton(InputData)){
					return true;
				}
			}
			pNode = m_VecControllers.GetNext(pNode);
		} while (pNode);
	}
	return false;
}