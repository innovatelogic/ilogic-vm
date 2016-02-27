#include "vector.h"

//------------------------------------------------------------------------
Vector::Vector()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

//------------------------------------------------------------------------
Vector::Vector(TFlt32 _x, TFlt32 _y, TFlt32 _z)
: x(_x)
, y(_y)
, z(_z)
{
	
	
}
//------------------------------------------------------------------------
Vector::Vector(const Vector& Source)
{
    if (this != &Source)
	{
        x = Source.x;
        y = Source.y;
		z = Source.z;
	}
}
//------------------------------------------------------------------------
void Vector::Set(TFlt32 _x, TFlt32 _y, TFlt32 _z)
{
	x = _x;
	y = _y;
	z = _z;
}

//------------------------------------------------------------------------
void Vector::Negate()
{
	x = -x;
	y = -y;
	z = -z;
}

//------------------------------------------------------------------------
TFlt32 Vector::normalize()
{
    TFlt32 norm = sqrtf(x * x + y * y + z * z);
	
	if (norm > m_eps){
        norm = m_one / norm;
	}else{
        norm = m_zero;
	}

    x *= norm;
    y *= norm;
    z *= norm;

    return norm;
}

//------------------------------------------------------------------------
Vector& sub(Vector& u, const Vector& v, const Vector& w)
{
	u.x = v.x - w.x;
	u.y = v.y - w.y;
	u.z = v.z - w.z;
	return u;
}

//------------------------------------------------------------------------
Vector& add(Vector & u, const Vector& v, const Vector& w)
{
	u.x = v.x + w.x;
	u.y = v.y + w.y;
	u.z = v.z + w.z;
	return u;
}
//------------------------------------------------------------------------
Vector& cross(Vector & u, const Vector& v, const Vector& w)
{
	u.x =v.y*w.z - v.z*w.y;
	u.y =v.z*w.x - v.x*w.z;
	u.z =v.x*w.y - v.y*w.x;
	return u;
}
//------------------------------------------------------------------------
Vector& scale(Vector & u, const TFlt32 s)
{
	u.x *= s;
	u.y *= s;
	u.z *= s;
	return u;
}

//------------------------------------------------------------------------
Vector& normalize(Vector& u)
{
	TFlt32 norm = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
	
	if (norm > m_eps){
		norm = m_one / norm;
	}else{
		norm = m_zero;
	}

	return scale(u,norm); 
}
