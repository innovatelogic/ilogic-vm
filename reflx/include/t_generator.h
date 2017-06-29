#pragma once

#include <tuple>
#include "i_generator.h"

namespace oes
{
    namespace rflex
    {
        template<typename T, class... Args>
        class Generator : public IGenerator
        {
            std::function<void(Args...)> func;
            std::tuple<Args...> _args;

        public:
            Generator() {}

            virtual void instance(const std::function<void()> &f)
            {
                std::tuple<Args...> tup;
                f();
            }

            void embed(T *obj) { embed_impl(obj); return obj; }

            virtual void embed_impl(T *obj) {}

            static T* instance(Args&&... args)
            {
                return new T(args...);
            }
        };
    }
}