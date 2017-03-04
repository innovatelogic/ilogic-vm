#ifndef __actionhandler_h__
#define __actionhandler_h__

#ifdef WIN32
#pragma once
#endif

#include "ActionBase.h"

class CORESDK_API ActionHandler : public ActionBase
{
	DECLARE_CLASS_SIMPLE(ActionHandler, ActionBase);

public:
	ActionHandler(const CObjectAbstract * Parent);
	ActionHandler(const ActionHandler & Source);
	~ActionHandler();

	// @TODO: Redesign
	virtual void	Render(HDC hdc, const Matrix &ViewMatrix);

protected:
private:
	SocketAction	*SocketOut;
};

#endif//__actionhandler_h__