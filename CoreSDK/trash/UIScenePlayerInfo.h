#ifndef __uisceneplayerinfo_h__
#define __uisceneplayerinfo_h__

#ifdef WIN32
#pragma once
#endif

#include "UIScene.h"

class CORESDK_API UIScenePlayerInfo : public UIScene
{
	DECLARE_CLASS_SIMPLE(UIScenePlayerInfo, UIScene);

public:
	UIScenePlayerInfo(const CObjectAbstract * Parent);
	UIScenePlayerInfo(const UIScenePlayerInfo & Source);
	~UIScenePlayerInfo();

	virtual void		Initialize();
	virtual void		Update(float DeltaTime);

protected:
private:
	UIProgressBar	*ProgressBarHP;
	UIProgressBar	*ProgressBarEnergy;
};
#endif//__uisceneplayerinfo_h__