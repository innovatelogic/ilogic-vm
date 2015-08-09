#include "coresdkafx.h"

REGISTER_CLASS(SplineAgregator_Brush, SplineAgregator);

//----------------------------------------------------------------------------------
SplineAgregator_Brush::SplineAgregator_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
	SetVisible(false);

	SetTypeGenerated(std::string("SplineAgregator"));
	SetKeyTypeGenerated(std::string("Point2D"));
	SetDetail(1);
}

//----------------------------------------------------------------------------------
SplineAgregator_Brush::~SplineAgregator_Brush()
{

}

//----------------------------------------------------------------------------------
bool SplineAgregator_Brush::DoEventPressed(const MouseInputData& InputData)
{
/*	if (InputData.event == MOUSE_Pressed && InputData.Code == MOUSE_LEFT)
	{
		TMapSubscribersIterator Iter = Subscribers.begin();
		
		while (Iter != Subscribers.end())
		{
			GenerateKey(InputData.MousePos); // generate point
			break;
		}
	}
	else if (InputData.event == MOUSE_Pressed && InputData.Code == MOUSE_RIGHT)
	{
		ReleaseGeneratedChilds();
	//	KillFocus();
	}*/
	return true;
}

//----------------------------------------------------------------------------------
void SplineAgregator_Brush::DoDraw()
{
/*	Matrix IWTM;
	invert(IWTM, WorldMatrixTransform);

	TVecIndexedPoint2fIterator Iter(PreRenderSpline.begin());

	while (Iter != PreRenderSpline.end())
	{
		TVecIndexedPoint2fIterator IterNext(GetIteratorNext(PreRenderSpline, Iter));

		if (IterNext == PreRenderSpline.begin()){
			break;
		}
		GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord(IterNext->Point, IWTM),
									   WorldMatrixTransform.t2 + transform_coord(Iter->Point, IWTM), COLOR_WHITE);
		++Iter;
	}*/
}

//----------------------------------------------------------------------------------
bool SplineAgregator_Brush::GenerateKey(const Vector2f& MousePos)
{
	bool bResult = false;

	GetAppMain()->EnterDrawCS();

	TVecActorChildIterator Iter = std::find_if(m_ChildNodes.begin(), m_ChildNodes.end(), boost::bind<bool>(&CActor::HitTest, _1, MousePos));

	if (Iter != m_ChildNodes.end() && Iter == GetChildIteratorNext(m_ChildNodes.end(), GetKeyTypeGenerated().c_str()))
	{
		GenerateObjectSpline();

		ReleaseGeneratedChilds();
		
		SetControlState(ActorState_Locked, true); // restore mouse input lock
		
		PreRenderSpline.clear();
		
		bResult = true;
	}
	else
	{
/*		CActor * SubObject = static_cast<CActor*>(GENERATE_OBJECT_SDK(GetKeyTypeGenerated().c_str(), CActor::ResolveName(GetKeyTypeGenerated(), this).c_str(), this));

		if (SubObject)
		{
			SubObject->Initialize();

 			Vector OutLocalPoint;
 			SubObject->GlobalToLocalTransform(OutLocalPoint, Vector(MousePos.x, MousePos.y, 0.f));

 			SubObject->SetPosition(OutLocalPoint);

			AddChildNode(SubObject);
			
			BuildSplineList(PreRenderSpline);

			if (GetControlState() == ActorState_Locked)
			{
				SetControlState(ActorState_None, true); // unlock object free movement
			}

			RebuildTransform();

			bResult = true;
		}*/
	}

	GetAppMain()->LeaveDrawCS();

	return bResult;
}

//----------------------------------------------------------------------------------
bool SplineAgregator_Brush::GenerateObjectSpline()
{
	bool bResult = false;

	GetAppMain()->EnterDrawCS();

	/*TMapSubscribersIterator Iter = Subscribers.begin();

	while (Iter != Subscribers.end())
	{
		CActor * NewObject = static_cast<CActor*>(SLICE_CLONE_OBJECT(this, GetTypeGenerated().c_str(), CActor::ResolveName(GetName(), Iter->first).c_str()));

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
		bResult = true;
		break;
	}*/

	GetAppMain()->LeaveDrawCS();

	return bResult;
}

//----------------------------------------------------------------------------------
void SplineAgregator_Brush::ReleaseGeneratedChilds()
{
	GetAppMain()->EnterDrawCS();

	for (TVecActorChildIterator Iter = m_ChildNodes.begin(); Iter != m_ChildNodes.end(); ++Iter)
	{
		if ((*Iter)->GetType() == GetKeyTypeGenerated().c_str()){
			(*Iter)->Release();
		}
	}

	GetAppMain()->LeaveDrawCS();
}