#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        D3DRenderTarget::D3DRenderTarget(const D3DDriver * Interface)
            : Super(Interface)
            , m_pRenderTarget(nullptr)
        {

        }

        //----------------------------------------------------------------------------------------------
        D3DRenderTarget::D3DRenderTarget(const D3DRenderTarget & Source)
            : Super(Source)
        {
            if (this != &Source)
            {

            }
        }

        //----------------------------------------------------------------------------------------------
        D3DRenderTarget::~D3DRenderTarget()
        {
            Release();
        }

        //----------------------------------------------------------------------------------------------
        bool D3DRenderTarget::CreateRenderTarget(size_t width, size_t height)
        {
            assert(m_pRenderTarget == nullptr);

            m_pRenderTarget = m_pD3DInterface->CreateRenderTarget(width, height);

            return (m_pRenderTarget != nullptr);
        }

        //----------------------------------------------------------------------------------------------
        void D3DRenderTarget::Release()
        {
            if (m_pRenderTarget)
            {
                m_pRenderTarget->Release();
                m_pRenderTarget = nullptr;
            }
        }
    }
}