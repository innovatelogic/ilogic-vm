#include "TypesBase.h"
#include "vector2f.h"

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
Vector2f::Vector2f(float _x, float _y)
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
void Vector2f::Set(float _x, float _y)
{
	x = _x;
	y = _y;
}

//------------------------------------------------------------------------
float Vector2f::Normalize()
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

float sq_length(const Vector2f & n)
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
float Vector2f::Length() const
{
    return ::sqrtf(x * x + y * y);
}
//----------------------------------------------------------------------------------
float Vector2f::LengthSqr() const
{
    return x * x + y * y;
}
//----------------------------------------------------------------------------------
float Vector2f::Dot(const Vector2f& _vector_other)const
{
    return DotProduct(*this, _vector_other);
}
}
}