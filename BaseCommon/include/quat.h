#pragma once

#include "matrix3.h"

namespace oes
{
namespace common_base
{

class Quaternion
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

const Quaternion operator*(const Quaternion& p, const Quaternion& q);
float dot(const Quaternion& q1, const Quaternion& q2);
Quaternion & Slerp(Quaternion & p, float s, const Quaternion & q1, const Quaternion & q2);

}
}