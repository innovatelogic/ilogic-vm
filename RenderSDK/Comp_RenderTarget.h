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
	Comp_RenderTarget(const CObjectAbstract *parent);
	Comp_RenderTarget(const Comp_RenderTarget &source);
	virtual ~Comp_RenderTarget();

	virtual void	Initialize();
	virtual bool	CreateRenderTarget(unsigned int width, unsigned int height);

	const oes::d3d::D3DRenderTarget * GetRenderTarget() const;

protected:
private:
    oes::d3d::D3DRenderTarget *m_pRenderTarget;
};

#endif//__comp_rendertarget_h__