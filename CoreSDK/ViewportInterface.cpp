#include "coresdkafx.h"

//------------------------------------------------------------------------
ViewportInterface::ViewportInterface(const CObjectAbstract *pParent)
: m_pNode(0)
, m_fNearPlane(0.f)
, m_fFarPlane(0.f)
{
	if (pParent) 
	{
		m_pCoreSDK = (CCoreSDK*)pParent->GetUserData();
	}
}

//------------------------------------------------------------------------
ViewportInterface::~ViewportInterface()
{
	UnregisterViewportInterface();
}

//----------------------------------------------------------------------------------------------
void ViewportInterface::RegisterViewportInterface(const CActor *pSrc)
{
	if (m_pCoreSDK){
		m_pNode = m_pCoreSDK->GetViewportManager()->RegisterViewport(this, pSrc, 0);
	}
}

//----------------------------------------------------------------------------------------------
void ViewportInterface::UnregisterViewportInterface()
{
	if (m_pCoreSDK && m_pNode){
		m_pCoreSDK->GetViewportManager()->UnregisterViewport(m_pNode);
	}
}