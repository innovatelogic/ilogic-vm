#pragma once

#include "../Foundation/StdafxFoundation.h"

namespace core_sdk_api
{
    class CViewportManager;

    class CInputControllerEdImpl
    {
    public:
        CInputControllerEdImpl(CViewportManager *pInterface);
        virtual ~CInputControllerEdImpl();

        void ProcessInputMouse(const MouseInputData &InputData);
        void ProcessInputMouse2(const MouseInputData &input, const TIViewport *viewport);
        void ProcessInputMouse(const MouseMoveInputData &InputData);
        void ProcessMouseWheel(float ds, SRenderContext *pRenderContext = 0);

    protected:

        /** Process controller move input */
        bool ProcessMoveControllers(const MouseMoveInputData &InputData);

        /** Process move */
        bool ProcessMove(const MouseMoveInputData &InputData);

        /** Process controller input */
        bool ProcessControllers(const MouseInputData &InputData);
        bool ProcessControllers2(const MouseInputData &input, const TIViewport *viewport);

        bool ProcessPress(const MouseInputData &InputData);

        bool ProcessRelease(const MouseInputData &InputData);

    private:
        core_sdk_api::CViewportManager *m_pInterface;
    };
}