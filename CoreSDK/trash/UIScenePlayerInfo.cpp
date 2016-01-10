#include "coresdkafx.h"

REGISTER_CLASS(UIScenePlayerInfo, UIScene);

//----------------------------------------------------------------------------------------------
UIScenePlayerInfo::UIScenePlayerInfo(const CObjectAbstract * Parent)
: Super(Parent)
{
	NEW_OBJECT_TRANSIENT_CHILD(ProgressBarHP, UIProgressBar, "ProgressBarHP", this);
	NEW_OBJECT_TRANSIENT_CHILD(ProgressBarEnergy, UIProgressBar, "ProgressBarEnergy", this);
}

//----------------------------------------------------------------------------------------------
UIScenePlayerInfo::UIScenePlayerInfo(const UIScenePlayerInfo & Source)
: Super(Source)
{
	if (this != &Source)
	{
		// clone transient object:
		ProgressBarHP = CLONE_OBJECT(UIProgressBar, Source.ProgressBarHP, "ProgressBarHP", this);
		ProgressBarEnergy = CLONE_OBJECT(UIProgressBar, Source.ProgressBarEnergy, "ProgressBarEnergy", this);
	}
}

//----------------------------------------------------------------------------------------------
UIScenePlayerInfo::~UIScenePlayerInfo()
{

}

//----------------------------------------------------------------------------------------------
void UIScenePlayerInfo::Initialize()
{
	Super::Initialize();
}

//----------------------------------------------------------------------------------------------
void UIScenePlayerInfo::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}