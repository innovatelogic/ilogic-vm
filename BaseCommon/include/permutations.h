#include <vector>
#include <functional>
#include <assert.h>
#include "OEMBase.h"

namespace oes
{
    namespace common_base
    {
        //----------------------------------------------------------------------------------------------
        template<typename T>
        COMMON_BASE_EXPORT void permutate_impl(const std::vector<T> &in,
            std::vector<T> &out,
            std::vector<bool> &used,
            const std::function<void(const std::vector<T>&)> &func)
        {
            if (out.size() == in.size())
            {
                func(out);
                return;
            }

            for (size_t i = 0; i < in.size(); ++i)
            {
                if (used[i]) continue;

                used[i] = true;

                out.push_back(in[i]);

                permutate_impl<T>(in, out, used, func);

                out.resize(out.size() - 1);

                used[i] = false;
            }
        }

        //----------------------------------------------------------------------------------------------
        template<class T>
        COMMON_BASE_EXPORT void permutate(const std::vector<T> &in,
            const std::function<void(const std::vector<T> &res)> &func)
        {
            std::vector<bool> used; // vector of bool revise
            used.resize(in.size());
            for (size_t i = 0; i < in.size(); ++i) {
                used[i] = false;
            }

            std::vector<T> out;
            permutate_impl<T>(in, out, used, func);

            used.resize(in.size());
        }
    }
}