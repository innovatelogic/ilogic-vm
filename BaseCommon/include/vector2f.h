#pragma once

#include "OEMBase.h"
#include "TypesBase.h"

namespace oes
{
    namespace common_base
    {
        //------------------------------------------------------------------------
        class COMMON_BASE_EXPORT Vector2f
        {
        public:
            Vector2f();
            Vector2f(TFlt32 _x, TFlt32 _y);
            Vector2f(const Vector2f &Source);

            void Set(TFlt32 _x, TFlt32 _y);

            TFlt32  operator[] (int i) const;
            TFlt32& operator[] (int i);

            TFlt32* GetPtr() { return vec_array; }
            const TFlt32* GetPtr() const { return vec_array; }
            TFlt32 Normalize();

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

            Vector2f Vector2f::operator*(TFlt32 f)const
            {
                Vector2f out;

                out.x = x * f;
                out.y = y * f;

                return out;
            }

            TFlt32 Length() const;
            TFlt32 LengthSqr() const;
            TFlt32 Dot(const Vector2f& _vector_other) const;

        public:
            union
            {
                struct {
                    TFlt32 x, y;
                };

                TFlt32 vec_array[2];     // array access
            };
        };

        //--------------------------------------------------------------------------------
        /** Computes the squared magnitude */
        COMMON_BASE_EXPORT float sq_length(const Vector2f &n);

        /** Computes the magnitude */
        COMMON_BASE_EXPORT float length(const Vector2f &n);
        COMMON_BASE_EXPORT float DotProduct(const Vector2f &v1, const Vector2f &v2);

    }
}