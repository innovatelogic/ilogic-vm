#ifndef __uiscenelogin_h__
#define __uiscenelogin_h__

#ifdef WIN32
#pragma once
#endif

#include "UIScene.h"
#include "IUIInputController.h"

/** 
 * Game specific code.
 */

class CUISceneLogin : public UIScene
{
	DECLARE_CLASS_SIMPLE(CUISceneLogin, UIScene);

public:
	CUISceneLogin(const CObjectAbstract *pParent);
	virtual ~CUISceneLogin();

	virtual void		Initialize();

	void	OnButtonPlayPressed(const CActor *pSender, ESystemEventID EventId);
	void	OnButtonQuitPressed(const CActor *pSender, ESystemEventID EventId);

protected:
private:
	class LabelButton * ButtonPlay;
	class LabelButton * ButtonOptions;
	class LabelButton * ButtonCredits;
	class LabelButton * ButtonQuit;
};

#endif//__uiscenelogin_h__