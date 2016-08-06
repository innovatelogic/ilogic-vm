#include "AutoTree.h"
#include "ClassNode.h"
#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------
TEST(TestAutoTree, RegisterBroadTree)
{
    oes::common_base::CAutoTree<ClassNode> tree;

    //    A   
    //  B   C
    // D E  F H

    //tree.Add("A", "");
    //tree.Add("B", "");
    tree.Add("B", "A");
    tree.Add("C", "A");
    tree.Add("D", "B");
    tree.Add("E", "B");
    tree.Add("F", "C");
    tree.Add("H", "C");

    const std::vector<ClassNode*> &roots = tree.GetRoots();

    EXPECT_EQ(roots.size(), 1);

    const ClassNode *node_root = roots[0];
    EXPECT_EQ(std::string(node_root->GetName()), "A");
    EXPECT_EQ(node_root->GetRootNode(), nullptr);

    const ClassNode *node_B = node_root->Childs[0];
    EXPECT_EQ(std::string(node_B->GetName()), "B");
    EXPECT_EQ(node_B->GetRootNode(), node_root);

    const ClassNode *node_C = node_root->Childs[1];
    EXPECT_EQ(std::string(node_C->GetName()), "C");
    EXPECT_EQ(node_C->GetRootNode(), node_root);

    const ClassNode *node_D = node_B->Childs[0];
    EXPECT_EQ(std::string(node_D->GetName()), "D");
    EXPECT_EQ(node_D->GetRootNode(), node_B);

    const ClassNode *node_E = node_B->Childs[1];
    EXPECT_EQ(std::string(node_E->GetName()), "E");
    EXPECT_EQ(node_E->GetRootNode(), node_B);

    const ClassNode *node_F = node_C->Childs[0];
    EXPECT_EQ(std::string(node_F->GetName()), "F");
    EXPECT_EQ(node_F->GetRootNode(), node_C);

    const ClassNode *node_H = node_C->Childs[1];
    EXPECT_EQ(std::string(node_H->GetName()), "H");
    EXPECT_EQ(node_H->GetRootNode(), node_C);

    tree.Release();
}
