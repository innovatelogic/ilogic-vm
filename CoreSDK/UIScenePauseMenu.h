#ifndef __uiscenepausemenu_h__
#define __uiscenepausemenu_h__

#ifdef WIN32
#pragma once
#endif

#include "UIScene.h"

class CORESDK_API UIScenePauseMenu : public UIScene
{
	DECLARE_CLASS_SIMPLE(UIScenePauseMenu, UIScene);

public:
	UIScenePauseMenu(const CObjectAbstract * Parent);
	UIScenePauseMenu(const UIScenePauseMenu & Source);
	virtual ~UIScenePauseMenu();

	virtual void	Initialize();

	virtual void	OnButtonResume(const CActor * Sender, ESystemEventID EventId);
	virtual void	OnButtonSaveLoad(const CActor * Sender, ESystemEventID EventId);
	virtual void	OnButtonOptions(const CActor * Sender, ESystemEventID EventId);
	virtual void	OnButtonExit(const CActor * Sender, ESystemEventID EventId);

	virtual bool	ProcessButtonUI(const EventInput &InputData);

protected:
private:
	class LabelButton *m_pButtonResume;
	class LabelButton *m_pButtonSaveLoad;
	class LabelButton *m_pButtonOptions;
	class LabelButton *m_pButtonExit;
};

#endif//__uiscenepausemenu_h__