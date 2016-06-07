#include "coresdkafx.h"

namespace core_sdk_api
{

    //----------------------------------------------------------------------------------------------
    CInputControllerEdImpl::CInputControllerEdImpl(core_sdk_api::CViewportManager *pInterface)
        : m_pInterface(pInterface)
    {

    }

    //----------------------------------------------------------------------------------------------
    CInputControllerEdImpl::~CInputControllerEdImpl()
    {
    }

    //----------------------------------------------------------------------------------------------
    void CInputControllerEdImpl::ProcessInputMouse(const MouseInputData &InputData)
    {
        MouseInputData TInputData = InputData;

        CCoreSDK *pCoreSDK = m_pInterface->GetCoreSDK();

        // TODO Revise it! 
        IDrawInterface *pFocused = pCoreSDK->GetViewportManager()->GetFocused();

        if (pFocused)
        {
            core_sdk_api::ViewportInterface *pInterface = pCoreSDK->GetViewportManager()->GetViewportInterface(pFocused);

            if (pInterface && pInterface == pCoreSDK->GetExplorerInstance()->GetExplorer2D())
            {
                Matrix IViewMatrix;
                const Matrix &ViewMatrix = pInterface->GetViewportViewMatrix();

                TInputData.MousePos = transform_coord((InputData.MousePos - ViewMatrix.t2), invert(IViewMatrix, ViewMatrix));
            }
        }

        switch (InputData.Code)
        {
        case MOUSE_LEFT:
        {
            if (InputData.event == MOUSE_Pressed)
            {
                if (!ProcessControllers(TInputData))
                {
                    CActor *pFocused = pCoreSDK->GetViewportManager()->GetFocusedActor(); // nothing pressed reset all stated for selected object
                    if (pFocused)
                    {
                        pFocused->SetControlState(ActorState_None); // set state iterative down
                        m_pInterface->SetFocusActor(pFocused, false);
                    }
                }
            }
            else if (InputData.event == MOUSE_Released)
            {
                ProcessRelease(InputData); // button released
            }
        }
        break;

        case MOUSE_RIGHT:
            //OnEventPressed(InputData); // button released
            break;

        case MOUSE_MIDDLE:
        {
            ProcessPress(InputData);
        }break;
        };
    }

    //----------------------------------------------------------------------------------------------
    void CInputControllerEdImpl::ProcessInputMouse2(const MouseInputData &InputData, const ViewportInterface *viewport)
    {
    }

    //----------------------------------------------------------------------------------------------
    void CInputControllerEdImpl::ProcessInputMouse(const MouseMoveInputData &InputData)
    {
        if (!ProcessMoveControllers(InputData))
        {
            if (!ProcessMove(InputData))
            {
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    void CInputControllerEdImpl::ProcessMouseWheel(float ds, SRenderContext *pRenderContext /*= 0*/)
    {
        bool bProcessed = false;

/*        TNodeMap<CActor, IDrawInterface> *pNode = m_pInterface->m_VecDrawList.m_pFirstElement;

        if (pNode)
        {
            do
            {
                if (pNode->m_pValue->IsFocused())
                {
                    bProcessed = const_cast<IDrawInterface*>(pNode->m_pValue)->ProcessMouseWheel(ds);

                    if (bProcessed) {
                        break;
                    }
                }
                pNode = m_pInterface->m_VecDrawList.GetNext(pNode);

            } while (pNode);
        }

        if (!bProcessed)
        {
            CameraManager *pMgr = m_pInterface->GetCoreSDK()->GetCameraManager();
            assert(pMgr);

            CCamera *pCam = pMgr->GetActiveCamera(pRenderContext);
            assert(pCam); // means at least one active camera should persist

            pCam->OnMouseWheel(ds);
        }*/
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerEdImpl::ProcessControllers(const MouseInputData &InputData)
    {
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_pInterface->m_VecDrawList.m_pFirstElement)
        {
            do
            {
                if (const_cast<IDrawInterface*>(pNode->m_pValue)->ProcessController(InputData))
                {
                    const_cast<CActor*>(pNode->m_pKey)->SetControlState(ActorState_Locked, false);	// set mouse input lock
                    return true;
                }
                pNode = m_pInterface->m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }
        return false;
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerEdImpl::ProcessControllers2(const MouseInputData &input, const ViewportInterface *viewport)
    {
        return false;
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerEdImpl::ProcessMoveControllers(const MouseMoveInputData &InputData)
    {
        MouseMoveInputData TInputData = InputData;

        CCoreSDK *pCoreSDK = m_pInterface->GetCoreSDK();

        // TODO Revise it! 
        IDrawInterface *pFocused = pCoreSDK->GetViewportManager()->GetFocused();

        if (pFocused)
        {
            core_sdk_api::ViewportInterface *pInterface = pCoreSDK->GetViewportManager()->GetViewportInterface(pFocused);

            if (pInterface && pInterface == pCoreSDK->GetExplorerInstance()->GetExplorer2D())
            {
                Matrix IViewMatrix;
                const Matrix &ViewMatrix = pInterface->GetViewportViewMatrix();

                TInputData.MousePos = transform_coord((InputData.MousePos - ViewMatrix.t2), invert(IViewMatrix, ViewMatrix));
            }
        }

        TNodeMap<CActor, IDrawInterface> *pNode = m_pInterface->m_VecDrawList.m_pFirstElement;

        if (pNode)
        {
            do
            {
                if (const_cast<IDrawInterface*>(pNode->m_pValue)->ProcessController(TInputData)) {
                    return true;
                }
                pNode = m_pInterface->m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }

        return false;
    }
    //----------------------------------------------------------------------------------------------
    bool CInputControllerEdImpl::ProcessMove(const MouseMoveInputData &InputData)
    {
        bool bProcessed = false;

        MouseMoveInputData InputDataMod = InputData;
        InputDataMod.bMiddleButtonPressed = IDrawInterface::GetMBPressed();

        TNodeMap<CActor, IDrawInterface> *pNode = m_pInterface->m_VecDrawList.m_pFirstElement;

        if (pNode)
        {
            do
            {
                if (pNode->m_pValue->IsFocused())
                {
                    bProcessed = const_cast<IDrawInterface*>(pNode->m_pValue)->ProcessMouseMove(InputDataMod);

                    if (bProcessed) {
                        break;
                    }
                }
                pNode = m_pInterface->m_VecDrawList.GetNext(pNode);

            } while (pNode);
        }

        if (!bProcessed)
        {
            CameraManager *pMgr = m_pInterface->GetCoreSDK()->GetCameraManager();
            assert(pMgr);

            CCamera *pCam = pMgr->GetActiveCamera(InputDataMod.pRenderContext);
            assert(pCam); // means at least one active camera should persist

            pCam->OnMouseMove(InputDataMod);
        }

        return true;
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerEdImpl::ProcessPress(const MouseInputData &InputData)
    {
        switch (InputData.Code)
        {
        case MOUSE_MIDDLE:
        {
            IDrawInterface::m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
            IDrawInterface::m_bSMiddleButtonPressed = (InputData.event == MOUSE_Pressed);
            return true;
        }break;
        };
        return false;
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerEdImpl::ProcessRelease(const MouseInputData &InputData)
    {
        TNodeMap<CActor, IDrawInterface> *pNode = m_pInterface->m_VecDrawList.m_pFirstElement;

        if (pNode)
        {
            do
            {
                if (const_cast<IDrawInterface*>(pNode->m_pValue)->ProcessControllerRelease(InputData)) {
                    return true;
                }
                pNode = m_pInterface->m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }
        return false;
    }
}