#include "reflx.h"
#include "json.hpp"
#include <fstream>

void _RegisterType(const char *key/*, rflex_alloc &alloc, const char *className, const char *baseClassName*/)
{

}

namespace oes {
    namespace rflex {

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
    }
}