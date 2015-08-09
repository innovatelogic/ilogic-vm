#include "coresdkafx.h"

REGISTER_CLASS(LabelButton_Brush, LabelButton);

//----------------------------------------------------------------------------------------------
LabelButton_Brush::LabelButton_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
	SetVisible(false);
}

//----------------------------------------------------------------------------------------------
LabelButton_Brush::LabelButton_Brush(const LabelButton_Brush & Source)
: Super(Source)
{

}

//----------------------------------------------------------------------------------------------
LabelButton_Brush::~LabelButton_Brush()
{

}

//----------------------------------------------------------------------------------------------
bool LabelButton_Brush::DoEventPressed(const MouseInputData& InputData)
{
	if (InputData.event == MOUSE_Pressed && InputData.Code == MOUSE_LEFT)
	{
		GetAppMain()->EnterDrawCS();

		/*TMapSubscribersIterator Iter = Subscribers.begin();

		if (Iter != Subscribers.end())
		{
			CActor * NewObject = static_cast<UIImage*>(SLICE_CLONE_OBJECT(this, "LabelButton", CActor::ResolveName(GetName(), Iter->first).c_str()));

			if (NewObject != NULL)
			{
				Event_CallSubscriber(NewObject, Event_ObjectGenerated);

				// drop brush states
				NewObject->SetTransient(false);
				NewObject->SetVisible(true);

				Matrix NewLTM;
				if (CActor::GetObjectAInLocalSpaceB(NewLTM, Iter->first, this))
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