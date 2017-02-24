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
class COMMON_BASE_EXPORT Vector
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

	 Vector& operator+= (const Vector &v); 
	 Vector& operator-= (const Vector &v); 
	 Vector& operator*= (const Vector &v); 
	 Vector& operator*= (const TFlt32 f); 
	 Vector& operator/= (const Vector &v); 
	 Vector& operator/= (const TFlt32  f); 

	Vector operator+ (const Vector &v)const;
	Vector operator- (const Vector &v)const;
	//Vector operator* (const Vector &v)const;
	//Vector operator/ (const Vector &v)const;
	Vector operator* (TFlt32 f);
    Vector operator* (TFlt32 f) const;
	Vector operator/ (TFlt32 f);

	Vector  Vector::operator-(void) const;

	Vector& operator= (const Vector &vOther);

	//void Random(Vector &v_min,Vector &v_max);
	//void Random(TFlt32 disp_x, TFlt32 disp_y, TFlt32 disp_z);

	void Negate();
	//bool IsZero(TFlt32 _tolerance);

	 TFlt32 Length();
	 TFlt32 LengthSqr();

	TFlt32 normalize();

     TFlt32* GetPtr() { return vec_array; }
	 const TFlt32* GetPtr() const { return vec_array; }

	 TFlt32  Dot(const Vector& _vector_other)const;
	 Vector Cross(const Vector& _vector_other)const;

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

COMMON_BASE_EXPORT  TFlt32   DotProduct(const Vector& v1, const Vector& v2);
COMMON_BASE_EXPORT  Vector  CrossProduct(const Vector& v1, const Vector& v2,Vector& _result);
COMMON_BASE_EXPORT  void    VectorAbs(const Vector& vIn, const Vector& v2,Vector& vOut);

COMMON_BASE_EXPORT  Vector& sub(Vector &u, const Vector &v, const Vector &w);
COMMON_BASE_EXPORT  Vector& add(Vector &u, const Vector &v, const Vector &w);
COMMON_BASE_EXPORT  Vector& cross(Vector &u, const Vector &v, const Vector &w);

COMMON_BASE_EXPORT  Vector& scale(Vector &u, const TFlt32 s);

// normalizes a vector and return a reference of itself
COMMON_BASE_EXPORT  Vector& normalize(Vector &u);

// Computes the squared magnitude
COMMON_BASE_EXPORT  TFlt32 sq_length(const Vector & n);

//----------------------------------------------------------------------------------------------
// Computes the magnitude
COMMON_BASE_EXPORT TFlt32 length(const Vector & n);
COMMON_BASE_EXPORT TFlt32 DotProduct(const Vector &v1, const Vector &v2);
COMMON_BASE_EXPORT Vector CrossProduct(const Vector &a, const Vector &b, Vector &_result);
COMMON_BASE_EXPORT void VectorAbs(const Vector &vIn, const Vector &v2, Vector &vOut);
}
}