#include "BaseSplineAgregator.h"
#include "RenderSDK.h"

REGISTER_CLASS_PURE(BaseSplineAgregator, UIScreenObject)
	new oes::rflex::PropertyUINT("SplineDetail", (BYTE*)&((BaseSplineAgregator*)NULL)->SplineDetail - (BYTE*)NULL, "UIComp_DrawFont", "Value", READ_WRITE, CTRL_EDIT, true, false),
	new oes::rflex::PropertyBOOL("SplineClosed", (BYTE*)&((BaseSplineAgregator*)NULL)->SplineClosed - (BYTE*)NULL, "UIComp_DrawFont", "Value", READ_WRITE, CTRL_COMBO, true, false),
END_REGISTER_CLASS_PURE(BaseSplineAgregator, UIScreenObject);

//----------------------------------------------------------------------------------------------
BaseSplineAgregator::BaseSplineAgregator(const CObjectAbstract * Parent /*= NULL*/)
: Super(Parent)
, SplineDetail(1)
, SplineClosed(true)
{

}

//----------------------------------------------------------------------------------------------
BaseSplineAgregator::~BaseSplineAgregator()
{
	;//
}

//----------------------------------------------------------------------------------------------
void BaseSplineAgregator::Initialize()
{	
	Super::Initialize();
	Rebuild();
}

//----------------------------------------------------------------------------------------------
void BaseSplineAgregator::OnPropertyChanged(const char* PropertyName)
{
	Rebuild();
}

//----------------------------------------------------------------------------------------------
// void BaseSplineAgregator::DoUpdate(float dt)
// {
// 	Super::DoUpdate(dt);
// 	Rebuild();
// }

//----------------------------------------------------------------------------------------------
bool BaseSplineAgregator::HitTest(const Vector2f &Position)
{
/*	Vector2f LocalMousePos;

	// calculate origin mouse position
	const Vector2f OriginMousePos = Position - WorldMatrixTransform.t2;

	Matrix IWTM = WorldMatrixTransform;
	IWTM = transpose(IWTM);

	TTriangleList2fIterator Iter = HitTestTriangleList.begin();

	while (Iter != HitTestTriangleList.end())
	{
		Vector2f P0, P1, P2;
		transform_coord( P0, (*Iter).P0, IWTM);
		transform_coord( P1, (*Iter).P1, IWTM);
		transform_coord( P2, (*Iter).P2, IWTM);

		if (IntersectRayWithTriangle( OriginMousePos, P0, P1, P2))
		{
			return true;
		}
		++Iter;
	}*/
	return false;
}

//----------------------------------------------------------------------------------------------
void BaseSplineAgregator::DoDraw()
{
/*	CRenderSDK * RenderComponent = static_cast<CRenderSDK*>(GetRenderComponent());

	TVectorListIterator Iter = SplineList.begin();
	while (Iter != SplineList.end())
	{
		TVectorListIterator IterNext = Iter;

		if (++IterNext == SplineList.end()){
			break;
		}

		Vector2f Normal = GetSegmentNormal(Vector2f((*Iter).x,(*Iter).y) , Vector2f((*IterNext).x,(*IterNext).y));
		Vector2f P0 = Vector2f((*Iter).x, (*Iter).y);
		Vector2f P1 = Vector2f((*IterNext).x, (*IterNext).y);
		Vector2f V = (P1 - P0) * 0.75;

		Normal = Normal * 5;

		Matrix IWTM;
		invert(IWTM, WorldMatrixTransform);

		RenderComponent->DrawLine(WorldMatrixTransform.t2 + transform_coord(P0, IWTM),				 WorldMatrixTransform.t2 + transform_coord(P1, IWTM), 0xffff0000);
		RenderComponent->DrawLine(WorldMatrixTransform.t2 + transform_coord(P0 + V, IWTM),			 WorldMatrixTransform.t2 + transform_coord(P0 + (V + Normal), IWTM), 0xffff0000);
		RenderComponent->DrawLine(WorldMatrixTransform.t2 + transform_coord(P0 + V, IWTM),			 WorldMatrixTransform.t2 + transform_coord(P0 + (V - Normal), IWTM), 0xffff0000);
		RenderComponent->DrawLine(WorldMatrixTransform.t2 + transform_coord(P0 + (V + Normal), IWTM), WorldMatrixTransform.t2 + transform_coord(P1, IWTM), 0xffff0000);
		RenderComponent->DrawLine(WorldMatrixTransform.t2 + transform_coord(P0 + (V - Normal), IWTM), WorldMatrixTransform.t2 + transform_coord(P1, IWTM), 0xffff0000);	
		++Iter;
	}

	TTriangleList2fIterator IterTriangle = HitTestTriangleList.begin();
	while (IterTriangle != HitTestTriangleList.end())
	{
		//RenderComponent->DrawTriangle((*IterTriangle).P0, (*IterTriangle).P1, (*IterTriangle).P2, 0xffff0000);
		++IterTriangle;
	}*/
}

//----------------------------------------------------------------------------------------------
void BaseSplineAgregator::Rebuild()
{
	BuildSpline(SplineList);
	BuildHitVolume(SplineList, HitTestTriangleList);
}

//----------------------------------------------------------------------------------------------
/*void BaseSplineAgregator::UpdateController(CActor* Actor, SubscriberData* Controller)
{
	float TotalTime = 0.f;
	float fSpeed = 300;

	TVectorListIterator Iter = SplineList.begin();
	TVectorListIterator IterNext = Iter;

	IterNext = Iter;
	IterNext++;
	
	while (IterNext != SplineList.end())
	{
		Vector P0 = *Iter;
		Vector P1 = *IterNext;

		float  nLength = (P1 - P0).Length(); // segment length
		float  nTime = nLength / fSpeed;	 // linear time to pass whole segment [t = dist / v];

		TotalTime += nTime;

		if (Controller->Time < TotalTime)
		{
			float dt = nTime - (TotalTime - Controller->Time);
			float k = dt / nTime;

			TransformObjectAToObjectBLocalPos(Actor, this, P0 + ((P1 - P0) * k));
			return;
		}
		Iter = IterNext;
		IterNext++;
	}
	Controller->Time = 0.f;
}*/

//----------------------------------------------------------------------------------------------
void BaseSplineAgregator::BuildSpline(TVectorList& OutSpline)
{
/*	OutSpline.clear();

	TVecActorChildIterator  P0, P1, P2, P3;
	std::vector<const CActor*> TmpArray;

	if (SplineClosed == false) // non closed spline
	{
		GetObjectsByClass(TmpArray, "Point2D");

		if (TmpArray.size() >= 4)
		{
			P0 = GetChildIteratorNext(ChildNodes.end(), "Point2D");
			P1 = GetChildIteratorNext(P0, "Point2D");
			P2 = GetChildIteratorNext(P1, "Point2D");
			P3 = GetChildIteratorNext(P2, "Point2D");

			// push start point
			OutSpline.push_back((*P0)->m_LTM.t);

			// build middle region
			while (P3 != GetChildIteratorNext(ChildNodes.end(), "Point2D"))
			{
				for (size_t Index = 0; Index < SplineDetail; ++Index)
				{
					float flT = (float)Index / (float)SplineDetail;

					Vector Result;
					Catmull_Rom_Spline((*P0)->m_LTM.t, (*P1)->m_LTM.t, (*P2)->m_LTM.t, (*P3)->m_LTM.t, flT, Result);
					OutSpline.push_back(Result);
				}
				OutSpline.push_back((*P2)->m_LTM.t);

				P0 = P1; // circle shift iterators
				P1 = P2;
				P2 = P3;
				P3 = GetChildIteratorNext(P3, "Point2D");
			}
			// add tail
			OutSpline.push_back((*P2)->m_LTM.t);	
		}
		else // add linear array
		{
			GetObjectsByClass(TmpArray, "Point2D");

			if (TmpArray.size() >= 2)
			{
				P0 = GetChildIteratorNext(ChildNodes.begin(), "Point2D");

				do
				{
					OutSpline.push_back((*P0)->m_LTM.t);

					P0 = GetChildIteratorNext(P0, "Point2D");

				}while(P0 != GetChildIteratorNext(ChildNodes.begin(), "Point2D"));
			}
		}
	}
	else
	{
		GetObjectsByClass(TmpArray, "Point2D");

		if (TmpArray.size() >= 4)
		{
			P0 = GetChildIteratorPrev(ChildNodes.begin(), "Point2D");
			P1 = GetChildIteratorNext(P0, "Point2D");
			P2 = GetChildIteratorNext(P1, "Point2D");
			P3 = GetChildIteratorNext(P2, "Point2D");

			while(1)
			{
				for (size_t Index = 0; Index < SplineDetail; ++Index)
				{
					float flT = (float)Index / (float)SplineDetail;

					Vector Result;
					Catmull_Rom_Spline((*P0)->m_LTM.t, (*P1)->m_LTM.t, (*P2)->m_LTM.t, (*P3)->m_LTM.t, flT, Result);

					OutSpline.push_back(Result);
				}

				P0 = GetChildIteratorNext(P0, "Point2D");
				P1 = GetChildIteratorNext(P1, "Point2D");
				P2 = GetChildIteratorNext(P2, "Point2D");
				P3 = GetChildIteratorNext(P3, "Point2D");

				if (P0 == GetChildIteratorPrev(ChildNodes.begin(), "Point2D"))
				{
					OutSpline.push_back((*P1)->m_LTM.t);
					break;
				}
			}
		}
		else
		{
			GetObjectsByClass(TmpArray, "Point2D");

			if (TmpArray.size() >= 2)
			{
				P0 = GetChildIteratorNext(ChildNodes.begin(), "Point2D");
				P1 = GetChildIteratorNext(P0, "Point2D");

				// add start
				OutSpline.push_back((*P0)->m_LTM.t);

				while (P1 != ChildNodes.begin())
				{
					OutSpline.push_back((*P1)->m_LTM.t);

					P0 = P1;
					P1 = GetChildIteratorNext(P1, "Point2D");
				}
				// add start as circle
				OutSpline.push_back((*P1)->m_LTM.t);
			}
		}
	}*/
}

//----------------------------------------------------------------------------------------------
/** build a simple volume of spline */
void BaseSplineAgregator::BuildHitVolume(TVectorList& InSpline, TTriangleList2f &OutVolume)
{
	TVectorListIterator  P0, P1;
	OutVolume.clear();

	if (InSpline.size() >= 2)
	{
		P0 = InSpline.begin();
		P1 = P0;	
		P1++;

		while (P1 != InSpline.end())
		{
			Vector2f Normal = GetSegmentNormal(Vector2f((*P0).x,(*P0).y) , Vector2f((*P1).x,(*P1).y));

			Vector2f Sq0 = Vector2f((*P0).x,(*P0).y) + Normal * 5.f;
			Vector2f Sq1 = Vector2f((*P1).x,(*P1).y) + Normal * 5.f;
			Vector2f Sq2 = Vector2f((*P1).x,(*P1).y) - Normal * 5.f;
			Vector2f Sq3 = Vector2f((*P0).x,(*P0).y) - Normal * 5.f;

			Triangle2D Triangle1(Sq0, Sq1, Sq2);
			Triangle2D Triangle2(Sq2, Sq3, Sq0);

			OutVolume.push_back(Triangle1);
			OutVolume.push_back(Triangle2);

			P0 = P1;
			++P1;
		}
	}
}

//----------------------------------------------------------------------------------------------
/*void BaseSplineAgregator::DoTick(float dt)
{
	TMapSubscribersIterator Iter = Subscribers.begin();
	while (Iter != Subscribers.end())
	{
		Iter->second->Time += dt;
		UpdateController(Iter->first, Iter->second);
		++Iter;
	}
}*/