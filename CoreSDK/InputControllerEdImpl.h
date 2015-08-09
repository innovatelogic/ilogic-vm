#ifndef __inputcontrolleredimpl_h__
#define __inputcontrolleredimpl_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"

class CInputControllerEdImpl
{
public:
	CInputControllerEdImpl(class CViewportManager *pInterface);
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
	class CViewportManager *m_pInterface;
};

#endif//__inputcontrolleredimpl_h__