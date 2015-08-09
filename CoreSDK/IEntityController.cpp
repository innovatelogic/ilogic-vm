#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
IEntityController::IEntityController(const CObjectAbstract *pParent)
: m_pNode(0)
{
	if (pParent){
		m_pCoreSDK = (CCoreSDK*)pParent->GetUserData();
	}
}

//----------------------------------------------------------------------------------------------
IEntityController::IEntityController(const IEntityController &Source)
: m_pNode(0)
{
	if (this != &Source){
		m_pCoreSDK = Source.m_pCoreSDK;
	}
}

//----------------------------------------------------------------------------------------------
IEntityController::~IEntityController()
{
	UnregisterEntityInterface();
}

//----------------------------------------------------------------------------------------------
void IEntityController::RegisterEntityInterface(const CActor *Src, bool bPlayer, int SrcParent /*= 1*/)
{
	if (m_pCoreSDK)
	{
		m_pNode = m_pCoreSDK->GetRemoteControlMgr()->RegisterObject(this, Src, (SrcParent == 1) ? Src->GetParent() : 0, bPlayer);
	}
}

//----------------------------------------------------------------------------------------------
void IEntityController::UnregisterEntityInterface()
{
	if (m_pCoreSDK && m_pNode)
	{
		m_pCoreSDK->GetRemoteControlMgr()->UnregisterObject(m_pNode);
	}
}