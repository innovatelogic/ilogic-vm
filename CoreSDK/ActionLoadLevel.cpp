#include "coresdkafx.h"

REGISTER_CLASS_ACTION(ActionLoadLevel, ActionBase);

//----------------------------------------------------------------------------------------------
ActionLoadLevel::ActionLoadLevel(const CObjectAbstract * Parent)
: Super(Parent)
{
	SocketIn = new SocketAction(this, ELink_In);
	SocketOut = new SocketAction(this, ELink_Out);

	RegisterSocket(SocketIn);
	RegisterSocket(SocketOut);
}

//----------------------------------------------------------------------------------------------
ActionLoadLevel::ActionLoadLevel(const ActionLoadLevel & Source)
: Super(Source)
{
	if (this != &Source)
	{
		SocketIn = new SocketAction(this, ELink_In);
		SocketOut = new SocketAction(this, ELink_Out);

		RegisterSocket(SocketIn);
		RegisterSocket(SocketOut);
	}
}

//----------------------------------------------------------------------------------------------
ActionLoadLevel::~ActionLoadLevel()
{
	UnRegisterSocket(SocketIn);
	UnRegisterSocket(SocketOut);

	delete SocketIn;
	delete SocketOut;
}

//----------------------------------------------------------------------------------------------
void ActionLoadLevel::Render(HDC hdc, const Matrix &ViewMatrix)
{
	Super::Render(hdc, ViewMatrix);

	SocketIn->Render(hdc, ViewMatrix);
	SocketOut->Render(hdc, ViewMatrix);
}

//----------------------------------------------------------------------------------------------
void ActionLoadLevel::DoExecute(CActor * EventActor)
{
	Super::DoExecute(EventActor);
	
	//GetAppMain()->GetUISceneManager()->Command_LoadScene("SceneLoading.xml");
	//GetAppMain()->GetUISceneManager()->Command_InitializeScene("SceneLoading.xml");
	//GetAppMain()->AddCommand(new Command<UIScene, UIScene>(&UIScene::Release, this));
}