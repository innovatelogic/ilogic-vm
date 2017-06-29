
#pragma once

#include "Refcount.h"

namespace oes
{
    namespace d3d
    {

        class D3DDRIVER_API VolumeTextureNode : public Refcount
        {

        public:
            VolumeTextureNode();
            virtual ~VolumeTextureNode();

        protected:
            virtual void DoRelease();

        public:
            //LPDIRECT3DVOLUMETEXTURE9 VolumeTexture;
        };
    }
}