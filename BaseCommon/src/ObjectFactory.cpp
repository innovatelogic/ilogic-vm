#include "ObjectFactory.h"
#include "IObjectAbstract.h"
/*
extern "C"
{
	EXTERN_EXPORT TClassFactory* GetClassFactoryA()
	{
		return GetClassFactoryA();
	}
}*/

//----------------------------------------------------------------------------------------------
namespace NObjectFactory
{
    NObjectFactory::TClassFactory* GetClassFactoryA()
    {
    return NObjectFactory::TGlobalClassFactory::GetInstance();
    }
}