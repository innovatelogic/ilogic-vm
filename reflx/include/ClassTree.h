#pragma once

#include "defexport.h"
#include "ClassNode.h"
#include "AutoTree.h"
#include <vector>
#include <list>

namespace oes
{
    namespace rflex
    {
        class REFLX_EXPORT AppClassTree
        {
        public:
            typedef std::vector<ClassNode*>			        TVecClassNodeInterface;
            typedef TVecClassNodeInterface::iterator		TVecClassNodeInterfaceIter;
            typedef TVecClassNodeInterface::const_iterator	TVecClassNodeInterfaceConstIter;

            AppClassTree();
            virtual ~AppClassTree();

            AppClassTree(const AppClassTree &other) = delete;
            AppClassTree& operator=(const AppClassTree &other) = delete;

            bool Add(const char *type, const char *baseType);
            bool Add(const char *type, const Property_Base **arr, int count);
            bool Add(const char *type, const IPropertiesAllocator * PropAlloc);

            void AddInterface(ClassNode *pInterface);

            ClassNode*	Find(const char *type);
            ClassNode*	FindInterface(const char *type) const;

            // returns parent node
            ClassNode*	RemoveNode(ClassNode *pNode);

            void		Release();

            const TVecClassNodeInterface& GetInterfaces() const { return m_VecInterfaces; }

            const CAutoTree<ClassNode> &GetTree() const { return m_tree; }

        private:
            TVecClassNodeInterface m_VecInterfaces;

            CAutoTree<ClassNode> m_tree;
        };

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        template<typename T>
        struct REFLX_EXPORT CEnumerateChildTypeStrategy
        {
            CEnumerateChildTypeStrategy(const char *InterfaceName, const CAutoTree<ClassNode> &tree, T &pStoreType)
            {
                assert(InterfaceName);

                const auto &roots = tree.GetRoots();

                for (const auto item : roots)
                {
                    if (item)
                    {
                        std::stack<const ClassNode*> stack;

                        stack.push(item);

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
                }
            }
        };
    }
}