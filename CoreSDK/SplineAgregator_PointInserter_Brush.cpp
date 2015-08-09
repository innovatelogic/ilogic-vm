#include "coresdkafx.h"

REGISTER_CLASS(SplineAgregator_PointInserter_Brush, UIScreenObject);

//----------------------------------------------------------------------------------
SplineAgregator_PointInserter_Brush::SplineAgregator_PointInserter_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
	SetVisible(false);
//	SetSize(Vector2f(7.f, 7.f));
}

//----------------------------------------------------------------------------------
SplineAgregator_PointInserter_Brush::~SplineAgregator_PointInserter_Brush()
{

}

//----------------------------------------------------------------------------------
bool SplineAgregator_PointInserter_Brush::DoEventPressed(const MouseInputData& InputData)
{
/*	if (InputData.event == MOUSE_Pressed && InputData.Code == MOUSE_LEFT)
	{
		TMapSubscribersIterator Iter = Subscribers.begin();

		while (Iter != Subscribers.end())
		{
			return dynamic_cast<SplineAgregator*>(Iter->first)->InsertPoint(InputData.MousePos, this);
		}
	}
	else if (InputData.event == MOUSE_Pressed && InputData.Code == MOUSE_RIGHT)
	{
		//KillFocus();
	}*/
	return true;
}

//----------------------------------------------------------------------------------
void SplineAgregator_PointInserter_Brush::DoDraw()
{

}

//----------------------------------------------------------------------------------
void SplineAgregator_PointInserter_Brush::DrawBounds() const
{
}
