#ifndef __inputcontrollerimpl_h__
#define __inputcontrollerimpl_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"

class CInputControllerImpl
{
public:
	CInputControllerImpl(class CViewportManager *pInterface);
	virtual ~CInputControllerImpl();

	bool ProcessInputMouse(const MouseInputData &InputData, IDrawInterface *pIObjectMask = 0);
	void ProcessInputMouse(const MouseMoveInputData &InputData, IDrawInterface *pIObjectMask = 0);
	void ProcessMouseWheel(float ds, class SRenderContext *pRenderContext = 0);

protected:
	bool ProcessPress(const MouseInputData &InputData, IDrawInterface *pIObjectMask = 0);
	bool ProcessPressNode(const MouseInputData &InputData, TNodeMap<CActor, IDrawInterface> *pNode);

	bool ProcessInputMouseNode(const MouseMoveInputData &InputData, TNodeMap<CActor, IDrawInterface> *pNode);

	bool ProcessRelease(const MouseInputData &InputData);

private:
	class CViewportManager *m_pInterface;
};

#endif//__inputcontrollerimpl_h__