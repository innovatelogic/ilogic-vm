#include "reflx.h"
#include <iostream>
#include <memory>
#include <gtest/gtest.h>

struct SNode
{
	typedef std::vector<SNode*>	TChilds;

	SNode(const char *id)
		: __id(id)
	{}

	void Release() {}

    void SetRootNode(SNode *pNode) {}

	SNode* AddChild(SNode *pNode)
	{
		// adds compatibility check
		childs.push_back(pNode);
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
			for (TChilds::iterator Iter = childs.begin(); Iter != childs.end(); ++Iter)
			{
				SNode * FindNode = (*Iter)->Find(key);
				if (FindNode) {
					return FindNode;
				}
			}
		}
		return nullptr;
	}

	TChilds	childs;

	std::string __id;
};

//----------------------------------------------------------------------------------------------
TEST(Rflex, BuildTreeTest)
{
	oes::rflex::CAutoTree<SNode> foo;

	foo.Add("6", "2");
	foo.Add("5", "2");
	foo.Add("3", "1");
	foo.Add("4", "1");
	foo.Add("1", "0");
	foo.Add("2", "0");
}

//----------------------------------------------------------------------------------------------

class A
{
public:
    A()
    {}

    A(int a, int b)
        : m_a(a)
        , m_b(b)
    {}

    int sum() { return m_a + m_b; }

private:
    int m_a;
    int m_b;
};

class B : public A
{
public:
    B(int a, int b, int c)
        : A(a, b)
        , m_c(c)
    {}

    int sum() { return A::sum() + m_c; }

private:
    int m_c;
};

//----------------------------------------------------------------------------------------------
TEST(Rflex, PlacementCtor)
{
    B *b = new B(1, 2, 3);
    A *a0 = new (b) A(4, 5);
    A *a1 = new (b) A();

    EXPECT_EQ((b->sum() + a0->sum() + a1->sum()), 12 + 9 + 9);

    delete b;
}



struct ADT
{
    struct inst : public oes::rflex::itype {
        void *instance() override { return oes::rflex::Generator<ADT, int, int, int>::instance(0, 0, 0); }
    };

    ADT(int a, int b, int c)
        : a(a)
        , b(b)
        , c(c){}

    //void *instance() override { return new ADT(c, b, a); }
    int a, b, c;
};

TEST(Rflex, TypeGenerator_ADT)
{
    std::unique_ptr<ADT> ptr(oes::rflex::Generator<ADT, int, int, int>::instance(0, 1, 2));

    EXPECT_EQ(ptr->a, 0);
    EXPECT_EQ(ptr->b, 1);
    EXPECT_EQ(ptr->c, 2);

    std::unique_ptr<oes::rflex::IGenerator> iptr(new oes::rflex::Generator<ADT, int, int, int>());
    
    ADT *adt = nullptr;
    iptr->instance([&]() {
        
    });

    oes::rflex::itype *type = new ADT::inst();
    ADT *copy = static_cast<ADT*>(type->instance());


}