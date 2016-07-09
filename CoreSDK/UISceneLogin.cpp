#include "coresdkafx.h"

REGISTER_CLASS(CUISceneLogin, UIScene);

//----------------------------------------------------------------------------------------------
CUISceneLogin::CUISceneLogin(const CObjectAbstract *pParent)
: Super(pParent)
{

}

//----------------------------------------------------------------------------------------------
CUISceneLogin::~CUISceneLogin()
{

}

//----------------------------------------------------------------------------------------------
void CUISceneLogin::Initialize()
{
	Super::Initialize();

	if (CActor * pButtonPlay = FindActor(("ButtonPlay"))) {
//		REGISTER_EVENT_(pButtonPlay, this, boost::bind<void>(&CUISceneLogin::OnButtonPlayPressed, this, _1, _2), Event_OnPressed);
	}
	if (CActor * pButtonQuit = FindActor(("ButtonQuit"))) {
//		REGISTER_EVENT_(pButtonQuit, this, boost::bind<void>(&CUISceneLogin::OnButtonQuitPressed, this, _1, _2), Event_OnPressed);
	}
}

//----------------------------------------------------------------------------------------------
void CUISceneLogin::OnButtonPlayPressed(const CActor *pSender, ESystemEventID EventId)
{
 	GetAppMain()->GetUISceneManager()->Command_LoadScene("SceneSelectLevel.xml");
 	GetAppMain()->GetUISceneManager()->Command_InitializeScene("SceneSelectLevel.xml");
 	
	GetAppMain()->AddCommand(new Command<UIScene, UIScene>(&UIScene::Release, this));
}

//----------------------------------------------------------------------------------------------
void CUISceneLogin::OnButtonQuitPressed(const CActor *pSender, ESystemEventID EventId)
{
	GetAppMain()->QuitApplication();
}