#pragma once

#include <list>
#include <vector>
#include <algorithm>
#include <math.h> 
#include <assert.h>
#include <functional>

#include "vector.h"
#include "vector2f.h"
#include "vector4f.h"
#include "matrix.h"
#include "matrix3.h"
#include "quat.h"
#include "Bounds2f.h"
#include "Bounds3f.h"
#include "OEMBase.h"
#include "TypesBase.h"

namespace oes
{
namespace common_base
{

/** macro. */
#define FABS(x) (float(fabs(x)))		/* implement as is fastest on your machine */
#define FMAX(x, y) (x > y ? x : y)
#define FMIN(x, y) (x < y ? x : y)

struct Plane
{
	Vector Point;
	Vector Normal;

	Plane(const Vector &N, const Vector &p)
		: Point(p)
		, Normal(N)
	{
	}
};

enum EFrustumPlane
{
	FRUSTUM_PLANE_NEAR   = 0, 
	FRUSTUM_PLANE_FAR    = 1, 
	FRUSTUM_PLANE_LEFT   = 2, 
	FRUSTUM_PLANE_RIGHT  = 3, 
	FRUSTUM_PLANE_TOP    = 4, 
	FRUSTUM_PLANE_BOTTOM = 5,
	FRUSTRUM_MAX		 = 6,
};

struct FrustrumCullinfo
{
	Vector	PlaneNormals[6];	// planes
	Vector	PlanePoints[6];		// plane point
};

struct UniCullinfo
{
	std::vector<Plane> Planes;
};

/** remove from here*/
struct Triangle2D
{
	Vector2f P0, P1, P2;
	Vector2f UV0, UV1, UV2;
	unsigned int Indexes[3];

	Triangle2D(Vector2f& p0, Vector2f& p1, Vector2f& p2)
	{
		P0 = p0;
		P1 = p1;
		P2 = p2;
	}

	Triangle2D(Vector2f &p0, Vector2f &p1, Vector2f &p2, Vector2f &uv0, Vector2f &uv1, Vector2f &uv2)
	{
		P0 = p0;
		P1 = p1;
		P2 = p2;

		UV0 = uv0;
		UV1 = uv1;
		UV2 = uv2;
	}
};

struct Pose
{
	Pose()
	{
	}

	Pose(const Quaternion & rot, const Vector& pos)
		: Rotation(rot)
		, Position(pos)
	{
	}
	Quaternion Rotation;
	Vector	   Position;
};

struct UniTriangle
{
	unsigned int Index0;
	unsigned int Index1;
	unsigned int Index2;
	unsigned int IdxNormal;

	bool operator == (const UniTriangle &rhs)
	{
		return Index0 == rhs.Index1 && 
			Index1 == rhs.Index1 &&
			Index2 == rhs.Index2 &&
			IdxNormal == rhs.IdxNormal;
	}

	UniTriangle(unsigned int index0, unsigned int index1, unsigned int index2, unsigned int idxNormal)
		: Index0(index0)
		, Index1(index1)
		, Index2(index2)
		, IdxNormal(idxNormal)
	{
	}
};

struct UniEdge
{
	unsigned int Index0;
	unsigned int Index1;

	UniTriangle triangle0;
	UniTriangle triangle1;

	bool bValid;

	bool operator == (const UniEdge& rhs)
	{
		return Index0 == rhs.Index0 && Index1 == rhs.Index1;
	}

	bool IsValid() const { return bValid; }

	UniEdge(unsigned int index0, unsigned int index1)
		: Index0(index0)
		, Index1(index1)
		, triangle0(0, 0, 0, 0)
		, triangle1(0, 0, 0, 0)
		, bValid(true)
	{
	}
};

typedef std::list<Vector>					  TVectorList;
typedef std::list<Vector>::iterator			  TVectorListIterator;
typedef std::list<Vector>::const_iterator	  TVectorListConstIterator;

typedef std::list<Vector2f>					  TVectorList2f;
typedef std::list<Vector2f>::iterator		  TVectorList2fIterator;
typedef std::list<Vector2f>::const_iterator   TVectorList2fConstIterator;

typedef std::list<Triangle2D>				  TTriangleList2f;
typedef std::list<Triangle2D>::iterator		  TTriangleList2fIterator;
typedef std::list<Triangle2D>::const_iterator TTriangleList2fConstIterator;

typedef std::list<Pose>						  TPoseList;
typedef std::list<Pose>::iterator			  TPoseListIterator;
typedef std::list<Pose>::const_iterator		  TPoseListConstIterator;

/**
   random value  
*/
COMMON_BASE_EXPORT int		rand(int r1, int r2);
COMMON_BASE_EXPORT float	rand(TFlt32 r1, TFlt32 r2);
COMMON_BASE_EXPORT float	GetHalfSpace(const Vector2f& Point, const Vector2f& LineStart, const Vector2f& LineEnd);
COMMON_BASE_EXPORT int		GetHalfSpace(const Vector& PlaneN, const Vector& PlanePt, const Vector& p);

// Computes B = inverse(A)
//       -1
//  B = A
COMMON_BASE_EXPORT Matrix& invert(Matrix &B, const Matrix &A);

// Computes B = Transpose(B)
//       T
//  B = B
COMMON_BASE_EXPORT Matrix & transpose(Matrix &B);
COMMON_BASE_EXPORT Matrix&  scale(Matrix &pValue, float sx, float sy, float sz);

 Vector&  mult_pos(Vector &vOut, const Matrix &mIn, const Vector &vIn);
 Vector&  mult_pos(Vector &vOut, const Vector &vIn, const Matrix &M);


// mult point by matrix
// Out: transformed point
 COMMON_BASE_EXPORT Vector&	transform_coord(Vector &u, const Vector &v, const Matrix &M);
 COMMON_BASE_EXPORT Vector	transform_coord(const Vector &v, const Matrix& M);
 COMMON_BASE_EXPORT Vector2f& transform_coord(Vector2f &u, const Vector2f &v, const Matrix &M);
 COMMON_BASE_EXPORT Vector2f	transform_coord(const Vector2f &v, const Matrix &M);
 COMMON_BASE_EXPORT Vector4f& transform_coord(Vector4f &u, const Vector4f &v, const Matrix &M);
 COMMON_BASE_EXPORT Vector4f	transform_coord(const Vector4f &v, const Matrix &M);
 COMMON_BASE_EXPORT Bounds3f&	transform_bound(Bounds3f &OutBound, const Bounds3f &InBound, const Matrix &M);
 COMMON_BASE_EXPORT Bounds3f	transform_bound(const Bounds3f InBound, const Matrix &M);

 COMMON_BASE_EXPORT inline_ Matrix & frustum( Matrix &M, const float l, const float r, const float b,
                          const float t, const float n, const float f);

/** build perspective matrix */
 COMMON_BASE_EXPORT inline_ Matrix& perspective(Matrix &vOut, float fov, float aspect, float n, float f);

 COMMON_BASE_EXPORT inline_ Matrix& ortho(Matrix &vOut, float l, float r, float b, float t, float zn, float zf);

/** view matrix */
 COMMON_BASE_EXPORT inline_ Matrix& look_at(Matrix &vOut, const Vector &eye, const Vector &center, const Vector &up);

 COMMON_BASE_EXPORT inline_ Vector UnprojectViewport(Vector &v_out,
										const Matrix &proj, 
										const Matrix &view,
                                        const Vector2f &pos, 
										const Vector2f &dimension, 
										float d = 1.f);

 COMMON_BASE_EXPORT inline_ Vector ProjectViewport(Vector &v_out,
									  const Vector &InPos, 
									  const Matrix &view, 
									  const Matrix &proj, 
									  const Vector2f &dimension);

/** intersection tests */
 COMMON_BASE_EXPORT inline_ Vector RayPlaneIntersect(const Vector &planePos,
                                                     const Vector &planeNorm,
                                                     const Vector &linePos,
                                                     const Vector &lineDir,
                                                     float &t);

 COMMON_BASE_EXPORT inline_ Vector RayPlaneIntersect( const Vector &planePos,
                                         const Vector &planeNorm,
                                         const Vector &linePos, 
                                         const Vector &lineDir);




/** ray triangle intersection */
 COMMON_BASE_EXPORT inline_ int IntersectRayWithTriangle(const Vector &start,
                                            const Vector &end,
                                            const Vector &v0,
                                            const Vector &v1,
                                            const Vector &v2, 
                                            float *t );
/** ray triangle intersection 2d */
 COMMON_BASE_EXPORT inline_ bool IntersectRayWithTriangle(const Vector2f &Point,
				  							 const Vector2f &v0,
											 const Vector2f &v1,
											 const Vector2f &v2);

 COMMON_BASE_EXPORT inline_ bool TriangleTriangleIntersection(const Vector &v00,
                                                 const Vector &v01,
                                                 const Vector &v02, 
                                                 const Vector &v10,
                                                 const Vector &v11,
                                                 const Vector &v12 );


 COMMON_BASE_EXPORT inline_ bool TriangleTriangleSplit(const Vector &v00,
                                          const Vector &v01,
                                          const Vector &v02, 
                                          const Vector &v10,
                                          const Vector &v11,
                                          const Vector &v12,
                                          int &out_triangle_count,
                                          Vector *triangle_strip);

 COMMON_BASE_EXPORT inline_ bool PlaneTriangleSplit(const Vector &v00,
                                       const Vector &v01,
                                       const Vector &v02, 
                                       const Vector &pN,
                                       const Vector &pPt,
                                       int &out_triangle_count,
                                       Vector *triangle_strip);

// Interpolate a Catmull-Rom spline.
// t is a [0,1] value and interpolates a curve between p2 and p3.
 COMMON_BASE_EXPORT inline_ void Catmull_Rom_Spline(const Vector &p1,
                                       const Vector &p2,
                                       const Vector &p3,
                                       const Vector &p4,
                                       float t, 
                                       Vector &output);

 COMMON_BASE_EXPORT inline_ void Cubic_Bezier_Spline(const Vector &p1,
										const Vector &p2,
										const Vector &p3,
										const Vector &p4,
										float t, 
										Vector &output);


 COMMON_BASE_EXPORT inline_ void RotateMatrixAxisSelf(Matrix &out,const Vector &u, float rad);
 COMMON_BASE_EXPORT inline_ void AngleMatrix( const float Yaw, const float Pitch, const float Roll, Matrix& matrix );

 COMMON_BASE_EXPORT inline_ bool IsPointInRect(float x, float y, const Vector2f &Start, const Vector2f &End);
 COMMON_BASE_EXPORT inline_ Vector2f GetSegmentNormal(const Vector2f &Start, const Vector2f &End);

//-------------------------------------------------------------------------
 COMMON_BASE_EXPORT inline_ bool SegmentIntersection2D(Vector2f & out_pos,
										  const Vector2f &s1_start,
										  const Vector2f &s1_end,
										  const Vector2f &s2_start,
										  const Vector2f &s2_end);

 COMMON_BASE_EXPORT inline_ bool SegmentSphereIntersection2D(const Vector2f &s_start,
												const Vector2f &s_end,
												const Vector2f &Center,
												float Radius,
												float *mu1,
												float *mu2);

 COMMON_BASE_EXPORT inline_ bool SegmentSphereIntersection(const Vector &s_start,
											  const Vector &s_end,
											  const Vector &Center,
											  float Radius,
											  float *mu1,
											  float *mu2);

 COMMON_BASE_EXPORT inline_ bool	TestBBoxIntercection(const Bounds2f& A, const Bounds2f& B);
 COMMON_BASE_EXPORT inline_ bool	TestBBoxIntercection(const Bounds3f& A, const Bounds3f& B);
 COMMON_BASE_EXPORT inline_ bool FrustrumIntersectBBox(const FrustrumCullinfo& cullinfo, const Bounds3f& B);
 COMMON_BASE_EXPORT inline_ bool FrustrumIntersectBBox(const UniCullinfo& cullinfo, const Bounds3f& B);

// linear interpolation
 COMMON_BASE_EXPORT inline_ float Lerp(float t, float a, float b);
 COMMON_BASE_EXPORT inline_ Vector& Lerp(Vector &OutVec, float t, const Vector &a, const Vector &b);
 COMMON_BASE_EXPORT inline_ Vector4f& Lerp(Vector4f &OutVec, float t, const Vector4f &a, const Vector4f &b);
 COMMON_BASE_EXPORT inline_ Pose& Lerp(Pose &OutPose, float t, const Pose &a, const Pose &b);

 COMMON_BASE_EXPORT inline_ bool BuildEdgeList(std::vector<UniEdge> &OutEdges, const std::vector<UniTriangle> &TriInfo);

 COMMON_BASE_EXPORT float det2x2(float a1, float a2,
						   float b1, float b2);

 COMMON_BASE_EXPORT float det3x3(float a1, float a2, float a3,
							float b1, float b2, float b3, 
							float c1, float c2, float c3);

 COMMON_BASE_EXPORT inline_ float GetDistToLine(const Vector &Point, const Vector &start, const Vector &end);

// clamp inBound into toBound if not overlap result invalid bound
 //COMMON_BASE_EXPORT Bounds2f ClampBound(const Bounds2f &inBound, const Bounds2f &toBound);

}
}