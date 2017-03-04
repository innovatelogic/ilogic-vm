#include "coresdkafx.h"

REGISTER_CLASS(Bezier_SplineAgregator, SplineAgregator);

//---------------------------------------------------------------------
Bezier_SplineAgregator::Bezier_SplineAgregator(const CObjectAbstract * Parent)
: Super(Parent)
{

}

//---------------------------------------------------------------------
Bezier_SplineAgregator::~Bezier_SplineAgregator()
{

}

//---------------------------------------------------------------------
void Bezier_SplineAgregator::BuildSplineList(TVecIndexedPoint2f& OutSpline)
{
	TVecActorChildIterator  PA, PB;
/*
	OutSpline.clear();

	PA = GetChildIteratorNext(ChildNodes.end(), "PointBezier2D");
	PB = GetChildIteratorNext(PA, "PointBezier2D");

	if (PA == ChildNodes.end() || PB == ChildNodes.end()){
		return;
	}

	do
	{
		PointBezier2D * PointA = static_cast<PointBezier2D*>(*PA);
		PointBezier2D * PointB = static_cast<PointBezier2D*>(*PB);

		if (!PointA || !PointB){
			return;
		}

		Vector A = PointA->GetPivot().t;
		Vector B = (PointA->GetPoint2() == NULL) ? PointA->GetPivot().t : (PointA->GetPivot() * PointA->GetLTMKey1()).t;
		Vector D = PointB->GetPivot().t;
		Vector C = (PointB->GetPoint1() == NULL) ? PointB->GetPivot().t : (PointB->GetPivot() * PointB->GetLTMKey0()).t;

		for (size_t Index = 0; Index < GetDetail(); ++Index)
		{
			Vector Result;
			Cubic_Bezier_Spline(D, C, B, A, (float)Index / (float)GetDetail(), Result);

			IndexedPoint2f Point(Vector2f(Result.x, Result.y), Vector2f(0.f, 0.f), (unsigned int)OutSpline.size());
			OutSpline.push_back(Point);
		}

		PA = GetChildIteratorNext(PA, "PointBezier2D");
		PB = GetChildIteratorNext(PB, "PointBezier2D");

	}while (PA != GetChildIteratorNext(ChildNodes.end(), "PointBezier2D"));
	*/
}

//---------------------------------------------------------------------
bool Bezier_SplineAgregator::IsChildFocused()
{
 /*	CActor * FocusedChild = GetFocused();
 
 	for (TVecActorChildIterator Iter = ChildNodes.begin(); Iter != ChildNodes.end(); ++Iter) 
 	{
 		if ((*Iter)->GetType() == "PointBezier2D")
 		{
 			if ((*Iter) == FocusedChild){
 				return true;
 			}
 			
 			for (TVecActorChildIterator IterChild = (*Iter)->ChildNodes.begin(); IterChild != (*Iter)->ChildNodes.end(); ++IterChild) 
 			{
 				if ((*IterChild)->GetType() == "Point2D" && *IterChild == FocusedChild){
 					return true;
 				}
 			}
 		}
 	}*/
	return false;
}

//---------------------------------------------------------------------
bool Bezier_SplineAgregator::InsertPoint(const Vector2f& MousePos, const UIScreenObject* Inserter)
{
/*	ViewportInterface * VInterface = ActorAllocator::GetViewportInterface(this);

	if (VInterface)
	{
		Matrix IWTM;
		invert(IWTM, Inserter->GetTransformedWTM_());
		Vector2f ViewAxisX = (IWTM.__row0 * Inserter->GetSize().x) * (1.f / VInterface->GetViewportViewMatrix().__row0.Length());

		// get point in subscriber space
		Matrix ALTM;
		if (CActor::GetObjectAInLocalSpaceB(ALTM, this, Inserter))
		{
			// check intersection
			int Index = 0;
			TVecIndexedPoint2fIterator Iter = GetSplineList().begin();

			while (Iter != GetSplineList().end())
			{
				TVecIndexedPoint2fIterator IterNext = GetIteratorNext(GetSplineList(), Iter);

				float mu1, mu2;
				Vector2f Position;

				if (SegmentSphereIntersection2D(Iter->Point, IterNext->Point, ALTM.t2, ViewAxisX.x, &mu1, &mu2))
				{
					if (mu1 > mu2){
						std::swap(mu1, mu2);
					}

					Vector2f v = IterNext->Point - Iter->Point;
					float d = mu1 + (mu2 - mu1) * 0.5f;
					v = v * d;

					int ControlPointIndex = Index / GetDetail();

					// find real index of control Point2D
					int InsertIndex  = 0;
					int PointCounter = 0;

					TVecActorChildIterator Iterator = ChildNodes.begin();
					while(Iterator != ChildNodes.end())
					{
						if (dynamic_cast<Point2D*>(*Iterator) != NULL) // Point2D
						{
							PointCounter++;
							if (PointCounter == ControlPointIndex + 1){ // find current point counter
								break;
							}
						}
						++InsertIndex;
						++Iterator;// inc. to next child
					}

					CActor * SubObject = NULL;
					SubObject = NEW_OBJECT(PointBezier2D, CActor::ResolveName(std::string("PointBezier2D"), this).c_str(), this);

					SubObject->Initialize();
					SubObject->SetPosition2f(Iter->Point + v);

					v.Normalize();
					SubObject->ChildNodes[0]->SetPosition2f(v * -30.f);
					SubObject->ChildNodes[1]->SetPosition2f(v * 30.f);

					// insert in correct position
					AddChildNode(SubObject, InsertIndex + 1);

					RebuildTransform();
					Rebuild();
					return true;
				}
				++Index;
				++Iter;
			}
		}
	}*/
	return false;
}