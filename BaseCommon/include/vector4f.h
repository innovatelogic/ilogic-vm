#ifndef __vector4f_h__
#define __vector4f_h__

#pragma once

#include <assert.h>
#include "OEMBase.h"

namespace oes
{
namespace common_base
{
class COMMON_BASE_EXPORT Vector4f
{
public:
    Vector4f();
    Vector4f(float x, float y, float z, float w = 1.f);
    Vector4f(const Vector4f &Source);

    void Set(const float _x, const float _y, const float _z, const float _w);

    float  operator[](int i)const;
    float& operator[](int i);

    bool operator!=(const Vector4f& _v) const;

     float* GetPtr() { return vec_array; }
     const float* GetPtr() const { return vec_array; }

public:

    union  // data
    {
        struct {
            float x, y, z, w;
        };

        struct {
            float U, V, UL, VL;
        };

        float vec_array[4];     // array access
    };

};

}
}

#endif