#include "ViewportManager.h"
#include "IDrawInterface.h"
#include "CoreSDK.h"
#include "RenderSDK.h"
#include "ViewportInterface.h"
#include <algorithm>

namespace core_sdk_api
{
    //------------------------------------------------------------------------
    ViewportInterface::ViewportInterface(const CObjectAbstract *pParent)
        : m_pNode(0)
        , m_fNearPlane(0.f)
        , m_fFarPlane(0.f)
    {
        if (pParent)
        {
            m_pCoreSDK = (CCoreSDK*)pParent->GetUserData();
        }
    }

    //------------------------------------------------------------------------
    ViewportInterface::~ViewportInterface()
    {
        UnregisterViewportInterface();
    }

    //----------------------------------------------------------------------------------------------
    void ViewportInterface::RegisterViewportInterface(const CActor *pSrc)
    {
        assert(m_pCoreSDK && pSrc);

        m_pNode = m_pCoreSDK->GetViewportManager()->RegisterViewport(this, pSrc, nullptr);
    }

    //----------------------------------------------------------------------------------------------
    void ViewportInterface::UnregisterViewportInterface()
    {
        assert(m_pCoreSDK && m_pNode);

        m_pCoreSDK->GetViewportManager()->UnregisterViewport(m_pNode);
    }

    //----------------------------------------------------------------------------------------------
    void ViewportInterface::DrawViewport() const
    {
        if (!m_SelectedList.empty())
        {
            Bounds3f out((*m_SelectedList.begin())->GetCompositeBounds_());

            for each (const IDrawInterface *var in m_SelectedList)
            {
                Bounds3f bound = var->GetBounds_();

                if (bound.IsValid())
                {
                    out += bound;
                }
            }
            
            Matrix I;
            I.Identitly();

            m_pCoreSDK->GetRenderSDK()->DrawBounds(I, out, COLOR_GREEN);
        }
    }

    //----------------------------------------------------------------------------------------------
    void ViewportInterface::SetSelect(IDrawInterface *object)
    {
        if (std::find(m_SelectedList.begin(), m_SelectedList.end(), object) == m_SelectedList.end())
        {
            m_SelectedList.push_back(object);
        }
    }

    //----------------------------------------------------------------------------------------------
    void ViewportInterface::DropSelect(IDrawInterface *object)
    {
        std::list<IDrawInterface*>::const_iterator iterFind = std::find(m_SelectedList.begin(), m_SelectedList.end(), object);
        if (iterFind != m_SelectedList.end())
        {
            m_SelectedList.erase(iterFind);
        }
    }

    //----------------------------------------------------------------------------------------------
    void ViewportInterface::UnselectAll()
    {
        m_SelectedList.clear();
    }
}