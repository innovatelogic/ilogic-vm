#pragma once

//namespace oes
//{
    namespace Utility
    {
        template <
            typename T_VALUE
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

            T_VALUE* operator->()
            {
                return CSingleton::GetInstance();
            }

            T_VALUE& operator* ()
            {
                return *CSingleton::GetInstance();
            }

            static T_VALUE* GetInstance(const T_VALUE& value)
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
                    CSingleton::m_Value = nullptr;
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
                if (CSingleton::m_Value == nullptr) {
                    CSingleton::m_Value = new T_VALUE();
                }
            }

            static void Instance(const T_VALUE& value)
            {
                if (CSingleton::m_Value == nullptr) {
                    CSingleton::m_Value = new T_VALUE(value);
                }
            }
            static T_VALUE *m_Value;
        };

        template<
            class T_VALUE
        >T_VALUE* CSingleton<T_VALUE>::m_Value = 0;
    }
//}