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
    }
}