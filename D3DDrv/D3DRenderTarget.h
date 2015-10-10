#ifndef __d3drendertarget_h__
#define __d3drendertarget_h__

#ifdef WIN32
#pragma once
#endif

#include "D3DRenderInterface.h"

namespace RenderDriver{
	class RenderTargetNode;
}

class D3DDRIVER_API D3DRenderTarget : public D3DRenderInterface
{
	DECL_CLASS_SIMPLE(D3DRenderTarget, D3DRenderInterface);

public:
	D3DRenderTarget(const class D3DDriver *pInterface);
	D3DRenderTarget(const D3DRenderTarget &Source);
	virtual ~D3DRenderTarget();

	virtual bool CreateRenderTarget(size_t width, size_t height);
	virtual void Release();
/*
	LPD3DXRENDERTOSURFACE	GetRenderToSurface() const { return RenderTarget->m_pRenderToSurface; } 
	LPDIRECT3DSURFACE9		GetSurface() const { return RenderTarget->pSurfaceBuffer; }
	LPDIRECT3DTEXTURE9		GetTexture() const { return RenderTarget->pTextureMap; }
	*/
protected:
private:
	RenderDriver::RenderTargetNode *m_pRenderTarget;
};


#endif//__d3drendertarget_h__