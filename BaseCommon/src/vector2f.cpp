
#include "vector2f.h"
#include <math.h>

namespace oes
{
namespace common_base
{

//------------------------------------------------------------------------
Vector2f::Vector2f()
: x(0.f)
, y(0.f)
{
}

//------------------------------------------------------------------------
Vector2f::Vector2f(TFlt32 _x, TFlt32 _y)
: x(_x)
, y(_y)
{
}

//------------------------------------------------------------------------
Vector2f::Vector2f(const Vector2f& Source)
{
	if (this != &Source)
	{
		x = Source.x;
		y = Source.y;
	}
}

//------------------------------------------------------------------------
void Vector2f::Set(TFlt32 _x, TFlt32 _y)
{
	x = _x;
	y = _y;
}

//------------------------------------------------------------------------
TFlt32 Vector2f::operator[] (int i) const
{
    return vec_array[i];
}

//------------------------------------------------------------------------
TFlt32& Vector2f::operator[] (int i)
{
    return vec_array[i];
}

//------------------------------------------------------------------------
TFlt32 Vector2f::Normalize()
{
	float norm = sqrtf(x * x + y * y);
	if (norm > m_eps)
		norm = m_one / norm;
	else
		norm = m_zero;

	x *= norm;
	y *= norm;

	return norm;
}

TFlt32 sq_length(const Vector2f & n)
{
    return n.x * n.x + n.y * n.y;
}

//--------------------------------------------------------------------------------
/** Computes the magnitude */
float length(const Vector2f & n)
{
    return sqrtf(sq_length(n));
}

//----------------------------------------------------------------------------------
float DotProduct(const Vector2f& v1, const Vector2f& v2)
{
    return  v1.x * v2.x + v1.y * v2.y;
}

//----------------------------------------------------------------------------------
TFlt32 Vector2f::Length() const
{
    return ::sqrtf(x * x + y * y);
}

//----------------------------------------------------------------------------------
TFlt32 Vector2f::LengthSqr() const
{
    return x * x + y * y;
}

//----------------------------------------------------------------------------------
TFlt32 Vector2f::Dot(const Vector2f& _vector_other)const
{
    return DotProduct(*this, _vector_other);
}
}
}