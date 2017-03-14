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