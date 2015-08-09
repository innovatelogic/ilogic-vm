#ifndef __lsd_quat_h__
#define __lsd_quat_h__

#ifdef _WIN32
#pragma once
#endif

#include "matrix3.h"

class EXPORT Quaternion
{
public:
    Quaternion();
	Quaternion(float _x, float _y, float _z, float _w);

	inline void Set(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 1.f) { x = _x; y = _y; z = _z; w = _w; }

	float  operator[] (int i)const;
	float  operator[] (int i);
	Quaternion& operator*= (const Quaternion& q);

	void FromMatrix(const Matrix3f &mat);
	void ToMatrix(Matrix3f &mat) const;
	void ToMatrix(class Matrix *mat) const;
	
	void set_rot(float yaw, float pitch, float roll);

	Quaternion Inverse();
	void Normalize();

	__forceinline float* GetPtr() { return q; }
	__forceinline const float* GetPtr() const { return q; }

public:
    union
	{
       struct{
          float x, y, z, w;
       }; 
       float q[4];
	};
};

EXTERN_EXPORT const Quaternion operator*(const Quaternion& p, const Quaternion& q);
EXTERN_EXPORT float dot(const Quaternion& q1, const Quaternion& q2);
EXTERN_EXPORT Quaternion & Slerp(Quaternion & p, float s, const Quaternion & q1, const Quaternion & q2);

#endif // __lsd_quat_h__