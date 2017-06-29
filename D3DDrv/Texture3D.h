
#pragma once

#ifdef WIN32
#pragma warning(disable: 4251)
#endif

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;
        class VolumeTextureNode;

        class D3DDRIVER_API Texture3D : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(Texture3D, D3DRenderInterface);

        public:
            Texture3D(const D3DDriver * Interface);
            Texture3D(const Texture3D &Sender);
            virtual ~Texture3D();

            virtual bool	Load(const std::string & URL, unsigned int width, unsigned int height, unsigned int depth, bool ImmidiateLoad = true);
            virtual void	Release();

            //LPDIRECT3DVOLUMETEXTURE9	GetTexture(bool NoNull = true) const;

        protected:
        private:
            VolumeTextureNode		*pVolumeTextureNode;
            std::string					Reference;
        };
    }
}