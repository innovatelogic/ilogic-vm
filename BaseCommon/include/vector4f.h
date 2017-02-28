#pragma once

#include "OEMBase.h"
#include "TypesBase.h"

namespace oes
{
    namespace common_base
    {
        class COMMON_BASE_EXPORT Vector4f
        {
        public:
            Vector4f();
            Vector4f(TFlt32 x, TFlt32 y, TFlt32 z, TFlt32 w = 1.f);
            Vector4f(const Vector4f &Source);

            void Set(TFlt32 _x, TFlt32 _y, TFlt32 _z, TFlt32 _w);

            TFlt32  operator[](int i)const;
            TFlt32& operator[](int i);

            bool operator!=(const Vector4f& _v) const;

            TFlt32* GetPtr() { return vec_array; }
            const TFlt32* GetPtr() const { return vec_array; }

        public:

            union  // data
            {
                struct {
                    TFlt32 x, y, z, w;
                };

                struct {
                    TFlt32 U, V, UL, VL;
                };

                TFlt32 vec_array[4];     // array access
            };
        };
    }
}