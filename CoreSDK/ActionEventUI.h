#ifndef __actioneventui_h__
#define __actioneventui_h__

#ifdef WIN32
#pragma once
#endif

class CORESDK_API ActionEventUI : public ActionBase
{
	DECLARE_CLASS_SIMPLE(ActionEventUI, ActionBase);

public:
	ActionEventUI(const CObjectAbstract * Parent);
	ActionEventUI(const ActionEventUI & Source);
	virtual ~ActionEventUI();

	// @TODO: Redesign
	virtual void	Render(HDC hdc);
};

#endif//__actioneventui_h__