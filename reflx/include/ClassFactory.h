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

#ifndef __classfactory_h__
#define __classfactory_h__

#include "defexport.h"
#include "ClassTree.h"
#include "ClassNode.h"
#include "ClassNodeInterface.h"
#include "ClassNodeType.h"
#include "singleton.h"
#include <map>

#ifdef _WIN32
#pragma once
#endif

namespace Utility
{
	template<class T> 
	struct CPoliceNullPtr
	{
		static T Police()
		{
			return static_cast<T>(0);
		}
	};

	template < 
		class T_BASE,
		class T_GEN_FUNCTION,
		class T_COPY_FUNCTION,
		template<class> class T_POLICE_DEFAULT = CPoliceNullPtr	
		> 
	class REFLX_EXPORT CClassFactory
	{
		typedef T_BASE          TBase;
		typedef T_GEN_FUNCTION  TGenFunction;
		typedef T_COPY_FUNCTION TCloneFunction;

		class AutoManager
		{
		public:
			AutoManager(TGenFunction gen, TCloneFunction clone)
				: GenFunc(gen)
				, CloneFunc(clone)
			{
			}
			TGenFunction	GenFunc;
			TCloneFunction	CloneFunc;
		};

	public:
		typedef std::map<std::string, AutoManager>		TMapGenerator;
		typedef std::map<std::string, size_t>			TMapId;

		void Release()
		{
			ClassTree.Release();
		}

		void _erase(const char* name)
		{
			m_MapGenerator.erase(name);
		}

		template <typename T_CLASS, typename CLASS_BASE>
		size_t Register(const char* Key, TGenFunction GenFunc, TCloneFunction CloneFunc, const char * ClassName, const char * BaseClassName)
		{
			TMapGenerator::iterator Iter = m_MapGenerator.find(Key);

			if (Iter == m_MapGenerator.end()){
				m_MapGenerator.insert(std::make_pair(Key, AutoManager(GenFunc, CloneFunc)));
			}

            ClassTree.Add(ClassName, BaseClassName);

			return m_MapGenerator.size();
		}
	
//----------------------------------------------------------------------------------------------
		template <typename T_CLASS, typename CLASS_BASE>
		void RegisterPure(const char* typeName, const Property_Base** arr, int count)
		{
            ClassTree.Add(typeName, arr, count);
		}

//----------------------------------------------------------------------------------------------
		template <typename T_CLASS, typename CLASS_BASE>
		void RegisterPure2(const char *type, const IPropertiesAllocator *prop)
		{
            ClassTree.Add(type, prop);
		}

//----------------------------------------------------------------------------------------------
		template <typename T_CLASS>
		void RegisterInterface(const char *TypeName, const IPropertiesAllocator *PropAlloc)
		{
			ClassNode *pInterface = ClassTree.FindInterface(TypeName);

			if (!pInterface)
			{
				pInterface = new CClassNodeInterface<T_CLASS>(TypeName);

				assert(pInterface);

				ClassTree.AddInterface(pInterface);
			}
			pInterface->SetProprties(PropAlloc);
		}

//----------------------------------------------------------------------------------------------
		inline TBase* Generation(const char *Type, const char* Name, TBase* Parent) const
		{
			typename TMapGenerator::const_iterator it_f = m_MapGenerator.find(Type);

			if (it_f != m_MapGenerator.end()){
				return it_f->second.GenFunc(Name, Parent);
			}
			return static_cast<TBase*>(T_POLICE_DEFAULT<TBase*>::Police());
		}
//----------------------------------------------------------------------------------------------
		inline TBase* Clone(TBase* Source, TBase* Parent) const
		{
			typename TMapGenerator::const_iterator it_f = m_MapGenerator.find(Source->GetType());

			if (it_f != m_MapGenerator.end()){
				return it_f->second.CloneFunc(Source, Parent);
			}
			return static_cast<TBase*>(T_POLICE_DEFAULT<TBase*>::Police());
		}
//----------------------------------------------------------------------------------------------
		inline TBase* SliceClone(TBase* Source, const char *NewType)
		{
			typename TMapGenerator::const_iterator it_f = m_MapGenerator.find(NewType);

			if (it_f != m_MapGenerator.end())
			{
				TBase *NewObject = it_f->second.CloneFunc(Source, nullptr);
				return NewObject;
			}

			return static_cast<TBase*>(T_POLICE_DEFAULT<TBase*>::Police());
		}
//----------------------------------------------------------------------------------------------
		inline size_t GetId(const char* Key)
		{
			static size_t Id = 0;

			TMapId::const_iterator IterFind = MapId.lower_bound(Key);

			if (IterFind == MapId.end())
			 {
				 MapId.insert(std::make_pair(Key,++Id));
				 return Id;
			 }
			return IterFind->second;
		}
//----------------------------------------------------------------------------------------------
		inline std::string GetThisClassName(size_t id) const
		{
			TMapId::const_iterator Iter = MapId.begin();

			while (Iter != MapId.end())
			{
				if (Iter->second == id)
				{
					return Iter->first;
				}
				++Iter;
			}
			return "UNDEF_CLASS";
		}
//----------------------------------------------------------------------------------------------
		inline AppClassTree & GetClassTree() { return ClassTree; }

//----------------------------------------------------------------------------------------------
	public:
 		class CAuto
 		{
 			typedef CSingleton<CClassFactory, CAuto> TGlobalFactory;
 
 		public:
 			CAuto(const char *Key, TGenFunction Func, TCloneFunction CloneFunc, const char * ClassName, const char * BaseClassName)
 			{
 				TGlobalFactory::GetInstance()->Register(Key, Func, CloneFunc, ClassName, BaseClassName);
 			}

			CAuto(const char *Key, std::vector<class Property_Base*> &VecPropInfo)
			{
				TGlobalFactory::GetInstance()->RegisterPure(Key, VecPropInfo);
			}
 		};

		typedef CSingleton<CClassFactory, CAuto> TGlobalFactory;

	//private:
		TMapGenerator	m_MapGenerator;
		AppClassTree	ClassTree;
		TMapId			MapId;
	};
}
#endif//__classfactory_h__
