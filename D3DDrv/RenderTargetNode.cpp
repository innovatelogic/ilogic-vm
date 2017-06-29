#include "RenderContext.h"
#include "RenderTargetNode.h"

namespace oes
{
    namespace d3d
    {
        namespace RenderDriver
        {
            RenderTargetNode::RenderTargetNode(size_t width, size_t height, SRenderContext *const pCtxt)
                : m_pContextOwner(pCtxt)
            {
                m_target.width = width;
                m_target.height = height;

                glGenFramebuffersEXT(1, &m_target.frameBuffer);
                glGenRenderbuffersEXT(1, &m_target.depthRenderBuffer);

                glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_target.depthRenderBuffer);
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_target.width, m_target.height);

                GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

                switch (status)
                {
                case GL_FRAMEBUFFER_COMPLETE_EXT:
                    //MessageBox(NULL,"GL_FRAMEBUFFER_COMPLETE_EXT!","SUCCESS",MB_OK|MB_ICONEXCLAMATION);
                    break;

                case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
                    //MessageBox(NULL,"GL_FRAMEBUFFER_UNSUPPORTED_EXT!","ERROR",MB_OK|MB_ICONEXCLAMATION);
                    exit(0);
                    break;

                default:
                    exit(0);
                }

                glGenTextures(1, &m_target.dynamicTextureID);

                glBindTexture(GL_TEXTURE_2D, m_target.dynamicTextureID);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_target.width, m_target.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }

            //----------------------------------------------------------------------------------------------
            RenderTargetNode::~RenderTargetNode()
            {

            }

            //----------------------------------------------------------------------------------------------
            void RenderTargetNode::DoRelease()
            {
                glDeleteTextures(1, &m_target.dynamicTextureID);

                glDeleteFramebuffersEXT(1, &m_target.frameBuffer);
                glDeleteRenderbuffersEXT(1, &m_target.depthRenderBuffer);

                Refcount::DoRelease();
            }
        }
    }
}