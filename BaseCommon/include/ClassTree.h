#pragma once

#include "OEMBase.h"
#include "ClassNode.h"
#include <vector>
#include <list>

class COMMON_BASE_EXPORT AppClassTree
{
public:
	typedef std::vector<ClassNode*>			        TVecClassNodeInterface;
	typedef TVecClassNodeInterface::iterator		TVecClassNodeInterfaceIter;
	typedef TVecClassNodeInterface::const_iterator	TVecClassNodeInterfaceConstIter;

	AppClassTree();
	AppClassTree(const AppClassTree &Source);
	virtual ~AppClassTree();

	bool Add(const char *type, const char *baseType);
	bool AddPure(const char *type, const class Property_Base **arr, int count);

	void AddInterface(ClassNode *pInterface);
	
	ClassNode*	Find(const char *type);
	ClassNode*	FindInterface(const char *type) const;

	// returns parent node
	ClassNode*	RemoveNode(ClassNode *pNode);

	void		Migrate(AppClassTree *pOther);
	void		Release();
	
	ClassNode*	GetRootNode() const { return m_pRootNode; }

	const TVecClassNodeInterface& GetInterfaces() const { return m_VecInterfaces; }

    bool Add2(const char *type, const char *baseType);

private:
	TVecClassNodeInterface m_VecInterfaces;	
	
	ClassNode	*m_pRootNode;

    std::list<ClassNode*> m_roots;
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