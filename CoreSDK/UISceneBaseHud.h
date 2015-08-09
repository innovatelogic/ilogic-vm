#ifndef __uiscenebasehud_h__
#define __uiscenebasehud_h__

#ifdef WIN32
#pragma once
#endif

#include "UIScene.h"

class CORESDK_API UISceneBaseHud : public UIScene
{
	DECLARE_CLASS_SIMPLE(UISceneBaseHud, UIScene);

public:
	UISceneBaseHud(const CObjectAbstract * Parent);
	UISceneBaseHud(const UISceneBaseHud & Source);
	virtual ~UISceneBaseHud();

	virtual void		Initialize();
	virtual void		Update(float DeltaTime);

	virtual void		BlinkLabelMessageLT(const std::string &Value, float Time = 2.5f);
	virtual void		BlinkLabelMessage1C(const std::string &Value, float Time = 2.5f);
	virtual void		BlinkLabelMessage2C(const std::string &Value, float Time = 2.5f);

	virtual bool		ProcessButtonUI(const EventInput &InputData);

protected:
private:
	class Label			*LabelMessageLT;
	class Label			*LabelMessage1C;
	class Label			*LabelMessage2C;

	bool				bBlink_LabelMessageLT;
	bool				bBlink_LabelMessage1C;
	bool				bBlink_LabelMessage2C;

	float				AnimTimeLT;
	float				AnimTime1C;
	float				AnimTime2C;
};

#endif//__uiscenebasehud_h__