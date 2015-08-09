#ifndef __bounds2f_h__
#define __bounds2f_h__

#include "vector2f.h"

class Bounds2f
{
public:
	Bounds2f()
	{
		SetBounds(Vector2f(0.f, 0.f), Vector2f(0.f, 0.f));
	}

	Bounds2f(float x0, float y0, float x1, float y1)
	{
		SetBounds(Vector2f(x0, y0), Vector2f(x1, y1));
	}

	Bounds2f(const Vector2f& min, const Vector2f& max)
	{
		SetBounds(min, max);
	}

	void SetBounds(const Vector2f& min, const Vector2f& max)
	{
        bound_min = min;
		bound_max = max;
	}

	__forceinline void operator += (const Vector2f& v)
	{
       Add(v);
	}

	__forceinline void Add(const Vector2f& v)
	{
		if (bound_max.x < v.x){
			bound_max.x = v.x;
		}

		if ( v.x < bound_min.x){
			bound_min.x = v.x;
		}

		if (bound_max.y < v.y){
			bound_max.y = v.y;
		}

		if ( v.y < bound_min.y){
			bound_min.y = v.y;
		}
	}

	__forceinline void operator += (const Bounds2f& bound)
	{
		Add(bound.bound_min);
        Add(bound.bound_max);
	}

	Vector2f bound_min;
	Vector2f bound_max;
};

#endif//__bounds2f_h__