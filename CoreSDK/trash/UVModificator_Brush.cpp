#include "UVModificator_Brush.h"

REGISTER_CLASS(UVModificator_Brush, UVModificator);

//----------------------------------------------------------------------------------
UVModificator_Brush::UVModificator_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
	SetVisible(false);
}

//----------------------------------------------------------------------------------
UVModificator_Brush::~UVModificator_Brush()
{

}