
#pragma once

#ifdef _WIN32
#pragma warning(disable: 4251)
#endif

#include "BaseMaterial.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;
        class Texture2D;

        class D3DDRIVER_API MaterialEffect : public BaseMaterial
        {
        public:
            MaterialEffect(const MaterialEffect &other) = delete;
            MaterialEffect operator= (const MaterialEffect &other) = delete;

            MaterialEffect(const D3DDriver *Interface);
            virtual ~MaterialEffect();

            virtual bool			Release();
            virtual void			Affect();

            /*const*/ Texture2D*	GetDiffuseTexture() const;
            const Texture2D*	GetNormalTexture() const;
            const Texture2D*	GetHeightTexure() const;

            virtual bool			LoadDiffuseMap(const char *filename, bool ImmidiateLoad = true);
            virtual bool			LoadNormalMap(const char *filename, bool ImmidiateLoad = true);
            virtual bool			LoadHeightMap(const char *filename, bool ImmidiateLoad = true);

            float GetDiffuseU() const;
            float GetDiffuseV() const;
            float GetDiffuseUL() const;
            float GetDiffuseVL() const;

        protected:
            Texture2D			*DiffuseMap; // diffuse texture
            Texture2D			*HeightMap;  // height texture
            Texture2D			*NormalMap;  // normal map 
        };
    }
}