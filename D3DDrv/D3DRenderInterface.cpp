#include "D3DRenderInterface.h"
#include "D3DDriver.h"

namespace oes
{
    namespace d3d
    {
        //------------------------------------------------------------------------
        D3DRenderInterface::D3DRenderInterface(const D3DDriver* Interface)
            : m_pD3DInterface(nullptr)
        {
            if (Interface)
            {
                m_pD3DInterface = const_cast<D3DDriver*>(Interface);
                assert(m_pD3DInterface);
            }
        }

        //------------------------------------------------------------------------
        D3DRenderInterface::D3DRenderInterface(const D3DRenderInterface& Source)
        {
            assert(Source.m_pD3DInterface);
            m_pD3DInterface = Source.m_pD3DInterface;
        }

        //------------------------------------------------------------------------
        D3DRenderInterface::~D3DRenderInterface()
        {

        }

        //------------------------------------------------------------------------
        D3DDriver* D3DRenderInterface::GetInterface() const
        {
            assert(m_pD3DInterface);
            return m_pD3DInterface;
        }
    }
}