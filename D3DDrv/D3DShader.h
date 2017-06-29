
#pragma once

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class D3DDRIVER_API D3DShader : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(D3DShader, D3DRenderInterface);

        public:
            D3DShader(const class D3DDriver * Interface);
            virtual ~D3DShader();

        protected:
        private:
            //	LPD3DXEFFECT		Shader;
        };
    }
}