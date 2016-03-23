#pragma once

#include "OEMBase.h"
#include <algorithm>
#include <functional>

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
class COMMON_BASE_EXPORT CAutoTree
{
public:
	CAutoTree()
	{

	}

	virtual ~CAutoTree()
	{
		Release();
	}

	T_CLASS* Add(const char *Type, const char *BaseType)
	{
		if (Type && BaseType)
		{
			T_CLASS *pBase = Find(BaseType);
			T_CLASS *pSiblin = Find(Type);

			if (pBase && pSiblin) // already exist
			{
				// check if already linked
				std::vector<T_CLASS*>::iterator IterFind = pBase->Childs.begin();
				for (; IterFind != pBase->Childs.end(); ++IterFind)
				{
					if (*IterFind == pSiblin){
						break;
					}
				}

				if (IterFind != pBase->Childs.end()) // siblin is not direct child of base need rearrange
				{
					// check Siblin class is in root's
					std::vector<T_CLASS*>::iterator IterSiblin = m_VRoots.begin();
					for (; IterSiblin != m_VRoots.end(); ++IterSiblin)
					{
						if (*IterSiblin == pSiblin){
							break;
						}
					}

					if (IterSiblin != m_VRoots.end())
					{
						m_VRoots.erase(IterSiblin); // rearrange
						pBase->AddChild(pSiblin);
					}
				}
				return pBase;
			}
			else if (!pBase && !pSiblin) // add plain
			{
				//     !Base   <- add root
				//       |
				//    !Siblin  <- add

				T_CLASS *pBaseClass = new T_CLASS(BaseType);
				T_CLASS *pInheritClass = pBaseClass->AddChild(new T_CLASS(Type));

				m_VRoots.push_back(pBaseClass);

				return pInheritClass;
			}
			else
			{
				if (pBase && !pSiblin)
				{
					//      Base
					//       |
					//    !Siblin  <- add

					T_CLASS *pInheritClass = new T_CLASS(Type);

					pBase->AddChild(pInheritClass);

					return pInheritClass;
				}
				else if (!pBase && pSiblin)
				{
					//allocate new base
					T_CLASS *pBaseClass = new T_CLASS(BaseType);

					// check Siblin class is in root's
					std::vector<T_CLASS*>::iterator IterFind = m_VRoots.begin();
					for (; IterFind != m_VRoots.end(); ++IterFind)
					{
						if (!strcmp((*IterFind)->GetName(), Type)){
							break;
						}
					}

					assert(IterFind != m_VRoots.end());

					if (IterFind != m_VRoots.end())
					{
						(*IterFind) = pBaseClass; // remap in roots
						pBaseClass->AddChild(pSiblin);
					}
					return pSiblin;
				}
			}
		}
		else
		{
			if (Type && !Find(Type))
			{
				T_CLASS *pClass = new T_CLASS(Type);
				m_VRoots.push_back(pClass);
				return pClass;
			}

			if (BaseType && !Find(BaseType))
			{
				T_CLASS *pClass = new T_CLASS(BaseType);
				m_VRoots.push_back(pClass);
				return pClass;
			}
		}
		return 0;
	}

	T_CLASS* Find(const char *Type)
	{
		for (std::vector<T_CLASS*>::iterator Iter = m_VRoots.begin(); Iter != m_VRoots.end(); ++Iter)
		{
			if (T_CLASS *FindNode = (*Iter)->Find(Type))
			{
				return FindNode;
			}
		}
		return 0;
	}

	void Release()
	{
		std::for_each(m_VRoots.begin(), m_VRoots.end(), std::mem_fun(&T_CLASS::Release));
		m_VRoots.clear();
	}

private:
	std::vector<T_CLASS*> m_VRoots;
};