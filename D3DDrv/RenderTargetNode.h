#pragma once

#include "Refcount.h"

namespace oes
{
    namespace d3d
    {
        class SRenderContext;

        namespace RenderDriver
        {
            class D3DDRIVER_API RenderTargetNode : public Refcount
            {
                struct STarget
                {
                    GLuint dynamicTextureID;
                    GLuint frameBuffer;
                    GLuint depthRenderBuffer;

                    size_t width;
                    size_t height;
                };

            public:
                RenderTargetNode(size_t width, size_t height, SRenderContext *const pCtxt);
                virtual ~RenderTargetNode();

                SRenderContext* GetOwnerContext() { return m_pContextOwner; }

                size_t GetWidth() const { return m_target.width; }
                size_t GetHeight() const { return m_target.height; }

                GLuint GetTexture() const { return m_target.dynamicTextureID; }
                GLuint GetFrameBuffer() const { return m_target.frameBuffer; }
                GLuint GetDepthBuffer() const { return m_target.depthRenderBuffer; }

            protected:
                virtual void DoRelease();

            public:
                STarget m_target;
                SRenderContext *const m_pContextOwner;
            };
        }
    }
}