#include "UIFilter_Brush.h"

REGISTER_CLASS(UIFilter_Brush, UIFilter);

//----------------------------------------------------------------------------------
UIFilter_Brush::UIFilter_Brush(const CObjectAbstract * Parent)
: Super(Parent)
{
	SetTransient(true);
	SetVisible(false);
}

//----------------------------------------------------------------------------------
UIFilter_Brush::UIFilter_Brush(const UIFilter_Brush& Source)
: Super(Source)
{
	if (&Source != this)
	{

	}
}

//----------------------------------------------------------------------------------
UIFilter_Brush::~UIFilter_Brush()
{

}