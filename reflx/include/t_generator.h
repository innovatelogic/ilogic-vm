#pragma once

#include "i_generator.h"

namespace oes
{
    namespace rflex
    {
        template<typename T, class... Args>
        class Generator : public IGenerator
        {
        public:
            Generator() {}

            virtual void instance(const std::function<void()> &f)
            {

            }

            T* instance(Args&&... args)
            {
                return new T(args...);
            }
        };
    }
}