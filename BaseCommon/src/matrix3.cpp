#include "matrix3.h"
#include "matrix.h"

//------------------------------------------------------------------------
Matrix3f::Matrix3f()
{
	Set(1.f, 0.f, 0.f, 
		0.f, 1.f, 0.f,  
		0.f, 0.f, 1.f);
}

//------------------------------------------------------------------------
Matrix3f::Matrix3f(float m_00, float m_01, float m_02,
		         float m_10, float m_11, float m_12, 
		         float m_20, float m_21, float m_22)
{
	Set(m_00, m_01, m_02, 
		m_10, m_11, m_12,  
		m_20, m_21, m_22);
}

//------------------------------------------------------------------------
Matrix3f::Matrix3f(const Matrix3f &source)
{
	if (this != &source)
	{
		row0 = source.row0;
		row1 = source.row1;
		row2 = source.row2;
	}
}

//------------------------------------------------------------------------
void Matrix3f::Set(float m_00, float m_01, float m_02, 
		          float m_10, float m_11, float m_12,  
		          float m_20, float m_21, float m_22)
{
	m[0] = m_00;
	m[1] = m_01;
	m[2] = m_02;
	m[3] = m_10;
	m[4] = m_11;
	m[5] = m_12;
	m[6] = m_20;
	m[7] = m_21;
	m[8] = m_22;
}

//----------------------------------------------------------------------------------------------
Matrix3f& Matrix3f::operator= (const Matrix3f &other)
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
	
	return *this;
}

//----------------------------------------------------------------------------------------------
Matrix3f& Matrix3f::operator= (const float *pOther)
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
	
	return *this;
}

//----------------------------------------------------------------------------------------------
Matrix3f Matrix3f::operator*(const Matrix3f &other) const
{
	Matrix3f matrix;
	mult(matrix, *this, other);
	return matrix;
}

//----------------------------------------------------------------------------------------------
Matrix3f Matrix3f::operator* (const float m)const
{
	Matrix3f matrix = *this;
	matrix.m[0] *= m;
	matrix.m[1] *= m;
	matrix.m[2] *= m;
	matrix.m[3] *= m;
	matrix.m[4] *= m;
	matrix.m[5] *= m;
	matrix.m[6] *= m;
	matrix.m[7] *= m;
	matrix.m[8] *= m;
	
	return matrix;
}

//----------------------------------------------------------------------------------------------
bool Matrix3f::operator== (const Matrix3f& _m)const
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
		m[8] == _m.m[8]
	);
}

//----------------------------------------------------------------------------------------------
bool Matrix3f::operator!= (const Matrix3f& _m)const
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
		m[8] != _m.m[8]
	);
}

//----------------------------------------------------------------------------------------------
float Matrix3f::operator[](int i) const
{
	float out = *(m+i);
	return out;
}

//----------------------------------------------------------------------------------------------
float& Matrix3f::operator[](int i) 
{
	return *(m+i);
}
//------------------------------------------------------------------------
void Matrix3f::set_rot(float yaw, float pitch, float roll)
{

// 	Matrix Result, rotatex, rotatey, rotatez;
// 
// 	rotatex.SetRotateAxis(Vector(1.0f,0.0f,0.0f), pitch);
// 	rotatey.SetRotateAxis(Vector(0.0f,1.0f,0.0f), yaw);
// 	rotatez.SetRotateAxis(Vector(0.0f,0.0f,1.0f), roll);
// 
// 	Result = rotatex * rotatey * rotatez;
// 
// 	a00 = Result.a00; a01 = Result.a01; a02 = Result.a02;
// 	a10 = Result.a10; a11 = Result.a11; a12 = Result.a12;
// 	a20 = Result.a20; a21 = Result.a21; a22 = Result.a22;

	// Euler Transform
 	float cosY = (float)cosf(yaw);
 	float cosP = (float)cosf(pitch);
 	float cosR = (float)cosf(roll);
 	float sinY = (float)sinf(yaw);
 	float sinP = (float)sinf(pitch);
 	float sinR = (float)sinf(roll);

 	a00 = cosR * cosY - sinR * sinP * sinY;
 	a01 = sinR * cosY + cosR * sinP * sinY;
 	a02 = -cosP * sinY;
 
 	a10 = -sinR * cosP;
 	a11 = cosR * cosP;
 	a12 = sinP;
 
 	a20 = cosR * sinY + sinR * sinP * cosY;
 	a21 = sinR * sinY - cosR * sinP * cosY;
 	a22 = cosP * cosY;
// 
// 	a00 = cosR * cosP + sinR * sinY * sinP;		a01 = sinR * cosY;	a02 = cosR * -sinP + sinR * sinY * cosP;
// 	a10 = -sinR * cosP + cosR * sinY * sinP;	a11 = cosR * cosY;	a12 = sinR * sinP + cosR * sinY * cosP;
// 	a20 = cosY * sinP;							a21 = -sinY;		a22 = cosY * cosP;
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

Matrix3f & mult(Matrix3f& C, const Matrix3f& A, const Matrix3f& B)
{
	// If there is selfassignment involved
	// we can't go without a temporary.
	if (&C == &A || &C == &B)
	{
		Matrix3f mTemp;

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