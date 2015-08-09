#include "coresdkafx.h"

REGISTER_CLASS(Brush_LevelActor, CLevelActor);
REGISTER_CLASS(Brush_TriggerObject, TriggerObject);

//----------------------------------------------------------------------------------------------
Brush_LevelActor::Brush_LevelActor(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
}

//----------------------------------------------------------------------------------------------
Brush_LevelActor::Brush_LevelActor(const Brush_LevelActor & Source)
: Super(Source)
{

}

//----------------------------------------------------------------------------------------------
Brush_LevelActor::~Brush_LevelActor()
{

}

//----------------------------------------------------------------------------------------------
bool Brush_LevelActor::DoEventPressed(const MouseInputData& InputData)
{
	return true;
}

//----------------------------------------------------------------------------------------------
Brush_TriggerObject::Brush_TriggerObject(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
}

//----------------------------------------------------------------------------------------------
Brush_TriggerObject::Brush_TriggerObject(const Brush_TriggerObject & Source)
: Super(Source)
{

}

//----------------------------------------------------------------------------------------------
Brush_TriggerObject::~Brush_TriggerObject()
{

}

//----------------------------------------------------------------------------------------------
bool Brush_TriggerObject::DoEventPressed(const MouseInputData& InputData)
{
	if (InputData.event == MOUSE_Pressed && InputData.Code == MOUSE_LEFT)
	{
		GetAppMain()->EnterDrawCS();

		/*TMapSubscribersIterator Iter = Subscribers.begin();

		if (Iter != Subscribers.end())
		{
			CActor * NewObject = static_cast<TriggerObject*>(SLICE_CLONE_OBJECT(this, "TriggerObject", CActor::ResolveName(GetName(), Iter->first).c_str()));

			if (NewObject != NULL)
			{
				Event_CallSubscriber(NewObject, Event_ObjectGenerated);

				// drop brush states
				NewObject->SetTransient(false);
				NewObject->SetVisible(true);

				Matrix NewLTM;
				//if (CActor::GetObjectAInLocalSpaceB(NewLTM, Iter->first, this))
				{
					NewObject->SetPivot(NewLTM);
					NewObject->SetParent(Iter->first);
					Iter->first->AddChildNode(NewObject);

					NewObject->Initialize();
					NewObject->RebuildTransform();
				}
			}
		}*/

		GetAppMain()->LeaveDrawCS();
	}
	else if (InputData.event == MOUSE_Pressed && InputData.Code == MOUSE_RIGHT)
	{
		//KillFocus();
	}
	return true;
}
