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

	size_t GetWidth() const { return m_pRenderTarget->GetWidth(); }
	size_t GetHeight() const { return m_pRenderTarget->GetHeight(); }

	GLuint GetTexture() const { return m_pRenderTarget->GetTexture(); }
	GLuint GetFrameBuffer() const { return m_pRenderTarget->GetFrameBuffer(); }
	GLuint GetDepthBuffer() const { return m_pRenderTarget->GetDepthBuffer(); }

protected:
private:
	RenderDriver::RenderTargetNode *m_pRenderTarget;
};


#endif//__d3drendertarget_h__