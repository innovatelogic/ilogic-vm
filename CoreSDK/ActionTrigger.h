#ifndef __actiontrigger_h__
#define __actiontrigger_h__

#ifdef WIN32
#pragma once
#endif

#include "ActionBase.h"

class CORESDK_API ActionTrigger : public ActionBase
{
	 DECLARE_CLASS_SIMPLE(ActionTrigger, ActionBase);

public:
	ActionTrigger(const CObjectAbstract * Parent);
	ActionTrigger(const ActionTrigger & Source);
	virtual ~ActionTrigger();

	// @TODO: Redesign
	virtual void	RenderFuncZone(HDC hdc);

protected:
private:
};

#endif//__actiontrigger_h__