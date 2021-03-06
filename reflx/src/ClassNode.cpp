#include "property.h"
#include "ClassNode.h"

namespace oes
{
    namespace rflex
    {
        //----------------------------------------------------------------------------------------------
        ClassNode::ClassNode(const char * name)
            : m_name(name)
            , m_pRootNode(nullptr)
            , m_pPropAlloc(nullptr)
            , m_pPropsRaw(nullptr)
            , m_PropsSize(0)
        {
        }

        //----------------------------------------------------------------------------------------------
        ClassNode::~ClassNode()
        {
            if (m_pPropsRaw)
            {
                for (unsigned int Index = 0; Index < m_PropsSize; ++Index)
                {
                    Property_Base * pNode = *(m_pPropsRaw + Index);

                    /*Property_Base * pNodeNext = pNode->GetNext();
                    while (pNodeNext)
                    {
                        Property_Base * pNextTo = pNodeNext->GetNext();
                        delete pNodeNext;
                        pNodeNext = pNextTo;
                    }*/

                    delete pNode;
                }
                delete[] m_pPropsRaw;
            }

            delete m_pPropAlloc;
        }

        //----------------------------------------------------------------------------------------------
        void ClassNode::Release()
        {
            for_each(childs.begin(), childs.end(), std::mem_fun(&ClassNode::Release));

            delete this;
        }

        //----------------------------------------------------------------------------------------------
        ClassNode* ClassNode::Find(const char * Type)
        {
            if (m_name.compare(Type) == 0) {
                return this;
            }
            else
            {
                for (TVecClassNodeIterator Iter = childs.begin(); Iter != childs.end(); ++Iter)
                {
                    ClassNode * FindNode = (*Iter)->Find(Type);
                    if (FindNode) {
                        return FindNode;
                    }
                }
            }
            return 0;
        }

        //----------------------------------------------------------------------------------------------
        ClassNode* ClassNode::AddChild(ClassNode *pNode)
        {
            // adds compatibility check
            childs.push_back(pNode);
            return pNode;
        }

        //----------------------------------------------------------------------------------------------
        ClassNode* ClassNode::RemoveNode(ClassNode * Node)
        {
            TVecClassNodeIterator IterFind = std::find(childs.begin(), childs.end(), Node);

            if (IterFind != childs.end())
            {
                childs.erase(IterFind);
                return this;
            }

            for (TVecClassNodeIterator Iter = childs.begin(); Iter != childs.end(); ++Iter)
            {
                ClassNode *pParentToRemove = (*Iter)->RemoveNode(Node);

                if (pParentToRemove) {
                    return pParentToRemove;
                }
            }
            return nullptr;
        }

        //----------------------------------------------------------------------------------------------
        Property_Base* ClassNode::GetProperty(const char *Name) const
        {
            for (TVecPropertyConstIterator Iter = propertyMap.begin(); Iter != propertyMap.end(); ++Iter)
            {
                if (!strcmp((*Iter)->m_Name.c_str(), Name))
                {
                    return *Iter;
                }
            }
            return 0;
        }

        //----------------------------------------------------------------------------------------------
        bool ClassNode::AddProperty(const char *Name, Property_Base *Prop)
        {
            propertyMap.push_back(Prop);
            return true;
        }

        //----------------------------------------------------------------------------------------------
        void ClassNode::SetProprties(const IPropertiesAllocator *pPropAlloc)
        {
            m_pPropAlloc = const_cast<IPropertiesAllocator*>(pPropAlloc);

            m_pPropsRaw = const_cast<Property_Base**>(pPropAlloc->GetProps());
            m_PropsSize = m_pPropAlloc->GetSize();

            for (unsigned int index = 0; index < m_PropsSize; ++index)
            {
                const_cast<Property_Base*>(*(m_pPropsRaw + index))->m_ClassNodePtr = this;
                propertyMap.push_back(const_cast<Property_Base*>(*(m_pPropsRaw + index)));
            }

            m_pInterfaces = const_cast<SInterfaceDecl**>(pPropAlloc->GetInterfaces());
            m_nIntfSize = m_pPropAlloc->GetIntfSize();

            for (unsigned int Index = 0; Index < m_nIntfSize; ++Index)
            {
                interfaces.push_back(const_cast<SInterfaceDecl*>(*(m_pInterfaces + Index)));
            }
        }

        //----------------------------------------------------------------------------------------------
        ClassNode* ClassNode::GetChild(char *type) const
        {
            for each (auto item in childs)
            {
                if (std::string(item->GetName()).compare(type) == 0)
                {
                    return item;
                }
            }
            return nullptr;
        }

        //----------------------------------------------------------------------------------------------
        void ClassNode::SetProprties(const Property_Base** Arr, int Count)
        {
            m_pPropsRaw = const_cast<Property_Base**>(Arr);
            m_PropsSize = (unsigned int)Count;

            for (unsigned int Index = 0; Index < m_PropsSize; ++Index)
            {
                propertyMap.push_back(const_cast<Property_Base*>(*(Arr + Index)));
            }
        }

        //----------------------------------------------------------------------------------------------
        bool ClassNode::PrePropertyChange(const void *prt, class Property_Base *Prop)
        {
            //OwnerClass *pClass = const_cast<OwnerClass*>(reinterpret_cast<const OwnerClass*>(pPtr));

            return true; // pClass->OnPrePropertyChange(Prop->GetName());
        }

        //----------------------------------------------------------------------------------------------
        void ClassNode::PostPropertyChange(const void * Prt, class Property_Base* Prop)
        {

        }

        //----------------------------------------------------------------------------------------------
        SInterfaceDecl* ClassNode::GetInterfaceDecl(const char *Name) const
        {
            for (TVecInterfaces::const_iterator Iter = interfaces.begin(); Iter != interfaces.end(); ++Iter)
            {
                if (!strcmp((*Iter)->strType, Name))
                {
                    return *Iter;
                }
            }

            return nullptr;
        }

        //----------------------------------------------------------------------------------------------
        bool ClassNode::IsInheritBaseClass(const char* pName) const
        {
            bool bResult = false;

            if (!strcmp(m_name.c_str(), pName)) {
                return true;
            }

            ClassNode *pIter = m_pRootNode;

            while (pIter)
            {
                if (!strcmp(pIter->GetName(), pName))
                {
                    bResult = true;
                    break;
                }
                pIter = pIter->GetRootNode();
            }

            return bResult;
        }
    }
}

