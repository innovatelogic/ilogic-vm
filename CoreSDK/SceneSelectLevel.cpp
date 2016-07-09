#include "coresdkafx.h"

REGISTER_CLASS(CSceneSelectLevel, UIScene);

//----------------------------------------------------------------------------------------------
CSceneSelectLevel::CSceneSelectLevel(const CObjectAbstract *pParent)
: Super(pParent)
{

}

//----------------------------------------------------------------------------------------------
CSceneSelectLevel::~CSceneSelectLevel()
{

}

//----------------------------------------------------------------------------------------------
void CSceneSelectLevel::Initialize()
{
	Super::Initialize();

	if (CActor *m_ButtonLevel0 = FindActor(("ButtonLevel0"))) {
//		REGISTER_EVENT_(m_ButtonLevel0, this, boost::bind<void>(&CSceneSelectLevel::OnButtonPlayPressed, this, _1, _2), Event_OnPressed);
	}
	if (CActor *m_ButtonBack = FindActor(("ButtonBack"))) {
//		REGISTER_EVENT_(m_ButtonBack, this, boost::bind<void>(&CSceneSelectLevel::OnButtonQuitPressed, this, _1, _2), Event_OnPressed);
	}
}

//----------------------------------------------------------------------------------------------
void CSceneSelectLevel::OnButtonPlayPressed(const CActor *pSender, ESystemEventID EventId)
{
	GetAppMain()->GetUISceneManager()->Command_LoadScene("SceneLoading.xml");
	GetAppMain()->GetUISceneManager()->Command_InitializeScene("SceneLoading.xml");

	GetAppMain()->AddCommand(new Command<UIScene, UIScene>(&UIScene::Release, this));
}

//----------------------------------------------------------------------------------------------
void CSceneSelectLevel::OnButtonQuitPressed(const CActor *pSender, ESystemEventID EventId)
{
	GetAppMain()->GetUISceneManager()->Command_LoadScene("SceneLogin01.xml");
	GetAppMain()->GetUISceneManager()->Command_InitializeScene("SceneLogin01.xml");

	GetAppMain()->AddCommand(new Command<UIScene, UIScene>(&UIScene::Release, this));
}