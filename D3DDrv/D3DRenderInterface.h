
#pragma once

#include "dxstdafx.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;

        class D3DDRIVER_API D3DRenderInterface
        {
        public:
            D3DRenderInterface(const D3DDriver* Interface);
            D3DRenderInterface(const D3DRenderInterface &Source);
            virtual ~D3DRenderInterface();

            D3DDriver* GetInterface() const;

        protected:
            D3DDriver * m_pD3DInterface;
        };
    }
}