#pragma once

#include "../Foundation/StdafxFoundation.h"
#include "ViewportInterface.h"
#include "IDrawInterface.h"
#include "game_types.h"

namespace core_sdk_api
{
    using TTreeMapViewport = TTreeMap<CActor, ViewportInterface>;
    using TTreeMapDraw = TTreeMap<CActor, IDrawInterface>;
    using TNodeIDraw = TNodeMap<CActor, IDrawInterface>;
    using TNodeIView = TNodeMap<CActor, ViewportInterface>;
    
    class CORESDK_API CViewportManager
    {
        friend class CInputControllerImpl;
        friend class CInputControllerEdImpl;

        CViewportManager(const CViewportManager&) = delete;
        CViewportManager& operator=(const CViewportManager&) = delete;

    public:
        CViewportManager(class CCoreSDK *pSDK);
        virtual ~CViewportManager();

        inline_ class CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }

        TNodeIDraw* RegisterObject(const IDrawInterface *pSrc, const CActor *pKey, const CActor *pKeyParent);
        void UnregisterObject(TNodeIDraw *pNode);

        TNodeIView* RegisterViewport(const ViewportInterface *pSrc, const CActor *pKey, const CActor *pKeyParent);
        void UnregisterViewport(TNodeIView *pNode);

        ViewportInterface* GetVeiwportInterface(const CActor *key);
        ViewportInterface* GetViewportInterface(const IDrawInterface *pIObject) const;
        
        bool IsRenderable(const CActor *key);

        void Draw();
        void DrawNode(TNodeIDraw *pNode);

        // controller input
        void ProcessInputMouse(const MouseInputData &InputData, IDrawInterface *pIObjectMask = 0);
        void ProcessInputMouse_2(const MouseInputData &InputData, const ViewportInterface *viewport);

        void ProcessInputMouse(const MouseMoveInputData &InputData, IDrawInterface *pIObjectMask = 0);
        void ProcessMouseWheel(float ds, class SRenderContext *pRenderContext = 0);

        int				SetFocus(IDrawInterface *pIObject, bool bFlag = true);
        int				SetFocusActor(CActor *pAObject, bool bFlag = true);

        IDrawInterface* GetFocused() const;
        CActor*			GetFocusedActor() const;

        int				SetMouseOver(IDrawInterface *pIObject, bool bFlag = true);
        IDrawInterface* GetMouseOver() const;

        IDrawInterface* GetByActor(const CActor *pAObject) const;

        void			RebildWorldTransform();
        void			RebuildSizeTransform();

        void			RebuildTransform(CActor *pAObject = nullptr);

        /*!
        * set selection objects
        * [paths] array of actors full paths
        */
        void            SetSelect(const std::vector<std::string> &paths, ViewportInterface *viewport);

    protected:
        TNodeIDraw*     GetNodeByKey(CActor *pAObject) const;

    private:
        void			SetSelectedImpl(IDrawInterface *pIObject, bool bFlag);

        void            DrawController() const;

    private:
        TTreeMapViewport m_VecViewports;
        TTreeMapDraw	 m_VecDrawList;

        class CInputControllerImpl	 *m_pInputControllerImpl;
        class CInputControllerEdImpl *m_pInputControllerEdImpl;

        class CCoreSDK* m_pCoreSDK;
    };
}