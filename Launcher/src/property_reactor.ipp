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
        void PropertyReactor<T>::FetchGroups(std::vector<std::string> &groups)
        {

        }

        //----------------------------------------------------------------------------------------------
        template<class T>
        void PropertyReactor<T>::FetchProperties(const std::string &group, TMapClassData &out)
        {

        }

        //----------------------------------------------------------------------------------------------
        template<class T>
        void PropertyReactor<T>::BuildObject(const T *object)
        {
            AppClassTree &classTree = oes::common_base::GetClassTree();

            bool bInitialInit = m_propertyClasses.empty();
            std::vector<std::string> matchClasses;

            if (ClassNode *classNode = classTree.Find(object->GetType()))
            {
                while (classNode)
                {
                    const std::string className = classNode->GetName();

                    if (bInitialInit)
                    {
                        std::pair<TMapClassData::iterator, bool> iterClass =
                            m_propertyClasses.insert(std::make_pair(className, SClassNode(className)));

                        TVecPropertyConstIterator iterProp = classNode->PropertyMap.begin();

                        while (iterProp != classNode->PropertyMap.end())
                        {
                            iterClass.first->second.inheritProperties.push_back(*iterProp);
                            ++iterProp;
                        }
                    }
                    else // sieve
                    {
                        auto itFind = m_propertyClasses.find(className);
                        if (itFind != m_propertyClasses.end())
                        {
                            matchClasses.push_back(className);
                        }
                    }

                    // add interface properties
                    ClassNode::TVecInterfaceIter iterIntf = classNode->m_VecInterfaces.begin();

                    while (iterIntf != classNode->m_VecInterfaces.end())
                    {
                        ClassNode *nodeInterface = classTree.FindInterface((*iterIntf)->strType);

                        assert(nodeInterface);
                                                
                        const std::string className = nodeInterface->GetName();
                            
                        if (bInitialInit)
                        {
                            const int shift = (*iterIntf)->byteShift;

                            std::pair<TMapClassData::iterator, bool> iterClass =
                                m_propertyClasses.insert(std::make_pair(className, SClassNode(className, shift)));

                            iterClass.first->second.nOverrideByteShift = shift;

                            TVecPropertyConstIterator iterPropIntf = nodeInterface->PropertyMap.begin();

                            while (iterPropIntf != nodeInterface->PropertyMap.end())
                            {
                                iterClass.first->second.inheritProperties.push_back(*iterPropIntf);
                                ++iterPropIntf;
                            }
                        }
                        else
                        {
                            auto itFind = m_propertyClasses.find(className);
                            if (itFind != m_propertyClasses.end())
                            {
                                matchClasses.push_back(className);
                            }
                        }
                                       
                        ++iterIntf;
                    }
                    classNode = classNode->GetRootNode();
                }
            }

            // sieve
            if (!matchClasses.empty())
            {
                for (auto it = m_propertyClasses.begin(); it != m_propertyClasses.end();)
                {
                    const std::string &name = it->first;

                    if (std::find(matchClasses.begin(), matchClasses.end(), name) != matchClasses.end())
                    {
                        m_propertyClasses.erase(it++);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    }
//}