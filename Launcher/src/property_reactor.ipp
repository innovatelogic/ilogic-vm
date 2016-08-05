#include "property_reactor.h"

//namespace oes
//{
    namespace nmLauncher
    {
        //----------------------------------------------------------------------------------------------
        template<class T>
        PropertyReactor<T>::PropertyReactor(const editors::TIEditor &editor)
        {
            m_editor = editor;
        }
        //----------------------------------------------------------------------------------------------
        template<class T>
        PropertyReactor<T>::~PropertyReactor()
        {
            Clear();
        }

        //----------------------------------------------------------------------------------------------
        template<class T>
        void PropertyReactor<T>::Build()
        {
            Clear();

            std::vector<const T*> selected = m_editor->GetSelected();

            for each (const T *item in selected)
            {
                BuildObject(item);
            }
        }

        //----------------------------------------------------------------------------------------------
        template<class T>
        void PropertyReactor<T>::Clear()
        {
            m_propertyClasses.clear();
        }

        //----------------------------------------------------------------------------------------------
        template<class T>
        void PropertyReactor<T>::BuildObject(const T *object)
        {
            AppClassTree &classTree = NObjectFactory::GetClassTree();

            bool bInitialInit = m_propertyClasses.empty();

            if (ClassNode *classNode = classTree.Find(object->GetType()))
            {
                while (classNode)
                {
                    TVecPropertyConstIterator iterProp = classNode->PropertyMap.begin();

                    while (iterProp != classNode->PropertyMap.end())
                    {
                        const std::string className = (*iterProp)->GetClassName();
                        if (bInitialInit)
                        {
                            m_propertyClasses.push_back(SClassNode(className));
                        }
                        else // sieve
                        {
                            std::vector<SClassNode>::iterator itFind = std::find_if(
                                m_propertyClasses.begin(),
                                m_propertyClasses.end(),
                                [&](SClassNode &s) { return className == s.name; });

                            if (itFind == m_propertyClasses.end()) {
                                m_propertyClasses.erase(itFind);
                            }
                        }
                        
                        ++iterProp;
                    }

                    // add interface properties
                    ClassNode::TVecInterfaceIter iterIntf = classNode->m_VecInterfaces.begin();

                    while (iterIntf != classNode->m_VecInterfaces.end())
                    {
                        ClassNode *nodeInterface = classTree.FindInterface((*iterIntf)->strType);

                        assert(nodeInterface);

                        TVecPropertyConstIterator iterPropIntf = nodeInterface->PropertyMap.begin();

                        while (iterPropIntf != nodeInterface->PropertyMap.end())
                        {
                            const std::string className = (*iterPropIntf)->GetClassName();
                            const int shift = (*iterIntf)->byteShift;

                            if (bInitialInit)
                            {
                                m_propertyClasses.push_back(SClassNode(className, shift));
                            }
                            else
                            {
                                std::vector<SClassNode>::iterator itFind = std::find_if(
                                    m_propertyClasses.begin(),
                                    m_propertyClasses.end(),
                                    [&](SClassNode &s) { return className == s.name; });

                                if (itFind == m_propertyClasses.end()){
                                    m_propertyClasses.erase(itFind);
                                }
                            }
                            ++iterPropIntf;
                        }
                        ++iterIntf;
                    }

                    classNode = classNode->GetRootNode();
                }
            }
        }
    }
//}