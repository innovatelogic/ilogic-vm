#include "coresdkafx.h"

//----------------------------------------------------------------------------------
BaseModificatorUI::BaseModificatorUI(const CObjectAbstract * Parent)
: Super(Parent)
{
	;//
}

//----------------------------------------------------------------------------------
BaseModificatorUI::BaseModificatorUI(const BaseModificatorUI& Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------
BaseModificatorUI::~BaseModificatorUI()
{
	;//
}