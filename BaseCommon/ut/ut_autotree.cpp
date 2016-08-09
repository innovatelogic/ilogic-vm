#include "AutoTree.h"
#include "ClassNode.h"
#include "permutations.h"
#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------
TEST(TestAutoTree, RegisterBroadTree)
{
    oes::common_base::CAutoTree<ClassNode> tree;

    //    A       I
    //  B   C     K
    // D E  F H

    std::vector<std::pair<std::string, std::string>> arr = {
    std::make_pair("B", "A"),
    std::make_pair("C", "A"),
    std::make_pair("D", "B"),
    std::make_pair("E", "B"),
    std::make_pair("F", "C"),
    std::make_pair("H", "C"),
    std::make_pair("K", "I"),
    };

    int count = 0;
    oes::common_base::permutate<std::pair<std::string, std::string>>(arr,
    [&](const std::vector<std::pair<std::string, std::string>> &res)
    {
        if (count == 1782) {
            int k = 0;
        }
        tree.Add(res[0].first.c_str(), res[0].second.c_str());
        tree.Add(res[1].first.c_str(), res[1].second.c_str());
        tree.Add(res[2].first.c_str(), res[2].second.c_str());
        tree.Add(res[3].first.c_str(), res[3].second.c_str());
        tree.Add(res[4].first.c_str(), res[4].second.c_str());
        tree.Add(res[5].first.c_str(), res[5].second.c_str());
        tree.Add(res[6].first.c_str(), res[6].second.c_str());

        const std::vector<ClassNode*> &roots = tree.GetRoots();

        EXPECT_EQ(roots.size(), 2);

        const ClassNode *node_rootA = tree.GetRoot("A");
        EXPECT_EQ(std::string(node_rootA->GetName()), "A");
        EXPECT_EQ(node_rootA->GetRootNode(), nullptr);

        const ClassNode *node_B = node_rootA->GetChild("B");
        EXPECT_EQ(std::string(node_B->GetName()), "B");
        EXPECT_EQ(node_B->GetRootNode(), node_rootA);

        const ClassNode *node_C = node_rootA->GetChild("C");
        EXPECT_EQ(std::string(node_C->GetName()), "C");
        EXPECT_EQ(node_C->GetRootNode(), node_rootA);

        const ClassNode *node_D = node_B->GetChild("D");
        EXPECT_EQ(std::string(node_D->GetName()), "D");
        EXPECT_EQ(node_D->GetRootNode(), node_B);

        const ClassNode *node_E = node_B->GetChild("E");
        EXPECT_EQ(std::string(node_E->GetName()), "E");
        EXPECT_EQ(node_E->GetRootNode(), node_B);

        const ClassNode *node_F = node_C->GetChild("F");
        EXPECT_EQ(std::string(node_F->GetName()), "F");
        EXPECT_EQ(node_F->GetRootNode(), node_C);

        const ClassNode *node_H = node_C->GetChild("H");
        EXPECT_EQ(std::string(node_H->GetName()), "H");
        EXPECT_EQ(node_H->GetRootNode(), node_C);

        const ClassNode *node_rootI = tree.GetRoot("I");
        EXPECT_EQ(std::string(node_rootI->GetName()), "I");
        EXPECT_EQ(node_rootI->GetRootNode(), nullptr);

        const ClassNode *node_K = node_rootI->GetChild("K");
        EXPECT_EQ(std::string(node_K->GetName()), "K");
        EXPECT_EQ(node_K->GetRootNode(), node_rootI);

        tree.Release();

        count++;
    });
    
    EXPECT_EQ(count, 5040);
}
