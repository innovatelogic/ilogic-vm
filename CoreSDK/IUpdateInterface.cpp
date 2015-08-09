#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
IUpdateInterface::IUpdateInterface(const CObjectAbstract *Parent)
: m_pNode(0)
{
	if (Parent) {
		m_pCoreSDK = (CCoreSDK*)Parent->GetUserData();
	}
}

//----------------------------------------------------------------------------------------------
IUpdateInterface::IUpdateInterface(const IUpdateInterface &Source)
{
	if (this != &Source)
	{
		m_pCoreSDK = Source.m_pCoreSDK;
	}
}

//----------------------------------------------------------------------------------------------
IUpdateInterface::~IUpdateInterface()
{
	UnregisterUpdateInterface();
}

//----------------------------------------------------------------------------------------------
void IUpdateInterface::RegisterUpdateInterface(const CObjectAbstract *Src, const CObjectAbstract *Parent)
{
	if (m_pCoreSDK){
		m_pNode = m_pCoreSDK->GetUpdateManager()->RegisterObject(this, Src, Parent);
	}
}

//----------------------------------------------------------------------------------------------
void IUpdateInterface::UnregisterUpdateInterface()
{
	if (m_pCoreSDK && m_pNode){
		m_pCoreSDK->GetUpdateManager()->UnregisterObject(m_pNode);
	}
}

//----------------------------------------------------------------------------------------------
void IUpdateInterface::Update(float dt)
{

}

//----------------------------------------------------------------------------------------------
void IUpdateInterface::PostPhysUpdate(float DeltaTime)
{

}

//----------------------------------------------------------------------------------------------
void IUpdateInterface::PostPhysEvent(EPhysEventCallback EVENT_ID, void *pParamA, void *pParamB)
{

}