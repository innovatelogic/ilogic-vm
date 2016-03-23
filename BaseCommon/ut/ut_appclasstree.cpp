#include <iostream>
#include <gtest/gtest.h>
#include "AutoTree.h"

struct SNode
{
	typedef std::vector<SNode*>	TChilds;

	SNode(const char *id)
		: __id(id)
	{}

	void Release() {}

	SNode* AddChild(SNode *pNode)
	{
		// adds compatibility check
		Childs.push_back(pNode);
		return pNode;
	}

	const char* GetName() const { return __id.c_str();  }

	//----------------------------------------------------------------------------------------------
	SNode* Find(const char *key)
	{
		if (__id.compare(key) == 0) {
			return this;
		}
		else
		{
			for (TChilds::iterator Iter = Childs.begin(); Iter != Childs.end(); ++Iter)
			{
				SNode * FindNode = (*Iter)->Find(key);
				if (FindNode) {
					return FindNode;
				}
			}
		}
		return nullptr;
	}

	TChilds	Childs;

	std::string __id;
};

//----------------------------------------------------------------------------------------------
TEST(TestUtils, BuildTreeTest)
{
	CAutoTree<SNode> foo;

	foo.Add("6", "2");
	foo.Add("5", "2");
	foo.Add("3", "1");
	foo.Add("4", "1");
	foo.Add("1", "0");
	foo.Add("2", "0");

}
