#include "quat.h"
#include "Matrix.h"

namespace oes
{
namespace common_base
{

//------------------------------------------------------------------------
Quaternion::Quaternion()
{
   Set(0.f,0.f,0.f,1.f);
}
//------------------------------------------------------------------------
Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
   Set(_x, _y, _z, _w);
}

//------------------------------------------------------------------------
float Quaternion::operator[](int i)const
{
	assert((i>=0)&&(i<=4));
	return ((float*)this)[i];
}

//------------------------------------------------------------------------
float Quaternion::operator[](int i) 
{
	assert((i>=0)&&(i<=4));
	return ((float*)this)[i];
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
	float trace = mat(0, 0) + mat(1, 1) + mat(2, 2);
	if (trace > 0.f) 
	{
		float scale = sqrtf(trace + 1.f);
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
		float scale = sqrtf(mat(i, i) - mat(j, j) - mat(k, k) + 1.f);
		float* q[] = { &x, &y, &z };
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
void Quaternion::ToMatrix(class Matrix* mat) const
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

    float x2 = x + x; 
    float y2 = y + y; 
    float z2 = z + z;
    float xx = x * x2;
    float xy = x * y2;
    float xz = x * z2;
    float yy = y * y2;
    float yz = y * z2;
    float zz = z * z2;
    float wx = w * x2;
    float wy = w * y2;
    float wz = w * z2;

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
void Quaternion::set_rot(float yaw, float pitch, float roll)
{
	float sinY, cosY, sinP, cosP, sinR, cosR;

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
 	float A, B, C, D, E, F, G, H;
 
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
	float  len = sqrtf(x * x + y * y + z * z + w * w);

	if (len > 0) 
	{
		float invLen = 1.f / len;

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
float dot(const Quaternion& q1, const Quaternion& q2)
{
	return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}
//------------------------------------------------------------------------
Quaternion & Slerp(Quaternion & p, float s, const Quaternion & q1, const Quaternion & q2)
{
	float cosine = dot(q1, q2);

	if (cosine < -1)
		cosine = -1;

	else if (cosine > 1)
		cosine = 1;

	float angle = (float)acosf(cosine);
	if (fabs(angle) < m_eps) {
		p = q1;
		return p;
	}
	float sine = sinf(angle);
	float sineInv = 1.0f / sine;
	float c1 = sinf((1.0f - s) * angle) * sineInv;
	float c2 = sinf(s * angle) * sineInv;
	p.x = c1 * q1.x + c2 * q2.x;
	p.y = c1 * q1.y + c2 * q2.y;
	p.z = c1 * q1.z + c2 * q2.z;
	p.w = c1 * q1.w + c2 * q2.w;
	return p;
}

}
}