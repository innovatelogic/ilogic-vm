#ifndef __basemodificatorui_h__
#define __basemodificatorui_h__

#ifdef WIN32
#pragma once
#endif

#include "UIScreenObject.h"

class BaseModificatorUI : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(BaseModificatorUI, UIScreenObject);

public:
	BaseModificatorUI(const CObjectAbstract * Parent);
	BaseModificatorUI(const BaseModificatorUI& Source);
	virtual ~BaseModificatorUI();

	virtual bool Apply(const CActor * Sender) { return false; }
	virtual void DrawBounds() const {}

protected:
private:
};

#endif//__basemodificatorui_h__