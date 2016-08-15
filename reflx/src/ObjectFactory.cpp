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
    namespace common_base
    {
        oes::common_base::TClassFactory* GetClassFactoryA()
        {
            return oes::common_base::TGlobalClassFactory::GetInstance();
        }
    }
}