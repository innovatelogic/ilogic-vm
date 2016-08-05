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

            if (ClassNode *pClassNode = classTree.Find(object->GetType()))
            {
                while (pClassNode)
                {
                    TVecPropertyConstIterator iterProp = pClassNode->PropertyMap.begin();

                    while (iterProp != pClassNode->PropertyMap.end())
                    {
                        m_propertyClasses.push_back(SClassNode((*iterProp)->GetClassName()));
                        
                        ++iterProp;
                    }

                    pClassNode = pClassNode->GetRootNode();
                }
            }
        }
    }
//}