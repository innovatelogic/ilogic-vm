#include "SplineCollision2DModificator_Brush.h"

REGISTER_CLASS(SplineCollision2DModificator_Brush, SplineCollision2DModificator);

//----------------------------------------------------------------------------------
SplineCollision2DModificator_Brush::SplineCollision2DModificator_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
}

//----------------------------------------------------------------------------------
SplineCollision2DModificator_Brush::~SplineCollision2DModificator_Brush()
{

}