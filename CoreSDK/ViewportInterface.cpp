#include "RenderContext.h"
#include "ViewportManager.h"
#include "IDrawInterface.h"
#include "CoreSDK.h"
#include "RenderSDK.h"
#include "Camera.h"
#include "Registry.h"
#include "CameraManager.h"
#include "ViewportInterface.h"
#include <algorithm>

#define COLOR_DARK_RED		0xff8b1818
#define COLOR_DARK_GREEN	0xff117900
#define COLOR_DARK_BLUE		0xff000079

namespace core_sdk_api
{
    Vector	ViewportInterface::m_SUserStartMousePosition = Vector(0.f, 0.f, 0.f);
    Vector	ViewportInterface::m_SUserStartMouseDisplace = Vector(0.f, 0.f, 0.f);
    bool	ViewportInterface::m_bSMiddleButtonPressed = false;

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
        Vector controllerPos;
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
    bool ViewportInterface::ProcessController(const MouseInputData &input)
    {
        const Vector2f &position = input.MousePos;

        switch (input.Code)
        {
        case MOUSE_LEFT:
        {
            if (m_pCoreSDK->GetCameraManager())
            {
                Vector controllerPos;

                if (GetControllerPos(controllerPos))
                {
                    const Vector2f &controlBox = m_pCoreSDK->GetRegistry()->ControlBox;

                    Matrix InvView, I;
                    invert(InvView, m_ViewMatrix);

                    Vector camStrafe = Vector(m_ViewMatrix._11, m_ViewMatrix._21, m_ViewMatrix._31);
                    Vector transDelta(m_ViewPoint - controllerPos);

                    camStrafe.normalize();

                    float fCathetusOppositLen = transDelta.Length() * ::tan(0.1f);
                    Vector vCathetusOpposit = transDelta.Cross(Vector(0.f, 1.f, 0.f));
                    vCathetusOpposit.normalize();

                    vCathetusOpposit *= fCathetusOppositLen;

                    float k = camStrafe.Dot(vCathetusOpposit); // projection length 

                    const Vector axisX = I._row0 * k;
                    const Vector axisY = I._row1 * k;
                    const Vector axisZ = I._row2 * k;

                    const Vector2f viewportSize((float)input.pRenderContext->m_displayModeWidth,
                                                (float)input.pRenderContext->m_displayModeHeight);

                    Vector ax, ay, az;

                    ProjectViewport(ax, controllerPos + axisX, m_ViewMatrix, m_ProjMatrix, viewportSize);
                    ProjectViewport(ay, controllerPos + axisY, m_ViewMatrix, m_ProjMatrix, viewportSize);
                    ProjectViewport(az, controllerPos + axisZ, m_ViewMatrix, m_ProjMatrix, viewportSize);

                    Vector viewDirection = UnprojectViewport(viewDirection,
                        m_ProjMatrix,
                        m_ViewMatrix,
                        position,
                        viewportSize);

                    if (IsPointInRect(position.x, position.y, Vector2f(ax.x, ax.y) - controlBox, Vector2f(ax.x, ax.y) + controlBox))
                    {
                        Vector planeNormal = cross(planeNormal, viewDirection, InvView._row1);
                        Vector intersect = RayPlaneIntersect(m_ViewPoint, planeNormal, controllerPos, I._row0);

                        m_SUserStartMouseDisplace = (intersect - controllerPos) * (1.f / k);
                        m_SUserStartMousePosition = Vector(position.x, position.y, 0.f);

                        SetControlMode(SOEvent_ControlLockX);
                        return true;
                    }
                    if (IsPointInRect(position.x, position.y, Vector2f(ay.x, ay.y) - controlBox, Vector2f(ay.x, ay.y) + controlBox))
                    {
                        Vector planeNormal;
                        cross(planeNormal, viewDirection, InvView._row0);

                        Vector intersect = RayPlaneIntersect(m_ViewPoint, planeNormal, controllerPos, I._row1);

                        m_SUserStartMouseDisplace = (intersect - controllerPos) * (1.f / k);
                        m_SUserStartMousePosition = Vector(position.x, position.y, 0.f);

                        SetControlMode(SOEvent_ControlLockY);
                        return true;
                    }
                    if (IsPointInRect(position.x, position.y, Vector2f(az.x, az.y) - controlBox, Vector2f(az.x, az.y) + controlBox))
                    {
                        Vector planeNormal;
                        cross(planeNormal, viewDirection, InvView._row1);

                        Vector intersect = RayPlaneIntersect(m_ViewPoint, planeNormal, controllerPos, I._row2);

                        m_SUserStartMouseDisplace = (intersect - controllerPos) * (1.f / k);
                        m_SUserStartMousePosition = Vector(position.x, position.y, 0.f);

                        SetControlMode(SOEvent_ControlLockZ);
                        return true;
                    }
                }
            }
        }
        break;

        case MOUSE_MIDDLE:
        {
            //            m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
            //            m_bSMiddleButtonPressed = (InputData.event == MOUSE_Pressed);
            return true;
        }break;
        };
        return false;
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

            CRenderSDK *const pRenderer = m_pCoreSDK->GetRenderSDK();

            pRenderer->DrawLine(position, position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED, false);
            pRenderer->DrawLine(position, position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE, false);
            pRenderer->DrawLine(position, position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN, false);

            // render arrows
            const float a = 0.8f;
            const float b = 0.1f;
            
            // X arrow
            pRenderer->DrawTriangle(position + (AxisX * a) + (AxisY * b), position + (AxisX * a) + (AxisZ * b), position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
            pRenderer->DrawTriangle(position + (AxisX * a) + (AxisZ * b), position + (AxisX * a) - (AxisY * b), position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
            pRenderer->DrawTriangle(position + (AxisX * a) - (AxisY * b), position + (AxisX * a) - (AxisZ * b), position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
            pRenderer->DrawTriangle(position + (AxisX * a) - (AxisZ * b), position + (AxisX * a) + (AxisY * b), position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);

            //back
            pRenderer->DrawTriangle(position + (AxisX * a) - (AxisY * b), position + (AxisX * a) + (AxisZ * b), position + (AxisX * a) + (AxisY * b), (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_DARK_RED);
            pRenderer->DrawTriangle(position + (AxisX * a) - (AxisZ * b), position + (AxisX * a) - (AxisY * b), position + (AxisX * a) + (AxisY * b), (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_DARK_RED);

            //Y arrow
            pRenderer->DrawTriangle(position + (AxisY * a) + (AxisZ * b), position + (AxisY * a) + (AxisX * b), position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
            pRenderer->DrawTriangle(position + (AxisY * a) + (AxisX * b), position + (AxisY * a) - (AxisZ * b), position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
            pRenderer->DrawTriangle(position + (AxisY * a) - (AxisZ * b), position + (AxisY * a) - (AxisX * b), position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
            pRenderer->DrawTriangle(position + (AxisY * a) - (AxisX * b), position + (AxisY * a) + (AxisZ * b), position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);

            //back
            pRenderer->DrawTriangle(position + (AxisY * a) - (AxisZ * b), position + (AxisY * a) + (AxisX * b), position + (AxisY * a) + (AxisZ * b), (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_DARK_BLUE);
            pRenderer->DrawTriangle(position + (AxisY * a) + (AxisZ * b), position + (AxisY * a) - (AxisX * b), position + (AxisY * a) - (AxisZ * b), (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_DARK_BLUE);

            //Z arrow
            pRenderer->DrawTriangle(position + (AxisZ * a) + (AxisX * b), position + (AxisZ * a) + (AxisY * b), position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
            pRenderer->DrawTriangle(position + (AxisZ * a) + (AxisY * b), position + (AxisZ * a) - (AxisX * b), position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
            pRenderer->DrawTriangle(position + (AxisZ * a) - (AxisX * b), position + (AxisZ * a) - (AxisY * b), position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
            pRenderer->DrawTriangle(position + (AxisZ * a) - (AxisY * b), position + (AxisZ * a) + (AxisX * b), position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);

            pRenderer->DrawTriangle(position + (AxisZ * a) + (AxisY * b), position + (AxisZ * a) + (AxisX * b), position + (AxisZ * a) - (AxisY * b), (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_DARK_GREEN);
            pRenderer->DrawTriangle(position + (AxisZ * a) - (AxisY * b), position + (AxisZ * a) - (AxisX * b), position + (AxisZ * a) + (AxisY * b), (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_DARK_GREEN);

            //render origin rects
            const float c = 0.3f;

            // X-Y
            pRenderer->DrawLine(position + (AxisX * c), position + (AxisX * c) + (AxisY * c), (Mode == SOEvent_ControlLockXY) ? COLOR_YELLOW : COLOR_RED, false);
            pRenderer->DrawLine(position + (AxisX * c) + (AxisY * c), position + (AxisY * c), (Mode == SOEvent_ControlLockXY) ? COLOR_YELLOW : COLOR_BLUE, false);

            // Y-Z
            pRenderer->DrawLine(position + (AxisY * c), position + (AxisY * c) + (AxisZ * c), (Mode == SOEvent_ControlLockYZ) ? COLOR_YELLOW : COLOR_BLUE, false);
            pRenderer->DrawLine(position + (AxisY * c) + (AxisZ * c), position + (AxisZ * c), (Mode == SOEvent_ControlLockYZ) ? COLOR_YELLOW : COLOR_GREEN, false);

            // X-Z
            pRenderer->DrawLine(position + (AxisX * c), position + (AxisX * c) + (AxisZ * c), (Mode == SOEvent_ControlLockXZ) ? COLOR_YELLOW : COLOR_RED, false);
            pRenderer->DrawLine(position + (AxisX * c) + (AxisZ * c), position + (AxisZ * c), (Mode == SOEvent_ControlLockXZ) ? COLOR_YELLOW : COLOR_GREEN, false);

            // higlight control axis
            if (m_pNode->m_pKey->GetControlState() == ActorState_Locked)
            {
                if (m_pCoreSDK->GetEditControlMode() == ECM_Move)
                {
                    switch (m_pNode->m_pKey->GetControlMode())
                    {
                    case SOEvent_ControlLockX:
                    {
                        pRenderer->DrawLine(position - AxisX, position, COLOR_GREEN, false);
                        pRenderer->DrawLine(position + AxisX, position + (AxisX * 2), COLOR_GREEN, false);
                    }break;

                    case SOEvent_ControlLockY:
                    {
                        pRenderer->DrawLine(position - AxisY, position, COLOR_GREEN, false);
                        pRenderer->DrawLine(position + AxisY, position + (AxisY * 2), COLOR_GREEN, false);
                    }break;

                    case SOEvent_ControlLockZ:
                    {
                        pRenderer->DrawLine(position - AxisZ, position, COLOR_GREEN, false);
                        pRenderer->DrawLine(position + AxisZ, position + (AxisZ * 2), COLOR_GREEN, false);
                    }break;
                    }
                }
                else if (m_pCoreSDK->GetEditControlMode() == ECM_Rotate)
                {
                    switch (m_pNode->m_pKey->GetControlMode())
                    {
                    case SOEvent_ControlLockX:
                    {
                        pRenderer->DrawLine(position - AxisY, position, COLOR_GREEN, false);
                        pRenderer->DrawLine(position + AxisY, position + (AxisY * 2), COLOR_GREEN, false);
                    }break;

                    case SOEvent_ControlLockY:
                    {
                        pRenderer->DrawLine(position - AxisZ, position, COLOR_GREEN, false);
                        pRenderer->DrawLine(position + AxisZ, position + (AxisZ * 2), COLOR_GREEN, false);
                    }break;

                    case SOEvent_ControlLockZ:
                    {
                        pRenderer->DrawLine(position - AxisX, position, COLOR_GREEN, false);
                        pRenderer->DrawLine(position + AxisX, position + (AxisX * 2), COLOR_GREEN, false);
                    }break;
                    }
                }
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    bool ViewportInterface::GetControllerPos(Vector &out) const
    {
        bool bResult = false;

        if (!m_SelectedList.empty())
        {
            Bounds3f bbox((*m_SelectedList.begin())->GetCompositeBounds_());

            for each (const IDrawInterface *var in m_SelectedList)
            {
                Bounds3f bound = var->GetBounds_();

                if (bound.IsValid())
                {
                    bbox += bound;
                }
            }

            out = bbox.bound_min + ((bbox.bound_max - bbox.bound_min) * 0.5f);

            bResult = true;
        }
        return bResult;
    }
}