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

#include "OEMBase.h"
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
	class COMMON_BASE_EXPORT CClassFactory
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

			ClassNode* Base = ClassTree.Find(BaseClassName);
			ClassNode* Siblin = ClassTree.Find(ClassName);

			if (!Base || !Siblin)
			{
				if (!Base && !Siblin) // add plain
				{
					//     !Base
					//       |
					//    !Siblin 

					ClassNode* BaseClass = new ClassNodeType<CLASS_BASE>(BaseClassName);
					ClassNode* InheritClass = BaseClass->AddChild(new ClassNodeType<T_CLASS>(ClassName));

					ClassTree.GetRootNode()->AddChild(BaseClass);

					BaseClass->SetRootNode(ClassTree.GetRootNode());
					InheritClass->SetRootNode(BaseClass);
				}
				else
				{
					if (!Base && Siblin)
					{
						//     Parent
						//       |
						//     !Base
						//       |
						//     Siblin 

						//allocate new base
						ClassNode* BaseClass = new ClassNodeType<CLASS_BASE>(BaseClassName);

						// connect siblings
						BaseClass->Childs.push_back(Siblin);

						// disconnect sibling
						ClassNode* ParentNode = ClassTree.RemoveNode(Siblin);

						ParentNode->Childs.push_back(BaseClass);

						BaseClass->SetRootNode(ParentNode);
						Siblin->SetRootNode(BaseClass);
					}
					else // Base && !Sibling
					{
						ClassNode* InheritClass = new ClassNodeType<T_CLASS>(ClassName);
						Base->AddChild(InheritClass);
						InheritClass->SetRootNode(Base);
					}
				}
			}
			else if (Base && Siblin)// reposition
			{
				if (Siblin->GetRootNode() && Siblin->GetRootNode() != Base)
				{
					ClassNode* RightBase = ClassTree.Find(BaseClassName);
					if (RightBase)
					{
						// disconnect
						Siblin->GetRootNode()->RemoveNode(Siblin);
						Siblin->SetRootNode(Base);
						Base->AddChild(Siblin);
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			return m_MapGenerator.size();
		}
	
//----------------------------------------------------------------------------------------------
		template <typename T_CLASS, typename CLASS_BASE>
		void RegisterPure(const char* TypeName, const Property_Base** Arr, int Count)
		{
			ClassNode* InheritClass = ClassTree.Find(TypeName);

			if (!InheritClass)
			{
				InheritClass = new ClassNodeType<T_CLASS>(TypeName);
				ClassTree.GetRootNode()->AddChild(InheritClass);
				InheritClass->SetRootNode(ClassTree.GetRootNode());
			}

			InheritClass->SetProprties(Arr, Count);
		}

//----------------------------------------------------------------------------------------------
		template <typename T_CLASS, typename CLASS_BASE>
		void RegisterPure2(const char* TypeName, const IPropertiesAllocator * PropAlloc)
		{
			ClassNode* pInheritClass = ClassTree.Find(TypeName);

			if (!pInheritClass)
			{
				pInheritClass = new ClassNodeType<T_CLASS>(TypeName);

				assert(pInheritClass);

				ClassTree.GetRootNode()->AddChild(pInheritClass);
				pInheritClass->SetRootNode(ClassTree.GetRootNode());
			}

			pInheritClass->SetProprties(PropAlloc);
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
