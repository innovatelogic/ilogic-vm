
#pragma once

#include "BaseMaterial.h"

namespace oes
{
    namespace d3d
    {
        class D3DDRIVER_API MaterialEffectUI : public BaseMaterial
        {
        public:
            MaterialEffectUI(const D3DDriver * Interface);
            virtual ~MaterialEffectUI();

            virtual	bool	Release();

            virtual void	Affect();
            virtual void	EndEffect();

            virtual bool		LoadDiffuseMap(const char *filename, bool ImmidiateLoad = true);
            virtual bool		LoadDiffuseMapW(const wchar_t *filename);

            class Texture2D*	GetDiffuseTexture() const;

            void	SetRenderTarget(const D3DRenderTarget *RT) { m_pRT = RT; }

            bool	GetInfo(unsigned int &Width, unsigned int &Height) const;
            bool	GetDIB(BYTE** pData, bool bResample = false, unsigned int NewWidth = 0, unsigned int NewHeight = 0) const;

        protected:
        private:
            Texture2D	*m_pDiffuseMap;		// diffuse texture
            const D3DRenderTarget *m_pRT; // outer resource
            SVector4f	m_Texcoord;
        };
    }
}