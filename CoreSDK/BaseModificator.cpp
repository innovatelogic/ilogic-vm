#include "coresdkafx.h"

REGISTER_CLASS(BaseModificator, ActorAllocator);

//----------------------------------------------------------------------------------
BaseModificator::BaseModificator(const CObjectAbstract * Parent)
: Super(Parent)
{
	;//
}

//----------------------------------------------------------------------------------
BaseModificator::BaseModificator(const BaseModificator& Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------
BaseModificator::~BaseModificator()
{
	;//
}
