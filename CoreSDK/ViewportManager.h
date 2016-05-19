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

    public:
        CViewportManager(class CCoreSDK *pSDK);
        virtual ~CViewportManager();

        inline_ class CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }

        TNodeIDraw* RegisterObject(const IDrawInterface *pSrc, const CActor *pKey, const CActor *pKeyParent);
        void UnregisterObject(TNodeIDraw *pNode);

        TNodeIView* RegisterViewport(const ViewportInterface *pSrc, const CActor *pKey, const CActor *pKeyParent);
        void UnregisterViewport(TNodeIView *pNode);

        ViewportInterface* GetViewportInterface(const IDrawInterface *pIObject) const;

        bool IsRenderable(const CActor *key);

        void Draw();
        void DrawNode(TNodeIDraw *pNode);

        // controller input
        void ProcessInputMouse(const MouseInputData &InputData, IDrawInterface *pIObjectMask = 0);
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

        void			RebuildTransform(CActor *pAObject = NULL);

    protected:
        TNodeIDraw*     GetNodeByKey(CActor *pAObject) const;

    private:
        TTreeMapViewport m_VecViewports;
        TTreeMapDraw	 m_VecDrawList;

        class CInputControllerImpl	 *m_pInputControllerImpl;
        class CInputControllerEdImpl *m_pInputControllerEdImpl;

        class CCoreSDK* m_pCoreSDK;
    };
}