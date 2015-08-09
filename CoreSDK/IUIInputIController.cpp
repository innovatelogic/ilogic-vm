#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
IUIInputController::IUIInputController(const CObjectAbstract *pParent)
: m_pNode(0)
{
	if (pParent){
		m_pCoreSDK_ = (CCoreSDK*)pParent->GetUserData();
	}
}

//----------------------------------------------------------------------------------------------
IUIInputController::IUIInputController(const IUIInputController &Source)
: m_pNode(0)
{
	if (this != &Source){
		m_pCoreSDK_ = Source.m_pCoreSDK_;
	}
}

//----------------------------------------------------------------------------------------------
IUIInputController::~IUIInputController()
{
	UnregisterUIInputInterface();
}

//----------------------------------------------------------------------------------------------
void IUIInputController::RegisterUIInputInterface(const UIScreenObject *pSrc)
{
	if (m_pCoreSDK_){
		m_pNode = m_pCoreSDK_->GetRemoteControlMgr()->RegisterUIObject(this, pSrc);
	}
}

//----------------------------------------------------------------------------------------------
void IUIInputController::UnregisterUIInputInterface()
{
	if (m_pCoreSDK_ && m_pNode){
		m_pCoreSDK_->GetRemoteControlMgr()->UnregisterUIObject(m_pNode);
	}
}