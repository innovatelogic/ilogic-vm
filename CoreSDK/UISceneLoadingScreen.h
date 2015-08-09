#ifndef __uisceneloadingscreen_h__
#define __uisceneloadingscreen_h__

#ifdef WIN32
#pragma once
#endif

/** 
 * Game specific code.
 */

class UISceneLoadingScreen : public UIScene
{
	DECLARE_CLASS_SIMPLE(UISceneLoadingScreen, UIScene);

public:
	UISceneLoadingScreen(const CObjectAbstract *pParent);
	virtual ~UISceneLoadingScreen();

	virtual void		Initialize();
	virtual void		Update(float DeltaTime);

	virtual void		OnButtonStartPlay(const CActor * Sender, ESystemEventID EventId);

protected:
private:
	void StartPlay();

private:
	class LabelButton	*m_pButtonPlay;
	class UIProgressBar *m_pProgressLoading;
	class Label			*m_pLabelMessage;
};

#endif//__uisceneloadingscreen_h__