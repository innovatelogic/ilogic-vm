
#pragma once

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class CFontNode;
        class BaseMaterial;
        class D3DDriver;

        class D3DDRIVER_API CFont2D : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(CFont2D, D3DRenderInterface);

        public:
            CFont2D(const D3DDriver *Interface);
            CFont2D(const CFont2D &Source);
            virtual ~CFont2D();

            virtual bool Load(const char *StrRef, int Height, BaseMaterial *pEffect = NULL);

            virtual void Release();
            virtual void Draw(const wchar_t *text, unsigned int color);

            CFontNode* GetFontNode() const { return m_pFontNode; }

        protected:
        private:
            CFontNode		*m_pFontNode;
            BaseMaterial	*m_pEffect;
        };
    }
}