#include "coresdkafx.h"

REGISTER_CLASS(BoxCollisionController_Brush,BoxCollisionController);

//----------------------------------------------------------------------------------
BoxCollisionController_Brush::BoxCollisionController_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	Set_FROZEN_POS_Z(true);
	Set_FROZEN_ROT_X(true);
	Set_FROZEN_ROT_Y(true);
}

//----------------------------------------------------------------------------------
BoxCollisionController_Brush::BoxCollisionController_Brush(const BoxCollisionController_Brush &Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------
BoxCollisionController_Brush::~BoxCollisionController_Brush()
{

}