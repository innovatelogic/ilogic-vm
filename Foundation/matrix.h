//------------------------------------------------------------------------
// Copyright 2006
// Auth: Yur.G
//
//------------------------------------------------------------------------
//     class Matrix 
//
//     X(axis).x X(axis).y X(axis).z 0 
//     Y(axis).x Y(axis).y Y(axis).z 0
//     Z(axis).x Z(axis).y Z(axis).z 0
//     T(rans).x T(rans).y T(rans).z 1

// links: http://www.euclideanspace.com/maths/geometry/affine/reflection/index.htm
//        http://www.euclideanspace.com/maths/geometry/elements/plane/lineOnPlane/index.htm

#ifndef __lsd_matrix_h__
#define __lsd_matrix_h__

#ifdef _WIN32
   #pragma once
#endif

#include "vector.h"
#include "vector2f.h"
#include "vector4f.h"
#include "quat.h"

//--------------------------------------------------------------------------------------
class EXPORT Matrix
{
public:
	Matrix();
	Matrix(const Matrix& Source);
	Matrix(const float* f);
	Matrix(const Quaternion& rot, const Vector& pos);

	Matrix(float m_00, float m_01, float m_02, float m_03,
		   float m_10, float m_11, float m_12, float m_13, 
		   float m_20, float m_21, float m_22, float m_23, 
		   float m_30, float m_31, float m_32, float m_33);
	
	void Set(float m_00, float m_01, float m_02, float m_03,
		     float m_10, float m_11, float m_12, float m_13, 
		     float m_20, float m_21, float m_22, float m_23, 
		     float m_30, float m_31, float m_32, float m_33);

	Matrix& operator= (const Matrix &other);
	Matrix& operator= (const float  *pOther);
    Matrix  operator* (const Matrix &other) const;
	Matrix  operator* (const Matrix3f &other) const;
	Matrix  operator* (const float m) const;

	bool operator== (const Matrix &other) const;
	bool operator!= (const Matrix &other) const;

	float& operator[] (int i);
	float  operator[] (int i) const;

	const float& operator()(const int i, const int j) const
	{
		return m[j * 4 + i];
	}

	// Computes B = inverse(A)
//       -1
//  B = A
    Matrix& invert(Matrix &B, const Matrix &A);
    Matrix	inverse() const;   

	void set_rot(const Quaternion &q);
	void set_rot(const Matrix3f &M);
	void set_rot(float yaw, float pitch, float roll);
	void set_translation(const Vector &v);

	void SetRotateAxis(const Vector &v, const float rad);

	float* GetPtr() { return m;	}
	const float* GetPtr() const { return m;	}

	void Identitly();
	void Transpose();

	float Determinant() const;

public:
	union {
		struct
		{
			float a00, a10, a20, a30;   // standard names for components
			float a01, a11, a21, a31;   // standard names for components
			float a02, a12, a22, a32;   // standard names for components
			float a03, a13, a23, a33;   // standard names for components
		};
		struct {
			float _11, _12, _13, _14;   // standard names for components
			float _21, _22, _23, _24;   // standard names for components
			float _31, _32, _33, _34;   // standard names for components
			float _41, _42, _43, _44;   // standard names for components
		};
		struct {
			float b00, b10, b20, p; // standard names for components
			float b01, b11, b21, q; // standard names for components
			float b02, b12, b22, r; // standard names for components
			float x, y, z, w;       // standard names for components
		};
		struct {
			Vector _row0; float _w0;
			Vector _row1; float _w1;
			Vector _row2; float _w2;
			Vector t;     float tw;
		};
		struct {
			Vector2f __row0; float __row0z; float __w0;
			Vector2f __row1; float __row1z; float __w1;
			Vector2f __row2; float __row2z; float __w2;
			Vector2f t2;    float t2z;    float t2w;
		};
        struct {
            Vector4f row0;
            Vector4f row1;
            Vector4f row2;
            Vector4f row3;
        };
	   float m[16];
	};

 	std::string ToString()
 	{
		return "TODO";/* boost::lexical_cast<std::string>(m[0]) + ";" +
 			boost::lexical_cast<std::string>(m[1]) + ";" +
 			boost::lexical_cast<std::string>(m[2]) + ";" +
 			boost::lexical_cast<std::string>(m[3]) + ";" + 
 			boost::lexical_cast<std::string>(m[4]) + ";" + 
 			boost::lexical_cast<std::string>(m[5]) + ";" + 
 			boost::lexical_cast<std::string>(m[6]) + ";" +
 			boost::lexical_cast<std::string>(m[7]) + ";" +
 			boost::lexical_cast<std::string>(m[8]) + ";" +
 			boost::lexical_cast<std::string>(m[9]) + ";" +
 			boost::lexical_cast<std::string>(m[10]) + ";" +
 			boost::lexical_cast<std::string>(m[11]) + ";" + 
 			boost::lexical_cast<std::string>(m[12]) + ";" + 
 			boost::lexical_cast<std::string>(m[13]) + ";" + 
 			boost::lexical_cast<std::string>(m[14]) + ";" + 
 			boost::lexical_cast<std::string>(m[15]) + ";";*/
 	}
};

EXPORT Matrix GetMatrixIdentitly(const Vector &_translation);
EXPORT Matrix GetMatrixScale(const Vector &_scale );
EXPORT Matrix GetMatrixTranslation(const Vector& _translation);

/** C = A * B */
Matrix & mult(Matrix & C, const Matrix & A, const Matrix & B);
Matrix & mult(Matrix & C, const Matrix & A, const Matrix3f & B);

/* invert */
//Matrix & invert(Matrix & B, const Matrix & A);

#endif //__lsd_matrix_h__