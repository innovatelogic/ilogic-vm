#include "coresdkafx.h"

namespace core_sdk_api
{
    //----------------------------------------------------------------------------------------------
    CInputControllerImpl::CInputControllerImpl(core_sdk_api::CViewportManager *pInterface)
        : m_pInterface(pInterface)
    {

    }

    //----------------------------------------------------------------------------------------------
    CInputControllerImpl::~CInputControllerImpl()
    {
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerImpl::ProcessInputMouse(const MouseInputData &InputData, IDrawInterface *pIObjectMask /*= 0*/)
    {
        bool bResult = false;

       /* CCoreSDK *pCoreSDK = m_pInterface->GetCoreSDK();

        switch (InputData.event)
        {
        case MOUSE_Pressed:
        case MOUSE_DoubleClick:
        {
            bResult = ProcessPress(InputData, pIObjectMask);

            if (!bResult)
            {
                if (IDrawInterface * pIFocused = m_pInterface->GetFocused())  // nothing pressed reset all stated for selected object
                {
                    pIFocused->SetFocus(false);
                    bResult = true; // treat this as user input
                }
            }
        }break;

        case MOUSE_Released:
        {
            bResult = ProcessRelease(InputData);
        }break;
        };
        */
        return bResult;
    }

    //----------------------------------------------------------------------------------------------
    void CInputControllerImpl::ProcessInputMouse(const MouseMoveInputData &InputData, IDrawInterface *pIObjectMask /*= 0*/)
    {
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_pInterface->m_VecDrawList.m_pFirstElement)
        {
            while (pNode)
            {
                if (!pIObjectMask || pIObjectMask == pNode->m_pValue)
                {
                    if (ProcessInputMouseNode(InputData, pNode)) {
                        return;
                    }
                }
                pNode = pNode->m_pNodeNextPlain;
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    void CInputControllerImpl::ProcessMouseWheel(float ds, oes::d3d::SRenderContext *pRenderContext /*= 0*/)
    {

    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerImpl::ProcessInputMouseNode(const MouseMoveInputData &InputData, TNodeMap<CActor, IDrawInterface> *pNode)
    {
       /* if (pNode->m_NumChilds > 0)
        {
            TNodeMap<CActor, IDrawInterface> *pChildNode = pNode->m_pNodeNext;
            for (size_t Index = 0; Index < pNode->m_NumChilds; ++Index)
            {
                if (ProcessInputMouseNode(InputData, pChildNode)) {
                    return true;
                }
                pChildNode = pChildNode->m_pNodeNextPlain;
            }
        }
        return const_cast<IDrawInterface*>(pNode->m_pValue)->ProcessMouseMove(InputData);*/

        return false;
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerImpl::ProcessPress(const MouseInputData &InputData, IDrawInterface *pIObjectMask /*= 0*/)
    {
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_pInterface->m_VecDrawList.m_pFirstElement)
        {
            while (pNode)
            {
                if (!pIObjectMask || pIObjectMask == pNode->m_pValue)
                {
                    if (ProcessPressNode(InputData, pNode)) {
                        return true;
                    }
                }
                pNode = pNode->m_pNodeNextPlain;
            }
        }
        return false;
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerImpl::ProcessPressNode(const MouseInputData &InputData, TNodeMap<CActor, IDrawInterface> *pNode)
    {
        /*if (pNode->m_NumChilds > 0)
        {
            TNodeMap<CActor, IDrawInterface> *pChildNode = pNode->m_pNodeNext;
            for (size_t Index = 0; Index < pNode->m_NumChilds; ++Index)
            {
                if (ProcessPressNode(InputData, pChildNode)) {
                    return true;
                }
                pChildNode = pChildNode->m_pNodeNextPlain;
            }
        }
        return const_cast<IDrawInterface*>(pNode->m_pValue)->ProcessPress(InputData);*/
        return false;
    }

    //----------------------------------------------------------------------------------------------
    bool CInputControllerImpl::ProcessRelease(const MouseInputData &InputData)
    {
        /*TNodeMap<CActor, IDrawInterface> *pNode = m_pInterface->m_VecDrawList.m_pFirstElement;

        if (pNode)
        {
            do
            {
                if (const_cast<IDrawInterface*>(pNode->m_pValue)->ProcessRelease(InputData)) {
                    return true;
                }
                pNode = m_pInterface->m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }*/
        return false;
    }
}