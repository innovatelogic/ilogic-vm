#pragma once

#include "../Foundation/StdafxFoundation.h"
#include "ViewportInterface.h"
#include "IDrawInterface.h"
#include "game_types.h"

class CCoreSDK;

namespace core_sdk_api
{
    using TTreeMapViewport = TTreeMap<CActor, TIViewport>;
    using TTreeMapDraw = TTreeMap<CActor, IDrawInterface>;
    using TNodeIDraw = TNodeMap<CActor, IDrawInterface>;
    using TNodeIView = TNodeMap<CActor, TIViewport>;
    
    class CORESDK_API CViewportManager
    {
        friend class CInputControllerImpl;
        friend class CInputControllerEdImpl;

        CViewportManager(const CViewportManager&) = delete;
        CViewportManager& operator=(const CViewportManager&) = delete;

    public:
        explicit CViewportManager(CCoreSDK *pSDK);
        virtual ~CViewportManager();

        inline_ CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }

        TNodeIDraw* RegisterObject(const IDrawInterface *pSrc, const CActor *pKey, const CActor *pKeyParent);
        void        UnregisterObject(TNodeIDraw *pNode);

        TNodeIView* RegisterViewport(const TIViewport *pSrc, const CActor *pKey, const CActor *pKeyParent);
        void        UnregisterViewport(TNodeIView *pNode);

        TIViewport* GetVeiwportInterface(const CActor *key);
        TIViewport* GetViewportInterface(const IDrawInterface *pIObject) const;
        
        bool IsRenderable(const CActor *key);

        void Draw();
        void DrawNode(TNodeIDraw *pNode);

        void InputMouse(const MouseInputData &input, const TIViewport *viewport = nullptr);
        void InputMouse(const MouseMoveInputData &input, const TIViewport *viewport = nullptr);
        void InputMouseWheel(float ds, int x, int y, const TIViewport *viewport = nullptr);

        IDrawInterface* GetFocused() const;
        CActor*			GetFocusedActor() const;

        int				SetMouseOver(IDrawInterface *pIObject, bool bFlag = true);
        IDrawInterface* GetMouseOver() const;

        IDrawInterface* GetByActor(const CActor *pAObject) const;

        void	RebildWorldTransform();
        void	RebuildSizeTransform();

        void	RebuildTransform(CActor *pAObject = nullptr);
        
        /*!
        * set selection objects
        * [paths] array of actors full paths
        */
        void    SetSelect(const std::vector<std::string> &paths, TIViewport *viewport);

        /**
        * apply cast
        */
        void    ApplyStateCast(const TMapState &state);

    protected:
        TNodeIDraw* GetNodeByKey(CActor *pAObject) const;
        
    private:
        void	SetSelectedImpl(IDrawInterface *pIObject, bool bFlag);

        void    DrawController() const;

    private:
        TTreeMapViewport m_VecViewports;
        TTreeMapDraw	 m_VecDrawList;

        CCoreSDK* m_pCoreSDK;
    };
}