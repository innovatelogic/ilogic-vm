#include "EventDesc.h"
#include "Actor.h"

template<class T> class DerivedClass;

std::vector< DerivedClass<int>* > VecTest;

template<class T> class BaseClass
{
public:
	T *Value;
};

template<class T> class DerivedClass : public BaseClass<T>
{
public:

};

