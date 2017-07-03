#include "texturedyn2d.h"
#include "D3DDriver.h"
#include "TextureNode.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        TextureDyn2D::TextureDyn2D(const D3DDriver *itf)
            : Super(itf)
            , m_node(nullptr)
        {

        }

        //----------------------------------------------------------------------------------------------
        TextureDyn2D::~TextureDyn2D()
        {
            Release();
        }

        //----------------------------------------------------------------------------------------------
        void TextureDyn2D::Release()
        {

        }

        //----------------------------------------------------------------------------------------------
        bool TextureDyn2D::Load(const char *filepath, bool immidiateLoad /*= true*/)
        {
            return true;
        }

        //----------------------------------------------------------------------------------------------
        bool TextureDyn2D::LoadW(const wchar_t *filepath)
        {
            return true;
        }

        //----------------------------------------------------------------------------------------------
        bool TextureDyn2D::LoadTextureFromPixels32(
            unsigned int *pixels,
            unsigned int imgWidth,
            unsigned int imgHeight,
            unsigned int texWidth,
            unsigned int texHeight)
        {
            Release();

            m_node = m_pD3DInterface->LoadTextureFromPixels32(
                pixels,
                imgWidth,
                imgHeight,
                texWidth,
                texHeight);

            return true;
        }

        //----------------------------------------------------------------------------------------------
        void TextureDyn2D::Lock()
        {
            if (m_node) {
                m_node->Lock();
            }
        }

        //----------------------------------------------------------------------------------------------
        void TextureDyn2D::Unlock()
        {
            if (m_node) {
                m_node->Unlock();
            }
        }
    }
}