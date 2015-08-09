#ifndef __rendertargetnode_h__
#define __rendertargetnode_h__

#ifdef WIN32
#pragma once
#pragma warning (disable:4251)
#endif

#include "Refcount.h"

class D3DDRIVER_API RenderTargetNode : public Refcount
{
public:
	RenderTargetNode(const class D3DDriver *pDriver);
	virtual ~RenderTargetNode();

protected:
	virtual void DoRelease();

public:
//	LPD3DXRENDERTOSURFACE   m_pRenderToSurface;
//	LPDIRECT3DSURFACE9		pSurfaceBuffer; // Depth-stencil buffer
//	LPDIRECT3DTEXTURE9		pTextureMap;
	class D3DDriver			*p3DDriver;
};

#endif//__rendertarget_h__