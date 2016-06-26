#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
CInputManager::CInputManager(class CCoreSDK* pSDK)
: m_pCoreSDK(pSDK)
{

}

//----------------------------------------------------------------------------------------------
CInputManager::~CInputManager()
{

}

//----------------------------------------------------------------------------------------------
TNodeMap<class CActor, class IInputInterface>* 
CInputManager::RegisterObject(const IInputInterface *pSrc, const CActor *pKey, const CActor *pKeyParent)
{
	return m_VecInputList.PushBack(pKey, pSrc, pKeyParent);
}

//----------------------------------------------------------------------------------------------
void CInputManager::UnregisterObject(class TNodeMap<CActor, IInputInterface> *pNode)
{
	m_VecInputList.Erase(pNode);
}

//----------------------------------------------------------------------------------------------
void CInputManager::ProcessInputKey(const EventInput &InputData)
{
	// Editor key hooks
	switch (InputData.Key)
	{
	case KEY_ESCAPE:
		{
			bool bPaused = m_pCoreSDK->GetPause();
			if (!bPaused)
			{
				m_pCoreSDK->RestoreState();

				m_pCoreSDK->SetPause(true);
				//m_pCoreSDK->GetPhysicsEngine()->SetEnableSimulation(false);

				if (m_pCoreSDK->IsAEditor()) {	// if editor mode active, while paused drop IsAGame state flag.
					m_pCoreSDK->SetIsAGame(false);
				}
			}
		}break;

	case KEY_G:
		{
			bool bPaused = m_pCoreSDK->GetPause();
			if (/*InputData.CtrlPressed &&*/ bPaused)
			{
                core_sdk_api::CViewportManager *pMgr = m_pCoreSDK->GetViewportManager();
				//pMgr->SetFocus(pMgr->GetFocused(), false);

				m_pCoreSDK->SaveState();

				m_pCoreSDK->SetPause(false);
				//m_pCoreSDK->GetPhysicsEngine()->SetEnableSimulation(true);

				if (m_pCoreSDK->IsAEditor()) {	// if editor mode active, while paused drop IsAGame state flag.
					m_pCoreSDK->SetIsAGame(true);
				}
			}
		}break;

	case KEY_DELETE:
		{
			//UIEditor_ProcessDelete();
		}break;
	};

	/*if (CActor *pFocused = m_pCoreSDK->GetViewportManager()->GetFocusedActor())
	{
		TNodeMap<CActor, IInputInterface> *pNode = m_VecInputList.m_pFirstElement;

		if (pNode)
		{
			do
			{
				if (pNode->m_pKey == pFocused)
				{
					const_cast<IInputInterface*>(pNode->m_pValue)->ProcessInputKey(InputData);
					break;
				}

				pNode = m_VecInputList.GetNext(pNode);

			} while(pNode);
		}
	}*/
}

//----------------------------------------------------------------------------------------------
void CInputManager::ProcessMouseWheel(float ds)
{

}