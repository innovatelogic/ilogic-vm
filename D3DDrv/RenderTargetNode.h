#pragma once

#include "Refcount.h"

const SRenderContext;

namespace RenderDriver
{
	class D3DDRIVER_API RenderTargetNode : public Refcount
	{
		struct STarget
		{
			GLuint texture;
			size_t width;
			size_t height;
		};

	public:
		RenderTargetNode(size_t width, size_t height, SRenderContext *const pCtxt);
		virtual ~RenderTargetNode();

		SRenderContext* GetOwnerContext() { return m_pContextOwner; }

	protected:
		virtual void DoRelease();

	public:
		STarget m_target;
		SRenderContext *const m_pContextOwner;
	};
}