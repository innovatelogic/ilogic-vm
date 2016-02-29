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

}
}