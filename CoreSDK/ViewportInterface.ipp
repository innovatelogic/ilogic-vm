#include "RenderContext.h"
#include "ViewportManager.h"
#include "IDrawInterface.h"
#include "CoreSDK.h"
#include "RenderSDK.h"
#include "Camera.h"
#include "Registry.h"
#include "CameraManager.h"
#include "ViewportInterface.h"
#include "mathlib.h"
#include <algorithm>

#define COLOR_DARK_RED		0xff8b1818
#define COLOR_DARK_GREEN	0xff117900
#define COLOR_DARK_BLUE		0xff000079

namespace core_sdk_api
{
    template<class TTranformTraits>
    Vector	ViewportInterface<TTranformTraits>::m_SUserStartMousePosition = Vector(0.f, 0.f, 0.f);

    template<class TTranformTraits>
    Vector	ViewportInterface<TTranformTraits>::m_SUserStartMouseDisplace = Vector(0.f, 0.f, 0.f);

    template<class TTranformTraits>
    bool	ViewportInterface<TTranformTraits>::m_bSMiddleButtonPressed = false;

//------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::RegisterViewportInterface(const CActor *pSrc)
    {
        assert(m_pCoreSDK && pSrc);

        CViewportManager *manager = m_pCoreSDK->GetViewportManager();

        assert(manager);

        m_pNode = manager->RegisterViewport(this, pSrc, nullptr);
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::UnregisterViewportInterface()
    {
        assert(m_pCoreSDK && m_pNode);

        m_pCoreSDK->GetViewportManager()->UnregisterViewport(m_pNode);
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::DrawViewport() const
    {
        Vector controllerPos;

        if (!m_SelectedList.empty())
        {
            IDrawInterface *idraw = m_SelectedList.begin()->first;

            Bounds3f out(idraw->GetCompositeBounds_());

            for each (const auto var in m_SelectedList)
            {
                Bounds3f bound = var.first->GetBounds_();

                if (bound.IsValid())
                {
                    out += bound;
                }
            }
            
            Matrix I;
			//const Matrix mj;
			//Foo(mj, mj.t, mj.t);
            I.Identitly();

            m_pCoreSDK->GetRenderSDK()->DrawBounds(I, out, COLOR_GREEN);

            DrawController(out.bound_min + ((out.bound_max - out.bound_min) * 0.5f));
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::SetSelection(const std::vector<IDrawInterface*> &selection)
    {
        m_SelectedList.clear();

        for each (const auto &item in selection)
        {
            m_SelectedList.insert(std::make_pair(item, SController()));
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::SetSelect(IDrawInterface *object)
    {
        if (m_SelectedList.find(object) == m_SelectedList.end())
        {
            m_SelectedList.insert(std::make_pair(object, SController()));
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::DropSelect(IDrawInterface *object)
    {
        TMapSelection::const_iterator iterFind = m_SelectedList.find( object);
        if (iterFind != m_SelectedList.end())
        {
            m_SelectedList.erase(iterFind);
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::UnselectAll()
    {
        m_SelectedList.clear();
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    bool ViewportInterface<TTranformTraits>::ProcessController(const MouseInputData &input)
    {
        const Vector2f &position = input.MousePos;

        if (GetControllerState() == ActorState_None)
        {
            Vector controllerPos;

            if (GetControllerPos(controllerPos))
            {
                Matrix InvView, I;
                invert(InvView, m_ViewMatrix);

                float k = GetControllerScaleMultiplicator(m_ViewMatrix, m_ViewPoint, controllerPos);

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

                const Vector2f &controlBox = m_pCoreSDK->GetRegistry()->ControlBox;
                if (IsPointInRect(position.x, position.y, Vector2f(ax.x, ax.y) - controlBox, Vector2f(ax.x, ax.y) + controlBox))
                {
                    Vector planeNormal = cross(planeNormal, viewDirection, InvView._row1);
                    Vector intersect = RayPlaneIntersect(m_ViewPoint, planeNormal, controllerPos, I._row0);

                    m_SUserStartMouseDisplace = (intersect - controllerPos) * (1.f / k);
                    m_SUserStartMousePosition = Vector(position.x, position.y, 0.f);

                    SetControlMode(SOEvent_ControlLockX);
                    UpdateSelectionState(SOEvent_ControlLockX, controllerPos);

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
                    UpdateSelectionState(SOEvent_ControlLockY, controllerPos);

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
                    UpdateSelectionState(SOEvent_ControlLockY, controllerPos);

                    return true;
                }
            }

//            case MOUSE_MIDDLE:
//            {
//                m_SUserStartMousePosition = Vector(input.MousePos.x, input.MousePos.y, 0.f);
//                m_bSMiddleButtonPressed = (input.event == MOUSE_Pressed);
//                return true;
//            }break;
        }
        return false;
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    bool ViewportInterface<TTranformTraits>::ProcessController(const MouseMoveInputData &input)
    {
        bool bResult = false;

        switch (m_controllerState)
        {
        case ActorState_None:
        {
            // do nothing: Logging in a very particular case.
        }break;

        case ActorState_Locked:
        {
            const EObjEditControlMode mode = m_pCoreSDK->GetEditControlMode();

            float k = 0.f;
            Vector intersect = GetIntersectPosition(input, m_controllerMode, k);

            switch (m_controllerMode)
            {
                // local object movement 
            case SOEvent_FreeMove:
            case SOEvent_ControlLockOrigin: // free object move
            {
            }break;

            case SOEvent_ControlLockX:
            {
                if (mode == EObjEditControlMode::ECM_Rotate) // rotate by x-axis
                {
                    ControllerRotateLocal(input);
                }
                else if (mode == EObjEditControlMode::ECM_Scale) // scale by x-axis
                {
                    //ProcessControllerScaleLocal(input);
                }
                else // move object by x-axis
                {
                    ControllerTranslate(intersect, k);
                }
                bResult = true;
            }break;

            case SOEvent_ControlLockY:
            {
                if (mode == EObjEditControlMode::ECM_Rotate) // rotate by y-axis
                {
                    //ProcessControllerRotateLocal(input);
                }
                else if (mode == EObjEditControlMode::ECM_Scale) // scale by y-axis
                {
                    //ProcessControllerScaleLocal(input);
                }
                else
                {
                    ControllerTranslate(intersect, k);
                }
                bResult = true;
            }break;

            case SOEvent_ControlLockZ:
            {
                if (mode == EObjEditControlMode::ECM_Rotate) // rotate by z-axis
                {
                    //ProcessControllerRotateLocal(InputData);
                }
                else if (mode == EObjEditControlMode::ECM_Scale) // scale by z-axis
                {
                    //ProcessControllerScaleLocal(InputData);
                }
                else
                {	// move object by z-axis
                    ControllerTranslate(intersect, k);
                }
                bResult = true;
            }break;
            };
        }break;
        default:
            assert(false);
            break;
        }
        return bResult;
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::ControllerRelease()
    {
        if (GetControllerState() == ActorState_Locked)
        {
            SetControlMode(SOEvent_None);
            SetControllerState(ActorState_None);
        }
    }
    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::ProcessMouseInput(const MouseInputData &input)
    {
        //case MOUSE_MIDDLE:
        m_SUserStartMousePosition = Vector(input.MousePos.x, input.MousePos.y, 0.f);
        m_bSMiddleButtonPressed = (input.event == MOUSE_Pressed);
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::ProcessMouseInput(const MouseMoveInputData &input)
    {

    }


    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::DrawController(const Vector &pos) const
    {
        RenderSDK::SRTVariant_Adjacency &queve = m_pCoreSDK->GetRenderSDK()->GetCurrQuevueAdjaency();

        if (const CCamera *pCamera = m_pCoreSDK->GetCameraManager()->GetActiveCamera(queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext))
        {
            EScrObjectEvent Mode = m_pNode->m_pKey->GetControlMode();

            const Vector &position = pos;

            float k = GetControllerScaleMultiplicator(m_ViewMatrix, m_ViewPoint, position);

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

            // highlight control axis
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
    template<class TTranformTraits>
    bool ViewportInterface<TTranformTraits>::GetControllerPos(Vector &out) const
    {
        bool bResult = false;

        if (!m_SelectedList.empty())
        {
            IDrawInterface *idraw = m_SelectedList.begin()->first;

            Bounds3f bbox(idraw->GetCompositeBounds_());

            for each (const auto var in m_SelectedList)
            {
                Bounds3f bound = (var.first)->GetBounds_();

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

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::ControllerTranslate(const Vector &pos, float k)
    {
        bool bTransformed = false;

        if (!m_SelectedList.empty())
        {
            for each (auto &item in m_SelectedList)
            {
                Vector tpos;

                IDrawInterface *idraw = item.first;
                const SController &ctrl = item.second;

                idraw->GlobalToLocalTransform(tpos, pos - (m_SUserStartMouseDisplace * k) + ctrl.displace);
                
                idraw->SetPosition_(tpos);

                CActor *actor = const_cast<CActor*>(idraw->GetNode()->key());

                m_pCoreSDK->GetViewportManager()->RebuildTransform(actor);

                actor->BroadcastEvent(Event_OnChangePivot);
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::ControllerRotateLocal(const MouseMoveInputData &input)
    {
        Vector controllerPos;

        if (GetControllerPos(controllerPos))
        {
            switch (m_controllerMode)
            {
            case SOEvent_ControlLockX:
            {
                Matrix InvView, I;
                invert(InvView, m_ViewMatrix);

                const Vector2f ViewportSize((float)input.pRenderContext->m_displayModeWidth,
                                      (float)input.pRenderContext->m_displayModeHeight);

                Vector ViewDirection = UnprojectViewport(ViewDirection,
                    m_ProjMatrix,
                    m_ViewMatrix,
                    input.MousePos,
                    ViewportSize);

                Vector PrevViewDirection = UnprojectViewport(PrevViewDirection,
                    m_ProjMatrix,
                    m_ViewMatrix,
                    Vector2f(m_SUserStartMousePosition.x, m_SUserStartMousePosition.y),
                    ViewportSize);

                Vector Intersect = RayPlaneIntersect(controllerPos, Vector(0.f, 0.f, 1.f), m_ViewPoint, ViewDirection);

                Vector IntersectPrev = RayPlaneIntersect(controllerPos, Vector(0.f, 0.f, 1.f), m_ViewPoint, PrevViewDirection);

                Vector d1 = Intersect - controllerPos;
                Vector d2 = IntersectPrev - controllerPos;

                d1.normalize();
                d2.normalize();

                float dot = d2.Dot(d1);

                if (dot < 1.f)
                {
                    float sign1 = d1.Dot(Vector(1.f, 0.f, 0.f));
                    float sign2 = d2.Dot(Vector(1.f, 0.f, 0.f));

                    if (d1.y > 0.f)
                    {
                        sign1 *= -1.f;
                        sign2 *= -1.f;
                    }

                    float fdelta = ((sign1 > sign2) ? 1.f : -1.f) * ::acosf(dot);

                    //TMP
                    IDrawInterface *idraw = m_SelectedList.begin()->first;

                    idraw->AddYawPitchRoll(Vector(fdelta, 0.f, 0.f));

                    Quaternion rot(0.f, 0.f, 0.f, 1.f);

                    rot.set_rot(fdelta, 0.f, 0.f);

                    Matrix M;
                    rot.Normalize();
                    rot.ToMatrix(&M);

                    Matrix LTM = idraw->GetLTM_();
                    Vector t = LTM.t;
                    LTM.t.Set(0.f, 0.f, 0.f);
                    LTM = LTM * M;
                    LTM.t = t;
                    idraw->SetLTM_(LTM);

                    CActor *actor = const_cast<CActor*>(idraw->GetNode()->key());

                    m_pCoreSDK->GetViewportManager()->RebuildTransform(actor);

                    actor->BroadcastEvent(Event_OnChangePivot);
                }
            }break;

            default:
                break;
            };
        }

        
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::ControllerScaleLocal(const MouseMoveInputData &input)
    {

    }

	    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    Vector ViewportInterface<TTranformTraits>::GetIntersectPosition(const MouseMoveInputData &input, EScrObjectEvent mode, float &out_mult) const
    {
        Vector out;

        Matrix invView, I;
        Vector ctrlPosition;
        
        GetControllerPos(ctrlPosition);

        invert(invView, m_ViewMatrix);

        float len = length(m_ViewPoint - ctrlPosition);

        const Vector2f viewportSize((float)input.pRenderContext->m_displayModeWidth,
                                    (float)input.pRenderContext->m_displayModeHeight);

        Vector viewDirection;
        
        UnprojectViewport(viewDirection,
            m_ProjMatrix,
            m_ViewMatrix,
            input.MousePos,
            viewportSize);

        float k = GetControllerScaleMultiplicator(m_ViewMatrix, m_ViewPoint, ctrlPosition);

        Vector planeNormal;

        switch (mode)
        {
        case SOEvent_ControlLockX:
        {
            cross(planeNormal, viewDirection, invView._row1);

            out = RayPlaneIntersect(m_ViewPoint, planeNormal, ctrlPosition, I._row0);
        }break;

        case SOEvent_ControlLockY:
        {
            cross(planeNormal, viewDirection, invView._row0);

            out = RayPlaneIntersect(m_ViewPoint, planeNormal, ctrlPosition, I._row1);
        }break;

        case SOEvent_ControlLockZ:
        {
            cross(planeNormal, viewDirection, invView._row1);
            out = RayPlaneIntersect(m_ViewPoint, planeNormal, ctrlPosition, I._row2);
        }break;

        default:
            assert(false);
            break;
        }

        out_mult = k;

        return out;
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits>
    void ViewportInterface<TTranformTraits>::UpdateSelectionState(EScrObjectEvent state, const Vector &ctrlPos)
    {
        for (TMapSelection::iterator iter = m_SelectedList.begin(); iter != m_SelectedList.end(); ++iter)
        {
            const IDrawInterface *idraw = iter->first;

            assert(idraw);

            Vector &displace = iter->second.displace;
            displace = idraw->GetTransformedWTM_().t - ctrlPos;
        }

       /* for each (auto &item in m_SelectedList)
        {
            const IDrawInterface *idraw = item.first;

            assert(idraw);

            Vector &displace = item.second.displace;
            displace = ctrlPos - idraw->GetTransformedWTM_().t;
        }*/
    }
}