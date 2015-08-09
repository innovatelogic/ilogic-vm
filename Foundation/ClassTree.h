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

#ifndef __classtree_h__
#define __classtree_h__

#ifdef WIN32
#pragma once
#endif

#include "platform_specific.h"

class EXPORT AppClassTree
{
public:
	typedef std::vector<class ClassNode*>			TVecClassNodeInterface;
	typedef TVecClassNodeInterface::iterator		TVecClassNodeInterfaceIter;
	typedef TVecClassNodeInterface::const_iterator	TVecClassNodeInterfaceConstIter;

	AppClassTree();
	AppClassTree(const AppClassTree &Source);
	virtual ~AppClassTree();

	bool Add(const char *Type, const char *BaseType);
	bool AddPure(const char *Type, const class Property_Base **Arr, int Count);

	void AddInterface(ClassNode *pInterface);
	
	ClassNode*	Find(const char *Type);
	ClassNode*	FindInterface(const char *Type) const;

	// returns parent node
	ClassNode*	RemoveNode(ClassNode *pNode);

	void		Migrate(AppClassTree *pOther);
	void		Release();
	
	ClassNode*	GetRootNode() const { return m_pRootNode; }

	const TVecClassNodeInterface& GetInterfaces() const { return m_VecInterfaces; }

private:
	TVecClassNodeInterface m_VecInterfaces;	
	
	ClassNode	*m_pRootNode;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<typename T>
struct CEnumerateChildTypeStrategy
{
	CEnumerateChildTypeStrategy(const char *InterfaceName, const ClassNode *pRootNode, T &pStoreType)
	{
		assert(pRootNode && InterfaceName);

		std::stack<const ClassNode*> stack;

		stack.push(pRootNode);

		while (!stack.empty())
		{
			const ClassNode *pTop = stack.top();

			stack.pop();

			for (ClassNode::TVecClassNode::const_iterator Iter = pTop->GetChilds().begin(), IterEnd = pTop->GetChilds().end(); Iter != IterEnd; ++Iter)
			{
				const ClassNode::TVecInterfaces& refInterfaces = (*Iter)->GetInterfaces();

				for (ClassNode::TVecInterfaceConstIter IterItf = refInterfaces.begin(), IterItfEnd = refInterfaces.end(); IterItf != IterItfEnd; ++IterItf)
				{
					if (!strcmp(InterfaceName, (*IterItf)->strType))
					{
						pStoreType.push_back(*Iter);
					}
				}
				stack.push(*Iter);
			}
		};
	}
};

#endif//__classtree_h__