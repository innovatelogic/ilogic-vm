#include "ClassTree.h"
#include "ClassNode.h"
#include "ClassNodeInterface.h"
#include "ClassNodeType.h"
#include <assert.h>

namespace oes
{
    namespace common_base
    {
        //----------------------------------------------------------------------------------------------
        AppClassTree::AppClassTree()
        {

        }

        //----------------------------------------------------------------------------------------------
        AppClassTree::~AppClassTree()
        {
            Release();
        }

        //----------------------------------------------------------------------------------------------
        bool AppClassTree::Add(const char *type, const char *baseType)
        {
            m_tree.Add(type, baseType);

            return true;
        }

        //----------------------------------------------------------------------------------------------
        bool AppClassTree::Add(const char *type, const Property_Base **arr, int count)
        {
            m_tree.AddPure(type, arr, count);

            return true;
        }

        //----------------------------------------------------------------------------------------------
        bool AppClassTree::Add(const char *type, const IPropertiesAllocator *propAlloc)
        {
            m_tree.Add(type, propAlloc);

            return true;
        }

        //----------------------------------------------------------------------------------------------
        void AppClassTree::AddInterface(ClassNode *pInterface)
        {
            m_VecInterfaces.push_back(pInterface);
        }

        //----------------------------------------------------------------------------------------------
        ClassNode* AppClassTree::Find(const char *type)
        {
            return m_tree.Find(type);
        }

        //----------------------------------------------------------------------------------------------
        ClassNode* AppClassTree::FindInterface(const char *type) const
        {
            for (TVecClassNodeInterfaceConstIter Iter = m_VecInterfaces.begin(); Iter != m_VecInterfaces.end(); ++Iter)
            {
                if (!strcmp((*Iter)->GetName(), type))
                {
                    return *Iter;
                }
            }
            return nullptr;
        }

        //----------------------------------------------------------------------------------------------
        ClassNode* AppClassTree::RemoveNode(ClassNode *pNode)
        {
            //return (m_pRootNode) ? m_pRootNode->RemoveNode(pNode) : nullptr;
            return nullptr;
        }

        //----------------------------------------------------------------------------------------------
        void AppClassTree::Release()
        {
            /*if (m_pRootNode)
            {
                m_pRootNode->Release();
                m_pRootNode = nullptr;
            }*/
        }
    }
}