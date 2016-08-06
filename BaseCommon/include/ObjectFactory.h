//----------------------------------------------------------------------------------------------
// OpenES: Open Entertainment System
// Copyright (C) 2010  Yura Gunko email: yura.gunko@gmail.com
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------------------------

#ifndef __h_object_factory__
#define __h_object_factory__

#ifdef _WIN32
#pragma once
#endif

#include "OEMBase.h"
#include "classfactory.h"
#include "IObjectAbstract.h"
#include <assert.h>

/**
 *  Class factory 
 */
namespace oes
{
    namespace common_base
    {
        // define generate function declaration
        typedef class IObjectAbstract* (*TGenFunction) (const char *pName, const IObjectAbstract *pParent);
        typedef class IObjectAbstract* (*TCloneFunction) (const IObjectAbstract *pSource, const IObjectAbstract *pParent);

        // define class factory with specified types
        typedef Utility::CClassFactory <
            IObjectAbstract,
            oes::common_base::TGenFunction,
            oes::common_base::TCloneFunction
        > TClassFactory;

        // define specified global class factory
        typedef TClassFactory::TGlobalFactory TGlobalClassFactory;

        extern "C"
        {
            COMMON_BASE_EXPORT oes::common_base::TClassFactory* GetClassFactoryA();
        }

        //----------------------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------------------
        static size_t GetId(const char *pKey)
        {
            TClassFactory *classFactory = GetClassFactoryA();
            return classFactory->GetId(pKey);
        }

        //----------------------------------------------------------------------------------------------
        template <typename CLASS_T, typename CLASS_BASE>
        static void GlobalRegister(const char *Key, TGenFunction Func, TCloneFunction CloneFunc, const char *ClassName, const char *BaseClassName)
        {
            TClassFactory *classFactory = GetClassFactoryA();

            classFactory->Register<CLASS_T, CLASS_BASE>(Key, Func, CloneFunc, ClassName, BaseClassName);
        }

        //----------------------------------------------------------------------------------------------
        template <typename CLASS_T, typename CLASS_BASE>
        static void GlobalRegisterPure(const char *TypeName, const class Property_Base** Arr, int Count)
        {
            TClassFactory *classFactory = GetClassFactoryA();

            classFactory->RegisterPure<CLASS_T, CLASS_BASE>(TypeName, Arr, Count);
        }

        //----------------------------------------------------------------------------------------------
        template <typename CLASS_T, typename CLASS_BASE>
        static void GlobalRegisterPure2(const char *TypeName, const class IPropertiesAllocator * PropAlloc)
        {
            TClassFactory * classFactory = GetClassFactoryA();

            classFactory->RegisterPure2<CLASS_T, CLASS_BASE>(TypeName, PropAlloc);
        }

        //----------------------------------------------------------------------------------------------
        template <typename CLASS_T>
        static void GlobalRegisterInterface(const char *TypeName, const class IPropertiesAllocator *PropAlloc)
        {
            TClassFactory *classFactory = GetClassFactoryA();

            classFactory->RegisterInterface<CLASS_T>(TypeName, PropAlloc);
        }

        //----------------------------------------------------------------------------------------------
        static IObjectAbstract* GenObject(const char *Type, const char *Name, IObjectAbstract * Parent = 0)
        {
            TClassFactory *classFactory = GetClassFactoryA();

            IObjectAbstract *pAbstract = classFactory->Generation(Type, Name, Parent);

            assert(pAbstract);
            return pAbstract;
        }

        //----------------------------------------------------------------------------------------------
        static IObjectAbstract * CloneObject(const IObjectAbstract *Source, IObjectAbstract *NewParent = nullptr)
        {
            TClassFactory *classFactory = GetClassFactoryA();

            IObjectAbstract *pAbstract = classFactory->Clone(const_cast<IObjectAbstract*>(Source), NewParent);
            assert(pAbstract);

            return pAbstract;
        }

        //----------------------------------------------------------------------------------------------
        static IObjectAbstract* SliceCloneObject(const IObjectAbstract *Source, const char *NewType)
        {
            TClassFactory *classFactory = GetClassFactoryA();

            IObjectAbstract *pAbstract = classFactory->SliceClone(const_cast<IObjectAbstract*>(Source), NewType);
            assert(pAbstract);

            return pAbstract;
        }

        //----------------------------------------------------------------------------------------------
        static std::string GetThisClassName(size_t id)
        {
            TClassFactory *classFactory = GetClassFactoryA();
            return classFactory->GetThisClassName(id);
        }

        //----------------------------------------------------------------------------------------------
        static AppClassTree& GetClassTree()
        {
            TClassFactory * classFactory = GetClassFactoryA();
            return classFactory->GetClassTree();
        }

        //----------------------------------------------------------------------------------------------
        //	auto registration class
        //----------------------------------------------------------------------------------------------
        template <class CLASS_T, class CLASS_BASE> class CAuto
        {
        public:
            CAuto(const char *Key, TGenFunction Func, TCloneFunction CopyFunc, const char * ClassName, const char * BaseClassName)
            {
                oes::common_base::GlobalRegister<CLASS_T, CLASS_BASE>(Key, Func, CopyFunc, ClassName, BaseClassName);
            }

            CAuto(const char* TypeName, const class Property_Base** Arr, int Count)
            {
                oes::common_base::GlobalRegisterPure<CLASS_T, CLASS_BASE>(TypeName, Arr, Count);
            }
        };

        //----------------------------------------------------------------------------------------------
        //	auto registration class
        //----------------------------------------------------------------------------------------------
        template <class CLASS_T, class CLASS_BASE, class T_PROPERTY_CLASS> class CAutoEx
        {
        public:
            CAutoEx(const char *Key, TGenFunction Func, TCloneFunction CopyFunc, const char * ClassName, const char * BaseClassName)
            {
                oes::common_base::GlobalRegister<CLASS_T, CLASS_BASE>(Key, Func, CopyFunc, ClassName, BaseClassName);

                T_PROPERTY_CLASS* pPropertyClass = new T_PROPERTY_CLASS();

                assert(pPropertyClass);

                oes::common_base::GlobalRegisterPure2<CLASS_T, CLASS_BASE>(Key, pPropertyClass);
            }
        };

        //----------------------------------------------------------------------------------------------
        template <class CLASS_T, class CLASS_BASE, class T_PROPERTY_CLASS> class CAutoPureEx
        {
        public:
            CAutoPureEx(const char *Key, TGenFunction Func, TCloneFunction CopyFunc, const char * ClassName, const char * BaseClassName)
            {
                T_PROPERTY_CLASS* pPropertyClass = new T_PROPERTY_CLASS();

                assert(pPropertyClass);

                oes::common_base::GlobalRegisterPure2<CLASS_T, CLASS_BASE>(Key, pPropertyClass);
            }
        };

        //----------------------------------------------------------------------------------------------
        template <class CLASS_T, class T_PROPERTY_CLASS> class CAutoInterface
        {
        public:
            CAutoInterface(const char *pKey)
            {
                T_PROPERTY_CLASS *pPropertyClass = new T_PROPERTY_CLASS();

                assert(pPropertyClass);

                oes::common_base::GlobalRegisterInterface<CLASS_T>(pKey, pPropertyClass);
            }
        };
    }
}

#endif //__h_object_factory__