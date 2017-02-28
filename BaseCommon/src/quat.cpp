#include "quat.h"
#include "Matrix.h"
#include <assert.h>

namespace oes
{
namespace common_base
{

//------------------------------------------------------------------------
Quaternion::Quaternion()
{
   Set(0.f, 0.f, 0.f, 1.f);
}

//------------------------------------------------------------------------
Quaternion::Quaternion(TFlt32 _x, TFlt32 _y, TFlt32 _z, TFlt32 _w)
{
   Set(_x, _y, _z, _w);
}

//------------------------------------------------------------------------
TFlt32 Quaternion::operator[](int i)const
{
	assert((i>=0)&&(i<=4));
	return ((TFlt32*)this)[i];
}

//------------------------------------------------------------------------
TFlt32 Quaternion::operator[](int i)
{
	assert((i>=0)&&(i<=4));
	return ((TFlt32*)this)[i];
}

//------------------------------------------------------------------------
Quaternion& Quaternion::operator*=(const Quaternion &q)
{
	*this = *this * q;
	return *this;
}

//------------------------------------------------------------------------
void Quaternion::FromMatrix(const Matrix3f &mat)
{
    TFlt32 trace = mat(0, 0) + mat(1, 1) + mat(2, 2);
	if (trace > 0.f) 
	{
        TFlt32 scale = sqrtf(trace + 1.f);
		w = 0.5f * scale;
		scale = 0.5f / scale;
		x = scale * (mat(2, 1) - mat(1, 2));
		y = scale * (mat(0, 2) - mat(2, 0));
		z = scale * (mat(1, 0) - mat(0, 1));
	}
	else 
	{
		static int next[] = { 1, 2, 0 };
		int i = 0;
		if (mat(1, 1) > mat(0, 0))
			i = 1;
		if (mat(2, 2) > mat(i, i))
			i = 2;
		int j = next[i];
		int k = next[j];
        TFlt32 scale = sqrtf(mat(i, i) - mat(j, j) - mat(k, k) + 1.f);
        TFlt32* q[] = { &x, &y, &z };
		*q[i] = 0.5f * scale;
		scale = 0.5f / scale;
		w = scale * (mat(k, j) - mat(j, k));
		*q[j] = scale * (mat(j, i) + mat(i, j));
		*q[k] = scale * (mat(k, i) + mat(i, k));
	}
}

//------------------------------------------------------------------------
void Quaternion::ToMatrix(Matrix3f &mat) const
{
    TFlt32 x2 = x * 2;
    TFlt32 y2 = y * 2;
    TFlt32 z2 = z * 2;
    TFlt32 wx = x2 * w;
    TFlt32 wy = y2 * w;
    TFlt32 wz = z2 * w;
    TFlt32 xx = x2 * x;
    TFlt32 xy = y2 * x;
    TFlt32 xz = z2 * x;
    TFlt32 yy = y2 * y;
    TFlt32 yz = z2 * y;
    TFlt32 zz = z2 * z;

	mat.a00 = 1.f - (yy + zz);
	mat.a01 = xy - wz;
	mat.a02 = xz + wy;
	mat.a10 = xy + wz;
	mat.a11 = 1.f - (xx + zz);
	mat.a12 = yz - wx;
	mat.a20 = xz - wy;
	mat.a21 = yz + wx;
	mat.a22 = 1.f - (xx + yy);
}

//------------------------------------------------------------------------
void Quaternion::ToMatrix(Matrix* mat) const
{
	 // Converts this quaternion to a rotation matrix.
    //
    //  | 1 - 2(y^2 + z^2)	2(xy + wz)			2(xz - wy)			0  |
    //  | 2(xy - wz)		1 - 2(x^2 + z^2)	2(yz + wx)			0  |
    //  | 2(xz + wy)		2(yz - wx)			1 - 2(x^2 + y^2)	0  |
    //  | 0					0					0					1  |
	/*
	float x2 = x * 2;
	float y2 = y * 2;
	float z2 = z * 2;
	float wx = x2 * w;
	float wy = y2 * w;
	float wz = z2 * w;
	float xx = x2 * x;
	float xy = y2 * x;
	float xz = z2 * x;
	float yy = y2 * y;
	float yz = z2 * y;
	float zz = z2 * z;

	mat->a00 = 1.f - (yy + zz);
	mat->a01 = xy - wz;
	mat->a02 = xz + wy;
	mat->a03 = 0.f;

	mat->a10 = xy + wz;
	mat->a11 = 1.f - (xx + zz);
	mat->a12 = yz - wx;
	mat->a13 = 0.f;

	mat->a20 = xz - wy;
	mat->a21 = yz + wx;
	mat->a22 = 1.f - (xx + yy);
	mat->a23 = 0.f;

	mat->a03 = mat->a13 = mat->a23 = 0.f;
	mat->a33 = 1.f;*/

	    // Converts this quaternion to a rotation matrix.
    //
    //  | 1 - 2(y^2 + z^2)	2(xy + wz)			2(xz - wy)			0  |
    //  | 2(xy - wz)		1 - 2(x^2 + z^2)	2(yz + wx)			0  |
    //  | 2(xz + wy)		2(yz - wx)			1 - 2(x^2 + y^2)	0  |
    //  | 0					0					0					1  |

    TFlt32 x2 = x + x;
    TFlt32 y2 = y + y;
    TFlt32 z2 = z + z;
    TFlt32 xx = x * x2;
    TFlt32 xy = x * y2;
    TFlt32 xz = x * z2;
    TFlt32 yy = y * y2;
    TFlt32 yz = y * z2;
    TFlt32 zz = z * z2;
    TFlt32 wx = w * x2;
    TFlt32 wy = w * y2;
    TFlt32 wz = w * z2;

	mat->a00 = 1.0f - (yy + zz);
    mat->a01 = xy + wz;
    mat->a02 = xz - wy;
    mat->a03 = 0.0f;

    mat->a10 = xy - wz;
    mat->a11 = 1.0f - (xx + zz);
    mat->a12 = yz + wx;
    mat->a13 = 0.0f;

    mat->a20 = xz + wy;
    mat->a21 = yz - wx;
    mat->a22 = 1.0f - (xx + yy);
    mat->a23 = 0.0f;

    mat->a30 = 0.0f;
    mat->a31 = 0.0f;
    mat->a32 = 0.0f;
    mat->a33 = 1.0f;
}

//------------------------------------------------------------------------
void Quaternion::set_rot(TFlt32 yaw, TFlt32 pitch, TFlt32 roll)
{
    TFlt32 sinY, cosY, sinP, cosP, sinR, cosR;

	sinY = sinf(0.5f * yaw);
	cosY = cosf(0.5f * yaw);

	sinP = sinf(0.5f * pitch);
	cosP = cosf(0.5f * pitch);

	sinR = sinf(0.5f * roll);
	cosR = cosf(0.5f * roll);

	x = cosR * sinP * cosY + sinR * cosP * sinY;
	y = cosR * cosP * sinY - sinR * sinP * cosY;
	z = sinR * cosP * cosY - cosR * sinP * sinY;
	w = cosR * cosP * cosY + sinR * sinP * sinY;
}

//------------------------------------------------------------------------
const Quaternion operator*(const Quaternion& p, const Quaternion& q)
{
    TFlt32 A, B, C, D, E, F, G, H;
 
 	A = (p.w + p.x) * (q.w + q.x);
 	B = (p.z - p.y) * (q.y - q.z);
 	C = (p.x - p.w) * (q.y + q.z);
 	D = (p.y + p.z) * (q.x - q.w);
 	E = (p.x + p.z) * (q.x + q.y);
 	F = (p.x - p.z) * (q.x - q.y);
 	G = (p.w + p.y) * (q.w - q.z);
 	H = (p.w - p.y) * (q.w + q.z);
 
 	return Quaternion(
 		A - ( E + F + G + H) * 0.5f,
 		-C + ( E - F + G - H) * 0.5f,
 		-D + ( E - F - G + H) * 0.5f,
 		B + (-E - F + G + H) * 0.5f
 		);
}

//------------------------------------------------------------------------
Quaternion Quaternion::Inverse()
{
	return Quaternion(-x, -y, -z, w);
}

//------------------------------------------------------------------------
void Quaternion::Normalize()
{
    TFlt32  len = sqrtf(x * x + y * y + z * z + w * w);

	if (len > 0) 
	{
        TFlt32 invLen = 1.f / len;

		x *= invLen;
		y *= invLen;
		z *= invLen;
		w *= invLen;
	}
}

#ifndef acosf
#define acosf acos
#endif

//------------------------------------------------------------------------
float dot(const Quaternion &q1, const Quaternion &q2)
{
	return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}

//------------------------------------------------------------------------
Quaternion & Slerp(Quaternion &p, TFlt32 s, const Quaternion &q1, const Quaternion &q2)
{
    TFlt32 cosine = dot(q1, q2);

	if (cosine < -1)
		cosine = -1;

	else if (cosine > 1)
		cosine = 1;

    TFlt32 angle = (TFlt32)acosf(cosine);
	if (fabs(angle) < m_eps) {
		p = q1;
		return p;
	}
    TFlt32 sine = sinf(angle);
    TFlt32 sineInv = 1.0f / sine;
    TFlt32 c1 = sinf((1.0f - s) * angle) * sineInv;
    TFlt32 c2 = sinf(s * angle) * sineInv;
	p.x = c1 * q1.x + c2 * q2.x;
	p.y = c1 * q1.y + c2 * q2.y;
	p.z = c1 * q1.z + c2 * q2.z;
	p.w = c1 * q1.w + c2 * q2.w;
	return p;
}

}
}