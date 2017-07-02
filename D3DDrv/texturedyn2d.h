#pragma once

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class TextureNode;

        class TextureDyn2D : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(TextureDyn2D, D3DRenderInterface);

        public:
            TextureDyn2D(const D3DDriver *itf);
            virtual ~TextureDyn2D();

            void    Release();

            bool    Load(const char *filepath, bool immidiateLoad = true);
            bool    LoadW(const wchar_t *filepath);

            bool    LoadTextureFromPixels32(
                unsigned int *pixels, 
                unsigned int imgWidth,
                unsigned int imgHeight,
                unsigned int texWidth,
                unsigned int texHeight);

            void    Lock();
            void    Unlock();

        protected:
        private:
            TextureNode         *m_node;
        };
    }
}