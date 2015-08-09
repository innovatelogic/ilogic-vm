#ifndef  _h_utility_singleton_
#define  _h_utility_singleton_

#ifdef _WIN32
#pragma once
#endif

namespace Utility
{
template <
	class T_VALUE,
    class T_NAMESPACE = T_VALUE 
>
class CSingleton
{

public:
    CSingleton()
	{
        ;// 
	}

	~CSingleton()
	{
		Destroy();
	}

	CSingleton(const T_VALUE& value)
	{
		CSingleton::Instance(value);
	}

	T_VALUE * operator->()
	{
		return CSingleton::GetInstance();
	}

    T_VALUE & operator* ()
	{
		return *CSingleton::GetInstance();
	}
    
	static T_VALUE * GetInstance(const T_VALUE& value)
	{
		CSingleton::Instance(value);
		return CSingleton::m_Value;
	}

	static T_VALUE * GetInstance()
	{
		CSingleton::Instance();
		return CSingleton::m_Value;
	}

	static void Destroy()
	{
		if (CSingleton::m_Value)
		{
			delete CSingleton::m_Value;
			CSingleton::m_Value = 0;
		}
	}

	template<class T_DESTROYER>
	static void Destroy(T_DESTROYER &Destroyer)
	{
		if (CSingleton::m_Value)
		{
			Destroyer(CSingleton::m_Value);
			CSingleton::m_Value = 0;
		}
	}

private:
	
	static void Instance()
	{
		if (CSingleton::m_Value == 0)
		{
			CSingleton::m_Value = new T_VALUE();
		}
	}

    static void Instance(const T_VALUE & value)
	{
		if (CSingleton::m_Value == 0)
		{
             CSingleton::m_Value = new T_VALUE(value);
		}
	}
	static T_VALUE * m_Value;
};

template<
	class T_VALUE, 
	class T_NAMESPACE
>T_VALUE * CSingleton<T_VALUE,T_NAMESPACE>::m_Value = 0;

}
#endif //_h_utility_singleton_