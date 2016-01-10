#include "UIPanel.h"
#include "MessageMap.h"

//-----------------------------------------------------------------------------
UIPanel::UIPanel(const CObjectAbstract * Parent)
: Super(Parent)
{
	
}

//-----------------------------------------------------------------------------
UIPanel::UIPanel(const UIPanel &Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//-----------------------------------------------------------------------------
UIPanel::~UIPanel()
{

}
