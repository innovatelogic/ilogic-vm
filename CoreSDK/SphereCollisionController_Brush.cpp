#include "coresdkafx.h"

REGISTER_CLASS(SphereCollisionController_Brush, CSphereCollisionController);

//----------------------------------------------------------------------------------
SphereCollisionController_Brush::SphereCollisionController_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	Set_FROZEN_POS_Z(true);
	Set_FROZEN_ROT_X(true);
	Set_FROZEN_ROT_Y(true);
}

//----------------------------------------------------------------------------------
SphereCollisionController_Brush::SphereCollisionController_Brush(const SphereCollisionController_Brush &Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------
SphereCollisionController_Brush::~SphereCollisionController_Brush()
{

}