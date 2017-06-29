
#pragma once

#include "MaterialEffect.h"

namespace oes
{
    namespace d3d
    {

        class D3DDRIVER_API MaterialEffectLava : public BaseMaterial
        {
        public:
            MaterialEffectLava(const D3DDriver * Interface);
            virtual ~MaterialEffectLava();

            virtual void			Affect();

            virtual bool			LoadDiffuseMap(const char *filename, bool ImmidiateLoad = true);
            virtual bool			LoadNormalMap(const char *filename, bool ImmidiateLoad = true);
            virtual bool			LoadHeightMap(const char *filename, bool ImmidiateLoad = true);
            virtual bool			LoadNoiseMap(const char *filename, bool ImmidiateLoad = true);

        protected:
            class Texture2D			*DiffuseMap; // diffuse texture
            class Texture2D			*HeightMap;  // height texture
            class Texture2D			*NormalMap;  // normal map
            class Texture3D			*NoiseMap;

        private:
            float	noiseScale;
            float	morphRate;
            float	bubbleRate;
            float	bubbleScale;
            float	scale;
            float	bias;
        };
    }
}