#include "ValueParser.h"
#include "json.hpp"
#include <fstream>
#include "reflx.h"

void _RegisterType(const char *key/*, rflex_alloc &alloc, const char *className, const char *baseClassName*/)
{

}

namespace oes {
    namespace rflex {

        //----------------------------------------------------------------------------------------------
        bool Rflex::Load(const std::string &filename)
        {
            using json = nlohmann::json;

            bool bResult = false;

            std::ifstream in(filename);
            if (in)
            {
                json j;

                in >> j;

                TClassFactory *factory = TGlobalClassFactory::GetInstance();

                for (json::iterator it = j.begin(); it != j.end(); ++it) 
                {
                    *it;
                }

                bResult = true;
            }

            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        std::string Rflex::GetValueString(const std::string &type, void *ptr)
        {
            std::string out;

            if (ptr)
            {
                oes::rflex::AppClassTree &tree = oes::rflex::GetClassTree();
                oes::rflex::ClassNode *node = tree.Find(type.c_str());

                if (node)
                {
                    // write down groups map
                    oes::rflex::ClassNode *p_node = node;

                    while (p_node)
                    {
                        const oes::rflex::ClassNode::TVecProperties &props = p_node->GetProperties();
                        oes::rflex::ClassNode::TVecPropertyConstIterator iter = props.begin();

                        while (iter != props.end())
                        {
                            char buff[1024] = { 0 };
                            char buffDefault[1024] = { 0 };

                            (*iter)->GetProperty(ptr, buff);

                            ++iter;
                        }

                        p_node = p_node->Parent();
                    }
                }
            }
            return out;
        }

        //----------------------------------------------------------------------------------------------
        void Rflex::Deserialize(void *ptr, const std::string &type, tinyxml2::XMLElement *xml_current_tree)
        {
            if (!ptr) {
                return;
            }

            oes::rflex::AppClassTree &tree = oes::rflex::GetClassTree();

            if (oes::rflex::ClassNode *node = tree.Find(type.c_str()))
            {
                oes::rflex::ClassNode::TVecPropertyConstIterator prop = node->propertyMap.begin();

                // deserialize embedded class props
                while (prop != node->propertyMap.end())
                {
                    if ((*prop)->IsSerializable())
                    {
                        std::string group = (*prop)->GetGroupName();

                        const char *attribute = xml_current_tree->Attribute(group.c_str());

                        const std::string VALUE(attribute);

                        if ((*prop)->IsCommonValue())
                        {
                            (*prop)->SetProperty(this, VALUE.c_str(), 0, true);
                        }
                        else
                        {
                            ValueParser ValueStore(VALUE);

                            std::string ValueName = (*prop)->GetName();

                            if (ValueStore.IsValue(ValueName))
                            {
                                (*prop)->SetProperty(ptr, ValueStore.GetValueString(ValueName).c_str(), 0, true);
                            }
                        }
                    }
                    ++prop;
                }

                // add interface properties
                oes::rflex::ClassNode::TVecInterfaceIter intf = node->interfaces.begin();

                while (intf != node->interfaces.end())
                {
                    oes::rflex  ::ClassNode *intf_node = tree.FindInterface((*intf)->strType);

                    if (node)
                    {
                        oes::rflex::ClassNode::TVecPropertyConstIterator intf_prop = intf_node->propertyMap.begin();

                        while (intf_prop != intf_node->propertyMap.end())
                        {
                            const std::string VALUE(xml_current_tree->Attribute((*intf_prop)->GetGroupName().c_str()));

                            if ((*intf_prop)->IsSerializable())
                            {
                                if ((*intf_prop)->IsCommonValue())
                                {
                                    (*intf_prop)->SetProperty(ptr, VALUE.c_str(), 0, true);
                                }
                                else
                                {
                                    ValueParser ValueStore(VALUE);

                                    std::string ValueName = (*intf_prop)->GetName();

                                    if (ValueStore.IsValue(ValueName))
                                    {
                                        (*intf_prop)->SetProperty(ptr, ValueStore.GetValueString(ValueName).c_str(), 0, true);
                                    }
                                }
                            }
                            ++intf_prop;
                        }
                    }
                    ++intf;
                }
            }
        }
    }
}