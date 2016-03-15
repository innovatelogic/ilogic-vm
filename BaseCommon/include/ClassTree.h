#pragma once

#include "OEMBase.h"
#include "ClassNode.h"
#include <vector>

class COMMON_BASE_EXPORT AppClassTree
{
public:
	typedef std::vector<ClassNode*>			TVecClassNodeInterface;
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
struct COMMON_BASE_EXPORT CEnumerateChildTypeStrategy
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