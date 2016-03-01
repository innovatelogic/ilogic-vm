#include "vector4f.h"

namespace oes
{
namespace common_base
{
//------------------------------------------------------------------------
Vector4f::Vector4f()
: x(0.f)
, y(0.f)
, z(0.f)
, w(0.f)
{
}

//------------------------------------------------------------------------
Vector4f::Vector4f(float _x, float _y, float _z, float _w)
: x(_x)
, y(_y)
, z(_z)
, w(_w)
{
}

//------------------------------------------------------------------------
Vector4f::Vector4f(const Vector4f& Source)
{
	if (this != &Source)
	{
		x = Source.x;
		y = Source.y;
		z = Source.z;
		w = Source.w;
	}
}
//------------------------------------------------------------------------
void Vector4f::Set(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

//------------------------------------------------------------------------
bool Vector4f::operator!= (const Vector4f& Other) const
{
    return (x != Other.x || y != Other.y || z != Other.z || w != Other.w);
}

//------------------------------------------------------------------------
float Vector4f::operator[](int i)const
{
    assert((i >= 0) && (i < 4));
    return ((float*)this)[i];
}

//------------------------------------------------------------------------
float& Vector4f::operator[](int i)
{
    assert((i >= 0) && (i < 4));
    return ((float*)this)[i];
}
}
}