#include "coresdkafx.h"

REGISTER_CLASS_ACTION(ActionHandler, ActionBase);

//----------------------------------------------------------------------------------------------
ActionHandler::ActionHandler(const CObjectAbstract * Parent)
: Super(Parent)
{
	SocketOut = new SocketAction(this, ELink_Out);
	RegisterSocket(SocketOut);
}

//----------------------------------------------------------------------------------------------
ActionHandler::ActionHandler(const ActionHandler & Source)
: Super(Source)
{
	if (this != &Source)
	{
		SocketOut = new SocketAction(this, ELink_Out);
		RegisterSocket(SocketOut);
	}
}

//----------------------------------------------------------------------------------------------
ActionHandler::~ActionHandler()
{
	UnRegisterSocket(SocketOut);
	delete SocketOut;
}

//----------------------------------------------------------------------------------------------
void ActionHandler::Render(HDC hdc, const Matrix &ViewMatrix)
{
	Super::Render(hdc, ViewMatrix);

 	SocketOut->Render(hdc, ViewMatrix);
}