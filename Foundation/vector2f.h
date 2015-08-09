#ifndef __lsd_vector2f_h__
#define __lsd_vector2f_h__

#ifdef _WIN32
#pragma once
#endif

#include "platform_specific.h"

//------------------------------------------------------------------------
class EXPORT Vector2f
{
public:
	Vector2f();
	Vector2f(float _x, float _y);
	Vector2f(const Vector2f &Source);

	void Set(const float _x, const float _y);

	__forceinline float* GetPtr() { return vec_array; }
	__forceinline const float* GetPtr() const { return vec_array;	}
	__forceinline scalar_t Normalize();

	__forceinline Vector2f& Vector2f::operator+=(const Vector2f& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	__forceinline Vector2f Vector2f::operator+(const Vector2f& v)const
	{
		Vector2f out; 

		out.x = x + v.x;
		out.y = y + v.y;
	
		return out;
	}

	__forceinline Vector2f Vector2f::operator-(const Vector2f& v)const
	{
		Vector2f out; 

		out.x = x - v.x;
		out.y = y - v.y;

		return out;
	}

	__forceinline Vector2f Vector2f::operator*(float f)const
	{
		Vector2f out; 

		out.x = x * f;
		out.y = y * f;

		return out;
	}

	__forceinline float Length() const;
	__forceinline float LengthSqr() const;
	__forceinline float Dot(const Vector2f& _vector_other)const;

public:
	union
	{
	  struct{
         float x,y;
	  };
     
      float vec_array[2];     // array access
	};
};

//--------------------------------------------------------------------------------
/** Computes the squared magnitude */
__forceinline  float sq_length(const Vector2f & n)
{ 
	return n.x * n.x + n.y * n.y; 
}
//--------------------------------------------------------------------------------
/** Computes the magnitude */
__forceinline EXPORT float length(const Vector2f & n)
{
	return sqrtf(sq_length(n)); 
}
//----------------------------------------------------------------------------------
__forceinline float DotProduct(const Vector2f& v1, const Vector2f& v2)
{
	return  v1.x * v2.x + v1.y * v2.y; 
}
//----------------------------------------------------------------------------------
__forceinline float Vector2f::Length() const
{
	return ::sqrtf(x * x + y * y);
}
//----------------------------------------------------------------------------------
__forceinline float Vector2f::LengthSqr() const
{
	return x * x + y * y;
}
//----------------------------------------------------------------------------------
__forceinline float Vector2f::Dot(const Vector2f& _vector_other)const
{
	return DotProduct(*this,_vector_other);
}
#endif //__lsd_vector2f_h__