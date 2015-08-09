#ifndef __d3drendertarget_h__
#define __d3drendertarget_h__

#ifdef WIN32
#pragma once
#endif

#include "D3DRenderInterface.h"

class D3DDRIVER_API D3DRenderTarget : public D3DRenderInterface
{
	DECL_CLASS_SIMPLE(D3DRenderTarget, D3DRenderInterface);

public:
	D3DRenderTarget(const class D3DDriver *pInterface);
	D3DRenderTarget(const D3DRenderTarget & Source);
	virtual ~D3DRenderTarget();

	virtual bool CreateRenderTarget(unsigned int Width, unsigned int Height);
	virtual void Release();
/*
	LPD3DXRENDERTOSURFACE	GetRenderToSurface() const { return RenderTarget->m_pRenderToSurface; } 
	LPDIRECT3DSURFACE9		GetSurface() const { return RenderTarget->pSurfaceBuffer; }
	LPDIRECT3DTEXTURE9		GetTexture() const { return RenderTarget->pTextureMap; }
	*/
protected:
private:
	class RenderTargetNode * RenderTarget;
};


#endif//__d3drendertarget_h__