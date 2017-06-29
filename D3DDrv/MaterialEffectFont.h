
#pragma once

#include "BaseMaterial.h"

namespace oes
{
    namespace d3d
    {
        class D3DDRIVER_API CMaterialEffectFont : public BaseMaterial
        {
        public:
            CMaterialEffectFont(const D3DDriver *pInterface);
            virtual ~CMaterialEffectFont();

            virtual	bool	Release();
            virtual void	Affect();

        protected:
        private:
        };
    }
}