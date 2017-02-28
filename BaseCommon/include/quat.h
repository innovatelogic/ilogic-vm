#pragma once

#include "TypesBase.h"
#include "matrix3.h"

namespace oes
{
    namespace common_base
    {

        class COMMON_BASE_EXPORT Quaternion
        {
        public:
            Quaternion();
            Quaternion(TFlt32 _x, TFlt32 _y, TFlt32 _z, TFlt32 _w);

            inline void Set(TFlt32 _x = 0.0f, TFlt32 _y = 0.0f, TFlt32 _z = 0.0f, TFlt32 _w = 1.f) { x = _x; y = _y; z = _z; w = _w; }

            float  operator[] (int i)const;
            float  operator[] (int i);
            Quaternion& operator*= (const Quaternion& q);

            void FromMatrix(const Matrix3f &mat);
            void ToMatrix(Matrix3f &mat) const;
            void ToMatrix(class Matrix *mat) const;

            void set_rot(TFlt32 yaw, TFlt32 pitch, TFlt32 roll);

            Quaternion Inverse();
            void Normalize();

            TFlt32* GetPtr() { return q; }
            const TFlt32* GetPtr() const { return q; }

        public:
            union
            {
                struct {
                    TFlt32 x, y, z, w;
                };
                TFlt32 q[4];
            };
        };

        COMMON_BASE_EXPORT const Quaternion operator*(const Quaternion &p, const Quaternion &q);
        COMMON_BASE_EXPORT TFlt32 dot(const Quaternion &q1, const Quaternion &q2);
        COMMON_BASE_EXPORT Quaternion& Slerp(Quaternion &p, TFlt32 s, const Quaternion &q1, const Quaternion &q2);

    }
}