#pragma once

#include <gtest/gtest.h>
#include "TArray.h"
#include <vector>
#include <stack>

class TestMapTest : public ::testing::Test
{
    struct TTestValue;

    struct TTestKey
    {
        TTestKey(const std::string &name, TTestValue *value)
            : m_name(name)
            , m_value(value)
        {
            assert(m_value);
            m_value->m_key = this;
        }
        std::string m_name;

        TTestValue *m_value;
    };

    struct TTestValue
    {
        TTestValue(const std::string &name, TTestValue *parent)
            : m_name(name)
            , m_parent(parent)
            , m_key(nullptr)
        {}

        void AddChild(TTestValue *val) { m_childs.push_back(val); }

        std::string m_name;
        TTestValue  *m_parent;

        TTestKey    *m_key; // pointer to key

        std::vector<TTestValue*> m_childs;
    };

protected:
    using TestTreeMap = oes::common_base::TTreeMap<TTestKey, TTestValue>;
    using TestTreeNode = oes::common_base::TNodeMap<TTestKey, TTestValue>;

private:
    //----------------------------------------------------------------------------------------------
    void SetUp() override
    {
        BuidTree();
    }

    //----------------------------------------------------------------------------------------------
    void TearDown() override
    {
        ClearTree();
    }

    //----------------------------------------------------------------------------------------------
    TestTreeNode* RegisterNode(const TTestKey *key, const TTestKey *parent)
    {
        return m_map.PushBack(key, key->m_value, parent);
    }

    //----------------------------------------------------------------------------------------------
    void UnregisterNode(TTestKey *key)
    {
        m_map.Erase(m_map.FindNodeByKey(key));
    }

    //----------------------------------------------------------------------------------------------
    void BuidTree()
    {
        //          0     10
        //       1     2  
        //     3   4  5  6
        //     7   8     9

        // depth-first traverse
        // [ 0,                          10 ]
        //     1,          2,
        //       3,   4,       5,   6,
        //         7,    8,             9,

        TTestKey *key0 = new TTestKey("0", new TTestValue("0", nullptr));
        RegisterNode(key0, nullptr);

        TTestKey *key10 = new TTestKey("10", new TTestValue("10", nullptr));
        RegisterNode(key10, nullptr);

        TTestKey *key1 = new TTestKey("1", new TTestValue("1", key0->m_value));
        key0->m_value->AddChild(key1->m_value);
        RegisterNode(key1, key0);

        TTestKey *key2 = new TTestKey("2", new TTestValue("2", key0->m_value));
        key0->m_value->AddChild(key2->m_value);
        RegisterNode(key2, key0);

        TTestKey *key3 = new TTestKey("3", new TTestValue("3", key1->m_value));
        key1->m_value->AddChild(key3->m_value);
        RegisterNode(key3, key1);

        TTestKey *key4 = new TTestKey("4", new TTestValue("4", key1->m_value));
        key1->m_value->AddChild(key4->m_value);
        RegisterNode(key4, key1);

        TTestKey *key5 = new TTestKey("5", new TTestValue("5", key2->m_value));
        key2->m_value->AddChild(key5->m_value);
        RegisterNode(key5, key2);

        TTestKey *key6 = new TTestKey("6", new TTestValue("6", key2->m_value));
        key2->m_value->AddChild(key6->m_value);
        RegisterNode(key6, key2);

        TTestKey *key7 = new TTestKey("7", new TTestValue("7", key3->m_value));
        key3->m_value->AddChild(key7->m_value);
        RegisterNode(key7, key3);

        TTestKey *key8 = new TTestKey("8", new TTestValue("8", key4->m_value));
        key4->m_value->AddChild(key8->m_value);
        RegisterNode(key8, key4);

        TTestKey *key9 = new TTestKey("9", new TTestValue("9", key6->m_value));
        key6->m_value->AddChild(key9->m_value);
        RegisterNode(key9, key6);

        m_root = key0;

        // depth-first traverse
        // [ 0,                          10 ]
        //     1,          2,
        //       3,   4,       5,   6,
        //         7,    8,             9,

        m_deepth_first_traverse.push_back(key0);
        m_deepth_first_traverse.push_back(key1);
        m_deepth_first_traverse.push_back(key3);
        m_deepth_first_traverse.push_back(key7);
        m_deepth_first_traverse.push_back(key4);
        m_deepth_first_traverse.push_back(key8);
        m_deepth_first_traverse.push_back(key2);
        m_deepth_first_traverse.push_back(key5);
        m_deepth_first_traverse.push_back(key6);
        m_deepth_first_traverse.push_back(key9);
        m_deepth_first_traverse.push_back(key10);
    }

    //----------------------------------------------------------------------------------------------
    void ClearTree()
    {
        std::stack<TTestValue*> sstack;

        sstack.push(m_root->m_value);

        while (!sstack.empty())
        {
            TTestValue *top = sstack.top();
            sstack.pop();

            for (auto *val : top->m_childs)
            {
                sstack.push(val);
            }

            //UnregisterNode(top->m_key);

            delete top->m_key;
            delete top;
        }
        
        m_root = nullptr;

        m_map.Clear();

        m_breath_traverse.clear();
    }

public:
    TestTreeMap m_map;

    TTestKey *m_root;

    std::vector<TTestKey*> m_breath_traverse;
    std::vector<TTestKey*> m_deepth_first_traverse;
};