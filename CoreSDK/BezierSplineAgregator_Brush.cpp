#include "coresdkafx.h"

REGISTER_CLASS(Bezier_SplineAgregator_Brush, Bezier_SplineAgregator);

//---------------------------------------------------------------------
Bezier_SplineAgregator_Brush::Bezier_SplineAgregator_Brush(const CObjectAbstract * Parent)
: Super(Parent)
, bPointEdit(false)
{
	SetTypeGenerated(std::string("Bezier_SplineAgregator"));
	SetKeyTypeGenerated(std::string("PointBezier2D"));
	SetDetail(5);
}
//---------------------------------------------------------------------
Bezier_SplineAgregator_Brush::~Bezier_SplineAgregator_Brush()
{

}

//----------------------------------------------------------------------------------
bool Bezier_SplineAgregator_Brush::DoEventPressed(const MouseInputData& InputData)
{
/*	if (InputData.Code == MOUSE_LEFT)
	{
		if (InputData.event == MOUSE_Pressed)
		{
			TMapSubscribersIterator Iter = Subscribers.begin();

			while (Iter != Subscribers.end())
			{
				if (GetControlState() == ActorState_Locked)
				{
					SetControlState(ActorState_None, true);	// unlock object free movement
				}
				GenerateKey(InputData.MousePos, WorldMatrixTransform); // add key
				bPointEdit = true;
				break;
			}
		}
		else if (InputData.event == MOUSE_Released)
		{
			bPointEdit = false;
		}
	}
	else if (InputData.event == MOUSE_Pressed && InputData.Code == MOUSE_RIGHT)
	{
		bPointEdit = false;
		ReleaseGeneratedChilds();
	}*/
	return true;
}

//----------------------------------------------------------------------------------
void Bezier_SplineAgregator_Brush::DoDraw()
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

//---------------------------------------------------------------------
bool Bezier_SplineAgregator_Brush::DoEventMove(const MouseMoveInputData& InputData)
{
	if (bPointEdit)
	{
		TVecActorChildIterator Iter = GetChildIteratorPrev(m_ChildNodes.end(), GetKeyTypeGenerated().c_str());
		if (Iter != m_ChildNodes.end())
		{
			TVecActorChildIterator P0 = (*Iter)->GetChildIteratorNext((*Iter)->m_ChildNodes.end(), "Point2DBezierKey");
			TVecActorChildIterator P1 = (*Iter)->GetChildIteratorNext(P0, "Point2DBezierKey");

			if (P0 != (*Iter)->m_ChildNodes.end() && P1 != (*Iter)->m_ChildNodes.end())
			{
			//	Vector OutLocalPoint;
			//	(*P1)->GlobalToLocalTransform(OutLocalPoint, Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f));
				
			//	(*P1)->SetPosition(OutLocalPoint);
			//	(*P0)->SetPosition(-OutLocalPoint);

			//	BuildSplineList(PreRenderSpline);
			}
		}
	}
	return CActor::DoEventMove(InputData);
}

//----------------------------------------------------------------------------------
bool Bezier_SplineAgregator_Brush::GenerateKey(const Vector2f& MousePos, const Matrix &WTM)
{
	bool bResult = false;
/*
	// find collision with first point
	Matrix WorldMatrix = WTM * GetPivot() * GetSTM();
	
	TVecActorChildIterator Iter = std::find_if(ChildNodes.begin(), ChildNodes.end(), boost::bind<bool>(&CActor::HitTest, _1, MousePos));

	if (Iter != ChildNodes.end() && Iter == GetChildIteratorNext(ChildNodes.end(), GetKeyTypeGenerated().c_str()))
	{
		GenerateObjectSpline();
		ReleaseGeneratedChilds();
		SetControlState(ActorState_Locked, true); // restore mouse input lock
		PreRenderSpline.clear();
		bResult = true;
	}
	else
	{
		CActor * SubObject = static_cast<CActor*>(GENERATE_OBJECT_SDK(GetKeyTypeGenerated().c_str(), CActor::ResolveName(GetKeyTypeGenerated(), this).c_str(), this));

		if (SubObject)
		{
			MouseMoveInputData InputData;
			InputData.MousePos = MousePos;

			Vector OutLocalPoint;
			SubObject->GlobalToLocalTransform(OutLocalPoint, Vector(MousePos.x, MousePos.y, 0.f));
			SubObject->SetPosition(OutLocalPoint);

			AddChildNode(SubObject);
			SubObject->Initialize();
			
			BuildSplineList(PreRenderSpline);

			if (GetControlState() == ActorState_Locked)
			{
				SetControlState(ActorState_None, true); // unlock object free movement
			}

			//RebuildTransform();

			bResult = true;
		}
	}*/
	return bResult;
}

//----------------------------------------------------------------------------------
bool Bezier_SplineAgregator_Brush::GenerateObjectSpline()
{
	bool bResult = false;

	/*TMapSubscribersIterator Iter = Subscribers.begin();

	while (Iter != Subscribers.end())
	{
		CActor * NewObject = static_cast<CActor*>(
			SLICE_CLONE_OBJECT(this, GetTypeGenerated().c_str(), CActor::ResolveName(GetName(), Iter->first).c_str())
			);

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
	return bResult;
}

//----------------------------------------------------------------------------------
void Bezier_SplineAgregator_Brush::ReleaseGeneratedChilds()
{
	for (TVecActorChildIterator Iter = m_ChildNodes.begin(); Iter != m_ChildNodes.end(); ++Iter)
	{
		if ((*Iter)->GetType() == GetKeyTypeGenerated().c_str()){
			(*Iter)->Release();
		}
	}
}

//---------------------------------------------------------------------
void Bezier_SplineAgregator_Brush::BuildSplineList(TVecIndexedPoint2f& OutSpline)
{
/*	TVecActorChildIterator  PA, PB;

	OutSpline.clear();

	PA = GetChildIteratorNext(ChildNodes.end(), GetKeyTypeGenerated().c_str());
	PB = GetChildIteratorNext(PA, GetKeyTypeGenerated().c_str());

	if (PA == ChildNodes.end() || PB == ChildNodes.end()){
		return;
	}

	do
	{		
		Vector A = (*PA)->m_LTM.t;
		Vector B = ((*PA)->m_LTM * (*PA)->ChildNodes[1]->m_LTM).t;
		Vector D = (*PB)->m_LTM.t;
		Vector C = ((*PB)->m_LTM * (*PB)->ChildNodes[0]->m_LTM).t;

		//float S = fabs((D - A).Cross(C - A).Length() + (C - A).Cross(B - A).Length()) * 0.5f;
		//float TriLength = (B - A).Length() + (C - D).Length() + (D - A).Length();
		//size_t Detail = ((D - A).Length() + ((B - A).Length() + (C - D).Length()) * 0.5f) / SplineDetail;
		//size_t Detail = SplineDetail;//(Detail == 0) ? 1 : Detail;

		bool bFinish = PA == GetChildIteratorPrev(ChildNodes.begin(), GetKeyTypeGenerated().c_str());

		for (size_t Index = 0; Index < GetDetail(); ++Index)
		{
			Vector Result;
			Cubic_Bezier_Spline(D, C, B, A, (float)Index / (float) GetDetail(), Result);

			IndexedPoint2f Point(Vector2f(Result.x, Result.y), Vector2f(0.f, 0.f), (unsigned int)OutSpline.size());
			OutSpline.push_back(Point);
			
			if (bFinish){
				return;
			}
		}
		PA = GetChildIteratorNext(PA, GetKeyTypeGenerated().c_str());
		PB = GetChildIteratorNext(PB, GetKeyTypeGenerated().c_str());

	}while (PA != GetChildIteratorNext(ChildNodes.end(), GetKeyTypeGenerated().c_str()));*/
}