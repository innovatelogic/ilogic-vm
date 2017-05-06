#include <stdio.h>
#include "mathlib.h"

namespace oes
{
namespace common_base
{

//----------------------------------------------------------------------------------------------
Matrix::Matrix()
{
	Set(1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f, 
		0.f, 0.f, 1.f, 0.f, 
		0.f, 0.f, 0.f, 1.f);
}
//----------------------------------------------------------------------------------------------
 Matrix::Matrix(const Matrix& Source)
 {
 	if (this != &Source)
 	{
		memcpy(m, Source.m, sizeof(float) * 16);
 	}
 }
//----------------------------------------------------------------------------------------------
 Matrix::Matrix(const Quaternion& rot, const Vector& pos)
{
	set_rot(rot);
	set_translation(pos);
}
//----------------------------------------------------------------------------------------------
Matrix::Matrix(float m_00, float m_01, float m_02, float m_03,
			   float m_10, float m_11, float m_12, float m_13, 
			   float m_20, float m_21, float m_22, float m_23, 
			   float m_30, float m_31, float m_32, float m_33)
{
	Set(m_00, m_01, m_02, m_03,
		m_10, m_11, m_12, m_13, 
		m_20, m_21, m_22, m_23, 
		m_30, m_31, m_32, m_33);
}

//----------------------------------------------------------------------------------------------
Matrix::Matrix(const float* f)
{
	// check size

	Set(*(f + 0), *(f + 1), *(f + 2), *(f + 3),
		*(f + 4), *(f + 5), *(f + 6), *(f + 7), 
		*(f + 8), *(f + 9), *(f + 10), *(f + 11), 
		*(f + 12), *(f + 13), *(f + 14), *(f + 15));
}

//----------------------------------------------------------------------------------------------
void Matrix::Set(float m_00, float m_01, float m_02, float m_03,
				 float m_10, float m_11, float m_12, float m_13, 
				 float m_20, float m_21, float m_22, float m_23, 
				 float m_30, float m_31, float m_32, float m_33)
{
	m[0] = m_00;
	m[1] = m_01;
	m[2] = m_02;
	m[3] = m_03;

	m[4] = m_10;
	m[5] = m_11;
	m[6] = m_12;
	m[7] = m_13;

	m[8] = m_20;
	m[9] = m_21;
	m[10] = m_22;
	m[11] = m_23;

	m[12] = m_30;
	m[13] = m_31;
	m[14] = m_32;
	m[15] = m_33;
}

//----------------------------------------------------------------------------------------------
Matrix& Matrix::operator= (const Matrix &other)
{
	m[0] = other.m[0];
	m[1] = other.m[1];
	m[2] = other.m[2];
	m[3] = other.m[3];

	m[4] = other.m[4];
	m[5] = other.m[5];
	m[6] = other.m[6];
	m[7] = other.m[7];

	m[8] = other.m[8];
	m[9] = other.m[9];
	m[10] = other.m[10];
	m[11] = other.m[11];

	m[12] = other.m[12];
	m[13] = other.m[13];
	m[14] = other.m[14];
	m[15] = other.m[15];

   return *this;
}

//----------------------------------------------------------------------------------------------
Matrix& Matrix::operator= (const float *pOther)
{
	m[0] = *(pOther + 0);
	m[1] = *(pOther + 1);
	m[2] = *(pOther + 2);
	m[3] = *(pOther + 3);

	m[4] = *(pOther + 4);
	m[5] = *(pOther + 5);
	m[6] = *(pOther + 6);
	m[7] = *(pOther + 7);

	m[8] = *(pOther + 8);
	m[9] = *(pOther + 9);
	m[10] = *(pOther + 10);
	m[11] = *(pOther + 11);

	m[12] = *(pOther + 12);
	m[13] = *(pOther + 13);
	m[14] = *(pOther + 14);
	m[15] = *(pOther + 15);

   return *this;
}

//----------------------------------------------------------------------------------------------
Matrix Matrix::operator* (const Matrix &other)const
{
	Matrix matrix;
	mult(matrix, *this, other);
	return matrix;
}

//----------------------------------------------------------------------------------------------
Matrix Matrix::operator* (const Matrix3f &other)const
{
	Matrix matrix;
	mult(matrix, *this, other);
	return matrix;
}

//----------------------------------------------------------------------------------------------
Matrix Matrix::operator* (const float m)const
{
	Matrix matrix = *this;
	matrix.m[0] *= m;
	matrix.m[1] *= m;
	matrix.m[2] *= m;
	matrix.m[3] *= m;

	matrix.m[4] *= m;
	matrix.m[5] *= m;
	matrix.m[6] *= m;
	matrix.m[7] *= m;

	matrix.m[8] *= m;
	matrix.m[9] *= m;
	matrix.m[10] *= m;
	matrix.m[11] *= m;

	matrix.m[12] *= m;
	matrix.m[13] *= m;
	matrix.m[14] *= m;
	matrix.m[15] *= m;
	return matrix;
}

//----------------------------------------------------------------------------------------------
bool Matrix::operator== (const Matrix& _m)const
{
	return (
			m[0] == _m.m[0] &&
			m[1] == _m.m[1] &&
			m[2] == _m.m[2] &&
			m[3] == _m.m[3] &&
			m[4] == _m.m[4] &&
			m[5] == _m.m[5] &&
			m[6] == _m.m[6] &&
			m[7] == _m.m[7] &&
			m[8] == _m.m[8] &&
			m[9] == _m.m[9] &&
			m[10] == _m.m[10] &&
			m[11] == _m.m[11] &&
			m[12] == _m.m[12] &&
			m[13] == _m.m[13] &&
			m[14] == _m.m[14] &&
			m[15] == _m.m[15]
		);
}

//----------------------------------------------------------------------------------------------
bool Matrix::operator!= (const Matrix& _m)const
{
	return (
		m[0] != _m.m[0] ||
		m[1] != _m.m[1] ||
		m[2] != _m.m[2] ||
		m[3] != _m.m[3] ||
		m[4] != _m.m[4] ||
		m[5] != _m.m[5] ||
		m[6] != _m.m[6] ||
		m[7] != _m.m[7] ||
		m[8] != _m.m[8] ||
		m[9] != _m.m[9] ||
		m[10] != _m.m[10] ||
		m[11] != _m.m[11] ||
		m[12] != _m.m[12] ||
		m[13] != _m.m[13] ||
		m[14] != _m.m[14] ||
		m[15] != _m.m[15]
	);
}

//----------------------------------------------------------------------------------------------
float Matrix::operator[](int i) const
{
	float out = *(m+i);
	return out;
}

//----------------------------------------------------------------------------------------------
float& Matrix::operator[](int i) 
{
	return *(m+i);
}

//----------------------------------------------------------------------------------------------
void Matrix::set_rot(const Quaternion & q)
{
	Matrix3f m;
	q.ToMatrix(m);
	set_rot(m);
}

//----------------------------------------------------------------------------------------------
void Matrix::set_rot(float yaw, float pitch, float roll)
{
	// Euler Transform
	float cosY = (float)cos(yaw);
	float cosP = (float)cos(pitch);
	float cosR = (float)cos(roll);
	float sinY = (float)sin(yaw);
	float sinP = (float)sin(pitch);
	float sinR = (float)sin(roll);

	a00 = cosR * cosY - sinR * sinP * sinY;
	a01 = sinR * cosY + cosR * sinP * sinY;
	a02 = -cosP * sinY;
	a03 = 0.0f;

	a10 = -sinR * cosP;
	a11 = cosR * cosP;
	a12 = sinP;
	a13 = 0.0f;

	a20 = cosR * sinY + sinR * sinP * cosY;
	a21 = sinR * sinY - cosR * sinP * cosY;
	a22 = cosP * cosY;
	a23 = 0.0f;

	a30 = 0.0f;
	a31 = 0.0f;
	a32 = 0.0f;
	a33 = 1.0f;
}

//----------------------------------------------------------------------------------------------
void Matrix::set_rot(const Matrix3f &M)
{
	// copy the 3x3 rotation block
	a00 = M.a00; a10 = M.a10; a20 = M.a20;
	a01 = M.a01; a11 = M.a11; a21 = M.a21;
	a02 = M.a02; a12 = M.a12; a22 = M.a22;
}
//----------------------------------------------------------------------------------------------
void Matrix::set_translation(const Vector& v)
{
    x = v.x;
	y = v.y;
	z = v.z;
}



//----------------------------------------------------------------------------------------------
void Matrix::Identitly()
{
	a00=1.f; a10=0.f; a20=0.f; a30=0.f;
	a01=0.f; a11=1.f; a21=0.f; a31=0.f;
	a02=0.f; a12=0.f; a22=1.f; a32=0.f;
	a03=0.f; a13=0.f; a23=0.f; a33=1.f;
}

//----------------------------------------------------------------------------------------------
void Matrix::Transpose()
{
	//float a00, a10, a20, a30;   // standard names for components
	//float a01, a11, a21, a31;   // standard names for components
	//float a02, a12, a22, a32;   // standard names for components
	//float a03, a13, a23, a33;   // standard names for components

	std::swap(a01, a10);
	std::swap(a02, a20);
	std::swap(a12, a21);
	std::swap(a03, a30);
	std::swap(a13, a31);
	std::swap(a23, a32);
}

//----------------------------------------------------------------------------------------------
Matrix GetMatrixIdentitly(const Vector& _translation)
{
	return Matrix(1.f, 0.f, 0.f, 0.f,
		          0.f, 1.f, 0.f, 0.f,
		          0.f, 0.f, 1.f, 0.f,
		          0.f, 0.f, 0.f, 1.f );
}

//----------------------------------------------------------------------------------------------
Matrix GetMatrixScale(const Vector& _scale )
{
	return Matrix(_scale.x, 0.f, 0.f, 0.f,
		           0.f, _scale.y, 0.f, 0.f,
		           0.f, 0.f, _scale.z, 0.f,
		           0.f, 0.f, 0.f, 1.f );
}

//----------------------------------------------------------------------------------------------
Matrix GetMatrixTranslation(const Vector& _translation)
{
	return Matrix(1.f, 0.f, 0.f, 0.f,
		          0.f, 1.f, 0.f, 0.f,
		          0.f, 0.f, 1.f, 0.f,
		          _translation.x, _translation.y, _translation.z, 1.f );
}

//----------------------------------------------------------------------------------------------
// C = A * B

// C.a00 C.a01 C.a02 C.a03   A.a00 A.a01 A.a02 A.a03   B.a00 B.a01 B.a02 B.a03
//                                                                            
// C.a10 C.a11 C.a12 C.a13   A.a10 A.a11 A.a12 A.a13   B.a10 B.a11 B.a12 B.a13
//                                                                         
// C.a20 C.a21 C.a22 C.a23   A.a20 A.a21 A.a22 A.a23   B.a20 B.a21 B.a22 B.a23  
//                                                                            
// C.a30 C.a31 C.a32 C.a33 = A.a30 A.a31 A.a32 A.a33 * B.a30 B.a31 B.a32 B.a33

Matrix & mult(Matrix& C, const Matrix& A, const Matrix& B)
{
    // If there is selfassignment involved
    // we can't go without a temporary.
    if (&C == &A || &C == &B)
    {
        Matrix mTemp;

        mTemp.a00 = A.a00 * B.a00 + A.a10 * B.a01 + A.a20 * B.a02 + A.a30 * B.a03;
        mTemp.a10 = A.a00 * B.a10 + A.a10 * B.a11 + A.a20 * B.a12 + A.a30 * B.a13;
        mTemp.a20 = A.a00 * B.a20 + A.a10 * B.a21 + A.a20 * B.a22 + A.a30 * B.a23;
        mTemp.a30 = A.a00 * B.a30 + A.a10 * B.a31 + A.a20 * B.a32 + A.a30 * B.a33;

        mTemp.a01 = A.a01 * B.a00 + A.a11 * B.a01 + A.a21 * B.a02 + A.a31 * B.a03;
        mTemp.a11 = A.a01 * B.a10 + A.a11 * B.a11 + A.a21 * B.a12 + A.a31 * B.a13;
        mTemp.a21 = A.a01 * B.a20 + A.a11 * B.a21 + A.a21 * B.a22 + A.a31 * B.a23;
        mTemp.a31 = A.a01 * B.a30 + A.a11 * B.a31 + A.a21 * B.a32 + A.a31 * B.a33;

        mTemp.a02 = A.a02 * B.a00 + A.a12 * B.a01 + A.a22 * B.a02 + A.a32 * B.a03;
        mTemp.a12 = A.a02 * B.a10 + A.a12 * B.a11 + A.a22 * B.a12 + A.a32 * B.a13;
        mTemp.a22 = A.a02 * B.a20 + A.a12 * B.a21 + A.a22 * B.a22 + A.a32 * B.a23;
        mTemp.a32 = A.a02 * B.a30 + A.a12 * B.a31 + A.a22 * B.a32 + A.a32 * B.a33;

        mTemp.a03 = A.a03 * B.a00 + A.a13 * B.a01 + A.a23 * B.a02 + A.a33 * B.a03;
        mTemp.a13 = A.a03 * B.a10 + A.a13 * B.a11 + A.a23 * B.a12 + A.a33 * B.a13;
        mTemp.a23 = A.a03 * B.a20 + A.a13 * B.a21 + A.a23 * B.a22 + A.a33 * B.a23;
        mTemp.a33 = A.a03 * B.a30 + A.a13 * B.a31 + A.a23 * B.a32 + A.a33 * B.a33;

        C = mTemp;
    }
    else
    {
		C.a00 = A.a00 * B.a00 + A.a10 * B.a01 + A.a20 * B.a02 + A.a30 * B.a03;
		C.a10 = A.a00 * B.a10 + A.a10 * B.a11 + A.a20 * B.a12 + A.a30 * B.a13;
		C.a20 = A.a00 * B.a20 + A.a10 * B.a21 + A.a20 * B.a22 + A.a30 * B.a23;
		C.a30 = A.a00 * B.a30 + A.a10 * B.a31 + A.a20 * B.a32 + A.a30 * B.a33;

		C.a01 = A.a01 * B.a00 + A.a11 * B.a01 + A.a21 * B.a02 + A.a31 * B.a03;
		C.a11 = A.a01 * B.a10 + A.a11 * B.a11 + A.a21 * B.a12 + A.a31 * B.a13;
		C.a21 = A.a01 * B.a20 + A.a11 * B.a21 + A.a21 * B.a22 + A.a31 * B.a23;
		C.a31 = A.a01 * B.a30 + A.a11 * B.a31 + A.a21 * B.a32 + A.a31 * B.a33;

		C.a02 = A.a02 * B.a00 + A.a12 * B.a01 + A.a22 * B.a02 + A.a32 * B.a03;
		C.a12 = A.a02 * B.a10 + A.a12 * B.a11 + A.a22 * B.a12 + A.a32 * B.a13;
		C.a22 = A.a02 * B.a20 + A.a12 * B.a21 + A.a22 * B.a22 + A.a32 * B.a23;
		C.a32 = A.a02 * B.a30 + A.a12 * B.a31 + A.a22 * B.a32 + A.a32 * B.a33;

		C.a03 = A.a03 * B.a00 + A.a13 * B.a01 + A.a23 * B.a02 + A.a33 * B.a03;
		C.a13 = A.a03 * B.a10 + A.a13 * B.a11 + A.a23 * B.a12 + A.a33 * B.a13;
		C.a23 = A.a03 * B.a20 + A.a13 * B.a21 + A.a23 * B.a22 + A.a33 * B.a23;
		C.a33 = A.a03 * B.a30 + A.a13 * B.a31 + A.a23 * B.a32 + A.a33 * B.a33;
    }

    return C;
}

//----------------------------------------------------------------------------------------------
Matrix & mult(Matrix& C, const Matrix& A, const Matrix3f& B)
{
	// If there is selfassignment involved
	// we can't go without a temporary.
	if (&C == &A)
	{
		Matrix mTemp = A;

		mTemp.a00 = A.a00 * B.a00 + A.a10 * B.a01 + A.a20 * B.a02;
		mTemp.a10 = A.a00 * B.a10 + A.a10 * B.a11 + A.a20 * B.a12;
		mTemp.a20 = A.a00 * B.a20 + A.a10 * B.a21 + A.a20 * B.a22;

		mTemp.a01 = A.a01 * B.a00 + A.a11 * B.a01 + A.a21 * B.a02;
		mTemp.a11 = A.a01 * B.a10 + A.a11 * B.a11 + A.a21 * B.a12;
		mTemp.a21 = A.a01 * B.a20 + A.a11 * B.a21 + A.a21 * B.a22;

		mTemp.a02 = A.a02 * B.a00 + A.a12 * B.a01 + A.a22 * B.a02;
		mTemp.a12 = A.a02 * B.a10 + A.a12 * B.a11 + A.a22 * B.a12;
		mTemp.a22 = A.a02 * B.a20 + A.a12 * B.a21 + A.a22 * B.a22;

		C = mTemp;
	}
	else
	{
		C = A;

		C.a00 = A.a00 * B.a00 + A.a10 * B.a01 + A.a20 * B.a02;
		C.a10 = A.a00 * B.a10 + A.a10 * B.a11 + A.a20 * B.a12;
		C.a20 = A.a00 * B.a20 + A.a10 * B.a21 + A.a20 * B.a22;

		C.a01 = A.a01 * B.a00 + A.a11 * B.a01 + A.a21 * B.a02;
		C.a11 = A.a01 * B.a10 + A.a11 * B.a11 + A.a21 * B.a12;
		C.a21 = A.a01 * B.a20 + A.a11 * B.a21 + A.a21 * B.a22;

		C.a02 = A.a02 * B.a00 + A.a12 * B.a01 + A.a22 * B.a02;
		C.a12 = A.a02 * B.a10 + A.a12 * B.a11 + A.a22 * B.a12;
		C.a22 = A.a02 * B.a20 + A.a12 * B.a21 + A.a22 * B.a22;
	}

	return C;
}
//----------------------------------------------------------------------------------------------
Matrix& Matrix::invert(Matrix & B, const Matrix & A)
{
// 	B = A.inverse();
// 	return B;

    float det, oodet;

	//if (&B == &A)

	B.a00 =  (A.a11 * A.a22 - A.a21 * A.a12);
	B.a10 = -(A.a10 * A.a22 - A.a20 * A.a12);
	B.a20 =  (A.a10 * A.a21 - A.a20 * A.a11);
	B.a01 = -(A.a01 * A.a22 - A.a21 * A.a02);
	B.a11 =  (A.a00 * A.a22 - A.a20 * A.a02);
	B.a21 = -(A.a00 * A.a21 - A.a20 * A.a01);
	B.a02 =  (A.a01 * A.a12 - A.a11 * A.a02);
	B.a12 = -(A.a00 * A.a12 - A.a10 * A.a02);
	B.a22 =  (A.a00 * A.a11 - A.a10 * A.a01);

	det = (A.a00 * B.a00) + (A.a01 * B.a10) + (A.a02 * B.a20);

	oodet = 1.f / det;

	B.a00 *= oodet; B.a01 *= oodet; B.a02 *= oodet;
	B.a10 *= oodet; B.a11 *= oodet; B.a12 *= oodet;
	B.a20 *= oodet; B.a21 *= oodet; B.a22 *= oodet;

    return B;
}

//----------------------------------------------------------------------------------------------
Matrix Matrix::inverse() const
{
    Matrix inv = getInverse3x3();
    inv.t = transformTranspose(-t);
    return inv;
	/*Matrix Result;
	float det = Determinant();

	if (det == 0.f){
		return Result;
	}
	
	memcpy(Result.m, m, sizeof(float) * 16);

	float oodet = 1.f / det;

	Result._11 *= oodet; Result._12 *= oodet; Result._13 *= oodet; Result._14 *= oodet;
	Result._21 *= oodet; Result._22 *= oodet; Result._23 *= oodet; Result._24 *= oodet;
	Result._31 *= oodet; Result._32 *= oodet; Result._33 *= oodet; Result._34 *= oodet;
	Result._41 *= oodet; Result._42 *= oodet; Result._43 *= oodet; Result._44 *= oodet;

	return Result;*/
}


//----------------------------------------------------------------------------------------------
void Matrix::SetRotateAxis(const Vector& v, const float rad)
{
	float s, c, t;
	float tx, ty, tz;
	float sx, sy, sz;
		
	s = (float)sin(rad);
	c = (float)cos(rad);
	t = 1.0f - c;

	tx = t * v.x;	ty = t * v.y;	tz = t * v.z;
	sx = s * v.x;	sy = s * v.y;	sz = s * v.z;

	a00 = tx*v.x + c;
	a10 = tx*v.y - sz;
	a20 = tx*v.z + sy;
	a30 = 0.f;

	a01 = tx*v.y + sz;
	a11 = ty*v.y + c; 
	a21 = ty*v.z - sx;
	a31 = 0.f;

	a02 = tx*v.z - sy;
	a12 = ty*v.z + sx;
	a22 = tz*v.z + c;
	a32 = 0.f;

	a03 = 0.f; a13 = 0.f; a23 = 0.f; a33 = 1.f;
}

//----------------------------------------------------------------------------------------------
float Matrix::Determinant() const
{
	return	_11 * det3x3(_22, _23, _24, 
						  _32, _33, _34, 
						  _42, _43, _44) -

			_12 * det3x3(_21, _23, _24, 
						  _31, _33, _34, 
						  _41, _43, _44) -

			_13 * det3x3(_21, _22, _24, 
						  _31, _32, _34, 
						  _41, _42, _44) +

			_14 * det3x3(_21, _22, _23, 
						  _31, _32, _33,
						  _41, _42, _43);
}

//----------------------------------------------------------------------------------------------
float Matrix::Determinant3x3() const
{
    return _row0.Dot(_row1.Cross(_row2));
}

//----------------------------------------------------------------------------------------------
Matrix Matrix::getInverse3x3() const
{
    const float det = Determinant3x3();
    Matrix inverse;

    if (det != 0)
    {
        const float invDet = 1.0f / det;

        inverse.row0.x = invDet * (row1.y * row2.z - row2.y * row1.z);
        inverse.row0.y = invDet * -(row0.y * row2.z - row2.y * row0.z);
        inverse.row0.z = invDet * (row0.y * row1.z - row0.z * row1.y);

        inverse.row1.x = invDet * -(row1.x * row2.z - row1.z * row2.x);
        inverse.row1.y = invDet * (row0.x * row2.z - row0.z * row2.x);
        inverse.row1.z = invDet * -(row0.x * row1.z - row0.z * row1.x);

        inverse.row2.x = invDet * (row1.x * row2.y - row1.y * row2.x);
        inverse.row2.y = invDet * -(row0.x * row2.y - row0.y * row2.x);
        inverse.row2.z = invDet * (row0.x * row1.y - row1.x * row0.y);

        return inverse;
    }
    else
    {
        return Matrix();
    }
}

//----------------------------------------------------------------------------------------------
bool Matrix::equal(const Matrix &other) const
{
    return
        fequal(_11, other._11) &&
        fequal(_12, other._12) &&
        fequal(_13, other._13) &&
        fequal(_14, other._14) &&
        fequal(_21, other._21) &&
        fequal(_22, other._22) &&
        fequal(_23, other._23) &&
        fequal(_24, other._24) &&
        fequal(_31, other._31) &&
        fequal(_32, other._32) &&
        fequal(_33, other._33) &&
        fequal(_34, other._34) &&
        fequal(_41, other._41) &&
        fequal(_42, other._42) &&
        fequal(_43, other._43) &&
        fequal(_44, other._44);
}

//----------------------------------------------------------------------------------------------
Vector Matrix::transformTranspose(const Vector &other) const
{
    return Vector(_row0.Dot(other), _row1.Dot(other), _row2.Dot(other));
}
}
}