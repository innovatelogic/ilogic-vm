#pragma once

#include <assert.h>
#include "OEMBase.h"

class EXPORT Vector4f
{

public:
	Vector4f();
	Vector4f(float x,float y,float z,float w = 1.f);
	Vector4f(const Vector4f &Source);

	void Set(const float _x, const float _y, const float _z, const float _w);

	float  operator[](int i)const;
	float& operator[](int i);

	bool operator!=(const Vector4f& _v) const;

	__forceinline float* GetPtr() { return vec_array; }
	__forceinline const float* GetPtr() const { return vec_array; }

public:

	union  // data
	{
		struct{
			float x,y,z,w;
		};

		struct{
			float U,V,UL,VL;
		};

		float vec_array[4];     // array access
	};

};

//////////////////////////////////////////////////////////////////////////
__forceinline float Vector4f::operator[](int i)const
{
	assert((i>=0)&&(i<4));
	return ((float*)this)[i]; 
}

//////////////////////////////////////////////////////////////////////////
__forceinline float& Vector4f::operator[](int i)
{
	assert((i>=0)&&(i<4));
	return ((float*)this)[i]; 
}