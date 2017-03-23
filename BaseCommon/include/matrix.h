
#pragma once

#include <string>
#include "vector.h"
#include "vector2f.h"
#include "vector4f.h"
#include "quat.h"

namespace oes
{
namespace common_base
{
//------------------------------------------------------------------------
//     class Matrix 
//
//     X(axis).x X(axis).y X(axis).z 0 
//     Y(axis).x Y(axis).y Y(axis).z 0
//     Z(axis).x Z(axis).y Z(axis).z 0
//     T(rans).x T(rans).y T(rans).z 1
//
// links: http://www.euclideanspace.com/maths/geometry/affine/reflection/index.htm
//        http://www.euclideanspace.com/maths/geometry/elements/plane/lineOnPlane/index.htm

//--------------------------------------------------------------------------------------
class COMMON_BASE_EXPORT Matrix
{
public:
	Matrix();
	Matrix(const Matrix& Source);
	Matrix(const TFlt32* f);
	Matrix(const Quaternion& rot, const Vector& pos);

	Matrix( TFlt32 m_00, TFlt32 m_01, TFlt32 m_02, TFlt32 m_03,
            TFlt32 m_10, TFlt32 m_11, TFlt32 m_12, TFlt32 m_13,
            TFlt32 m_20, TFlt32 m_21, TFlt32 m_22, TFlt32 m_23,
            TFlt32 m_30, TFlt32 m_31, TFlt32 m_32, TFlt32 m_33);
	
	void Set(TFlt32 m_00, TFlt32 m_01, TFlt32 m_02, TFlt32 m_03,
        TFlt32 m_10, TFlt32 m_11, TFlt32 m_12, TFlt32 m_13,
        TFlt32 m_20, TFlt32 m_21, TFlt32 m_22, TFlt32 m_23,
        TFlt32 m_30, TFlt32 m_31, TFlt32 m_32, TFlt32 m_33);

	Matrix& operator= (const Matrix &other);
	Matrix& operator= (const TFlt32  *pOther);
    Matrix  operator* (const Matrix &other) const;
	Matrix  operator* (const Matrix3f &other) const;
	Matrix  operator* (const TFlt32 m) const;

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
	void set_rot(TFlt32 yaw, TFlt32 pitch, TFlt32 roll);
	void set_translation(const Vector &v);

	void SetRotateAxis(const Vector &v, const TFlt32 rad);

	float* GetPtr() { return m;	}
	const float* GetPtr() const { return m;	}

	void Identitly();
	void Transpose();

	float Determinant() const;

public:
	union {
		struct
		{
            TFlt32 a00, a10, a20, a30;   // standard names for components
            TFlt32 a01, a11, a21, a31;   // standard names for components
            TFlt32 a02, a12, a22, a32;   // standard names for components
            TFlt32 a03, a13, a23, a33;   // standard names for components
		};
		struct {
            TFlt32 _11, _12, _13, _14;   // standard names for components
            TFlt32 _21, _22, _23, _24;   // standard names for components
            TFlt32 _31, _32, _33, _34;   // standard names for components
            TFlt32 _41, _42, _43, _44;   // standard names for components
		};
		struct {
            TFlt32 b00, b10, b20, p; // standard names for components
            TFlt32 b01, b11, b21, q; // standard names for components
            TFlt32 b02, b12, b22, r; // standard names for components
            TFlt32 x, y, z, w;       // standard names for components
		};
		struct {
			Vector _row0; TFlt32 _w0;
			Vector _row1; TFlt32 _w1;
			Vector _row2; TFlt32 _w2;
			Vector t;     TFlt32 tw;
		};
		struct {
			Vector2f __row0; TFlt32 __row0z; TFlt32 __w0;
			Vector2f __row1; TFlt32 __row1z; TFlt32 __w1;
			Vector2f __row2; TFlt32 __row2z; TFlt32 __w2;
			Vector2f t2;     TFlt32 t2z;     TFlt32 t2w;
		};
        struct {
            Vector4f row0;
            Vector4f row1;
            Vector4f row2;
            Vector4f row3;
        };
        TFlt32 m[16];
	};

 	std::string ToString()
 	{
		return std::to_string(m[0]) + ";" +
				std::to_string(m[1]) + ";" +
				std::to_string(m[2]) + ";" +
				std::to_string(m[3]) + ";" +
				std::to_string(m[4]) + ";" +
				std::to_string(m[5]) + ";" +
				std::to_string(m[6]) + ";" +
				std::to_string(m[7]) + ";" +
				std::to_string(m[8]) + ";" +
				std::to_string(m[9]) + ";" +
				std::to_string(m[10]) + ";" +
				std::to_string(m[11]) + ";" +
				std::to_string(m[12]) + ";" +
				std::to_string(m[13]) + ";" +
				std::to_string(m[14]) + ";" +
				std::to_string(m[15]) + ";";
 	}
};

Matrix GetMatrixIdentitly(const Vector &_translation);
Matrix GetMatrixScale(const Vector &_scale );
Matrix GetMatrixTranslation(const Vector& _translation);

/** C = A * B */
Matrix & mult(Matrix & C, const Matrix & A, const Matrix & B);
Matrix & mult(Matrix & C, const Matrix & A, const Matrix3f & B);

/* invert */
//Matrix & invert(Matrix & B, const Matrix & A);

}
}
