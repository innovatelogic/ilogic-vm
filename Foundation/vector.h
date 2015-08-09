//----------------------------------------------------------------------------------------------
// Copyright 2006
// Auth: Yur.G
//
//----------------------------------------------------------------------------------------------

#ifndef __lsd_vector_h__
#define __lsd_vector_h__

#ifdef _WIN32
#pragma once
#endif

#include "platform_specific.h"

//----------------------------------------------------------------------------------------------
class EXPORT Vector
{
public:

	Vector();
	Vector(const float _x, const float _y, const float _z);
	Vector(const Vector &Source);

	void Set(const float _x, const float _y, const float _z);

	float  operator[](int i)const;
	float& operator[](int i);

	bool operator==(const Vector& _v)const;
	bool operator!=(const Vector& _v)const;

	__forceinline Vector& operator+=(const Vector& v); 
	__forceinline Vector& operator-=(const Vector& v); 
	__forceinline Vector& operator*=(const Vector& v); 
	__forceinline Vector& operator*=(const float f); 
	__forceinline Vector& operator/=(const Vector& v); 
	__forceinline Vector& operator/=(const float  f); 

	Vector operator+(const Vector& v)const;
	Vector operator-(const Vector& v)const;
	Vector operator*(const Vector& v)const;
	Vector operator/(const Vector& v)const;
	Vector operator*(float f);
	Vector operator/(float f);

	Vector  Vector::operator-(void) const;

	Vector& operator=(const Vector &vOther);

	void Random(Vector& v_min,Vector& v_max);
	void Random(float disp_x, float disp_y, float disp_z);

	void Negate();
	bool IsZero(float _tolerance);

	__forceinline float Length();
	__forceinline float LengthSqr();

	scalar_t normalize();

    __forceinline float* GetPtr() { return vec_array; }
	__forceinline const float* GetPtr() const { return vec_array; }

	__forceinline float  Dot(const Vector& _vector_other)const;
	__forceinline Vector Cross(const Vector& _vector_other)const;

	__forceinline Vector Vector::operator*(float f)const
	{
		Vector out; 

		out.x = x * f;
		out.y = y * f;
		out.z = z * f;

		return out;
	}

public:
	union  // data
	{
		struct{
			float x,y,z;
		};

		float vec_array[3];     // array access
	};
};

//----------------------------------------------------------------------------------------------

__forceinline EXPORT float   DotProduct(const Vector& v1, const Vector& v2);
__forceinline EXPORT Vector  CrossProduct(const Vector& v1, const Vector& v2,Vector& _result);
__forceinline EXPORT void    VectorAbs(const Vector& vIn, const Vector& v2,Vector& vOut); 

__forceinline EXPORT Vector& sub(Vector &u, const Vector &v, const Vector &w);
__forceinline EXPORT Vector& add(Vector &u, const Vector &v, const Vector &w);
__forceinline EXPORT Vector& cross(Vector &u, const Vector &v, const Vector &w);

__forceinline EXPORT Vector& scale(Vector &u, const float s);

// normalizes a vector and return a reference of itself
__forceinline EXPORT Vector& normalize(Vector &u);

// Computes the squared magnitude
__forceinline  float sq_length(const Vector & n)
{ 
	return n.x * n.x + n.y * n.y + n.z * n.z; 
}

//----------------------------------------------------------------------------------------------
// Computes the magnitude
__forceinline EXPORT float length(const Vector & n)
{ 
	return sqrtf(sq_length(n)); 
}

//----------------------------------------------------------------------------------------------
__forceinline float Vector::operator[](int i) const
{
	assert((i>=0)&&(i<3));
	return ((float*)this)[i]; 
}

//----------------------------------------------------------------------------------------------
__forceinline float& Vector::operator[](int i)
{
	assert((i>=0)&&(i<3));
	return ((float*)this)[i]; 
}
//----------------------------------------------------------------------------------------------
inline Vector& Vector::operator=(const Vector &vOther)	
{
	x=vOther.x; y=vOther.y; z=vOther.z; 
	return *this; 
}
//----------------------------------------------------------------------------------------------
__forceinline Vector& Vector::operator+=(const Vector& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

//----------------------------------------------------------------------------------------------
__forceinline Vector& Vector::operator-=(const Vector& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}
//----------------------------------------------------------------------------------------------
__forceinline Vector& Vector::operator*=(const Vector& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

//----------------------------------------------------------------------------------------------
__forceinline Vector Vector::operator*(float f)
{
	Vector out;

	out.x = x * f;
	out.y = y * f;
	out.z = z * f;

	return out;
}

//----------------------------------------------------------------------------------------------
__forceinline Vector Vector::operator/(float f)
{
	assert(f!=0.f);

	Vector out;

	out.x = x / f;
	out.y = y / f;
	out.z = z / f;

	return out;
}

//----------------------------------------------------------------------------------------------
__forceinline Vector& Vector::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}
//----------------------------------------------------------------------------------------------
__forceinline Vector& Vector::operator/=(const Vector &v)
{
	assert(v.x!=0.f);
	assert(v.y!=0.f);
	assert(v.z!=0.f);

	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}
//----------------------------------------------------------------------------------------------
__forceinline Vector& Vector::operator/=(const float f)
{
	assert(f!=0.f);

	x *= f;
	y *= f;
	z *= f;

	return *this;
}

//----------------------------------------------------------------------------------------------
__forceinline Vector Vector::operator+(const Vector &v)const
{
    Vector out; 

	out.x = x + v.x;
	out.y = y + v.y;
	out.z = z + v.z;

	return out;
}
//----------------------------------------------------------------------------------------------
__forceinline Vector Vector::operator-(const Vector &v)const
{
    Vector out; 

	out.x = x - v.x;
	out.y = y - v.y;
	out.z = z - v.z;

	return out;
}

//----------------------------------------------------------------------------------------------
inline bool Vector::operator == (const Vector &v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

//----------------------------------------------------------------------------------------------
inline bool Vector::operator !=(const Vector &v) const
{
	return (x != v.x) || (y != v.y) || (z != v.z);
}

//----------------------------------------------------------------------------------------------
__forceinline Vector  Vector::operator-(void) const
{
	return Vector(-x,-y,-z);
}

//----------------------------------------------------------------------------------------------
__forceinline float Vector::Length()
{
	return ::sqrtf(x * x + y * y + z * z);
}

//----------------------------------------------------------------------------------------------
__forceinline float Vector::LengthSqr()
{
   return x * x + y * y + z * z;
}

//----------------------------------------------------------------------------------------------
__forceinline float Vector::Dot(const Vector &_vector_other) const
{
	return DotProduct(*this,_vector_other);
}

//----------------------------------------------------------------------------------------------
__forceinline Vector Vector::Cross(const Vector &_vector_other) const
{
	Vector out;
	return CrossProduct(*this,_vector_other,out);
}
//----------------------------------------------------------------------------------------------
__forceinline float DotProduct(const Vector &v1, const Vector &v2)
{
	return  v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; 
}

//----------------------------------------------------------------------------------------------
__forceinline Vector CrossProduct(const Vector &a, const Vector &b, Vector &_result)
{
	_result.x = a.y*b.z - a.z*b.y;
	_result.y = a.z*b.x - a.x*b.z;
	_result.z = a.x*b.y - a.y*b.x;
	return _result;
} 

//----------------------------------------------------------------------------------------------
__forceinline void VectorAbs(const Vector &vIn, const Vector &v2, Vector &vOut)
{
	vOut.x = fabs(vIn.x);
	vOut.y = fabs(vIn.y);
	vOut.z = fabs(vIn.z);
}

#endif // __lsd_vector_h__