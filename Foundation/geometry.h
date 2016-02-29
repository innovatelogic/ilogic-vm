#pragma once

#include "mathlib.h"
#include <map>

using namespace oes::common_base;

struct EXPORT SPolygonFace
{
	SPolygonFace()
		: smg_id(0)
		, mat_id(0)
	{
	}

	unsigned int smg_id;
	unsigned int mat_id;

	Vector Normal;

	std::vector<unsigned int> VertexIndexes;
	std::vector<unsigned int> ColorIndexes;

	// Out topology info
	std::vector<unsigned int> FaceIndexes;	
};

struct EXPORT SPolygonSubFace
{
	SPolygonSubFace()
		: Idx(0)
	{
	}

	unsigned int Idx;
	Vector Normal;

	// Out topology info
	std::vector<unsigned int> SubFaceIndexes;	
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
struct EXPORT vert_opt
{
	vert_opt()
		: num_tmaps(0)
		, v(0.f,0.f,0.f)
		, n(0.f,0.f,0.f)
		, c(0.f,0.f,0.f,0.f)
		, t(0.f,0.f,0.f)
		, smg_id(BAD_IDX)
		, face_idx(BAD_IDX)
	{ 
	}

	vert_opt(const vert_opt& face);
	~vert_opt();

	vert_opt & operator= (const vert_opt & face);

	unsigned int    smg_id;              // smooth group
	unsigned int    face_idx;            // face index 
	Vector          v;                   // point
	Vector          t;                    
	Vector          n;                   // normal 
	Vector4f        c;                   
	Vector4f        weights;
	unsigned int    bones[4];               

	unsigned int    num_tmaps;           // texture maps
	Vector*         tmaps;                 
};


//----------------------------------------------------------------------------------------------
// keep track of duplicated vertices to limit vertex duplication
// when smoothing group defers.
//----------------------------------------------------------------------------------------------
struct idxvert_opt
{
	unsigned int    new_idx;
	vert_opt        face;

	idxvert_opt(unsigned int NewIndex, vert_opt & Face)
		: new_idx(NewIndex)
		, face(Face)
	{
	}
};

typedef std::map<unsigned int, vert_opt>            FaceMapType;
typedef FaceMapType::value_type                     FaceMapPair;
typedef FaceMapType::iterator                       FaceMapIt;

typedef std::multimap<unsigned int, idxvert_opt>    FaceMMapType;
typedef FaceMMapType::value_type                    FaceMMapPair;
typedef FaceMMapType::iterator                      FaceMMapIt;

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
typedef struct mesh_topology
{
	unsigned int    count;
	FaceMapType     face_map;
	FaceMMapType    face_mmap;

	mesh_topology()
		: count(0)
	{
	}
}mesh_opt;


struct MatTextureInfo
{
	MatTextureInfo() : mat_id(INDEX_NONE), base_channel(INDEX_NONE), new_channel(INDEX_NONE) {}

	unsigned int  mat_id;
	unsigned int  base_channel;
	Matrix        tex_mat;
	Vector2f      uv_scale;
	Vector2f      uv_offset;
	unsigned int  new_channel;
};

typedef std::list<MatTextureInfo>	coord_type;
typedef coord_type::iterator		coord_it;

EXPORT bool operator==(const MatTextureInfo & a, const MatTextureInfo & b);

typedef std::list<unsigned int>             IdxType;
typedef std::map<unsigned int, IdxType*>    TriMapType;
typedef TriMapType::value_type              TriMapPair;
typedef TriMapType::iterator                TriMapIt;
typedef TriMapType::const_iterator          TriMapConstIter;

typedef std::map<unsigned int, mesh_opt*>   MatFaceMapType;
typedef MatFaceMapType::value_type          MatFaceMapPair;
typedef MatFaceMapType::iterator            MatFaceMapIt;
typedef MatFaceMapType::const_iterator      MatFaceMapConstIter;

typedef struct _segmesh
{
	TriMapType      tri_map;
	MatFaceMapType  matface_map;
} segmesh_opt;


struct w_offset
{
	float        weight;
	unsigned int bone_id;
};

/** 
 * Compare vertex by matching smoothing group, texture, color and mapping levels.
 *
 * @return true if vertex similar and false if test fails
 */
EXPORT bool is_matching(const vert_opt & a, const vert_opt & b);

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
struct IndexedPoint2f
{
	Vector2f		Point;
	Vector2f		UV;
	unsigned int	Index;

	IndexedPoint2f()
		: Point(0.f, 0.f)
		, UV(0.f, 0.f)
		, Index(0)
	{
	}

	IndexedPoint2f(Vector2f &point, Vector2f& uv, unsigned int index)
		: Point(point)
		, UV(uv)
		, Index(index)
	{
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
struct SIndexedPoint
{
	Vector			Point;
	unsigned int	Index;

	SIndexedPoint()
		: Point(0.f, 0.f, 0.f)
		, Index(0)
	{
	}

	SIndexedPoint(const Vector &point, unsigned int index)
		: Point(point)
		, Index(index)
	{
	}
};

typedef std::vector<Vector>					TVecVectorList;
typedef TVecVectorList::iterator			TVecVectorListIter;
typedef TVecVectorList::const_iterator		TVecVectorListConstIter;

typedef std::vector<IndexedPoint2f>			TVecIndexedPoint2f;
typedef TVecIndexedPoint2f::iterator		TVecIndexedPoint2fIterator;
typedef TVecIndexedPoint2f::const_iterator  TVecIndexedPoint2fConstIterator;

typedef std::vector<SIndexedPoint>			TVecSIndexedPoint3f;
typedef TVecSIndexedPoint3f::iterator		TVecSIndexedPoint3fIter;
typedef TVecSIndexedPoint3f::const_iterator  TVecSIndexedPoint3fConstIter;

typedef std::vector<SIndexedPoint>			TVecIndexedPoint;
typedef TVecIndexedPoint::iterator			TVecIndexedPointIterator;
typedef TVecIndexedPoint::const_iterator	TVecIndexedPointConstIterator;

template <typename T, class C_CONT, class C_ITER>
C_ITER GetIterNext(C_ITER &Iter, C_CONT &Cont)
{
	if (Cont.size() == 0) { // check size
		return Cont.end();
	}

	C_ITER CopyIter(Iter);

	if (CopyIter == Cont.end()){
		CopyIter = Cont.begin();
	}
	else
	{
		if (++CopyIter == Cont.end()){
			CopyIter = Cont.begin();
		}
	}
	return CopyIter;
}

template <typename T, class C_CONT, class C_ITER>
C_ITER GetIterPrev(C_ITER &Iter, C_CONT &Cont)
{
	if (Cont.size() == 0){ // check size
		return Cont.end();
	}

	C_ITER CopyIter(Iter);

	if (CopyIter == Cont.begin()){
		CopyIter = Cont.end();
	}
	else if (CopyIter == Cont.end()){
		return Cont.begin();
	}
	return --CopyIter;
}

EXPORT bool		IsComplanarPoints(const Vector *ptrPoints, size_t Size, const Vector &N);

/*
*	Ear checking for vector sequence.
*
*	Prev	in previous position
*	Origin	in center position
*	Next	in next position
*/
EXPORT bool		IsEar(const Vector2f &Prev, const Vector2f &Origin, const Vector2f &Next);
EXPORT bool		IsEar(const Vector &Prev, const Vector &Origin, const Vector &Next, const Vector &Normal);

EXPORT bool		IsEarTest(TVecIndexedPoint2f &SourceSplineList, TVecIndexedPoint2fIterator &Iterator);
EXPORT bool		IsEarTest(TVecSIndexedPoint3f &SourceSplineList, TVecSIndexedPoint3fIter &Iterator, const Vector &Normal);

EXPORT bool		IsEarTest(const TVecVectorList &InVertices, SPolygonFace &InPolygon, std::vector<unsigned int>::iterator &Iterator);

/*
*	border left position
*
*	srcSpline		in SplineList
*	OutIterator		out result Point
*/
EXPORT bool		GetMinLeftPoint(TVecIndexedPoint2f &srcSpline, TVecIndexedPoint2fIterator &OutIterator);

EXPORT bool		GetPointCloudBounds(TVecSIndexedPoint3f &srcPoints, 
									TVecSIndexedPoint3fIter &OutIterMinX,
									TVecSIndexedPoint3fIter &OutIterMaxX,
									TVecSIndexedPoint3fIter &OutIterMinY,
									TVecSIndexedPoint3fIter &OutIterMaxY,
									TVecSIndexedPoint3fIter &OutIterMinZ,
									TVecSIndexedPoint3fIter &OutIterMaxZ);

EXPORT bool		GetMinPoint(const TVecVectorList &InVertices, SPolygonFace &InPolygon, std::vector<unsigned int>::iterator &OutIterator);


/** 
*	cycled spline iterator 
*/
EXPORT TVecIndexedPoint2fIterator GetIteratorNext(TVecIndexedPoint2f &Spline, const TVecIndexedPoint2fIterator& Iterator);
EXPORT TVecIndexedPoint2fIterator GetIteratorPrev(TVecIndexedPoint2f &Spline, const TVecIndexedPoint2fIterator& Iterator);

/** 
*	split poligon in two
*	
*	SourceSpline	in source spline
*	IterStart		in iter clip border
*	IterEnd			in iter clip border second
*	OutSplineList1	out clipped spline 1
*	OutSplineList2	out clipped spline 2
*/
EXPORT bool		Split2Poly(TVecIndexedPoint2f &SourceSpline,
							TVecIndexedPoint2fIterator IterStart,
							TVecIndexedPoint2fIterator IterEnd,
							TVecIndexedPoint2f &OutSplineList1,
							TVecIndexedPoint2f &OutSplineList2);

EXPORT void		Split2Poly(TVecSIndexedPoint3f &SourceSpline,
						   TVecSIndexedPoint3fIter &IterStart,
						   TVecSIndexedPoint3fIter &IterEnd,
						   TVecSIndexedPoint3f &OutSplineList1,
						   TVecSIndexedPoint3f &OutSplineList2);

EXPORT void		Split2Poly(SPolygonFace &SourceSpline,
							std::vector<unsigned int>::iterator &IterStart,
							std::vector<unsigned int>::iterator &IterEnd,
							std::vector<unsigned int> &OutSplineList1,
							std::vector<unsigned int> &OutSplineList2);

/** 
*	test polygon convex
*/
EXPORT bool		IsPolygonConvex(TVecIndexedPoint2f &SourceSplineList, 
								const TVecIndexedPoint2fIterator &CWIterator, 
								const TVecIndexedPoint2fIterator &CCWIterator, bool CCW = true);

EXPORT bool		IsPolygonConvex(TVecSIndexedPoint3f &SourceSplineList, 
								TVecSIndexedPoint3f::const_iterator &CWIterator, 
								TVecSIndexedPoint3f::const_iterator &CCWIterator,
								const Vector &Normal, bool CCW = true);
/** 
*	helper function
*/
EXPORT bool		IsPointInsideRegion(Vector2f& Point, float MinX, float MaxX, float MinY, float MaxY);

EXPORT bool		IsPointInsidePoligon(TVecIndexedPoint2f &SourceSplineList, 
									 const Vector2f& PointTest, 
									 const TVecIndexedPoint2fIterator &CWIterator,
									 const TVecIndexedPoint2fIterator &CCWIterator);

EXPORT bool		IsPointInsidePoligon(TVecSIndexedPoint3f &SourceSplineList, 
									 const Vector &PointTest, 
									 TVecSIndexedPoint3fConstIter &CWIterator,
									 TVecSIndexedPoint3fConstIter &CCWIterator,
									 const Vector &Normal);

EXPORT bool		IsPointInsidePoligon(const TVecVectorList &InVertices,
									 SPolygonFace &InPolygon,
									 const Vector &PointTest, 
									 std::vector<unsigned int>::iterator &CWIterator,
									 std::vector<unsigned int>::iterator &CCWIterator);

/** 
*	build boundering box
*/
EXPORT void		CalcSplineBBox(TVecIndexedPoint2f& srcSpline);

/**
*	check close convex monotine polygon for overlapping right part of spline
*	true means polygon need to close.
*/
EXPORT bool		MonotonePolygonCorrection(TVecIndexedPoint2f &SourceSplineList,
										  const TVecIndexedPoint2fIterator& InIterStart, 
										  TVecIndexedPoint2fIterator &OutCWIterator,
										  TVecIndexedPoint2fIterator &OutCCWIterator, 
										  TVecIndexedPoint2fIterator &OutDividePoint);

EXPORT bool		MonotonePolygonCorrection(TVecSIndexedPoint3f &SourceSplineList,
										  TVecSIndexedPoint3fIter &InIterStart, 
										  TVecSIndexedPoint3fIter &OutCWIterator,
										  TVecSIndexedPoint3fIter &OutCCWIterator, 
										  TVecSIndexedPoint3fIter &OutDividePoint,
										  const Vector &Normal);

EXPORT bool		MonotonePolygonCorrection(const TVecVectorList &InVertices,
											SPolygonFace &InPolygon,
											std::vector<unsigned int>::iterator &InIterStart, 
											std::vector<unsigned int>::iterator &OutCWIterator,
											std::vector<unsigned int>::iterator &OutCCWIterator, 
											std::vector<unsigned int>::iterator &OutDividePoint);


EXPORT bool		GetMonotonePolygon(TVecIndexedPoint2f &SourceSplineList, 
								   TVecIndexedPoint2fIterator &IterStart, 
								   TVecIndexedPoint2fIterator &CCIter, 
								   TVecIndexedPoint2fIterator &CCWIter, 
								   TVecIndexedPoint2fIterator &DividePoint);

EXPORT bool		GetMonotonePolygon(TVecSIndexedPoint3f &SourceSplineList, 
								   TVecSIndexedPoint3fIter &IterStart, 
								   TVecSIndexedPoint3fIter &CCIter, 
								   TVecSIndexedPoint3fIter &CCWIter, 
								   TVecSIndexedPoint3fIter &DividePoint,
								   const Vector &Normal);

EXPORT bool		GetMonotonePolygon(const TVecVectorList &InVertices,
									SPolygonFace &InPolygon,
									std::vector<unsigned int>::iterator &InIterStart, 
									std::vector<unsigned int>::iterator &OutCWIter, 
									std::vector<unsigned int>::iterator &OutCCWIter, 
									std::vector<unsigned int>::iterator &OutDividePoint);

/** 
 * Build coplanar patches
 */
EXPORT void		BuildFacePatches(const TVecVectorList &InVertices,
								 SPolygonFace &InPolygon,
								 std::vector<unsigned int>::iterator &InIterStartCW,
								 std::vector<unsigned int>::iterator &InIterStartCCW,
								 std::vector<SPolygonSubFace> &OutPatches);

/** 
*	spline tessellation
*/
EXPORT bool		Tesselate(TVecIndexedPoint2f &Spline, std::vector<size_t> &OutIndexList);
EXPORT bool		Tesselate(const TVecVectorList &InVertices, SPolygonFace &InPolygon, std::vector<unsigned int> &OutTriangleIndexes);
/** 
*	monotone polygon triangulation
*/
EXPORT void		Triangulate(TVecIndexedPoint2f &SourceSplineList,
							TVecIndexedPoint2fIterator IterStart, 
							TVecIndexedPoint2fIterator &CWIterator,
							TVecIndexedPoint2fIterator &CCWIterator,
							std::vector<size_t>		   &OutIndexList);

EXPORT void		Triangulate(TVecSIndexedPoint3f &SourceSplineList,
							TVecSIndexedPoint3fIter &IterStart, 
							TVecSIndexedPoint3fIter &CWIterator,
							TVecSIndexedPoint3fIter &CCWIterator,
							std::vector<size_t>		   &OutIndexList);

EXPORT void		Triangulate(SPolygonFace &InOutPolygon,
							std::vector<unsigned int>::iterator &IterStart, 
							std::vector<unsigned int>::iterator &CWIterator,
							std::vector<unsigned int>::iterator &CCWIterator,
							std::vector<unsigned int> &OutTriangleIndexes);
