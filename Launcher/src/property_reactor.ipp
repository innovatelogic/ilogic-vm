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
            groups.clear();

            groups.push_back("Value"); // by default

            for each(auto &item in m_propertyClasses)
            {
                for each (auto &prop in item.properties)
                {
                    const std::string name = prop->GetGroupName();

                    if (std::find(groups.begin(), groups.end(), name) == groups.end()) {
                        groups.push_back(name);
                    }
                }
            }
        }

        //----------------------------------------------------------------------------------------------
        template<class T>
        void PropertyReactor<T>::FetchProperties(const std::string &group, TMapClassData &out)
        {
            out.clear();

            for each(auto &item in m_propertyClasses)
            {
                SClassNode node(item.name, item.nOverrideByteShift);

                for each (auto &prop in item.properties)
                {
                    if (prop->GetGroupName() == group){
                        node.properties.push_back(prop);
                    }
                }

                if (node.properties.size() > 0) {
                    out.push_back(node);
                }
            }
        }

        //----------------------------------------------------------------------------------------------
        template<class T>
        void PropertyReactor<T>::BuildObject(const T *object)
        {
            AppClassTree &classTree = oes::common_base::GetClassTree();

            int id = 0;

            bool bInitialInit = m_propertyClasses.empty();
            std::vector<std::string> matchClasses;

            if (ClassNode *classNode = classTree.Find(object->GetType()))
            {
                while (classNode)
                {
                    const std::string className = classNode->GetName();

                    if (bInitialInit)
                    {
                        SClassNode node(className);

                        TVecPropertyConstIterator iterProp = classNode->PropertyMap.begin();

                        while (iterProp != classNode->PropertyMap.end())
                        {
                            node.properties.push_back(*iterProp);

                            // array
                            if ((*iterProp)->GetCtrl() == CTRL_ARRAY)
                            {
                                int size = (*iterProp)->GetSize();
                                while (size)
                                {
                                    // go through child nodes
                                    Property_Base *childProp = (*iterProp)->GetNext();
                                    while (childProp)
                                    {
                                        node.properties.push_back(childProp);
                                        childProp = childProp->GetNext();
                                    }
                                    --size;
                                }
                            }
                            ++iterProp;
                        }

                        m_propertyClasses.push_back(node);
                    }
                    else // sieve
                    {
                        TMapClassData::iterator it = std::find_if(m_propertyClasses.begin(), m_propertyClasses.end(), 
                            [&className](const SClassNode &v)
                        {
                            return className == v.name;
                        });

                        if (it != m_propertyClasses.end())
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

                            SClassNode node(className, shift);

                            TVecPropertyConstIterator iterPropIntf = nodeInterface->PropertyMap.begin();

                            while (iterPropIntf != nodeInterface->PropertyMap.end())
                            {
                                node.properties.push_back(*iterPropIntf);

                                // array
                                if ((*iterPropIntf)->GetCtrl() == CTRL_ARRAY)
                                {
                                    int size = (*iterPropIntf)->GetSize();
                                    while (size)
                                    {
                                        // go through child nodes
                                        Property_Base *childProp = (*iterPropIntf)->GetNext();
                                        while (childProp)
                                        {
                                            node.properties.push_back(childProp);
                                            childProp = childProp->GetNext();
                                        }
                                        --size;
                                    }
                                }


                                ++iterPropIntf;
                            }

                            m_propertyClasses.push_back(node);
                        }
                        else
                        {
                            TMapClassData::iterator it = std::find_if(m_propertyClasses.begin(), m_propertyClasses.end(),
                                [&className](const SClassNode &v) { return className == v.name; });

                            if (it != m_propertyClasses.end())
                            {
                                matchClasses.push_back(className);
                            }
                        }
                                       
                        ++iterIntf;
                    }
                    classNode = classNode->GetRootNode();
                }
            }

            if (!bInitialInit)
            {
                // sieve erase non common nodes
                if (!matchClasses.empty())
                {
                    m_propertyClasses.erase(std::remove_if(m_propertyClasses.begin(), m_propertyClasses.end(),
                        [matchClasses](const SClassNode &item)
                    {
                        return std::find(matchClasses.begin(), matchClasses.end(), item.name) == matchClasses.end();
                    }), m_propertyClasses.end());
                }
            }
            else
            {
                std::reverse(m_propertyClasses.begin(), m_propertyClasses.end());
            }
        }
    }
//}