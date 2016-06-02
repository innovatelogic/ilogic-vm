#include "ViewportManager.h"
#include "IDrawInterface.h"
#include "CoreSDK.h"
#include "RenderSDK.h"
#include "ViewportInterface.h"
#include "Camera.h"
#include "CameraManager.h"
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

        CViewportManager *manager = m_pCoreSDK->GetViewportManager();

        assert(manager);

        m_pNode = manager->RegisterViewport(this, pSrc, nullptr);
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

            DrawController(out.bound_min + ((out.bound_max - out.bound_min) * 0.5f));
        }
    }

    //----------------------------------------------------------------------------------------------
    void ViewportInterface::SetSelection(const std::vector<IDrawInterface*> &selection)
    {
        m_SelectedList.clear();

        std::copy(selection.begin(), selection.end(), std::back_inserter(m_SelectedList));
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

    //----------------------------------------------------------------------------------------------
    void ViewportInterface::DrawController(const Vector &pos) const
    {
        RenderSDK::SRTVariant_Adjacency &queve = m_pCoreSDK->GetRenderSDK()->GetCurrQuevueAdjaency();

        if (const CCamera *pCamera = m_pCoreSDK->GetCameraManager()->GetActiveCamera(queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext))
        {
            EScrObjectEvent Mode = m_pNode->m_pKey->GetControlMode();

            const Vector &position = pos;

            Vector CamStrafe(pCamera->GetStrafe());
            Vector TransDelta(pCamera->GetTransformedWTM_().t - position);

            CamStrafe.normalize();

            float fCathetusOppositLen = TransDelta.Length() * ::tan(0.1f);
            Vector vCathetusOpposit = TransDelta.Cross(Vector(0.f, 1.f, 0.f));
            vCathetusOpposit.normalize();
            vCathetusOpposit *= fCathetusOppositLen;

            float k = CamStrafe.Dot(vCathetusOpposit);

            Matrix I;
            const Vector AxisX = I._row0 * k;
            const Vector AxisY = I._row1 * k;
            const Vector AxisZ = I._row2 * k;

            m_pCoreSDK->GetRenderSDK()->DrawLine(position, position + AxisX, COLOR_WHITE/*(Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED*/, false);
            m_pCoreSDK->GetRenderSDK()->DrawLine(position, position + AxisY, COLOR_WHITE/*(Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE*/, false);
            m_pCoreSDK->GetRenderSDK()->DrawLine(position, position + AxisZ, COLOR_WHITE/*(Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN*/, false);

        }
    }
}