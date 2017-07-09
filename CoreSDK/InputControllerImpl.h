#pragma once

#include "../Foundation/StdafxFoundation.h"

namespace core_sdk_api
{
    class CViewportManager;

    class CInputControllerImpl
    {
    public:
        CInputControllerImpl(CViewportManager *pInterface);
        virtual ~CInputControllerImpl();

        bool ProcessInputMouse(const MouseInputData &InputData, IDrawInterface *pIObjectMask = 0);
        void ProcessInputMouse(const MouseMoveInputData &InputData, IDrawInterface *pIObjectMask = 0);
        void ProcessMouseWheel(float ds, oes::d3d::SRenderContext *pRenderContext = 0);

    protected:
        bool ProcessPress(const MouseInputData &InputData, IDrawInterface *pIObjectMask = 0);
        bool ProcessPressNode(const MouseInputData &InputData, TNodeMap<CActor, IDrawInterface> *pNode);

        bool ProcessInputMouseNode(const MouseMoveInputData &InputData, TNodeMap<CActor, IDrawInterface> *pNode);

        bool ProcessRelease(const MouseInputData &InputData);

    private:
        core_sdk_api::CViewportManager *m_pInterface;
    };
}