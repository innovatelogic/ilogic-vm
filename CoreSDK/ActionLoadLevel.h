#ifndef __actionloadlevel_h__
#define __actionloadlevel_h__

#ifdef WIN32
#pragma once
#endif

#include "ActionBase.h"

class ActionLoadLevel : public ActionBase
{
	DECLARE_CLASS_SIMPLE(ActionLoadLevel, ActionBase);

public:
	ActionLoadLevel(const CObjectAbstract * Parent);
	ActionLoadLevel(const ActionLoadLevel & Source);
	virtual ~ActionLoadLevel();

	virtual void	Render(HDC hdc, const Matrix &ViewMatrix);

	void			SetResourceTag(std::string &URL) { LevelURL = URL; }
	std::string		GetResourceTag() const { return LevelURL; }

	virtual void	DoExecute(class CActor * EventActor);

private:
	std::string		LevelURL;

	SocketAction	*SocketIn;
	SocketAction	*SocketOut;
};

#endif//__actionloadlevel_h__