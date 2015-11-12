#ifndef __m_collision_h__
#define __m_collision_h__

#ifdef _WIN32
#pragma once
#endif

#include "m_stdafx.h"

struct m_collision
{
	enum collision_type
	{
		BOX			= 0,
		SPHERE		= 1,
		CAPSULE		= 2,
		CONVEX		= 3,
		TRMESH		= 4,
	};

	collision_type ctype;
};

#endif//__m_collision_h__