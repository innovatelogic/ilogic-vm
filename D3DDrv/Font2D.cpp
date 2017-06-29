#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        CFont2D::CFont2D(const D3DDriver * Interface)
            : Super(Interface)
            , m_pFontNode(NULL)
            , m_pEffect(NULL)
        {

        }

        //----------------------------------------------------------------------------------------------
        CFont2D::CFont2D(const CFont2D& Source)
            : Super(Source)
            , m_pFontNode(NULL)
        {
            if (&Source != this)
            {

            }
        }

        //----------------------------------------------------------------------------------------------
        CFont2D::~CFont2D()
        {
            Release();
        }

        //----------------------------------------------------------------------------------------------
        bool CFont2D::Load(const char *StrRef, int Height, BaseMaterial *pEffect /*= NULL*/)
        {
            Release();

            m_pFontNode = m_pD3DInterface->LoadFont(StrRef, Height);

            return m_pFontNode ? true : false;
        }

        //----------------------------------------------------------------------------------------------
        void CFont2D::Release()
        {
            if (m_pFontNode)
            {
                m_pFontNode->Release();
                m_pFontNode = NULL;
            }
        }

        //----------------------------------------------------------------------------------------------
        void CFont2D::Draw(const wchar_t *atext, unsigned int color)
        {

        }
    }
}