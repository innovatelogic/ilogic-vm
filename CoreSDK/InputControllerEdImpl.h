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
        void ProcessInputMouse(const MouseMoveInputData &InputData);
        void ProcessMouseWheel(float ds, SRenderContext *pRenderContext = 0);

    protected:

        /** Process controller move input */
        bool ProcessMoveControllers(const MouseMoveInputData &InputData);

        /** Process move */
        bool ProcessMove(const MouseMoveInputData &InputData);

        /** Process controller input */
        bool ProcessControllers(const MouseInputData &InputData);

        bool ProcessPress(const MouseInputData &InputData);

        bool ProcessRelease(const MouseInputData &InputData);

    private:
        core_sdk_api::CViewportManager *m_pInterface;
    };
}