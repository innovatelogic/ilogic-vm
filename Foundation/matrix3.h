//------------------------------------------------------------------------
// Copyright 2006
// Auth: Yur.G
//
//------------------------------------------------------------------------
//
//     class Matrix 
//
//     X(axis).x X(axis).y X(axis).z 0 
//     Y(axis).x Y(axis).y Y(axis).z 0
//     Z(axis).x Z(axis).y Z(axis).z 0
//     T(rans).x T(rans).y T(rans).z 1

// links: http://www.euclideanspace.com/maths/geometry/affine/reflection/index.htm
//        http://www.euclideanspace.com/maths/geometry/elements/plane/lineOnPlane/index.htm

#ifndef __matrix3_h__ 
#define __matrix3_h__

#ifdef _WIN32
   #pragma once
#endif

#include "platform_specific.h"
#include "vector.h"
#include "vector2f.h"

//--------------------------------------------------------------------------------------
class EXPORT Matrix3f
{
	public:
		Matrix3f();
		Matrix3f(float m_00, float m_01, float m_02,
				float m_10, float m_11, float m_12, 
				float m_20, float m_21, float m_22);

		void Set(float m_00, float m_01, float m_02, 
				 float m_10, float m_11, float m_12,  
				 float m_20, float m_21, float m_22);

		void set_rot(float yaw, float pitch, float roll);

		Matrix3f& operator= (const Matrix3f &other);
		Matrix3f& operator= (const float  *pOther);
		Matrix3f  operator* (const Matrix3f &other) const;
		Matrix3f  operator* (const float m) const;

		bool operator== (const Matrix3f &other) const;
		bool operator!= (const Matrix3f &other) const;

		float& operator[] (int i);
		float  operator[] (int i) const;

		const float& operator()(const int &i, const int &j) const
		{
			return m[j * 3 + i];
		}

	public:
		union
		{
			struct
			{
				float a00, a10, a20;   // standard names for components
				float a01, a11, a21;   // standard names for components
				float a02, a12, a22;   // standard names for components
			};

			struct {
				float _11, _12, _13;   // standard names for components
				float _21, _22, _23;   // standard names for components
				float _31, _32, _33;   // standard names for components
			};

			struct {
				Vector row0;
				Vector row1;
				Vector row2;
			};

			struct {
				Vector2f _row0; float w0;
				Vector2f _row1; float w1;
				Vector2f _row2; float w2;
			};

			float m[9];
		};
};

/** C = A * B */
Matrix3f & mult(Matrix3f & C, const Matrix3f & A, const Matrix3f & B);

#endif//__matrix3_h__