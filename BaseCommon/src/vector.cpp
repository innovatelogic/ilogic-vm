#include "vector.h"
#include "mathlib.h"

namespace oes
{
namespace common_base
{
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
//----------------------------------------------------------------------------------------------
TFlt32 Vector::operator[](int i) const
{
    assert((i >= 0) && (i < 3));
    return ((TFlt32*)this)[i];
}

//----------------------------------------------------------------------------------------------
TFlt32& Vector::operator[](int i)
{
    assert((i >= 0) && (i < 3));
    return ((TFlt32*)this)[i];
}

//----------------------------------------------------------------------------------------------
Vector Vector::operator*(TFlt32 f)const
{
    Vector out;

    out.x = x * f;
    out.y = y * f;
    out.z = z * f;

    return out;
}

//----------------------------------------------------------------------------------------------
Vector& Vector::operator=(const Vector &vOther)
{
    x = vOther.x;
    y = vOther.y;
    z = vOther.z;

    return *this;
}
//----------------------------------------------------------------------------------------------
Vector& Vector::operator+=(const Vector& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

//----------------------------------------------------------------------------------------------
Vector& Vector::operator-=(const Vector& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}
//----------------------------------------------------------------------------------------------
Vector& Vector::operator*=(const Vector& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;

    return *this;
}

//----------------------------------------------------------------------------------------------
Vector Vector::operator*(TFlt32 f)
{
    Vector out;

    out.x = x * f;
    out.y = y * f;
    out.z = z * f;

    return out;
}

//----------------------------------------------------------------------------------------------
Vector Vector::operator/(TFlt32 f)
{
    assert(f != 0.f);

    Vector out;

    out.x = x / f;
    out.y = y / f;
    out.z = z / f;

    return out;
}

//----------------------------------------------------------------------------------------------
Vector& Vector::operator*=(const TFlt32 f)
{
    x *= f;
    y *= f;
    z *= f;

    return *this;
}
//----------------------------------------------------------------------------------------------
Vector& Vector::operator/=(const Vector &v)
{
    assert(v.x != 0.f);
    assert(v.y != 0.f);
    assert(v.z != 0.f);

    x /= v.x;
    y /= v.y;
    z /= v.z;

    return *this;
}
//----------------------------------------------------------------------------------------------
Vector& Vector::operator/=(const TFlt32 f)
{
    assert(f != 0.f);

    x *= f;
    y *= f;
    z *= f;

    return *this;
}

//----------------------------------------------------------------------------------------------
Vector Vector::operator+(const Vector &v)const
{
    Vector out;

    out.x = x + v.x;
    out.y = y + v.y;
    out.z = z + v.z;

    return out;
}
//----------------------------------------------------------------------------------------------
Vector Vector::operator-(const Vector &v)const
{
    Vector out;

    out.x = x - v.x;
    out.y = y - v.y;
    out.z = z - v.z;

    return out;
}

//----------------------------------------------------------------------------------------------
bool Vector::operator == (const Vector &v) const
{
    return (x == v.x) && (y == v.y) && (z == v.z);
}

//----------------------------------------------------------------------------------------------
bool Vector::operator !=(const Vector &v) const
{
    return (x != v.x) || (y != v.y) || (z != v.z);
}

//----------------------------------------------------------------------------------------------
Vector  Vector::operator-(void) const
{
    return Vector(-x, -y, -z);
}

//----------------------------------------------------------------------------------------------
TFlt32 Vector::Length()
{
    return ::sqrtf(x * x + y * y + z * z);
}

//----------------------------------------------------------------------------------------------
TFlt32 Vector::LengthSqr()
{
    return x * x + y * y + z * z;
}

//----------------------------------------------------------------------------------------------
TFlt32 Vector::Dot(const Vector &_vector_other) const
{
    return DotProduct(*this, _vector_other);
}

//----------------------------------------------------------------------------------------------
Vector Vector::Cross(const Vector &_vector_other) const
{
    Vector out;
    return CrossProduct(*this, _vector_other, out);
}

//----------------------------------------------------------------------------------------------
bool Vector::equal(const Vector &other) const
{
    return
        fequal(x, other.x) &&
        fequal(y, other.y) &&
        fequal(z, other.z);
}

//----------------------------------------------------------------------------------------------
TFlt32 sq_length(const Vector & n)
{
    return n.x * n.x + n.y * n.y + n.z * n.z;
}

//----------------------------------------------------------------------------------------------
// Computes the magnitude
TFlt32 length(const Vector & n)
{
    return sqrtf(sq_length(n));
}

//----------------------------------------------------------------------------------------------
TFlt32 DotProduct(const Vector &v1, const Vector &v2)
{
    return  v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//----------------------------------------------------------------------------------------------
Vector CrossProduct(const Vector &a, const Vector &b, Vector &_result)
{
    _result.x = a.y * b.z - a.z * b.y;
    _result.y = a.z * b.x - a.x * b.z;
    _result.z = a.x * b.y - a.y * b.x;
    return _result;
}

//----------------------------------------------------------------------------------------------
void VectorAbs(const Vector &vIn, const Vector &v2, Vector &vOut)
{
    vOut.x = static_cast<float>(fabs(vIn.x));
    vOut.y = static_cast<float>(fabs(vIn.y));
    vOut.z = static_cast<float>(fabs(vIn.z));
}
}
}