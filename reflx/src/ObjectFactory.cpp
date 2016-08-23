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
namespace oes
{
    namespace rflex
    {
        TClassFactory* GetClassFactoryA()
        {
            return TGlobalClassFactory::GetInstance();
        }
    }
}