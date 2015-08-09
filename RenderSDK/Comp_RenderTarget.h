#ifndef __comp_rendertarget_h__
#define __comp_rendertarget_h__

#ifdef WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API Comp_RenderTarget : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(Comp_RenderTarget, CRenderObject);
public:
	Comp_RenderTarget(const CObjectAbstract * Parent);
	Comp_RenderTarget(const Comp_RenderTarget & Source);
	virtual ~Comp_RenderTarget();

	virtual void	Initialize();
	virtual bool	CreateRenderTarget(unsigned int Width, unsigned int Height);

	const D3DRenderTarget * GetRenderTarget() const;

protected:
private:
	class D3DRenderTarget * RenderTarget;
};

#endif//__comp_rendertarget_h__