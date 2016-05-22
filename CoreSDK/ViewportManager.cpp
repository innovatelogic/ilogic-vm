#include "coresdkafx.h"

namespace core_sdk_api
{
    //----------------------------------------------------------------------------------------------
    CViewportManager::CViewportManager(class CCoreSDK* pSDK)
        : m_pCoreSDK(pSDK)
    {
        m_pInputControllerImpl = new CInputControllerImpl(this);
        m_pInputControllerEdImpl = new CInputControllerEdImpl(this);
    }

    //----------------------------------------------------------------------------------------------
    CViewportManager::~CViewportManager()
    {
        SAFE_DELETE(m_pInputControllerImpl);
        SAFE_DELETE(m_pInputControllerEdImpl);
    }

    //----------------------------------------------------------------------------------------------
    TNodeIDraw* CViewportManager::RegisterObject(const IDrawInterface *pSrc, const CActor *pKey, const CActor *pKeyParent)
    {
        return m_VecDrawList.PushBack(pKey, pSrc, pKeyParent);
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::UnregisterObject(TNodeIDraw *pNode)
    {
        m_VecDrawList.Erase(pNode);
    }

    //----------------------------------------------------------------------------------------------
    TNodeIView* CViewportManager::RegisterViewport(const ViewportInterface *pSrc, const CActor *pKey, const CActor *pKeyParent)
    {
        return m_VecViewports.PushBack(pKey, pSrc, pKeyParent);
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::UnregisterViewport(TNodeIView *pNode)
    {
        m_VecViewports.Erase(pNode);
    }

    //----------------------------------------------------------------------------------------------
    ViewportInterface* CViewportManager::GetViewportInterface(const IDrawInterface *pIObject) const
    {
        ViewportInterface *outViewport = nullptr;

        if (pIObject)
        {
            // find in draw array
            TNodeIDraw *pNodeObject = 0;
            if (TNodeIDraw *pNode = m_VecDrawList.m_pFirstElement)
            {
                do
                {
                    if (pNode->m_pValue == pIObject)
                    {
                        pNodeObject = pNode;
                        break;
                    }
                    pNode = m_VecDrawList.GetNext(pNode);
                } while (pNode);
            }

            assert(pNodeObject);

            // iterate to root viewport
            if (pNodeObject)
            {
                while (pNodeObject->GetRootNode()) {
                    pNodeObject = pNodeObject->GetRootNode();
                }

                // find in view port array
                TNodeMap<CActor, ViewportInterface> *pVNode = m_VecViewports.FindNodeByKey(pNodeObject->m_pKey);

                assert(pVNode);
                
                outViewport = const_cast<ViewportInterface*>(pVNode->m_pValue);
            }
        }
        return outViewport;
    }

    //----------------------------------------------------------------------------------------------
    bool CViewportManager::IsRenderable(const CActor *pKey)
    {
        return (m_VecDrawList.FindNodeByKey(pKey) != 0);
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::Draw()
    {
        CRenderSDK *pRenderSDK = m_pCoreSDK->GetRenderSDK();

        TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement;

        while (pNode)
        {
            pRenderSDK->PushRenderQuevueAdjaency();

            DrawNode(pNode);

            pRenderSDK->PopRenderQuevueAdjaency();

            pNode = pNode->m_pNodeNextPlain;
        }
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::DrawNode(TNodeMap<CActor, IDrawInterface> *pNode)
    {
        if ((pNode->m_pValue)->DoVisibilityTest_())
        {
            const_cast<IDrawInterface*>(pNode->m_pValue)->Draw();

            if (pNode->m_NumChilds > 0)
            {
                TNodeMap<CActor, IDrawInterface> *pChildNode = pNode->m_pNodeNext;
                for (size_t Index = 0; Index < pNode->m_NumChilds; ++Index)
                {
                    DrawNode(pChildNode);
                    pChildNode = pChildNode->m_pNodeNextPlain;
                }
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::ProcessInputMouse(const MouseInputData &InputData, IDrawInterface *pIObjectMask /*= 0*/)
    {
        if (m_pCoreSDK->IsAEditor() && !m_pCoreSDK->IsAGame())
        {
            m_pInputControllerEdImpl->ProcessInputMouse(InputData);
        }
        else {
            m_pInputControllerImpl->ProcessInputMouse(InputData, pIObjectMask);
        }
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::ProcessInputMouse(const MouseMoveInputData &InputData, IDrawInterface *pIObjectMask /*= 0*/)
    {
        if (m_pCoreSDK->IsAEditor() && !m_pCoreSDK->IsAGame())
        {
            m_pInputControllerEdImpl->ProcessInputMouse(InputData);
        }
        else {
            m_pInputControllerImpl->ProcessInputMouse(InputData);
        }
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::ProcessMouseWheel(float ds, SRenderContext *pRenderContext /*= 0*/)
    {
        if (m_pCoreSDK->IsAEditor() && !m_pCoreSDK->IsAGame())
        {
            m_pInputControllerEdImpl->ProcessMouseWheel(ds, pRenderContext);
        }
        else {
            m_pInputControllerImpl->ProcessMouseWheel(ds, pRenderContext);
        }
    }

    //----------------------------------------------------------------------------------------------
    int CViewportManager::SetFocus(IDrawInterface *pIObject, bool bFlag /*= true*/)
    {
        // kill focus
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                if (bool bFocused = pNode->m_pValue->IsFocused())
                {
                    if (pIObject && (bFocused == bFlag && pIObject == pNode->m_pValue)) {
                        return 1; // do not need change
                    }

                    const_cast<IDrawInterface*>(pNode->m_pValue)->SetFocus(false);
                    break;
                }
                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }

        if (pIObject) {
            pIObject->SetFocus(bFlag);
        }
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    int	CViewportManager::SetFocusActor(CActor *pAObject, bool bFlag /*= true*/)
    {
        // kill focus
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                if (bool bFocused = pNode->m_pValue->IsFocused())
                {
                    if (pAObject && (bFocused == bFlag && pAObject == pNode->m_pKey)) {
                        return 1; // do not need change
                    }
                    const_cast<IDrawInterface*>(pNode->m_pValue)->SetFocus(false);
                    break;
                }
                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }

        // set focus
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                if (pAObject == pNode->m_pKey)
                {
                    const_cast<IDrawInterface*>(pNode->m_pValue)->SetFocus(true);
                    break;
                }
                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    IDrawInterface* CViewportManager::GetFocused() const
    {
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                if (pNode->m_pValue->IsFocused())
                {
                    return const_cast<IDrawInterface*>(pNode->m_pValue);
                }
                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    int	CViewportManager::SetMouseOver(IDrawInterface *pIObject, bool bFlag /*= true*/)
    {
        if (pIObject)
        {
            if (bFlag)
            {
                if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
                {
                    do
                    {
                        if (bool bOver = pNode->m_pValue->IsMouseOver())
                        {
                            if (bOver  && pNode->m_pValue == pIObject) {
                                return 1; // do not need change
                            }

                            const_cast<IDrawInterface*>(pNode->m_pValue)->SetMouseOver(false);
                            break;
                        }
                        pNode = m_VecDrawList.GetNext(pNode);
                    } while (pNode);
                }
            }

            pIObject->SetMouseOver(bFlag);
        }
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    IDrawInterface* CViewportManager::GetMouseOver() const
    {
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                if (pNode->m_pValue->IsMouseOver())
                {
                    return const_cast<IDrawInterface*>(pNode->m_pValue);
                }
                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    CActor* CViewportManager::GetFocusedActor() const
    {
        if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                if (pNode->m_pValue->IsFocused())
                {
                    return const_cast<CActor*>(pNode->m_pKey);
                }
                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    IDrawInterface* CViewportManager::GetByActor(const CActor *pAObject) const
    {
        if (pAObject)
        {
            if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
            {
                do
                {
                    if (pNode->m_pKey == pAObject)
                    {
                        return const_cast<IDrawInterface*>(pNode->m_pValue);
                    }
                    pNode = m_VecDrawList.GetNext(pNode);
                } while (pNode);
            }
        }
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::RebildWorldTransform()
    {
        Matrix Indently;

        if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                TNodeMap<CActor, IDrawInterface> *pRoot = pNode->GetRootNode();

                const_cast<IDrawInterface*>(pNode->m_pValue)->DoBuildWorldTransform_(pRoot ? pRoot->m_pValue->GetTransformedWTM_() : Indently);

                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::RebuildSizeTransform()
    {
        Matrix Indently;

        if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                const_cast<IDrawInterface*>(pNode->m_pValue)->DoBuildSizeTransform();

                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::RebuildTransform(CActor *pAObject /*= NULL*/)
    {
        if (pAObject)
        {
            TNodeMap<CActor, IDrawInterface> *pNode = GetNodeByKey(pAObject);

            if (pNode)
            {
                Matrix Indently;
                TNodeMap<CActor, IDrawInterface> *pTmpNode = pNode;

                do
                {
                    const_cast<IDrawInterface*>(pTmpNode->m_pValue)->DoBuildSizeTransform();
                    pTmpNode = m_VecDrawList.GetNext(pTmpNode);
                } while (pTmpNode);

                pTmpNode = pNode;

                TNodeMap<CActor, IDrawInterface> *pRoot = pNode->GetRootNode();
                TNodeMap<CActor, IDrawInterface> *pParentPlainNext = pRoot ? pRoot->GetPlainNext() : 0;
                do
                {
                    pRoot = pTmpNode->GetRootNode();
                    const_cast<IDrawInterface*>(pTmpNode->m_pValue)->DoBuildWorldTransform_(pRoot ? pRoot->m_pValue->GetTransformedWTM_() : Indently);

                    pTmpNode = m_VecDrawList.GetNext(pTmpNode);
                } while (pTmpNode &&  pTmpNode != pParentPlainNext);
            }
        }
        else
        {
            RebildWorldTransform();
            RebuildSizeTransform();

            //RebuildChildBounds();
            //RebuildChildCompoziteBound();
        }
    }

    //----------------------------------------------------------------------------------------------
    TNodeMap<CActor, IDrawInterface>* CViewportManager::GetNodeByKey(CActor *pAObject) const
    {
        TNodeMap<CActor, IDrawInterface>* pOutNode = nullptr;

        bool bFind = false;
        TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement;

        if (pNode)
        {
            do
            {
                if (pNode->m_pKey == pAObject)
                {
                    pOutNode = pNode;
                    break;
                }
                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }

        return pOutNode;
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::SetSelectedImpl(IDrawInterface *pIObject, bool bFlag)
    {

    }
}