#include "SplineCollision2DModificator_Brush.h"

REGISTER_CLASS(SplineCollision2DModificator_Brush, SplineCollision2DModificator);

//----------------------------------------------------------------------------------
SplineCollision2DModificator_Brush::SplineCollision2DModificator_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
}

//----------------------------------------------------------------------------------
SplineCollision2DModificator_Brush::SplineCollision2DModificator_Brush(const SplineCollision2DModificator_Brush& Source)
: Super(Source)
{
	if (&Source != this)
	{

	}
}

//----------------------------------------------------------------------------------
SplineCollision2DModificator_Brush::~SplineCollision2DModificator_Brush()
{

}