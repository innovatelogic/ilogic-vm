#include "d3ddriverstdafx.h"

#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/fttrigon.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        // CFontNode
        //----------------------------------------------------------------------------------------------
        CFontNode::CFontNode(D3DDriver *p3DDriver, const char* faceName, float height)
            : m_FaceName(faceName)
            , m_fHeight(height)
            , m_p3DDriver(p3DDriver)
        {
            m_pRenderContext = m_p3DDriver->GetCurrentContext();
        }

        //----------------------------------------------------------------------------------------------
        CFontNode::~CFontNode()
        {

        }

        //----------------------------------------------------------------------------------------------
        void CFontNode::DoRelease()
        {
            glDeleteTextures(1, &m_BitmapTexID);

            m_p3DDriver->UnregisterFont(this);

            Refcount::DoRelease();
        }
    }
}