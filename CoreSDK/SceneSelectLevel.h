#ifndef __sceneselectlevel_h__
#define __sceneselectlevel_h__

#ifdef WIN32
#pragma once
#endif

#include "UIScene.h"

class CORESDK_API CSceneSelectLevel : public UIScene
{
	DECLARE_CLASS_SIMPLE(CSceneSelectLevel, UIScene);

public:
	CSceneSelectLevel(const CObjectAbstract *pParent);
	CSceneSelectLevel(const CSceneSelectLevel &Source);
	virtual ~CSceneSelectLevel();

	virtual void	Initialize();

	void	OnButtonPlayPressed(const CActor *pSender, ESystemEventID EventId);
	void	OnButtonQuitPressed(const CActor *pSender, ESystemEventID EventId);

protected:
private:
	class LabelButton *m_ButtonLevel0;

	class LabelButton *m_ButtonBack;
};

#endif//__sceneselectlevel_h__