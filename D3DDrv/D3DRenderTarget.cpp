#include "d3ddriverstdafx.h"

//----------------------------------------------------------------------------------------------
D3DRenderTarget::D3DRenderTarget(const class D3DDriver * Interface)
: Super(Interface)
, RenderTarget(NULL)
{

}

//----------------------------------------------------------------------------------------------
D3DRenderTarget::D3DRenderTarget(const D3DRenderTarget & Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------------------
D3DRenderTarget::~D3DRenderTarget()
{
	Release();
}

//----------------------------------------------------------------------------------------------
bool D3DRenderTarget::CreateRenderTarget(unsigned int Width, unsigned int Height)
{
	bool bResult = false;
	
	RenderTarget = m_pD3DInterface->CreateRenderTarget(Width, Height);

	bResult = true;

	return bResult;
}

//----------------------------------------------------------------------------------------------
void D3DRenderTarget::Release()
{
	if (RenderTarget)
	{
		RenderTarget->Release();
		RenderTarget = NULL;
	}
}