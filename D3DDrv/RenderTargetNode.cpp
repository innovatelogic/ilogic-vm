#include "d3ddriverstdafx.h"

//----------------------------------------------------------------------------------------------
RenderTargetNode::RenderTargetNode(const D3DDriver *pDriver)
: /*m_pRenderToSurface(NULL)
, pSurfaceBuffer(NULL)
, pTextureMap(NULL)
,*/ p3DDriver(const_cast<D3DDriver*>(pDriver))
{

}

//----------------------------------------------------------------------------------------------
RenderTargetNode::~RenderTargetNode()
{

}

//----------------------------------------------------------------------------------------------
void RenderTargetNode::DoRelease()
{
/*	if (m_pRenderToSurface)
	{
		SAFE_RELEASE(m_pRenderToSurface);
		m_pRenderToSurface = NULL;
	}

	if (pSurfaceBuffer)
	{
		SAFE_RELEASE(pSurfaceBuffer);
		pSurfaceBuffer = NULL;
	}

	if (pTextureMap)
	{
		SAFE_RELEASE(pTextureMap);
		pTextureMap = NULL;
	}
	*/
	Refcount::DoRelease();
}