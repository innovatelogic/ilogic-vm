#include "coresdkafx.h"

REGISTER_CLASS(PointBezier2D, Point2D);

//---------------------------------------------------------------------
PointBezier2D::PointBezier2D(const CObjectAbstract * Parent)
: Super(Parent)
, Point1(NULL)
, Point2(NULL)
{
	NEW_OBJECT_TRANSIENT_CHILD(Point1, Point2DBezierKey, "Point1", this);
	NEW_OBJECT_TRANSIENT_CHILD(Point2, Point2DBezierKey, "Point2", this);

	//REGISTER_EVENT_(Point1, this, boost::bind<bool>(&PointBezier2D::KeyUpdated, this, _1, _2), Event_PreRelease);
	//REGISTER_EVENT_(Point2, this, boost::bind<bool>(&PointBezier2D::KeyUpdated, this, _1, _2), Event_PreRelease);
}

//---------------------------------------------------------------------
PointBezier2D::~PointBezier2D()
{

}

//---------------------------------------------------------------------
void PointBezier2D::DoDraw()
{
/*	CActor * Parent = GetParent();
	
	if (Parent)
	{
		bool bRenderExt = Parent->IsFocused() || Parent->GetExtraSelection();

		for (TVecActorChildIterator Iter = Parent->ChildNodes.begin(); !bRenderExt && Iter != Parent->ChildNodes.end(); ++Iter) 
		{
			if ((*Iter)->GetType() == GetType())
			{
				for (TVecActorChildIterator IterChild = (*Iter)->ChildNodes.begin(); IterChild != (*Iter)->ChildNodes.end(); ++IterChild)
				{
					if ((*IterChild)->IsFocused() && (*IterChild)->GetType() == "Point2D")
					{
						bRenderExt = true;
						break;
					}
				}

				if ((*Iter)->IsFocused())
				{
					bRenderExt = true;
					break;
				}
			}
		}

		if (IsFocused() || bRenderExt)
		{	
			TVecActorChildIterator  Point1, Point2;
			Point1 = GetChildIteratorNext(ChildNodes.end(), "Point2DBezierKey");
			Point2 = GetChildIteratorNext(Point1, "Point2DBezierKey");

			Matrix IWTM;
			invert(IWTM, WorldMatrixTransform);

			if (Point1 != ChildNodes.end()){
				GetRenderComponent()->DrawLine(Vector2f(WorldMatrixTransform.t2.x, WorldMatrixTransform.t2.y), 
												WorldMatrixTransform.t2 + transform_coord((*Point1)->GetPosition2f(), IWTM), 0xff0100ff);
			}
			if (Point1 != ChildNodes.end()){
				GetRenderComponent()->DrawLine(Vector2f(WorldMatrixTransform.t2.x, WorldMatrixTransform.t2.y), 
												WorldMatrixTransform.t2 + transform_coord((*Point2)->GetPivot().t2, IWTM), 0xff0100ff);
			}
		}
	}
	
	Super::DoDraw();
	*/
}

//---------------------------------------------------------------------
Vector PointBezier2D::GetPositionKey0()
{
	/*if (Point1 != NULL){
		return Point1->GetPosition();
	}*/
	return GetPosition_();
}

//---------------------------------------------------------------------
Vector PointBezier2D::GetPositionKey1()
{
/*	if (Point2 != NULL){
		return Point2->GetPosition();
	}*/
	return GetPosition_();
}

//---------------------------------------------------------------------
Matrix PointBezier2D::GetLTMKey0()
{
	/*if (Point1 != NULL){
		return Point1->GetPivot();
	}*/
	return GetPivot_();
}

//---------------------------------------------------------------------
Matrix PointBezier2D::GetLTMKey1()
{
	/*if (Point2 != NULL){
		return Point2->GetPivot();
	}*/
	return GetPivot_();
}

//---------------------------------------------------------------------
bool PointBezier2D::KeyUpdated(const CActor * Sender, ESystemEventID EventId)
{
	if (EventId == Event_PreRelease)
	{
		if (Sender == Point1){
			Point1 = NULL;
		}
		if (Sender == Point2){
			Point2 = NULL;
		}
	}
	return true;
}

//---------------------------------------------------------------------
/*bool PointBezier2D::ProcessControllers(const Vector2f& Position)
{
	if (GetParent() && (GetParent()->IsFocused() || GetParent()->GetExtraSelection()))
	{	
		return Super::ProcessControllers(Position);
	}
	return false;
}*/