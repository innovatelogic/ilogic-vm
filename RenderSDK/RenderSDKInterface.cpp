#include "RenderSDKInterface.h"
#include "RenderSDK.h"

//------------------------------------------------------------------------
RenderSDKInterface::RenderSDKInterface(class CRenderSDK * pRenderSDK)
: m_pRenderSDK(pRenderSDK)
{

}

//------------------------------------------------------------------------
RenderSDKInterface::~RenderSDKInterface()
{
}

//------------------------------------------------------------------------
void RenderSDKInterface::SetRenderComponent(CRenderSDK* RenderSDK) const
{
	m_pRenderSDK = RenderSDK;
}