#pragma once

#include <assert.h>
#include <math.h>
#include "OEMBase.h"
#include "TypesBase.h"

namespace oes
{
namespace common_base
{
//----------------------------------------------------------------------------------------------
class Vector
{
public:

	Vector();
	Vector(const TFlt32 _x, const TFlt32 _y, const TFlt32 _z);
	Vector(const Vector &Source);

	void Set(const TFlt32 _x, const TFlt32 _y, const TFlt32 _z);

	TFlt32  operator[] (int i)const;
	TFlt32& operator[] (int i);

	bool operator== (const Vector &_v)const;
	bool operator!= (const Vector &_v)const;

	__forceinline Vector& operator+= (const Vector &v); 
	__forceinline Vector& operator-= (const Vector &v); 
	__forceinline Vector& operator*= (const Vector &v); 
	__forceinline Vector& operator*= (const TFlt32 f); 
	__forceinline Vector& operator/= (const Vector &v); 
	__forceinline Vector& operator/= (const TFlt32  f); 

	Vector operator+ (const Vector &v)const;
	Vector operator- (const Vector &v)const;
	Vector operator* (const Vector &v)const;
	Vector operator/ (const Vector &v)const;
	Vector operator* (TFlt32 f);
	Vector operator/ (TFlt32 f);

	Vector  Vector::operator-(void) const;

	Vector& operator= (const Vector &vOther);

	void Random(Vector &v_min,Vector &v_max);
	void Random(TFlt32 disp_x, TFlt32 disp_y, TFlt32 disp_z);

	void Negate();
	bool IsZero(TFlt32 _tolerance);

	__forceinline TFlt32 Length();
	__forceinline TFlt32 LengthSqr();

	TFlt32 normalize();

    __forceinline TFlt32* GetPtr() { return vec_array; }
	__forceinline const TFlt32* GetPtr() const { return vec_array; }

	__forceinline TFlt32  Dot(const Vector& _vector_other)const;
	__forceinline Vector Cross(const Vector& _vector_other)const;

	__forceinline Vector Vector::operator*(TFlt32 f)const
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
			TFlt32 x,y,z;
		};

		TFlt32 vec_array[3];     // array access
	};
};

//----------------------------------------------------------------------------------------------

__forceinline TFlt32   DotProduct(const Vector& v1, const Vector& v2);
__forceinline Vector  CrossProduct(const Vector& v1, const Vector& v2,Vector& _result);
__forceinline void    VectorAbs(const Vector& vIn, const Vector& v2,Vector& vOut); 

__forceinline Vector& sub(Vector &u, const Vector &v, const Vector &w);
__forceinline Vector& add(Vector &u, const Vector &v, const Vector &w);
__forceinline Vector& cross(Vector &u, const Vector &v, const Vector &w);

__forceinline Vector& scale(Vector &u, const TFlt32 s);

// normalizes a vector and return a reference of itself
__forceinline Vector& normalize(Vector &u);

// Computes the squared magnitude
__forceinline  TFlt32 sq_length(const Vector & n)
{ 
	return n.x * n.x + n.y * n.y + n.z * n.z; 
}

//----------------------------------------------------------------------------------------------
// Computes the magnitude
__forceinline TFlt32 length(const Vector & n)
{ 
	return sqrtf(sq_length(n)); 
}

//----------------------------------------------------------------------------------------------
__forceinline TFlt32 Vector::operator[](int i) const
{
	assert((i>=0)&&(i<3));
	return ((TFlt32*)this)[i]; 
}

//----------------------------------------------------------------------------------------------
__forceinline TFlt32& Vector::operator[](int i)
{
	assert((i>=0)&&(i<3));
	return ((TFlt32*)this)[i]; 
}
//----------------------------------------------------------------------------------------------
inline Vector& Vector::operator=(const Vector &vOther)	
{
	x=vOther.x;
    y=vOther.y;
    z=vOther.z; 

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
__forceinline Vector Vector::operator*(TFlt32 f)
{
	Vector out;

	out.x = x * f;
	out.y = y * f;
	out.z = z * f;

	return out;
}

//----------------------------------------------------------------------------------------------
__forceinline Vector Vector::operator/(TFlt32 f)
{
	assert(f!=0.f);

	Vector out;

	out.x = x / f;
	out.y = y / f;
	out.z = z / f;

	return out;
}

//----------------------------------------------------------------------------------------------
__forceinline Vector& Vector::operator*=(const TFlt32 f)
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
__forceinline Vector& Vector::operator/=(const TFlt32 f)
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
	return Vector(-x, -y, -z);
}

//----------------------------------------------------------------------------------------------
__forceinline TFlt32 Vector::Length()
{
	return ::sqrtf(x * x + y * y + z * z);
}

//----------------------------------------------------------------------------------------------
__forceinline TFlt32 Vector::LengthSqr()
{
   return x * x + y * y + z * z;
}

//----------------------------------------------------------------------------------------------
__forceinline TFlt32 Vector::Dot(const Vector &_vector_other) const
{
	return DotProduct(*this,_vector_other);
}

//----------------------------------------------------------------------------------------------
__forceinline Vector Vector::Cross(const Vector &_vector_other) const
{
	Vector out;
	return CrossProduct(*this, _vector_other, out);
}
//----------------------------------------------------------------------------------------------
__forceinline TFlt32 DotProduct(const Vector &v1, const Vector &v2)
{
	return  v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
}

//----------------------------------------------------------------------------------------------
__forceinline Vector CrossProduct(const Vector &a, const Vector &b, Vector &_result)
{
	_result.x = a.y * b.z - a.z * b.y;
	_result.y = a.z * b.x - a.x * b.z;
	_result.z = a.x * b.y - a.y * b.x;
	return _result;
} 

//----------------------------------------------------------------------------------------------
__forceinline void VectorAbs(const Vector &vIn, const Vector &v2, Vector &vOut)
{
	vOut.x = fabs(vIn.x);
	vOut.y = fabs(vIn.y);
	vOut.z = fabs(vIn.z);
}

}
}