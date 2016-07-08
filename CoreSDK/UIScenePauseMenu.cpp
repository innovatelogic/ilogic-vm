#include "coresdkafx.h"

REGISTER_CLASS(UIScenePauseMenu, UIScene);

//----------------------------------------------------------------------------------------------
UIScenePauseMenu::UIScenePauseMenu(const CObjectAbstract *pParent)
: Super(pParent)
{
//	NEW_OBJECT_TRANSIENT_CHILD(ButtonResume, LabelButton, "ButtonResume", this);
//	NEW_OBJECT_TRANSIENT_CHILD(ButtonSaveLoad, LabelButton, "ButtonSaveLoad", this);
//	NEW_OBJECT_TRANSIENT_CHILD(ButtonOptions, LabelButton, "ButtonOptions", this);
//	NEW_OBJECT_TRANSIENT_CHILD(ButtonExit, LabelButton, "ButtonExit", this);
}

//----------------------------------------------------------------------------------------------
UIScenePauseMenu::~UIScenePauseMenu()
{

}

//----------------------------------------------------------------------------------------------
void UIScenePauseMenu::Initialize()
{
	Super::Initialize();

	if (CActor * pButtonResume = FindActor(("ButtonResume"))) {
		REGISTER_EVENT_(pButtonResume, this, boost::bind<void>(&UIScenePauseMenu::OnButtonResume, this, _1, _2), Event_OnPressed);
	}
	if (CActor * pButtonSaveLoad = FindActor(("ButtonSaveLoad"))) {
		REGISTER_EVENT_(pButtonSaveLoad, this, boost::bind<void>(&UIScenePauseMenu::OnButtonSaveLoad, this, _1, _2), Event_OnPressed);
	}
	if (CActor * pButtonOptions = FindActor(("ButtonOptions"))) {
		REGISTER_EVENT_(pButtonOptions, this, boost::bind<void>(&UIScenePauseMenu::OnButtonOptions, this, _1, _2), Event_OnPressed);
	}
	if (CActor * pButtonExit = FindActor(("ButtonExit"))) {
		REGISTER_EVENT_(pButtonExit, this, boost::bind<void>(&UIScenePauseMenu::OnButtonExit, this, _1, _2), Event_OnPressed);
	}

	SetVisible(false);				// initially hide
	SetInputMode(INPUTMODE_None);	// deprecate input on startup

	GetAppMain()->SetPause(true);
}

//----------------------------------------------------------------------------------------------
void UIScenePauseMenu::OnButtonResume(const CActor * Sender, ESystemEventID EventId)
{
	GetAppMain()->GetUISceneManager()->Command_CloseScene(this);
	GetAppMain()->SetPause(false);
}

//----------------------------------------------------------------------------------------------
void UIScenePauseMenu::OnButtonSaveLoad(const CActor * Sender, ESystemEventID EventId)
{
}

//----------------------------------------------------------------------------------------------
void UIScenePauseMenu::OnButtonOptions(const CActor * Sender, ESystemEventID EventId)
{
}

//----------------------------------------------------------------------------------------------
void UIScenePauseMenu::OnButtonExit(const CActor * Sender, ESystemEventID EventId)
{
	GetAppMain()->GetSceneManager()->Command_CloseAll();
	GetAppMain()->GetUISceneManager()->CloseAll();

	GetAppMain()->GetUISceneManager()->Command_LoadScene("SceneSelectLevel.xml");
	GetAppMain()->GetUISceneManager()->Command_InitializeScene("SceneSelectLevel.xml");

	GetAppMain()->SetPause(false);
}

//----------------------------------------------------------------------------------------------
bool UIScenePauseMenu::ProcessButtonUI(const EventInput &InputData)
{
	bool bResult = false;
	switch (InputData.Key)
	{
	case KEY_ESCAPE:
		{
			GetAppMain()->GetUISceneManager()->Command_CloseScene(this);
			GetAppMain()->SetPause(false);

			bResult = true;
		}break;
	default:
		break;
	};
	return bResult;
}