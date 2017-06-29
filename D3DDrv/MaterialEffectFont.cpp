#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        CMaterialEffectFont::CMaterialEffectFont(const D3DDriver *pInterface)
            : BaseMaterial(pInterface)
        {

        }

        //----------------------------------------------------------------------------------------------
        CMaterialEffectFont::~CMaterialEffectFont()
        {

        }

        //----------------------------------------------------------------------------------------------
        bool CMaterialEffectFont::Release()
        {
            return true;
        }

        //----------------------------------------------------------------------------------------------
        void CMaterialEffectFont::Affect()
        {

        }
    }
}