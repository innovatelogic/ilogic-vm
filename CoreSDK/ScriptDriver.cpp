#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
ScriptDriver::ScriptDriver(class CCoreSDK * pSDK)
: pCoreSDK(pSDK)
{

}

//----------------------------------------------------------------------------------------------
ScriptDriver::~ScriptDriver()
{

}

//----------------------------------------------------------------------------------------------
bool ScriptDriver::RegisterAction(ActionBase * Action)
{
	TVecActions::iterator IterFind = std::find(VecActions.begin(), VecActions.end(), Action);

	if (IterFind == VecActions.end()){
		VecActions.push_back(Action);
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool ScriptDriver::UnRegisterAction(ActionBase * Action)
{
	VecActions.erase(std::remove(VecActions.begin(), VecActions.end(), Action), VecActions.end());
	return true;
}

//----------------------------------------------------------------------------------------------
void ScriptDriver::Render(CActor* Actor, HDC hdc, const Matrix &ViewMatrix)
{
	for (TVecActions::iterator Iter = VecActions.begin(); Iter != VecActions.end(); ++Iter)
	{
		if ((*Iter)->GetOwnerActor() == Actor)
		{
			(*Iter)->Render(hdc, ViewMatrix);
		}
	}
}

//----------------------------------------------------------------------------------------------
ActionBase* ScriptDriver::GetActionByActor(class CActor *Actor)
{
	for (TVecActions::iterator Iter = VecActions.begin(); Iter != VecActions.end(); ++Iter)
	{
		if ((*Iter)->GetOwnerActor() == Actor)
		{
			return (*Iter);
		}
	}
	return NULL;
}

//----------------------------------------------------------------------------------------------
SocketAction* ScriptDriver::ProcessClickSocket(CActor* OwnerActor, const Vector2f &Point, bool link /*= false*/) const
{
 	for (TVecActions::const_iterator Iter = VecActions.begin(); Iter != VecActions.end(); ++Iter)
 	{
 		if ((*Iter)->GetOwnerActor() == OwnerActor)
 		{
			if (SocketAction* Soc = (*Iter)->ProcessMouseClickSocket(Point, link)){
				return Soc;
			}
 		}
 	}
	return 0;
}

//----------------------------------------------------------------------------------------------
const ActionBase* ScriptDriver::ProcessClickAction(CActor* OwnerActor, const Vector2f &Point) const
{
	for (TVecActions::const_iterator Iter = VecActions.begin(); Iter != VecActions.end(); ++Iter)
	{
		if ((*Iter)->GetOwnerActor() == OwnerActor)
		{
			if (const ActionBase* Action = (*Iter)->ProcessMouseClick(Point)){
				return Action;
			}
		}
	}
	return 0;
}