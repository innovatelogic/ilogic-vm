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
    template<class TTranformTraits, class TTransformHistory>
    Vector	ViewportInterface<TTranformTraits, TTransformHistory>::m_SUserStartMousePosition = Vector(0.f, 0.f, 0.f);

    template<class TTranformTraits, class TTransformHistory>
    Vector	ViewportInterface<TTranformTraits, TTransformHistory>::m_SUserStartMouseDisplace = Vector(0.f, 0.f, 0.f);

    template<class TTranformTraits, class TTransformHistory>
    Vector	ViewportInterface<TTranformTraits, TTransformHistory>::m_SUserStartControllerPos = Vector(0.f, 0.f, 0.f);

    template<class TTranformTraits, class TTransformHistory>
    Vector	ViewportInterface<TTranformTraits, TTransformHistory>::m_SUserAccumRotation = Vector(0.f, 0.f, 0.f);
    
	template<class TTranformTraits, class TTransformHistory>
    bool	ViewportInterface<TTranformTraits, TTransformHistory>::m_bSMiddleButtonPressed = false;

//------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    ViewportInterface<TTranformTraits, TTransformHistory>::ViewportInterface(const CObjectAbstract *pParent)
        : m_pNode(nullptr)
        , m_fNearPlane(0.f)
        , m_fFarPlane(0.f)
        , m_controllerMode(SOEvent_None)
        , m_controllerState(ActorState_None)
    {
        if (pParent)
        {
            m_pCoreSDK = (CCoreSDK*)pParent->GetUserData();
        }
    }

    //------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    ViewportInterface<TTranformTraits, TTransformHistory>::~ViewportInterface()
    {
        UnregisterViewportInterface();
    }
    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::RegisterViewportInterface(const CActor *pSrc)
    {
        assert(m_pCoreSDK && pSrc);

        CViewportManager *manager = m_pCoreSDK->GetViewportManager();

        assert(manager);

        m_pNode = manager->RegisterViewport(this, pSrc, nullptr);
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::UnregisterViewportInterface()
    {
        assert(m_pCoreSDK && m_pNode);

        m_pCoreSDK->GetViewportManager()->UnregisterViewport(m_pNode);
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::DrawViewport() const
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

            I.Identitly();

            m_pCoreSDK->GetRenderSDK()->DrawBounds(I, out, COLOR_GREEN);


			Vector pos;
			if (m_pCoreSDK->GetEditControlMode() == ECM_Rotate && m_controllerState == ActorState_Locked){
				pos = m_SUserStartControllerPos;
			}
			else{
				GetControllerPos(pos);
			}
			
            DrawController(pos);
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::SetSelection(const std::vector<IDrawInterface*> &selection)
    {
        m_SelectedList.clear();

        for each (const auto &item in selection)
        {
            m_SelectedList.insert(std::make_pair(item, SController()));
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::SetSelect(IDrawInterface *object)
    {
        if (m_SelectedList.find(object) == m_SelectedList.end())
        {
            m_SelectedList.insert(std::make_pair(object, SController()));
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::DropSelect(IDrawInterface *object)
    {
        TMapSelection::const_iterator iterFind = m_SelectedList.find( object);
        if (iterFind != m_SelectedList.end())
        {
            m_SelectedList.erase(iterFind);
        }
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::UnselectAll()
    {
        m_SelectedList.clear();
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    bool ViewportInterface<TTranformTraits, TTransformHistory>::ProcessController(const MouseInputData &input)
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
					m_SUserAccumRotation.Set(0.f, 0.f, 0.f);
					m_SUserStartControllerPos = controllerPos;

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
					m_SUserAccumRotation.Set(0.f, 0.f, 0.f);
					m_SUserStartControllerPos = controllerPos;

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
					m_SUserAccumRotation.Set(0.f, 0.f, 0.f);
					m_SUserStartControllerPos = controllerPos;

                    SetControlMode(SOEvent_ControlLockZ);
                    UpdateSelectionState(SOEvent_ControlLockY, controllerPos);

                    return true;
                }

                // check plane origin hit
                /** ray triangle intersection */
                const float c = 0.3f;
                float t = 0.f;

                Vector P0 = (controllerPos + (axisX * c));
                Vector P1 = (controllerPos + (axisX * c) + (axisY * c));
                Vector P2 = (controllerPos + (axisY * c));

                if ((IntersectRayWithTriangle(m_ViewPoint, m_ViewPoint + viewDirection, controllerPos, P0, P1, &t) == 1) ||
                    (IntersectRayWithTriangle(m_ViewPoint, m_ViewPoint + viewDirection, controllerPos, P1, P2, &t) == 1))
                {
                    m_SUserStartMouseDisplace = (m_ViewPoint + viewDirection * t) - (controllerPos + (axisX * c) + (axisY * c));
                    m_SUserStartMousePosition = Vector(position.x, position.y, 0.f);
                    m_SUserAccumRotation.Set(0.f, 0.f, 0.f);
                    m_SUserStartControllerPos = controllerPos;

                    SetControlMode(SOEvent_ControlLockXY);
                    UpdateSelectionState(SOEvent_ControlLockXY, controllerPos);

                    return true;
                }

                P0 = (controllerPos + (axisZ * c));
                P1 = (controllerPos + (axisZ * c) + (axisY * c));
                P2 = (controllerPos + (axisY * c));

                if ((IntersectRayWithTriangle(m_ViewPoint, m_ViewPoint + viewDirection, controllerPos, P0, P1, &t) == 1) ||
                    (IntersectRayWithTriangle(m_ViewPoint, m_ViewPoint + viewDirection, controllerPos, P1, P2, &t) == 1))
                {
                    m_SUserStartMouseDisplace = (m_ViewPoint + viewDirection * t) - (controllerPos + (axisY * c) + (axisZ * c));
                    m_SUserStartMousePosition = Vector(position.x, position.y, 0.f);
                    m_SUserAccumRotation.Set(0.f, 0.f, 0.f);
                    m_SUserStartControllerPos = controllerPos;

                    SetControlMode(SOEvent_ControlLockYZ);
                    UpdateSelectionState(SOEvent_ControlLockYZ, controllerPos);

                    return true;
                }

                P0 = (controllerPos + (axisX * c));
                P1 = (controllerPos + (axisX * c) + (axisZ * c));
                P2 = (controllerPos + (axisZ * c));

                if ((IntersectRayWithTriangle(m_ViewPoint, m_ViewPoint + viewDirection, controllerPos, P0, P1, &t) == 1) ||
                    (IntersectRayWithTriangle(m_ViewPoint, m_ViewPoint + viewDirection, controllerPos, P1, P2, &t) == 1))
                {
                    m_SUserStartMouseDisplace = (m_ViewPoint + viewDirection * t) - (controllerPos + (axisX * c) + (axisZ * c));
                    m_SUserStartMousePosition = Vector(position.x, position.y, 0.f);
                    m_SUserAccumRotation.Set(0.f, 0.f, 0.f);
                    m_SUserStartControllerPos = controllerPos;

                    SetControlMode(SOEvent_ControlLockXZ);
                    UpdateSelectionState(SOEvent_ControlLockXZ, controllerPos);

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
    template<class TTranformTraits, class TTransformHistory>
    bool ViewportInterface<TTranformTraits, TTransformHistory>::ProcessController(const MouseMoveInputData &input)
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

            switch (m_controllerMode)
            {
                // local object movement 
            case SOEvent_FreeMove:
            case SOEvent_ControlLockOrigin: // free object move
            {
            }break;

            case SOEvent_ControlLockX:
            case SOEvent_ControlLockY:
            case SOEvent_ControlLockZ:
            {
                if (mode == EObjEditControlMode::ECM_Rotate) // rotate by z-axis
                {
                    ControllerRotateLocal(input);
                }
                else if (mode == EObjEditControlMode::ECM_Scale) // scale by z-axis
                {
                    //ProcessControllerScaleLocal(InputData);
                }
                else
                {	// move object by axis
					float k = 0.f;
					Vector intersect = GetIntersectPosition(input, m_controllerMode, k);
                    ControllerTranslate(intersect, k);
                }
                bResult = true;
            }break;

            case SOEvent_ControlLockXY:
            case SOEvent_ControlLockYZ:
            case SOEvent_ControlLockXZ:
            {
                if (mode == EObjEditControlMode::ECM_Move) 
                {
                    float k = 0.f;
                    Vector intersect = GetIntersectPosition(input, m_controllerMode, k);
                    ControllerTranslate(intersect, k);
                }
            }break;

            default:
                break;
            };
        }break;
        default:
            assert(false);
            break;
        }
        return bResult;
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::ControllerRelease()
    {
        if (GetControllerState() == ActorState_Locked)
        {
            SetControlMode(SOEvent_None);
            SetControllerState(ActorState_None);

            TVecObjects vec;

            for (TMapSelection::iterator iter = m_SelectedList.begin(); iter != m_SelectedList.end(); ++iter)
            {
                const IDrawInterface *idraw = iter->first;

                assert(idraw);

                vec.push_back(idraw);
            }

            CommitState(vec);
        }
    }
    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::ProcessMouseInput(const MouseInputData &input)
    {
        //case MOUSE_MIDDLE:
        m_SUserStartMousePosition = Vector(input.MousePos.x, input.MousePos.y, 0.f);
        m_bSMiddleButtonPressed = (input.event == MOUSE_Pressed);
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::ProcessMouseInput(const MouseMoveInputData &input)
    {

    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::DrawController(const Vector &pos) const
    {
        EScrObjectEvent mode = m_controllerMode;

        const Vector &position = pos;

        float k = GetControllerScaleMultiplicator(m_ViewMatrix, m_ViewPoint, position);

        Matrix I;
        const Vector AxisX = I._row0 * k;
        const Vector AxisY = I._row1 * k;
        const Vector AxisZ = I._row2 * k;

        CRenderSDK *const pRenderer = m_pCoreSDK->GetRenderSDK();

        pRenderer->DrawLine(position, position + AxisX, (mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED, false);
        pRenderer->DrawLine(position, position + AxisY, (mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE, false);
        pRenderer->DrawLine(position, position + AxisZ, (mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN, false);

        // render arrows
        const float a = 0.8f;
        const float b = 0.1f;
            
        // X arrow
        pRenderer->DrawTriangle(position + (AxisX * a) + (AxisY * b), position + (AxisX * a) + (AxisZ * b), position + AxisX, (mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
        pRenderer->DrawTriangle(position + (AxisX * a) + (AxisZ * b), position + (AxisX * a) - (AxisY * b), position + AxisX, (mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
        pRenderer->DrawTriangle(position + (AxisX * a) - (AxisY * b), position + (AxisX * a) - (AxisZ * b), position + AxisX, (mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
        pRenderer->DrawTriangle(position + (AxisX * a) - (AxisZ * b), position + (AxisX * a) + (AxisY * b), position + AxisX, (mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);

        //back
        pRenderer->DrawTriangle(position + (AxisX * a) - (AxisY * b), position + (AxisX * a) + (AxisZ * b), position + (AxisX * a) + (AxisY * b), (mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_DARK_RED);
        pRenderer->DrawTriangle(position + (AxisX * a) - (AxisZ * b), position + (AxisX * a) - (AxisY * b), position + (AxisX * a) + (AxisY * b), (mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_DARK_RED);

        //Y arrow
        pRenderer->DrawTriangle(position + (AxisY * a) + (AxisZ * b), position + (AxisY * a) + (AxisX * b), position + AxisY, (mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
        pRenderer->DrawTriangle(position + (AxisY * a) + (AxisX * b), position + (AxisY * a) - (AxisZ * b), position + AxisY, (mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
        pRenderer->DrawTriangle(position + (AxisY * a) - (AxisZ * b), position + (AxisY * a) - (AxisX * b), position + AxisY, (mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
        pRenderer->DrawTriangle(position + (AxisY * a) - (AxisX * b), position + (AxisY * a) + (AxisZ * b), position + AxisY, (mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);

        //back
        pRenderer->DrawTriangle(position + (AxisY * a) - (AxisZ * b), position + (AxisY * a) + (AxisX * b), position + (AxisY * a) + (AxisZ * b), (mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_DARK_BLUE);
        pRenderer->DrawTriangle(position + (AxisY * a) + (AxisZ * b), position + (AxisY * a) - (AxisX * b), position + (AxisY * a) - (AxisZ * b), (mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_DARK_BLUE);

        //Z arrow
        pRenderer->DrawTriangle(position + (AxisZ * a) + (AxisX * b), position + (AxisZ * a) + (AxisY * b), position + AxisZ, (mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
        pRenderer->DrawTriangle(position + (AxisZ * a) + (AxisY * b), position + (AxisZ * a) - (AxisX * b), position + AxisZ, (mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
        pRenderer->DrawTriangle(position + (AxisZ * a) - (AxisX * b), position + (AxisZ * a) - (AxisY * b), position + AxisZ, (mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
        pRenderer->DrawTriangle(position + (AxisZ * a) - (AxisY * b), position + (AxisZ * a) + (AxisX * b), position + AxisZ, (mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);

        pRenderer->DrawTriangle(position + (AxisZ * a) + (AxisY * b), position + (AxisZ * a) + (AxisX * b), position + (AxisZ * a) - (AxisY * b), (mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_DARK_GREEN);
        pRenderer->DrawTriangle(position + (AxisZ * a) - (AxisY * b), position + (AxisZ * a) - (AxisX * b), position + (AxisZ * a) + (AxisY * b), (mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_DARK_GREEN);

        //render origin rects
        const float c = 0.3f;

        // X-Y
        pRenderer->DrawLine(position + (AxisX * c), position + (AxisX * c) + (AxisY * c), (mode == SOEvent_ControlLockXY) ? COLOR_YELLOW : COLOR_RED, false);
        pRenderer->DrawLine(position + (AxisX * c) + (AxisY * c), position + (AxisY * c), (mode == SOEvent_ControlLockXY) ? COLOR_YELLOW : COLOR_BLUE, false);

        // Y-Z
        pRenderer->DrawLine(position + (AxisY * c), position + (AxisY * c) + (AxisZ * c), (mode == SOEvent_ControlLockYZ) ? COLOR_YELLOW : COLOR_BLUE, false);
        pRenderer->DrawLine(position + (AxisY * c) + (AxisZ * c), position + (AxisZ * c), (mode == SOEvent_ControlLockYZ) ? COLOR_YELLOW : COLOR_GREEN, false);

        // X-Z
        pRenderer->DrawLine(position + (AxisX * c), position + (AxisX * c) + (AxisZ * c), (mode == SOEvent_ControlLockXZ) ? COLOR_YELLOW : COLOR_RED, false);
        pRenderer->DrawLine(position + (AxisX * c) + (AxisZ * c), position + (AxisZ * c), (mode == SOEvent_ControlLockXZ) ? COLOR_YELLOW : COLOR_GREEN, false);

        // highlight control axis
        if (m_controllerState == ActorState_Locked)
        {
            if (m_pCoreSDK->GetEditControlMode() == ECM_Move)
            {
                switch (m_controllerMode)
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
                switch (m_controllerMode)
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

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    bool ViewportInterface<TTranformTraits, TTransformHistory>::GetControllerPos(Vector &out) const
    {
        bool bResult = false;

        if (!m_SelectedList.empty())
        {
			TMapSelection::const_iterator iter = m_SelectedList.begin();

			IDrawInterface *idraw = iter->first;

			out = idraw->GetTransformedWTM_().t;

            Bounds3f bbox(idraw->GetCompositeBounds_());

			++iter;

			while (iter != m_SelectedList.end())
            {
                Bounds3f bound = (iter->first)->GetBounds_();

                if (bound.IsValid())
                {
                    bbox += bound;
					
					out = bbox.bound_min + ((bbox.bound_max - bbox.bound_min) * 0.5f);
                }

				++iter;
            }

            bResult = true;
        }
        return bResult;
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::ControllerTranslate(const Vector &pos, float k)
    {
        if (!m_SelectedList.empty())
        {
            Matrix InvView, I;
            invert(InvView, m_ViewMatrix);

            const Vector axisX = I._row0 * k;
            const Vector axisY = I._row1 * k;
            const Vector axisZ = I._row2 * k;
            float c = 0.3f;

            for each (auto &item in m_SelectedList)
            {
                IDrawInterface *idraw = item.first;
                const SController &ctrl = item.second;

                Vector newpos = pos;
                switch (m_controllerMode)
                {
                case SOEvent_ControlLockX:
                case SOEvent_ControlLockY:
                case SOEvent_ControlLockZ:
                {
                    newpos = pos - (m_SUserStartMouseDisplace * k) + ctrl.displace;
                }break;
                case SOEvent_ControlLockXY:
                {
                    newpos = pos - (m_SUserStartMouseDisplace + (axisX * c + axisY * c)) + ctrl.displace;
                }break;

                case SOEvent_ControlLockYZ:
                {
                    newpos = pos - (m_SUserStartMouseDisplace + (axisY * c + axisZ * c)) + ctrl.displace;
                }break;
                case SOEvent_ControlLockXZ:
                {
                    newpos = pos - (m_SUserStartMouseDisplace + (axisX * c + axisZ * c)) + ctrl.displace;
                }break;

                default:
                    break;
                };

                Vector tpos;
                idraw->GlobalToLocalTransform(tpos, newpos);
                
                idraw->SetPosition_(tpos);

                CActor *actor = const_cast<CActor*>(idraw->GetNode()->key());

                m_pCoreSDK->GetViewportManager()->RebuildTransform(actor);

                actor->BroadcastEvent(Event_OnChangePivot);
            }
        }
    }

//#define _TRENARY0 (VAL, C0, R0, RDEF) (VAL == C0 ? R0 : RDEF)
//#define _TRENARY (VAL, C0, R0, C1, R1, RDEF) (VAL == C0 ? R0 : (_TRENARY0(VAL, C1, R1, RDEF))

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::ControllerRotateLocal(const MouseMoveInputData &input)
    {
        float fdelta = GetDeltaRotationAngle(m_ViewMatrix,
            m_ProjMatrix,
            m_ViewPoint,
            Vector2f(input.pRenderContext->m_displayModeWidth, input.pRenderContext->m_displayModeHeight),
            input.MousePos,
            Vector2f(m_SUserStartMousePosition.x, m_SUserStartMousePosition.y),
            m_SUserStartControllerPos,
            (m_controllerMode == SOEvent_ControlLockX) ? EAxisX : ((m_controllerMode == SOEvent_ControlLockY) ? EAxisY : EAxisZ)
            );

       Vector local(0.f, 0.f, 0.f);
        if  (m_controllerMode == SOEvent_ControlLockX)
        {
             m_SUserAccumRotation.z += fdelta;
             local.z = fdelta;
        }
        else if (m_controllerMode == SOEvent_ControlLockY)
        {
            m_SUserAccumRotation.y += fdelta;
            local.y = fdelta;
        }
        else 
        {
            m_SUserAccumRotation.x += fdelta;
            local.x = fdelta;
        }

        if (!m_SelectedList.empty())
        {
            for each (auto &item in m_SelectedList)
            {
                IDrawInterface *idraw = item.first;

                idraw->AddYawPitchRoll(local);

                Quaternion rotTrans(0.f, 0.f, 0.f, 1.f);
                rotTrans.set_rot(m_SUserAccumRotation.x, m_SUserAccumRotation.y, m_SUserAccumRotation.z);

                Matrix localTransMat;
                rotTrans.Normalize();
                rotTrans.ToMatrix(&localTransMat);

                Vector ltmpos = item.second.displace;
                transform_coord(ltmpos, ltmpos, localTransMat);

                Vector tpos;
                idraw->GlobalToLocalTransform(tpos, m_SUserStartControllerPos + ltmpos);

                Matrix ltm = idraw->GetLTM_();

                Quaternion localRotQuat(0.f, 0.f, 0.f, 1.f);
                localRotQuat.set_rot(local.x, local.y, local.z);

                Matrix localRotMat;
                localRotQuat.Normalize();
                localRotQuat.ToMatrix(&localRotMat);

                //Vector t = ltm.t;
                ltm.t.Set(0.f, 0.f, 0.f);
                ltm = ltm * localRotMat;
                ltm.t = tpos;
                idraw->SetLTM_(ltm);

                CActor *actor = const_cast<CActor*>(idraw->GetNode()->key());

                m_pCoreSDK->GetViewportManager()->RebuildTransform(actor);

                actor->BroadcastEvent(Event_OnChangePivot);
            }
        }

		m_SUserStartMousePosition = Vector(input.MousePos.x, input.MousePos.y, 0.f);
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::ControllerScaleLocal(const MouseMoveInputData &input)
    {

    }

	    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    Vector ViewportInterface<TTranformTraits, TTransformHistory>::GetIntersectPosition(const MouseMoveInputData &input, EScrObjectEvent mode, float &out_mult) const
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

        case SOEvent_ControlLockXY:
        {
            out = RayPlaneIntersect(m_SUserStartControllerPos, I._row2, m_ViewPoint, viewDirection);
        }break;

        case SOEvent_ControlLockYZ:
        {
            out = RayPlaneIntersect(m_SUserStartControllerPos, I._row0, m_ViewPoint, viewDirection);
        }break;

        case SOEvent_ControlLockXZ:
        {
            out = RayPlaneIntersect(m_SUserStartControllerPos, I._row1, m_ViewPoint, viewDirection);
        }break;

        default:
            assert(false);
            break;
        }

        out_mult = k;

        return out;
    }

    //----------------------------------------------------------------------------------------------
    template<class TTranformTraits, class TTransformHistory>
    void ViewportInterface<TTranformTraits, TTransformHistory>::UpdateSelectionState(EScrObjectEvent state, const Vector &ctrlPos)
    {
        TVecObjects vec;

        for (TMapSelection::iterator iter = m_SelectedList.begin(); iter != m_SelectedList.end(); ++iter)
        {
            const IDrawInterface *idraw = iter->first;

            assert(idraw);

            Vector &displace = iter->second.displace;
            displace = idraw->GetTransformedWTM_().t - ctrlPos;

            vec.push_back(idraw);
        }

        SaveState(vec);

       /* for each (auto &item in m_SelectedList)
        {
            const IDrawInterface *idraw = item.first;

            assert(idraw);

            Vector &displace = item.second.displace;
            displace = ctrlPos - idraw->GetTransformedWTM_().t;
        }*/
    }
}