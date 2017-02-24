#pragma once

#include <math.h>
#include "OEMBase.h"

namespace oes
{
namespace common_base
{
//------------------------------------------------------------------------
class COMMON_BASE_EXPORT Vector2f
{
public:
    Vector2f();
    Vector2f(float _x, float _y);
    Vector2f(const Vector2f &Source);

    void Set(const float _x, const float _y);

     float* GetPtr() { return vec_array; }
     const float* GetPtr() const { return vec_array; }
     float Normalize();

     Vector2f& Vector2f::operator+=(const Vector2f& v)
    {
        x += v.x;
        y += v.y;

        return *this;
    }

     Vector2f Vector2f::operator+(const Vector2f& v)const
    {
        Vector2f out;

        out.x = x + v.x;
        out.y = y + v.y;

        return out;
    }

     Vector2f Vector2f::operator-(const Vector2f& v)const
    {
        Vector2f out;

        out.x = x - v.x;
        out.y = y - v.y;

        return out;
    }

     Vector2f Vector2f::operator*(float f)const
    {
        Vector2f out;

        out.x = x * f;
        out.y = y * f;

        return out;
    }

     float Length() const;
     float LengthSqr() const;
     float Dot(const Vector2f& _vector_other)const;

public:
    union
    {
        struct {
            float x, y;
        };

        float vec_array[2];     // array access
    };
};

//--------------------------------------------------------------------------------
/** Computes the squared magnitude */
COMMON_BASE_EXPORT float sq_length(const Vector2f & n);

/** Computes the magnitude */
COMMON_BASE_EXPORT float length(const Vector2f & n);
COMMON_BASE_EXPORT float DotProduct(const Vector2f& v1, const Vector2f& v2);

}
}