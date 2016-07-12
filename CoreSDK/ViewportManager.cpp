#include "coresdkafx.h"

namespace core_sdk_api
{
    //----------------------------------------------------------------------------------------------
    CViewportManager::CViewportManager(class CCoreSDK* pSDK)
        : m_pCoreSDK(pSDK)
    {
    }

    //----------------------------------------------------------------------------------------------
    CViewportManager::~CViewportManager()
    {
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
    TNodeIView* CViewportManager::RegisterViewport(const TIViewport *pSrc, const CActor *pKey, const CActor *pKeyParent)
    {
        return m_VecViewports.PushBack(pKey, pSrc, pKeyParent);
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::UnregisterViewport(TNodeIView *pNode)
    {
        m_VecViewports.Erase(pNode);
    }

    //----------------------------------------------------------------------------------------------
    TIViewport* CViewportManager::GetVeiwportInterface(const CActor *key)
    {
        TIViewport *out = nullptr;

        if (TNodeIView *node = m_VecViewports.begin())
        {
            do
            {
                if (node->key() == key)
                {
                    out = const_cast<TIViewport*>(node->m_pValue);
                }
                node = node->GetNext();
            } while (node);
        }
        return out;
    }

    //----------------------------------------------------------------------------------------------
    TIViewport* CViewportManager::GetViewportInterface(const IDrawInterface *pIObject) const
    {
        TIViewport *outViewport = nullptr;

        if (pIObject)
        {
            // find in draw array
            TNodeIDraw *pNodeObject = nullptr;
            if (TNodeIDraw *pNode = m_VecDrawList.begin())
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
                TNodeMap<CActor, TIViewport> *pVNode = m_VecViewports.FindNodeByKey(pNodeObject->m_pKey);

                assert(pVNode);
                
                outViewport = const_cast<TIViewport*>(pVNode->m_pValue);
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
    void CViewportManager::InputMouse(const MouseInputData &input, const TIViewport *viewport /*=nullptr*/)
    {
        // TODO: get default viewport if nullptr
        assert(viewport);

        TIViewport *ivprt = const_cast<TIViewport*>(viewport);

        switch (input.Code)
        {
        case MOUSE_LEFT:
        {
            if (input.event == MOUSE_Pressed)
            {
                if (ivprt->ProcessController(input))
                {
                    ivprt->SetControllerState(ActorState_Locked);
                }
                else
                {
                    ivprt->ProcessMouseInput(input);
                }
            }
            else if (input.event == MOUSE_Released)
            {
                ivprt->ControllerRelease();
            }
        }break;

        case MOUSE_RIGHT:
            //OnEventPressed(InputData); // button released
            break;

        case MOUSE_MIDDLE:
        {
            ivprt->ProcessMouseInput(input);
        }break;

        default:
            assert(false);
            break;
        }
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::InputMouse(const MouseMoveInputData &input, const TIViewport *viewport /*= nullptr*/)
    {
        // TODO: get default viewport if nullptr
        assert(viewport);

        TIViewport *ivprt = const_cast<TIViewport*>(viewport);

        if (!ivprt->ProcessController(input))
        {
            CameraManager *pMgr = m_pCoreSDK->GetCameraManager();
            assert(pMgr);

            CCamera *pCam = pMgr->GetActiveCamera(input.pRenderContext);
            assert(pCam); // means at least one active camera should persist

            MouseMoveInputData inputMod = input;
            inputMod.bMiddleButtonPressed = ivprt->GetMiddleButtonPressed();

            pCam->OnMouseMove(inputMod);
        }
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::InputMouseWheel(float ds, int x, int y, const TIViewport *viewport /*= nullptr*/)
    {
        // TODO: get default viewport if nullptr
        assert(viewport);

        TIViewport *ivprt = const_cast<TIViewport*>(viewport);

        //ivprt->Mo
    }

    //----------------------------------------------------------------------------------------------
    IDrawInterface* CViewportManager::GetFocused() const
    {
        /*if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.m_pFirstElement)
        {
            do
            {
                if (pNode->m_pValue->IsFocused())
                {
                    return const_cast<IDrawInterface*>(pNode->m_pValue);
                }
                pNode = m_VecDrawList.GetNext(pNode);
            } while (pNode);
        }*/
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    int	CViewportManager::SetMouseOver(IDrawInterface *pIObject, bool bFlag /*= true*/)
    {
       /* if (pIObject)
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
        }*/
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    IDrawInterface* CViewportManager::GetByActor(const CActor *pAObject) const
    {
        if (pAObject)
        {
            if (TNodeMap<CActor, IDrawInterface> *pNode = m_VecDrawList.begin())
            {
                do
                {
                    if (pNode->m_pKey == pAObject)
                    {
                        return const_cast<IDrawInterface*>(pNode->m_pValue);
                    }
                    pNode = m_VecDrawList.next(pNode);
                } while (pNode);
            }
        }
        return nullptr;
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::RebildWorldTransform()
    {
        Matrix Indently;

        if (TNodeIDraw *pNode = m_VecDrawList.begin())
        {
            do
            {
                TNodeIDraw *pRoot = pNode->parent();

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
    void CViewportManager::RebuildTransform(CActor *pAObject /*= nullptr*/)
    {
        if (pAObject)
        {
            TNodeIDraw *pNode = GetNodeByKey(pAObject);
            if (pNode)
            {
                Matrix Indently;
                TNodeIDraw *pTmpNode = pNode;

                do
                {
                    const_cast<IDrawInterface*>(pTmpNode->m_pValue)->DoBuildSizeTransform();
                    pTmpNode = m_VecDrawList.next(pTmpNode);
                } while (pTmpNode);

                pTmpNode = pNode;

                TNodeIDraw *pRoot = pNode->parent();

                TNodeIDraw *pParentPlainNext = pRoot ? pRoot->GetPlainNext() : nullptr; // stop condition

                do
                {
                    TNodeIDraw *parent = pTmpNode->parent();
         
                    const_cast<IDrawInterface*>(pTmpNode->m_pValue)->DoBuildWorldTransform_(parent ? parent->m_pValue->GetTransformedWTM_() : Indently);

                    if (pTmpNode->GetNumChilds() == 0) // rebuild composite box upwards
                    {
                        TNodeIDraw *up = pTmpNode->parent();

                        Bounds3f cmpBox = pTmpNode->m_pValue->GetCompositeBounds_();

                        if (cmpBox.IsValid())
                        {
                            while (up)
                            {
                                Bounds3f upBox = up->m_pValue->GetCompositeBounds_();

                                if (cmpBox.IsValid())
                                {
                                    upBox += cmpBox;
                                }
                                else
                                {
                                    upBox = cmpBox;
                                }

                                const_cast<IDrawInterface*>(up->m_pValue)->SetCompositeBounds_(upBox);

                                up = up->parent();
                            }
                        }
                    }

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
    void CViewportManager::SetSelect(const std::vector<std::string> &paths, TIViewport *viewport)
    {
        assert(viewport);

        std::vector<IDrawInterface*> objects;

        for each (auto &item in paths)
        {
            if (CActor *actor = CActor::GetActorByFullPath(item, m_pCoreSDK->GetRootActor()))
            {
                if (IDrawInterface *pIDrawable = GetByActor(actor))
                {
                    objects.push_back(pIDrawable);
                }
            }
        }

        viewport->SetSelection(objects);
    }

    //----------------------------------------------------------------------------------------------
    void CViewportManager::ApplyStateCast(const TMapState &state)
    {
        for each (auto &item in state)
        {
            TNodeIDraw *pNode = m_VecDrawList.begin();

            if (pNode)
            {
                do
                {
                    if (pNode->key()->GetUID() == item.first)
                    {
                        pNode->value()->SetLTM_(item.second.LTM);
                        pNode->value()->SetSTM_(item.second.STM);
                        //pNode->value()->SetRotator(item.second.YPR);

                        CActor *actor = const_cast<CActor*>(pNode->key());

                        RebuildTransform(actor);

                        actor->BroadcastEvent(Event_OnChangePivot);
                        
                        break;
                    }
                    pNode = m_VecDrawList.next(pNode);
                } while (pNode);
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    TNodeIDraw* CViewportManager::GetNodeByKey(CActor *pAObject) const
    {
        TNodeIDraw *pOutNode = nullptr;

        bool bFind = false;
        TNodeIDraw *pNode = m_VecDrawList.begin();

        if (pNode)
        {
            do
            {
                if (pNode->m_pKey == pAObject)
                {
                    pOutNode = pNode;
                    break;
                }
                pNode = m_VecDrawList.next(pNode);
            } while (pNode);
        }

        return pOutNode;
    }
    
    //----------------------------------------------------------------------------------------------
    void CViewportManager::SetSelectedImpl(IDrawInterface *pIObject, bool bFlag)
    {
        TIViewport *viewport = GetViewportInterface(pIObject);

        if (viewport)
        {
            if (bFlag) {
                viewport->SetSelect(pIObject);
            }
            else {
                viewport->DropSelect(pIObject);
            }
        }
    }
}