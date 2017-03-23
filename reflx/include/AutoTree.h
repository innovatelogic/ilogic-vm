#pragma once

#include "defexport.h"
#include "property.h"
#include <algorithm>
#include <functional>
#include <assert.h>

//----------------------------------------------------------------------------------------------
namespace oes
{
    namespace rflex
    {
        template<class T_CLASS>
        class REFLX_EXPORT CAutoTree
        {
        public:
            CAutoTree()
            {
            }

            virtual ~CAutoTree()
            {
                Release();
            }

            const std::vector<T_CLASS*>& GetRoots() const { return m_roots; }

            //----------------------------------------------------------------------------------------------
            T_CLASS* Add(const char *Type, const char *BaseType)
            {
                if (Type && BaseType)
                {
                    T_CLASS *pBase = Find(BaseType);
                    T_CLASS *pSiblin = Find(Type);

                    if (pBase && pSiblin) // already exist
                    {
                        // check if already linked
                        std::vector<T_CLASS*>::iterator iterFind = std::find(pBase->childs.begin(), pBase->childs.end(), pSiblin);

                        if (iterFind != pBase->childs.end()) {
                            return pSiblin;
                        }
                        else // need rearrange
                        {
                            // try to remove in roots if exist
                            std::vector<T_CLASS*>::iterator iterFindInRoot = std::find(m_roots.begin(), m_roots.end(), pSiblin);
                            if (iterFindInRoot != m_roots.end())
                            {
                                m_roots.erase(iterFindInRoot);

                                pBase->AddChild(pSiblin);
                                pSiblin->SetRootNode(pBase);
                            }
                            else
                            {
                                assert(false); // Exception
                            }
                        }

                        return pSiblin;
                    }
                    else if (!pBase && !pSiblin) // add plain
                    {
                        //     !Base   <- add root
                        //       |
                        //    !Siblin  <- add

                        T_CLASS *pBaseClass = new T_CLASS(BaseType);

                        T_CLASS *pInheritClass = pBaseClass->AddChild(new T_CLASS(Type));

                        pInheritClass->SetRootNode(pBaseClass);

                        m_roots.push_back(pBaseClass);

                        return pInheritClass;
                    }
                    else
                    {
                        if (pBase && !pSiblin)
                        {
                            //      Base
                            //       |
                            //    !Siblin  <- add

                            T_CLASS *pInheritClass = new T_CLASS(Type);

                            pBase->AddChild(pInheritClass);

                            pInheritClass->SetRootNode(pBase);

                            return pInheritClass;
                        }
                        else if (!pBase && pSiblin)
                        {
                            std::vector<T_CLASS*>::iterator iterFind = std::find(m_roots.begin(), m_roots.end(), pSiblin);

                            assert(iterFind != m_roots.end());

                            //allocate new base
                            T_CLASS *pBaseClass = new T_CLASS(BaseType);

                            pBaseClass->AddChild(pSiblin);

                            *iterFind = pBaseClass;

                            pSiblin->SetRootNode(pBaseClass);

                            return pSiblin;
                        }
                    }
                }
                else
                {
                    if (Type && !Find(Type))
                    {
                        T_CLASS *pClass = new T_CLASS(Type);
                        m_roots.push_back(pClass);
                        return pClass;
                    }

                    if (BaseType && !Find(BaseType))
                    {
                        T_CLASS *pClass = new T_CLASS(BaseType);
                        m_roots.push_back(pClass);
                        return pClass;
                    }
                }

                return nullptr;
            }

            //----------------------------------------------------------------------------------------------
            T_CLASS* AddPure(const char *type, const Property_Base **arr, int count)
            {
                T_CLASS *pBase = Find(type);

                if (!pBase)
                {
                    T_CLASS *pClass = new ClassNode(type);
                    m_roots.push_back(pClass);
                    pBase = pClass;
                }
                pBase->SetProprties(arr, count);
                return pBase;
            }

            //----------------------------------------------------------------------------------------------
            T_CLASS* Add(const char *type, const IPropertiesAllocator *propAlloc)
            {
                T_CLASS *pBase = Find(type);

                if (!pBase)
                {
                    T_CLASS *pClass = new ClassNode(type);
                    m_roots.push_back(pClass);
                    pBase = pClass;
                }
                pBase->SetProprties(propAlloc);
                return pBase;
            }

            //----------------------------------------------------------------------------------------------
            T_CLASS* Find(const char *Type)
            {
                for (std::vector<T_CLASS*>::iterator Iter = m_roots.begin(); Iter != m_roots.end(); ++Iter)
                {
                    if (*Iter != nullptr)
                    {
                        if (T_CLASS *FindNode = (*Iter)->Find(Type))
                        {
                            return FindNode;
                        }
                    }
                }
                return nullptr;
            }

            //----------------------------------------------------------------------------------------------
            T_CLASS* GetRoot(const char *type) const
            {
                for each (auto item in m_roots)
                {
                    if (std::string(item->GetName()).compare(type) == 0)
                    {
                        return item;
                    }
                }
                return nullptr;
            }

            //----------------------------------------------------------------------------------------------
            void Release()
            {
                std::for_each(m_roots.begin(), m_roots.end(), std::mem_fun(&T_CLASS::Release));
                m_roots.clear();
            }

        private:
            std::vector<T_CLASS*> m_roots;
        };
    }
}