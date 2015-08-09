#ifndef __bounds3f_h__
#define __bounds3f_h__

#ifdef _WIN32
#pragma once
#endif

#ifndef __lsd_vector_h__
#include "vector.h"
#endif

class Bounds3f
{

public:
	Bounds3f()
	{
		SetBounds(Vector(0.f, 0.f, 0.f), Vector(0.f, 0.f, 0.f));
	}

	Bounds3f(float x0, float y0, float z0,
			 float x1, float y1, float z1)
	{
		SetBounds(Vector(x0, y0, z0), Vector(x1, y1, z1));
	}

	Bounds3f(const Vector& min, const Vector& max)
	{
		SetBounds(min, max);
	}

	void SetBounds(const Vector& min, const Vector& max)
	{
		bound_min = min;
		bound_max = max;
	}

	void operator += (const Vector& v)
	{
		Add(v);
	}

	void Add(const Vector& v)
	{
		if (bound_max.x < v.x){
			bound_max.x = v.x;
		}
		if (v.x < bound_min.x){
			bound_min.x = v.x;
		}
		if (bound_max.y < v.y){
			bound_max.y = v.y;
		}
		if (v.y < bound_min.y){
			bound_min.y = v.y;
		}
		if (bound_max.z < v.z){
			bound_max.z = v.z;
		}
		if (v.z < bound_min.z){
			bound_min.z = v.z;
		}
	}

	void operator += (const Bounds3f& other)
	{
		if (!IsValid())
		{
			SetBounds(other.bound_min, other.bound_max);
			return;
		}

		Add(other.bound_min);
		Add(other.bound_max);
	}

	Bounds3f& operator = (const Bounds3f& other)
	{
		this->bound_min = other.bound_min;
		this->bound_max = other.bound_max;
		return *this;
	}

	bool HitTest(const Vector &Position) const
	{
		bool inX = (Position.x >= bound_min.x && Position.x <= bound_max.x);
		bool inY = (Position.y >= bound_min.y && Position.y <= bound_max.y);
		bool inZ = (Position.z >= bound_min.y && Position.y <= bound_max.y);
		return inX && inY && inZ; 
	}

	bool HitTest(const Vector2f &Position) const
	{
		bool inX = (Position.x >= bound_min.x && Position.x <= bound_max.x);
		bool inY = (Position.y >= bound_min.y && Position.y <= bound_max.y);
		return inX && inY; 
	}

	std::string ToString() const
	{
		return boost::lexical_cast<std::string>(bound_min.x) + ";" +
				boost::lexical_cast<std::string>(bound_min.y) + ";" +
				boost::lexical_cast<std::string>(bound_min.z) + ";" +
				boost::lexical_cast<std::string>(bound_max.x) + ";" + 
				boost::lexical_cast<std::string>(bound_max.y) + ";" + 
				boost::lexical_cast<std::string>(bound_max.z) + ";";
	}

	bool IsValid() const
	{
		if (bound_min.x == MAX_FLOAT){
			return false;
		}
		if (bound_min.y == MAX_FLOAT){
			return false;
		}
		if (bound_min.z == MAX_FLOAT){
			return false;
		}
		if (bound_max.x == -MAX_FLOAT){
			return false;
		}
		if (bound_max.y == -MAX_FLOAT){
			return false;
		}
		if (bound_max.z == -MAX_FLOAT){
			return false;
		}
		return true;
	}

	void SetUnvalid() { SetBounds(Vector(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT), Vector(-MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT)); }

	union  // data
	{
		struct 
		{
			Vector bound_min;
			Vector bound_max;
		};

		float vec_array[6];
	};
};

#endif//__bounds3f_h__