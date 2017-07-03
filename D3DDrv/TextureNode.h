
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
            TextureNode(D3DDriver *drv);
            virtual ~TextureNode();

            bool GetInfo(unsigned int &Width, unsigned int &Height) const;
            bool GetDIB(BYTE** pData, bool bResample = false, unsigned int NewWidth = 0, unsigned int NewHeight = 0) const;

            SRenderContext* GetOwnerContext() const { return m_pRenderContext; }

            bool Lock();
            bool Unlock();

            GLuint* GetPixelData32() { return m_PixelsTmp; }
            GLuint  GetPixel32(unsigned int x, unsigned int y) const { return m_PixelsTmp[m_pSTextureBitmap.width * y + x]; }
            GLuint  SetPixel32(unsigned int x, unsigned int y, unsigned int pixel) { return m_PixelsTmp[m_pSTextureBitmap.width * y + x] = pixel; }
        
        protected:
            virtual void DoRelease();

        public:
            STGADesc			m_TGADesc;
            STextureBitmap		m_pSTextureBitmap;

            SRenderContext		*m_pRenderContext;
            D3DDriver			*m_p3DDriver;

            GLuint* m_PixelsTmp;
        };
    }
}