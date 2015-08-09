#include "coresdkafx.h"

REGISTER_CLASS(UISceneLoadingScreen, UIScene);

//----------------------------------------------------------------------------------------------
UISceneLoadingScreen::UISceneLoadingScreen(const CObjectAbstract *pParent)
: Super(pParent)
{

}

//----------------------------------------------------------------------------------------------
UISceneLoadingScreen::~UISceneLoadingScreen()
{

}

//----------------------------------------------------------------------------------------------
void UISceneLoadingScreen::Initialize()
{
	Super::Initialize();

	//REGISTER_EVENT_PROXY_INPUT(this, this, boost::bind<bool>(&UISceneLoadingScreen::ProxyProcessInputKey, this, _1, _2));
	//REGISTER_EVENT_(ButtonPlay, this, boost::bind<void>(&UISceneLoadingScreen::OnButtonStartPlay, this, _1, _2), Event_OnPressed);

	//ProgressLoading->SetValue(0.f);
	//LabelMessage->SetVisible(false);
	//ButtonPlay->SetVisible(false);

	{
		GameRealmInfo * RI = GetAppMain()->GetRealmInfo();
		std::string LevelName = RI->GetDefaultInfo()->GetLevelNameById(0);
		CSceneManager * SceneMgr = GetAppMain()->GetSceneManager();

		SceneMgr->Command_LoadScene(LevelName.c_str());

		//GetAppMain()->AddCommand(new Command_Val<UIProgressBar, UIProgressBar, float>(&UIProgressBar::SetValue, ProgressLoading, 0.5f));

		SceneMgr->Command_InitializeScene(LevelName.c_str());

		GetAppMain()->GetUISceneManager()->Command_LoadScene("BaseHUD.xml");
		GetAppMain()->GetUISceneManager()->Command_InitializeScene("BaseHUD.xml");

		//GetAppMain()->AddCommand(new Command_Val<UIProgressBar, UIProgressBar, float>(&UIProgressBar::SetValue, ProgressLoading, 1.f));
		//GetAppMain()->AddCommand(new Command_Val<Label, CActor, bool>(&CActor::SetVisible, LabelMessage, true));
		//GetAppMain()->AddCommand(new Command_Val<LabelButton, CActor, bool>(&CActor::SetVisible, ButtonPlay, true));

		GetAppMain()->GetUISceneManager()->Command_CloseAll();
	}
}

//----------------------------------------------------------------------------------------------
void UISceneLoadingScreen::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	/*
	float Val = ProgressLoading->GetValue();

	SceneManager * SceneMgr = GetAppMain()->GetExplorerInstance()->GetExplorer3D()->GetSceneManager();

	static float BlinkTime = 0.f;

	if (SceneMgr->IsLevelLoad("Scene"))
	{
		BlinkTime += DeltaTime;

		if (BlinkTime >= 1.2f)
		{
			LabelMessage->SetVisible(!LabelMessage->GetVisible());
			BlinkTime = 0.f;
		}
	}*/
}

//----------------------------------------------------------------------------------------------
void UISceneLoadingScreen::OnButtonStartPlay(const CActor * Sender, ESystemEventID EventId)
{
	StartPlay();
}
/*
//----------------------------------------------------------------------------------------------
bool UISceneLoadingScreen::ProxyProcessInputKey(const CActor * Sender, const EventInput &InputData)
{
	if (InputData.Key == KEY_SPACE){
		StartPlay();
	}
	return true; 
}*/

//----------------------------------------------------------------------------------------------
void UISceneLoadingScreen::StartPlay()
{
	CUISceneManager * pUISceneMgr = GetAppMain()->GetUISceneManager();

	pUISceneMgr->Command_CloseAll();

	pUISceneMgr->Command_LoadScene("BaseHUD.xml");
	pUISceneMgr->Command_InitializeScene("BaseHUD.xml");

	pUISceneMgr->Command_LoadScene("ScenePlayerInfo.xml");
	pUISceneMgr->Command_InitializeScene("ScenePlayerInfo.xml");

	pUISceneMgr->Command_LoadScene("ScenePauseMenu.xml");
	pUISceneMgr->Command_InitializeScene("ScenePauseMenu.xml");
}