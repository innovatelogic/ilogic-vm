
#pragma once

#ifdef _WIN32
#pragma warning (disable:4251)
#endif

#include "Refcount.h"

namespace oes
{
    namespace d3d
    {
        class SRenderContext;
        class D3DDriver;

        /**
         * Platform dependent primitives.
         */
        class D3DDRIVER_API TextureNode : public Refcount
        {
        public:
            TextureNode(D3DDriver *p3DDriver);
            virtual ~TextureNode();

            bool GetInfo(unsigned int &Width, unsigned int &Height) const;
            bool GetDIB(BYTE** pData, bool bResample = false, unsigned int NewWidth = 0, unsigned int NewHeight = 0) const;

            SRenderContext* GetOwnerContext() const { return m_pRenderContext; }

        protected:
            virtual void DoRelease();

        public:
            STGADesc			m_TGADesc;
            STextureBitmap		m_pSTextureBitmap;

            SRenderContext		*m_pRenderContext;
            D3DDriver			*m_p3DDriver;
        };
    }
}