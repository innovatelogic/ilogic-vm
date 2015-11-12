#include "geometry.h"

//----------------------------------------------------------------------------------------------
vert_opt::vert_opt(const vert_opt & face)
{
	num_tmaps = 0; 
	*this = face;
}

//----------------------------------------------------------------------------------------------
vert_opt::~vert_opt()
{
	if (num_tmaps)
	{
		delete [] tmaps;
		num_tmaps = 0;
		tmaps = 0;
	}
}

//----------------------------------------------------------------------------------------------
vert_opt & vert_opt::operator= (const vert_opt & face)
{
	face_idx = face.face_idx;
	smg_id = face.smg_id;
	v = face.v;
	t = face.t;
	n = face.n;
	c = face.c;
	weights = face.weights;
	bones[0] = face.bones[0];
	bones[1] = face.bones[1];
	bones[2] = face.bones[2];
	bones[3] = face.bones[3];

	if (num_tmaps)
	{
		delete [] tmaps;
	}

	num_tmaps = face.num_tmaps;

	if (num_tmaps)
	{
		tmaps = new Vector[num_tmaps];
	}

	for (unsigned int i = 0; i < num_tmaps; ++i)
	{
		tmaps[i] = face.tmaps[i];
	}

	return *this;
}

//----------------------------------------------------------------------------------------------
bool operator==(const MatTextureInfo & a, const MatTextureInfo & b)
{
	if (memcmp(&a, &b, sizeof(MatTextureInfo)) == 0)
		return true;
	return false;
}

//----------------------------------------------------------------------------------------------
bool is_matching(const vert_opt & a, const vert_opt & b)
{
	bool tex_test = true;
	bool map_test = true;
	bool col_test = true;
	bool smg_test = true;

	if ((a.smg_id != b.smg_id) || a.smg_id == 0 || b.smg_id == 0){
		smg_test = false;
	}
	if (a.t != b.t){
		tex_test = false;
	}
	if (a.c != b.c){
		col_test = false;
	}

	if (a.num_tmaps != b.num_tmaps)
	{
		map_test = false;
	}
	else
	{
		for (unsigned int j = 0; j < a.num_tmaps && map_test == true; ++j)
		{
			if (a.tmaps[j] != b.tmaps[j])
			{
				map_test = false;
				break;
			}
		}
	}
	return tex_test && map_test && col_test && smg_test;
}

//----------------------------------------------------------------------------------------------
bool IsComplanarPoints(const Vector *ptrPoints, size_t Size, const Vector &N)
{
	bool bResult = false;

	if (Size > 2)
	{
		bResult = true;
		size_t Idx = 1;

		do
		{
			Vector V = *(ptrPoints + Idx) - *(ptrPoints);
			V.normalize();

			float dot = V.Dot(N);

			if ( dot < -m_eps || dot > m_eps )
			{
				bResult = false;
				break;
			}

		}while (Idx < Size);
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IsEar(const Vector2f& Prev, const Vector2f& Origin, const Vector2f& Next)
{
	float x = Next.x;
	float y = Next.y;

	float x1 = Origin.x;
	float y1 = Origin.y;

	float x2 = Prev.x;
	float y2 = Prev.y;

	float hs = ((x - x1) * (y2 - y1)) - ((x2 - x1) * (y - y1));

	return hs > 0.f;
}

//----------------------------------------------------------------------------------------------
bool IsEar(const Vector &Prev, const Vector &Origin, const Vector &Next, const Vector &Normal)
{
	Vector SecPlaneN = (Origin - Prev).Cross(Normal);
	SecPlaneN.normalize();

	return GetHalfSpace(SecPlaneN, Origin, Next) < 0.f;
}

//----------------------------------------------------------------------------------------------
bool IsEarTest(TVecIndexedPoint2f &SourceSplineList, TVecIndexedPoint2fIterator &Iterator)
{
	TVecIndexedPoint2fIterator IterPrev = GetIteratorPrev(SourceSplineList, Iterator);
	TVecIndexedPoint2fIterator IterNext = GetIteratorNext(SourceSplineList, Iterator);

	return IsEar((*IterPrev).Point, (*Iterator).Point, (*IterNext).Point);
}

//----------------------------------------------------------------------------------------------
bool IsEarTest(TVecSIndexedPoint3f &SourceSplineList, TVecSIndexedPoint3fIter &Iterator, const Vector &Normal)
{
	TVecSIndexedPoint3fIter IterPrev = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(Iterator, SourceSplineList);
	TVecSIndexedPoint3fIter IterNext = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(Iterator, SourceSplineList);

	return IsEar((*IterPrev).Point, (*Iterator).Point, (*IterNext).Point, Normal);
}

//----------------------------------------------------------------------------------------------
bool IsEarTest(const TVecVectorList &InVertices, SPolygonFace &InPolygon, std::vector<unsigned int>::iterator &Iterator)
{
	std::vector<unsigned int>::iterator IterPrev = GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(Iterator, InPolygon.VertexIndexes);
	std::vector<unsigned int>::iterator IterNext = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(Iterator, InPolygon.VertexIndexes);

	return IsEar(InVertices[*IterPrev], InVertices[*Iterator], InVertices[*IterNext], InPolygon.Normal);
}

//----------------------------------------------------------------------------------------------
bool GetMinLeftPoint(TVecIndexedPoint2f& srcSpline, TVecIndexedPoint2fIterator &OutIterator)
{
	bool bResult = false;

	float MinX = V_FLT_MAX;

	TVecIndexedPoint2fIterator Iterator = srcSpline.begin();

	while (Iterator != srcSpline.end())
	{
		if ((*Iterator).Point.x < MinX)
		{
			MinX = (*Iterator).Point.x;
			OutIterator = Iterator;
			bResult = true;   
		}
		++Iterator;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool GetPointCloudBounds(TVecSIndexedPoint3f &srcPoints, 
	 					 TVecSIndexedPoint3fIter &OutIterMinX,
						 TVecSIndexedPoint3fIter &OutIterMaxX,
						 TVecSIndexedPoint3fIter &OutIterMinY,
						 TVecSIndexedPoint3fIter &OutIterMaxY,
						 TVecSIndexedPoint3fIter &OutIterMinZ,
						 TVecSIndexedPoint3fIter &OutIterMaxZ)
{
	bool bResult = false;

	Bounds3f Bounds(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX);

	TVecSIndexedPoint3fIter Iter = srcPoints.begin();

	if (srcPoints.size() > 0)
	{
		while (Iter != srcPoints.end())
		{
			// x
			if ((*Iter).Point.x < Bounds.bound_min.x)
			{
				Bounds.bound_min.x = (*Iter).Point.x;
				OutIterMinX = Iter;  
			}

			if ((*Iter).Point.x > Bounds.bound_max.x)
			{
				Bounds.bound_max.x = (*Iter).Point.x;
				OutIterMaxX = Iter;  
			}

			// y
			if ((*Iter).Point.y < Bounds.bound_min.y)
			{
				Bounds.bound_min.y = (*Iter).Point.y;
				OutIterMinY = Iter;  
			}

			if ((*Iter).Point.y > Bounds.bound_max.y)
			{
				Bounds.bound_max.y = (*Iter).Point.y;
				OutIterMaxY = Iter;  
			}
			// z
			if ((*Iter).Point.z < Bounds.bound_min.z)
			{
				Bounds.bound_min.z = (*Iter).Point.z;
				OutIterMinZ = Iter;  
			}

			if ((*Iter).Point.z > Bounds.bound_max.z)
			{
				Bounds.bound_max.z = (*Iter).Point.z;
				OutIterMaxZ = Iter;  
			}
			++Iter;
		}
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool GetMinPoint(const TVecVectorList &InVertices,
				 SPolygonFace &InPolygon,
				 std::vector<unsigned int>::iterator &OutIterator)
{
	bool bResult = false;

	if (InPolygon.VertexIndexes.size() >= 3 && InVertices.size() >= 3)
	{
		//@TODO check not the same 
		Vector T = InVertices[InPolygon.VertexIndexes[1]] - InVertices[InPolygon.VertexIndexes[0]];
		T.normalize();

		Vector CrossN = InPolygon.Normal.Cross(T);
		CrossN.normalize();

		float fMin = V_FLT_MAX;

		Vector p = InVertices[InPolygon.VertexIndexes[0]];

		std::vector<unsigned int>::iterator Iterator = OutIterator = InPolygon.VertexIndexes.begin();

		++Iterator;

		while (Iterator != InPolygon.VertexIndexes.end())
		{
			Vector PlanePt(InVertices[*Iterator]);
			float val = CrossN.x * (p.x - PlanePt.x) + CrossN.y * (p.y - CrossN.y)  + CrossN.z * (p.z - PlanePt.z);

			if (val < fMin) // check distance to plane
			{
				fMin = val;
				OutIterator = Iterator;
				bResult = true; 
			}
			++Iterator;
		}
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
/** helper function for cycled access*/
TVecIndexedPoint2fIterator GetIteratorNext(TVecIndexedPoint2f &Spline, const TVecIndexedPoint2fIterator& Iterator)
{
	TVecIndexedPoint2fIterator CopyIter(Iterator);

	if (CopyIter == Spline.end()) // swap end() -> begin()
	{ 
		CopyIter = Spline.begin();

		if (CopyIter == Spline.end()){ // additional check for empty
			return Spline.end();
		}
	}

	if (++CopyIter == Spline.end()){
		CopyIter = Spline.begin();
	}
	return CopyIter;
}

//----------------------------------------------------------------------------------------------
/** helper function for cycled access*/
TVecIndexedPoint2fIterator GetIteratorPrev(TVecIndexedPoint2f &Spline, const TVecIndexedPoint2fIterator& Iterator)
{
	TVecIndexedPoint2fIterator CopyIter(Iterator);

	if (CopyIter == Spline.begin()){
		CopyIter = Spline.end();
	}

	if (CopyIter == Spline.end() && Spline.size() == 0){
		return Spline.end();
	}
	return --CopyIter;
}

//----------------------------------------------------------------------------------------------
bool Split2Poly(TVecIndexedPoint2f &SourceSpline,
				TVecIndexedPoint2fIterator IterStart,
				TVecIndexedPoint2fIterator IterEnd,
				TVecIndexedPoint2f &OutSplineList1,
				TVecIndexedPoint2f &OutSplineList2)
{
	TVecIndexedPoint2fIterator CopyIterator(IterStart);

	while (CopyIterator != GetIteratorNext(SourceSpline, IterEnd))
	{
		OutSplineList1.push_back(*CopyIterator);
		CopyIterator = GetIteratorNext(SourceSpline, CopyIterator);
	}

	OutSplineList2.push_back(*IterStart);
	CopyIterator = IterEnd;
	while (CopyIterator != IterStart)
	{
		OutSplineList2.push_back(*CopyIterator);
		CopyIterator = GetIteratorNext(SourceSpline, CopyIterator);
	}
	return true;
}

//----------------------------------------------------------------------------------------------
void Split2Poly(TVecSIndexedPoint3f &SourceSpline,
				TVecSIndexedPoint3fIter &IterStart,
				TVecSIndexedPoint3fIter &IterEnd,
				TVecSIndexedPoint3f &OutSplineList1,
				TVecSIndexedPoint3f &OutSplineList2)
{
	TVecSIndexedPoint3fIter CopyIterator(IterStart);

	while (CopyIterator != GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(IterEnd, SourceSpline))
	{
		OutSplineList1.push_back(*CopyIterator);
		CopyIterator = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(CopyIterator, SourceSpline);
	}

	OutSplineList2.push_back(*IterStart);
	CopyIterator = IterEnd;
	while (CopyIterator != IterStart)
	{
		OutSplineList2.push_back(*CopyIterator);
		CopyIterator = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(CopyIterator, SourceSpline);
	}
}

//----------------------------------------------------------------------------------------------
void Split2Poly(SPolygonFace &SourceSpline,
				std::vector<unsigned int>::iterator &IterStart,
				std::vector<unsigned int>::iterator &IterEnd,
				std::vector<unsigned int> &OutSplineList1,
				std::vector<unsigned int> &OutSplineList2)
{
	std::vector<unsigned int>::iterator CopyIterator(IterStart);

	std::vector<unsigned int>::iterator IterStop = 
		GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterEnd, SourceSpline.VertexIndexes);

	while (CopyIterator != IterStop)
	{
		OutSplineList1.push_back(*CopyIterator);
		CopyIterator = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(CopyIterator, SourceSpline.VertexIndexes);
	}

	OutSplineList2.push_back(*IterStart);
	CopyIterator = IterEnd;

	while (CopyIterator != IterStart)
	{
		OutSplineList2.push_back(*CopyIterator);
		CopyIterator = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(CopyIterator, SourceSpline.VertexIndexes);
	}
}

//----------------------------------------------------------------------------------------------
bool IsPolygonConvex(TVecIndexedPoint2f &SourceSplineList,
					 const TVecIndexedPoint2fIterator &CWIterator,
					 const TVecIndexedPoint2fIterator &CCWIterator, bool CCW /*= true*/)
{
	float x1 = (*CWIterator).Point.x;
	float y1 = (*CWIterator).Point.y;

	float x2 = (*CCWIterator).Point.x;
	float y2 = (*CCWIterator).Point.y;

	if (CCW == true)
	{
		TVecIndexedPoint2fIterator IterCWRunnerStart = CWIterator;
		TVecIndexedPoint2fIterator IterCWRunnerEnd = GetIteratorPrev(SourceSplineList, CWIterator);

		do
		{
			float x = (*IterCWRunnerEnd).Point.x;
			float y = (*IterCWRunnerEnd).Point.y;

			float hs = ((x - x1) * (y2 - y1)) - ((x2 - x1) * (y - y1));

			if (hs > 0.f) {
				return false;
			}

			IterCWRunnerStart = IterCWRunnerEnd;
			IterCWRunnerEnd = GetIteratorPrev(SourceSplineList, IterCWRunnerEnd);

		}while(IterCWRunnerStart != CCWIterator);
	}
	else
	{
		// TODO
	}

	return true;
}

//----------------------------------------------------------------------------------------------
bool IsPolygonConvex(TVecSIndexedPoint3f &SourceSplineList, 
					 TVecSIndexedPoint3fConstIter &CWIterator, 
					 TVecSIndexedPoint3fConstIter &CCWIterator,
					 const Vector &Normal,
					 bool CCW /*= true*/)
{
// 	if (CCW == true)
// 	{
// 		TVecSIndexedPoint3fConstIter IterCWRunnerStart = CCWIterator;
// 		TVecSIndexedPoint3fConstIter IterCWRunnerEnd =  GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fConstIter >(IterCWRunnerStart, SourceSplineList);
// 
//  		do
//  		{
// 			Vector SecPlaneN = (*Prev - *Origin).Cross(Normal);
// 			SecPlaneN.normalize();
// 
// 			if (GetHalfSpace(SecPlaneN, *Origin, *Next) < 0.f){
// 				return false;
// 			}
// 
// 			Prev = Origin;
//  			Origin = Next;
//  			Next = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fConstIter >(Next, SourceSplineList);
//  
//  		}while(IterCWRunnerStart != CCWIterator);
// 	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool IsPointInsideRegion(Vector2f& Point, float MinX, float MaxX, float MinY, float MaxY)
{
	return (Point.x >= MinX && Point.x <= MaxX) && (Point.y >= MinY && Point.y <= MaxY);
}

//----------------------------------------------------------------------------------------------
bool IsPointInsidePoligon(TVecIndexedPoint2f &SourceSplineList,
						  const Vector2f& PointTest,
						  const TVecIndexedPoint2fIterator &CWIterator,
						  const TVecIndexedPoint2fIterator &CCWIterator)
{
	float x = PointTest.x;
	float y = PointTest.y;

	float x1 = (*CWIterator).Point.x;
	float y1 = (*CWIterator).Point.y;

	float x2 = (*CCWIterator).Point.x;
	float y2 = (*CCWIterator).Point.y;

	float hs = ((x - x1) * (y2 - y1)) - ((x2 - x1) * (y - y1));

	if (hs >= 0.f) {
		return false;
	}

	TVecIndexedPoint2fIterator IterCWRunnerStart = CCWIterator;
	TVecIndexedPoint2fIterator IterCWRunnerEnd   = GetIteratorNext(SourceSplineList, CCWIterator);

	while (IterCWRunnerStart != CWIterator)
	{
		x1 = (*IterCWRunnerStart).Point.x;
		y1 = (*IterCWRunnerStart).Point.y;

		x2 = (*IterCWRunnerEnd).Point.x;
		y2 = (*IterCWRunnerEnd).Point.y;

		hs = ((x - x1) * (y2 - y1)) - ((x2 - x1) * (y - y1));

		if (hs >= 0.f){
			return false; 
		}

		IterCWRunnerStart = IterCWRunnerEnd;
		IterCWRunnerEnd = GetIteratorNext(SourceSplineList, IterCWRunnerEnd);
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool IsPointInsidePoligon(TVecSIndexedPoint3f &SourceSplineList, 
							const Vector &PointTest, 
							TVecSIndexedPoint3fConstIter &CWIterator,
							TVecSIndexedPoint3fConstIter &CCWIterator,
							const Vector &Normal)
{
	TVecSIndexedPoint3fConstIter IterCWRunnerStart = CCWIterator;
	TVecSIndexedPoint3fConstIter IterCWRunnerEnd   = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fConstIter >(CCWIterator, SourceSplineList);

	while (IterCWRunnerStart != CWIterator)
	{
		Vector Diff = (*IterCWRunnerEnd).Point - (*IterCWRunnerStart).Point;
		Vector SecPlaneN = Diff.Cross(Normal);
		SecPlaneN.normalize();

		// normal is outer looking
		if (GetHalfSpace(SecPlaneN, (*IterCWRunnerEnd).Point, PointTest) >= 0.f){ // check point is out
			return false; 
		}

		IterCWRunnerStart = IterCWRunnerEnd;
		IterCWRunnerEnd   = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fConstIter >(IterCWRunnerEnd, SourceSplineList);
	}

	// check a divide line
	Vector Diff = (*CCWIterator).Point - (*CWIterator).Point;
	Vector SecPlaneN = Diff.Cross(Normal);
	SecPlaneN.normalize();

	// normal is outer looking
	if (GetHalfSpace(SecPlaneN, (*CWIterator).Point, PointTest) >= 0.f){ // check point is out
		return false; 
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool IsPointInsidePoligon(const TVecVectorList &InVertices,
						  SPolygonFace &InPolygon,
						  const Vector &PointTest, 
						  std::vector<unsigned int>::iterator &CWIterator,
						  std::vector<unsigned int>::iterator &CCWIterator)
{
	std::vector<unsigned int>::iterator IterCWRunnerStart = CCWIterator;
	std::vector<unsigned int>::iterator IterCWRunnerEnd   = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(CCWIterator, InPolygon.VertexIndexes);

	while (IterCWRunnerStart != CWIterator)
	{
		Vector Diff = InVertices[*IterCWRunnerEnd] - InVertices[*IterCWRunnerStart];
		Vector SecPlaneN = Diff.Cross(InPolygon.Normal);
		SecPlaneN.normalize();

		// normal is outer looking
		if (GetHalfSpace(SecPlaneN, InVertices[*IterCWRunnerEnd], PointTest) >= 0.f){ // check point is out
			return false; 
		}

		IterCWRunnerStart = IterCWRunnerEnd;
		IterCWRunnerEnd   = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterCWRunnerEnd, InPolygon.VertexIndexes);
	}

	// check a divide line
	Vector Diff = InVertices[*CCWIterator] - InVertices[*CWIterator];
	Vector SecPlaneN = Diff.Cross(InPolygon.Normal);
	SecPlaneN.normalize();

	// normal is outer looking
	if (GetHalfSpace(SecPlaneN, InVertices[*CWIterator], PointTest) >= 0.f){ // check point is out
		return false; 
	}
	return true;
}

//----------------------------------------------------------------------------------------------
void CalcSplineBBox(TVecIndexedPoint2f& srcSpline)
{
	Bounds2f BoundChilds;

	if (srcSpline.size() == 0) {
		return;
	}

	BoundChilds.bound_max = Vector2f(-V_FLT_MAX, -V_FLT_MAX);
	BoundChilds.bound_min = Vector2f(V_FLT_MAX, V_FLT_MAX);

	TVecIndexedPoint2fIterator Iterator = srcSpline.begin();

	while (Iterator != srcSpline.end())
	{
		Vector2f SplinePoint = Iterator->Point;

		if (SplinePoint.x < BoundChilds.bound_min.x){
			BoundChilds.bound_min.x = SplinePoint.x;
		}

		if (SplinePoint.y < BoundChilds.bound_min.y){
			BoundChilds.bound_min.y = SplinePoint.y;
		}

		if (SplinePoint.x > BoundChilds.bound_max.x){
			BoundChilds.bound_max.x = SplinePoint.x;
		}

		if (SplinePoint.y > BoundChilds.bound_max.y){
			BoundChilds.bound_max.y = SplinePoint.y;
		}
		++Iterator;
	}
}

//----------------------------------------------------------------------------------------------
bool MonotonePolygonCorrection(TVecIndexedPoint2f &SourceSplineList,
							   const TVecIndexedPoint2fIterator& InIterStart, 
							   TVecIndexedPoint2fIterator &OutCWIterator,
							   TVecIndexedPoint2fIterator &OutCCWIterator, 
							   TVecIndexedPoint2fIterator &OutDividePoint)
{
	bool bPolygonNeedClosed = false;

	float LengthCW  = length((*OutCWIterator).Point - (*InIterStart).Point);
	float LengthCCW = length((*OutCCWIterator).Point - (*InIterStart).Point);
	float LengthMin = LengthCW > LengthCCW ? LengthCW : LengthCCW;

	TVecIndexedPoint2fIterator IterCWRunnerStart = OutCWIterator;
	TVecIndexedPoint2fIterator IterCWRunnerEnd   = GetIteratorNext(SourceSplineList, OutCWIterator);

	// check outer polygon intersection
	do
	{
		if (IsPointInsidePoligon(SourceSplineList, (*IterCWRunnerEnd).Point, OutCWIterator, OutCCWIterator))
		{
			float Length = length((*IterCWRunnerEnd).Point - (*InIterStart).Point);

			if (Length < LengthMin)
			{
				LengthMin = Length;
				OutDividePoint = IterCWRunnerEnd; // save divide point
				bPolygonNeedClosed = true;		  // polygon need be divided 2poly
			}
		}

		IterCWRunnerStart = IterCWRunnerEnd;
		IterCWRunnerEnd = GetIteratorNext(SourceSplineList, IterCWRunnerEnd);

	}while (IterCWRunnerStart != OutCCWIterator);

	if (bPolygonNeedClosed) // find intersection
	{
		TVecIndexedPoint2fIterator IterateCW  = GetIteratorNext(SourceSplineList, InIterStart);
		TVecIndexedPoint2fIterator IterateCCW = GetIteratorPrev(SourceSplineList, InIterStart);

		while (IterateCW != GetIteratorNext(SourceSplineList, OutCWIterator) &&
			IterateCCW != GetIteratorPrev(SourceSplineList, OutCCWIterator))
		{
			if (IsPointInsidePoligon(SourceSplineList, (*OutDividePoint).Point, IterateCW, IterateCCW))
			{
				OutCWIterator = GetIteratorPrev(SourceSplineList, IterateCW);
				OutCCWIterator = GetIteratorNext(SourceSplineList, IterateCCW);
				break;
			}

			IterateCW = GetIteratorNext(SourceSplineList, IterateCW);
			IterateCCW = GetIteratorPrev(SourceSplineList, IterateCCW);
		}
	}

// 	if (IsPolygonConvex(SourceSplineList, OutCWIterator, OutCCWIterator) == false)
// 	{
// 		while( OutCWIterator != InIterStart && OutCCWIterator != InIterStart )
// 		{
// 			OutCWIterator = GetIteratorPrev(SourceSplineList, OutCWIterator);
// 			OutCCWIterator = GetIteratorNext(SourceSplineList, OutCCWIterator);
// 
// 			if (IsPolygonConvex(SourceSplineList, OutCWIterator, OutCCWIterator))
// 			{
// 				bPolygonNeedClosed = true;
// 				break;
// 			}
// 		}
// 	}
	return bPolygonNeedClosed;
}

//----------------------------------------------------------------------------------------------
bool MonotonePolygonCorrection(TVecSIndexedPoint3f &SourceSplineList,
								TVecSIndexedPoint3fIter &InIterStart, // start point
								TVecSIndexedPoint3fIter &OutCWIterator, 
								TVecSIndexedPoint3fIter &OutCCWIterator, 
								TVecSIndexedPoint3fIter &OutDividePoint,
								const Vector &Normal)
{
	bool bPolygonNeedClosed = false;

	float LengthCW  = length((*OutCWIterator).Point - (*InIterStart).Point);
	float LengthCCW = length((*OutCCWIterator).Point - (*InIterStart).Point);
	float LengthMin = LengthCW > LengthCCW ? LengthCW : LengthCCW;

	TVecSIndexedPoint3fIter IterCWRunnerStart = OutCWIterator;
	TVecSIndexedPoint3fIter IterCWRunnerEnd = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCWIterator, SourceSplineList);

	// check outer polygon intersection
	do
	{
		if (IsPointInsidePoligon(SourceSplineList, (*IterCWRunnerEnd).Point, OutCWIterator, OutCCWIterator, Normal))
		{
			float Length = length((*IterCWRunnerEnd).Point - (*InIterStart).Point);

			if (Length < LengthMin) // find closest point to start point
			{
				LengthMin = Length;
				OutDividePoint = IterCWRunnerEnd; // save divide point
				bPolygonNeedClosed = true;		  // polygon need be divided 2poly
			}
		}

		IterCWRunnerStart = IterCWRunnerEnd;
		IterCWRunnerEnd = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(IterCWRunnerEnd, SourceSplineList);

	}while (IterCWRunnerStart != OutCCWIterator);

	// adjust minimum monotone polygon for divide point
  	if (bPolygonNeedClosed) // find intersection
  	{
  		TVecSIndexedPoint3fIter IterateCW  = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(InIterStart, SourceSplineList);
  		TVecSIndexedPoint3fIter IterateCCW = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(InIterStart, SourceSplineList);
  
  		while (IterateCW != GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCWIterator, SourceSplineList) &&
  				IterateCCW != GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCCWIterator, SourceSplineList))
  		{
  			if (IsPointInsidePoligon(SourceSplineList, (*OutDividePoint).Point, IterateCW, IterateCCW, Normal))
  			{
  				OutCWIterator = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(IterateCW, SourceSplineList);
  				OutCCWIterator = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(IterateCCW, SourceSplineList);
  				break;
  			}
  
  			IterateCW = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(IterateCW, SourceSplineList);
  			IterateCCW = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(IterateCCW, SourceSplineList);
  		}
  	}
 
//  	if (!IsPolygonConvex(SourceSplineList, OutCWIterator, OutCCWIterator, Normal))
//  	{
//  		while( OutCWIterator != InIterStart && OutCCWIterator != InIterStart )
//  		{
//  			OutCWIterator = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCWIterator, SourceSplineList);
//  			OutCCWIterator = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCCWIterator, SourceSplineList);
//  
//  			if (IsPolygonConvex(SourceSplineList, OutCWIterator, OutCCWIterator, Normal))
//  			{
//  				bPolygonNeedClosed = true;
//  				break;
//  			}
//  		}
//  	}

	return bPolygonNeedClosed;
}

//----------------------------------------------------------------------------------------------
bool MonotonePolygonCorrection(const TVecVectorList &InVertices,
							   SPolygonFace &InPolygon,
							   std::vector<unsigned int>::iterator &InIterStart, 
							   std::vector<unsigned int>::iterator &OutCWIterator,
							   std::vector<unsigned int>::iterator &OutCCWIterator, 
							   std::vector<unsigned int>::iterator &OutDividePoint)
{
	bool bPolygonNeedClosed = false;

	float LengthCW  = length(InVertices[*OutCWIterator] - InVertices[*InIterStart]);
	float LengthCCW = length(InVertices[*OutCCWIterator] - InVertices[*InIterStart]);
	float LengthMin = LengthCW > LengthCCW ? LengthCW : LengthCCW;

	std::vector<unsigned int>::iterator IterCWRunnerStart = OutCWIterator;
	std::vector<unsigned int>::iterator IterCWRunnerEnd = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(OutCWIterator, InPolygon.VertexIndexes);

	// check outer polygon intersection
	do
	{
		if (IsPointInsidePoligon(InVertices, InPolygon, InVertices[*IterCWRunnerEnd], OutCWIterator, OutCCWIterator))
		{
			float Length = length(InVertices[*IterCWRunnerEnd] - InVertices[*InIterStart]);

			if (Length < LengthMin) // find closest point to start point
			{
				LengthMin = Length;
				OutDividePoint = IterCWRunnerEnd; // save divide point
				bPolygonNeedClosed = true;		  // polygon need be divided 2poly
			}
		}

		IterCWRunnerStart = IterCWRunnerEnd;
		IterCWRunnerEnd = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterCWRunnerEnd, InPolygon.VertexIndexes);

	}while (IterCWRunnerStart != OutCCWIterator);

	// adjust minimum monotone polygon for divide point
	if (bPolygonNeedClosed) // find intersection
	{
		std::vector<unsigned int>::iterator IterateCW  = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(InIterStart, InPolygon.VertexIndexes);
		std::vector<unsigned int>::iterator IterateCCW = GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(InIterStart, InPolygon.VertexIndexes);

		while (IterateCW != GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(OutCWIterator, InPolygon.VertexIndexes) &&
			IterateCCW != GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(OutCCWIterator, InPolygon.VertexIndexes))
		{
			if (IsPointInsidePoligon(InVertices, InPolygon, InVertices[*OutDividePoint], IterateCW, IterateCCW))
			{
				OutCWIterator = GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterateCW, InPolygon.VertexIndexes);
				OutCCWIterator = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterateCCW, InPolygon.VertexIndexes);
				break;
			}

			IterateCW = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterateCW, InPolygon.VertexIndexes);
			IterateCCW = GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterateCCW, InPolygon.VertexIndexes);
		}
	}

	//  	if (!IsPolygonConvex(SourceSplineList, OutCWIterator, OutCCWIterator, Normal))
	//  	{
	//  		while( OutCWIterator != InIterStart && OutCCWIterator != InIterStart )
	//  		{
	//  			OutCWIterator = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCWIterator, SourceSplineList);
	//  			OutCCWIterator = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCCWIterator, SourceSplineList);
	//  
	//  			if (IsPolygonConvex(SourceSplineList, OutCWIterator, OutCCWIterator, Normal))
	//  			{
	//  				bPolygonNeedClosed = true;
	//  				break;
	//  			}
	//  		}
	//  	}

	return bPolygonNeedClosed;
}

//----------------------------------------------------------------------------------------------
bool GetMonotonePolygon(TVecIndexedPoint2f &SourceSplineList,
						TVecIndexedPoint2fIterator& InIterStart,
						TVecIndexedPoint2fIterator& OutCWIterator,
						TVecIndexedPoint2fIterator& OutCCWIterator,
						TVecIndexedPoint2fIterator& OutDividePoint)
{
	OutCWIterator = GetIteratorNext(SourceSplineList,  InIterStart);
	OutCCWIterator = GetIteratorPrev(SourceSplineList, InIterStart);

	do 
	{
		bool bCWEar = IsEarTest(SourceSplineList, OutCWIterator);
		bool bCCWEar = IsEarTest(SourceSplineList, OutCCWIterator);

		if (bCWEar && bCCWEar)
		{
			if (MonotonePolygonCorrection(SourceSplineList, InIterStart, OutCWIterator, OutCCWIterator, OutDividePoint)){
				return true; // polygon need to be closed
			}
		}
		else
		{  
			MonotonePolygonCorrection(SourceSplineList, InIterStart, OutCWIterator, OutCCWIterator, OutDividePoint);
			return true; // polygon need to be closed
		}

		OutCWIterator = GetIteratorNext(SourceSplineList, OutCWIterator);
		OutCCWIterator = GetIteratorPrev(SourceSplineList, OutCCWIterator);

	}while (OutCWIterator != OutCCWIterator);

	return false;
}

//----------------------------------------------------------------------------------------------
bool GetMonotonePolygon(TVecSIndexedPoint3f &SourceSplineList, 
						TVecSIndexedPoint3fIter &InIterStart, 
						TVecSIndexedPoint3fIter &OutCWIter, 
						TVecSIndexedPoint3fIter &OutCCWIter, 
						TVecSIndexedPoint3fIter &OutDividePoint,
						const Vector &Normal)

{
 	OutCWIter = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(InIterStart, SourceSplineList);
 	OutCCWIter = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(InIterStart, SourceSplineList);
 
	// iterate clockwise & counterclockwise simultaneously sinse InIterStart point
 	do 
 	{
 		bool bCWEar = IsEarTest(SourceSplineList, OutCWIter, Normal);
 		bool bCCWEar = IsEarTest(SourceSplineList, OutCCWIter, Normal);
 
 		if (bCWEar && bCCWEar)
 		{
			if (MonotonePolygonCorrection(SourceSplineList, InIterStart, OutCWIter, OutCCWIter, OutDividePoint, Normal)){
				return true; // polygon need to be closed
			}
 		}
 		else
 		{
			MonotonePolygonCorrection(SourceSplineList, InIterStart, OutCWIter, OutCCWIter, OutDividePoint, Normal);
			return true; // polygon need to be closed
 		}

		OutCWIter = GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCWIter, SourceSplineList);
		OutCCWIter = GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(OutCCWIter, SourceSplineList);
 
 	}while (OutCWIter != OutCCWIter);

	return false;
}

//------------------------------------------------------------------------
bool GetMonotonePolygon(const TVecVectorList &InVertices,
						SPolygonFace &InPolygon,
						std::vector<unsigned int>::iterator &InIterStart, 
						std::vector<unsigned int>::iterator &OutCWIter, 
						std::vector<unsigned int>::iterator &OutCCWIter, 
						std::vector<unsigned int>::iterator &OutDividePoint)
{
	bool bCoplanar = true;
	bool bClosed = false; 

	OutCWIter = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(InIterStart, InPolygon.VertexIndexes);
	OutCCWIter = GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(InIterStart, InPolygon.VertexIndexes);

	if (OutCWIter == OutCCWIter){
		return false;
	}

	Vector Normal = (InVertices[*OutCCWIter] - InVertices[*InIterStart]).Cross(InVertices[*OutCWIter] - InVertices[*InIterStart]);
	Normal.normalize();

	// iterate clockwise & counterclockwise simultaneously since InIterStart point
	do 
	{
		// test triangles coplanar
		std::vector<unsigned int>::iterator IterNextCW = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(OutCWIter, InPolygon.VertexIndexes);
		std::vector<unsigned int>::iterator IterNextCCW = GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(OutCCWIter, InPolygon.VertexIndexes);

		if (OutCWIter != OutCCWIter)
		{
			Vector N_CW = (InVertices[*OutCCWIter] - InVertices[*OutCWIter]).Cross(InVertices[*IterNextCW] - InVertices[*OutCWIter]);
			Vector N_CCW = (InVertices[*IterNextCCW] - InVertices[*OutCWIter]).Cross(InVertices[*OutCWIter] - InVertices[*OutCCWIter]);

			N_CW.normalize();
			N_CCW.normalize();

			float dot0 = Normal.Dot(N_CW);
			float dot1 = Normal.Dot(N_CCW);

			if ((dot0 > 1.f - m_eps && dot0 < 1.f + m_eps) &&
				(dot1 > 1.f - m_eps && dot1 < 1.f + m_eps)) // strict coplanar test
			{
				bool bCWEar = IsEarTest(InVertices, InPolygon, OutCWIter);
				bool bCCWEar = IsEarTest(InVertices, InPolygon, OutCCWIter);

				if (bCWEar && bCCWEar)
				{
					if (MonotonePolygonCorrection(InVertices, InPolygon, InIterStart, OutCWIter, OutCCWIter, OutDividePoint)){
						return true; // polygon need to be closed
					}
				}
				else
				{
					MonotonePolygonCorrection(InVertices, InPolygon, InIterStart, OutCWIter, OutCCWIter, OutDividePoint);
					return true; // polygon need to be closed
				}
			}
			else
			{
				bCoplanar = false;
			}
		}

		OutCWIter = IterNextCW;
		OutCCWIter = IterNextCCW;

		bClosed = OutCWIter == OutCCWIter;

	}while (!bClosed && bCoplanar);

	return false;
}

//----------------------------------------------------------------------------------------------
void BuildFacePatches(const TVecVectorList &InVertices,
					  SPolygonFace &InPolygon,
					  std::vector<unsigned int>::iterator &InIterStartCW,
					  std::vector<unsigned int>::iterator &InIterStartCCW,
					  std::vector<SPolygonSubFace> &OutPatches)
{
	std::vector<unsigned int>::iterator IterStart, CWIter, CCWIter, DividePoint;

	IterStart = InIterStartCW;

	if (InIterStartCW == InIterStartCCW)
	{
		CWIter = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterStart, InPolygon.VertexIndexes);
		CCWIter = GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterStart, InPolygon.VertexIndexes);
	}
	else
	{

	}
	
	if (CWIter == CCWIter){
		return;
	}

	Vector Normal = (InVertices[*CCWIter] - InVertices[*IterStart]).Cross(InVertices[*CWIter] - InVertices[*IterStart]);
	Normal.normalize();

	bool bCoplanar = true;

	SPolygonSubFace SubFace;
	SubFace.Normal = Normal;
	SubFace.SubFaceIndexes.push_back(*IterStart);

	// iterate clockwise & counterclockwise simultaneously since InIterStart point
	do 
	{
		// test triangles coplanar
		std::vector<unsigned int>::iterator IterNextCW = GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(CWIter, InPolygon.VertexIndexes);
		std::vector<unsigned int>::iterator IterNextCCW = GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(CCWIter, InPolygon.VertexIndexes);

		if (IterNextCW != IterNextCCW)
		{
			Vector N_CW = (InVertices[*CCWIter] - InVertices[*CWIter]).Cross(InVertices[*IterNextCW] - InVertices[*CWIter]);
			Vector N_CCW = (InVertices[*IterNextCCW] - InVertices[*CWIter]).Cross(InVertices[*CWIter] - InVertices[*CCWIter]);

			N_CW.normalize();
			N_CCW.normalize();

			float dot0 = Normal.Dot(N_CW);
			float dot1 = Normal.Dot(N_CCW);

			SubFace.SubFaceIndexes.push_back(*CWIter);
			SubFace.SubFaceIndexes.insert(SubFace.SubFaceIndexes.begin(), *CCWIter); //@TODO not optimal change container

			if ((dot0 < 1.f - m_eps || dot0 > 1.f + m_eps) || // strict coplanar test
				(dot1 < 1.f - m_eps || dot1 > 1.f + m_eps)) 
			{
				BuildFacePatches(InVertices, InPolygon, CWIter, CCWIter, OutPatches);
				break;
			}
		}

		CWIter = IterNextCW;
		CCWIter = IterNextCCW;

	}while (CWIter != CCWIter);

	OutPatches.push_back(SubFace);
}

//----------------------------------------------------------------------------------------------
bool Tesselate(TVecIndexedPoint2f &SourceSplineList, std::vector<size_t> &OutIndexList)
{
	TVecIndexedPoint2fIterator MinLeftPoint, CWIterator, CCWIterator, DividePoint;

	if (GetMinLeftPoint(SourceSplineList, MinLeftPoint)) // found minimum left bound point
	{
		DividePoint = MinLeftPoint;

		if (GetMonotonePolygon(SourceSplineList, MinLeftPoint, CWIterator, CCWIterator, DividePoint))
		{
			if (DividePoint != MinLeftPoint) // need to slice
			{
				TVecIndexedPoint2f SplineList1, SplineList2;
				Split2Poly(SourceSplineList, MinLeftPoint, DividePoint, SplineList1, SplineList2);

				Tesselate(SplineList1, OutIndexList);
				Tesselate(SplineList2, OutIndexList);
				return false;
			}
			else
			{
				TVecIndexedPoint2f SplineList1, SplineList2;
				Split2Poly(SourceSplineList, CWIterator, CCWIterator, SplineList1, SplineList2);

				Triangulate(SourceSplineList, MinLeftPoint, CWIterator, CCWIterator, OutIndexList);
				Tesselate(SplineList1, OutIndexList);
				return true;
			}
		}
		else
		{
			Triangulate(SourceSplineList, MinLeftPoint, CWIterator, CCWIterator, OutIndexList);
			return false;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------------------------
// bool Tesselate(TVecSIndexedPoint3f &SourceSplineList, std::vector<size_t> &OutIndexList, const Vector &Normal)
// {
// 	TVecSIndexedPoint3fIter IterBBox[6], CWIterator, CCWIterator, DividePoint;
// 
// 	if (GetPointCloudBounds(SourceSplineList, IterBBox[0],	IterBBox[1], IterBBox[2], IterBBox[3], IterBBox[4],	IterBBox[5]))
// 	{
// 		DividePoint = IterBBox[0]; // minimum x point
// 
// 		if (GetMonotonePolygon(SourceSplineList, IterBBox[0], CWIterator, CCWIterator, DividePoint, Normal))
// 		{
// 			if (DividePoint != IterBBox[0]) // need to slice
// 			{
// 				TVecSIndexedPoint3f SplineList1, SplineList2;
// 				Split2Poly(SourceSplineList, IterBBox[0], DividePoint, SplineList1, SplineList2);
// 
// 				Tesselate(SplineList1, OutIndexList, Normal);
// 				Tesselate(SplineList2, OutIndexList, Normal);
// 				return false;
// 			}
// 			else
// 			{
// 				TVecSIndexedPoint3f SplineList1, SplineList2;
// 
// 				Split2Poly(SourceSplineList, CWIterator, CCWIterator, SplineList1, SplineList2);
// 				Tesselate(SplineList1, OutIndexList, Normal);
// 
// 				Triangulate(SourceSplineList, IterBBox[0], CWIterator, CCWIterator, OutIndexList);
// 				return true;
// 			}
// 		}
// 		else
// 		{
// 			Triangulate(SourceSplineList, IterBBox[0], CWIterator, CCWIterator, OutIndexList);
// 			return false;
// 		}
// 	}
// 
// 	return false;
// }

//----------------------------------------------------------------------------------------------
bool Tesselate(const TVecVectorList &InVertices, SPolygonFace &InPolygon, std::vector<unsigned int> &OutTriangleIndexes)
{
	bool bResult = false;

	std::vector<unsigned int>::iterator IteratorMin, CWIterator, CCWIterator, DividePoint;

	if (GetMinPoint(InVertices, InPolygon, IteratorMin)) // found minimum left bound point
	{
		DividePoint = IteratorMin;

		if (GetMonotonePolygon(InVertices, InPolygon, IteratorMin, CWIterator, CCWIterator, DividePoint))
		{
			if (DividePoint != IteratorMin) // need to slice
			{
				SPolygonFace SubPoly0, SubPoly1;

				SubPoly0.Normal = SubPoly1.Normal = InPolygon.Normal;

				Split2Poly(InPolygon, IteratorMin, DividePoint, SubPoly0.VertexIndexes, SubPoly1.VertexIndexes);

				Tesselate(InVertices, SubPoly0, OutTriangleIndexes);
				Tesselate(InVertices, SubPoly1, OutTriangleIndexes);

				return false;
			}
			else
			{
				SPolygonFace SubPoly0, SubPoly1;

				SubPoly0.Normal = SubPoly1.Normal = InPolygon.Normal;

				Split2Poly(InPolygon, CWIterator, CCWIterator, SubPoly0.VertexIndexes, SubPoly1.VertexIndexes);

				Tesselate(InVertices, SubPoly0, OutTriangleIndexes);

				Triangulate(InPolygon, IteratorMin, CWIterator, CCWIterator, OutTriangleIndexes);

				return true;
			}
		}
		else
		{
			Triangulate(InPolygon, IteratorMin, CWIterator, CCWIterator, OutTriangleIndexes);
			return false;
		}
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
void Triangulate(TVecIndexedPoint2f &SourceSplineList,
				 TVecIndexedPoint2fIterator IterStart,
				 TVecIndexedPoint2fIterator &CWIterator,
				 TVecIndexedPoint2fIterator &CCWIterator,
				 std::vector<size_t>		&OutIndexList)
{
	if (CWIterator != CCWIterator) // closing
	{
		TVecIndexedPoint2fIterator BeginCCIterator(CWIterator);
		TVecIndexedPoint2fIterator EndCCIterator(GetIteratorPrev(SourceSplineList, BeginCCIterator));

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutIndexList.push_back(BeginCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(EndCCIterator->Index);
#else
			OutIndexList.push_back(EndCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(BeginCCIterator->Index);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIteratorPrev(SourceSplineList, BeginCCIterator);
		}

		// index list
#ifdef CULLMODE_CCW
		OutIndexList.push_back(CCWIterator->Index);
		OutIndexList.push_back(IterStart->Index);
		OutIndexList.push_back(CWIterator->Index);
#else
		OutIndexList.push_back(CWIterator->Index);
		OutIndexList.push_back(IterStart->Index);
		OutIndexList.push_back(CCWIterator->Index);
#endif
		BeginCCIterator = CCWIterator; 
		EndCCIterator	= GetIteratorNext(SourceSplineList, BeginCCIterator);

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutIndexList.push_back(EndCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(BeginCCIterator->Index);
#else
			OutIndexList.push_back(BeginCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(EndCCIterator->Index);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIteratorNext(SourceSplineList, EndCCIterator);
		}
	}
	else
	{
		TVecIndexedPoint2fIterator BeginCCIterator(GetIteratorNext(SourceSplineList, IterStart));
		TVecIndexedPoint2fIterator EndCCIterator(GetIteratorNext(SourceSplineList, BeginCCIterator));

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutIndexList.push_back(EndCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(BeginCCIterator->Index);
#else
			OutIndexList.push_back(BeginCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(EndCCIterator->Index);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIteratorNext(SourceSplineList, BeginCCIterator);
		}
	}
}

//----------------------------------------------------------------------------------------------
void Triangulate(TVecSIndexedPoint3f &SourceSplineList,
				 TVecSIndexedPoint3fIter &IterStart, 
				 TVecSIndexedPoint3fIter &CWIterator,
				 TVecSIndexedPoint3fIter &CCWIterator,
				 std::vector<size_t>		   &OutIndexList)
{
	if (CWIterator != CCWIterator) // closing
	{
		TVecSIndexedPoint3fIter BeginCCIterator(CWIterator);
		TVecSIndexedPoint3fIter EndCCIterator(GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(BeginCCIterator, SourceSplineList));

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutIndexList.push_back(BeginCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(EndCCIterator->Index);
#else
			OutIndexList.push_back(EndCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(BeginCCIterator->Index);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIterPrev< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(BeginCCIterator, SourceSplineList);
		}

		// index list
#ifdef CULLMODE_CCW
		OutIndexList.push_back(CCWIterator->Index);
		OutIndexList.push_back(IterStart->Index);
		OutIndexList.push_back(CWIterator->Index);
#else
		OutIndexList.push_back(CWIterator->Index);
		OutIndexList.push_back(IterStart->Index);
		OutIndexList.push_back(CCWIterator->Index);
#endif
		BeginCCIterator = CCWIterator; 
		EndCCIterator	= GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(BeginCCIterator, SourceSplineList);

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutIndexList.push_back(EndCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(BeginCCIterator->Index);
#else
			OutIndexList.push_back(BeginCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(EndCCIterator->Index);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(EndCCIterator, SourceSplineList);
		}
	}
	else
	{
		TVecSIndexedPoint3fIter BeginCCIterator(GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(IterStart, SourceSplineList));
		TVecSIndexedPoint3fIter EndCCIterator(GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(BeginCCIterator, SourceSplineList));

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutIndexList.push_back(EndCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(BeginCCIterator->Index);
#else
			OutIndexList.push_back(BeginCCIterator->Index);
			OutIndexList.push_back(IterStart->Index);
			OutIndexList.push_back(EndCCIterator->Index);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIterNext< SIndexedPoint, TVecSIndexedPoint3f, TVecSIndexedPoint3fIter >(BeginCCIterator, SourceSplineList);
		}
	}
}

//----------------------------------------------------------------------------------------------
void Triangulate(SPolygonFace &InOutPolygon,
				 std::vector<unsigned int>::iterator &IterStart, 
				 std::vector<unsigned int>::iterator &CWIterator,
				 std::vector<unsigned int>::iterator &CCWIterator,
				 std::vector<unsigned int> &OutTriangleIndexes)
{
	if (CWIterator != CCWIterator) // closing
	{
		std::vector<unsigned int>::iterator BeginCCIterator(CWIterator);
		std::vector<unsigned int>::iterator EndCCIterator(GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(BeginCCIterator, InOutPolygon.VertexIndexes));

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutTriangleIndexes.push_back(*BeginCCIterator);
			OutTriangleIndexes.push_back(*IterStart);
			OutTriangleIndexes.push_back(*EndCCIterator);
#else
			OutTriangleIndexes.push_back(*EndCCIterator);
			OutTriangleIndexes.push_back(*IterStart);
			OutTriangleIndexes.push_back(*BeginCCIterator);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIterPrev< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(BeginCCIterator, InOutPolygon.VertexIndexes);
		}

		// index list
#ifdef CULLMODE_CCW
		OutTriangleIndexes.push_back(*CCWIterator);
		OutTriangleIndexes.push_back(*IterStart);
		OutTriangleIndexes.push_back(*CWIterator);
#else
		OutTriangleIndexes.push_back(*CWIterator);
		OutTriangleIndexes.push_back(*IterStart);
		OutTriangleIndexes.push_back(*CCWIterator);
#endif
		BeginCCIterator = CCWIterator; 
		EndCCIterator	= GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(BeginCCIterator, InOutPolygon.VertexIndexes);

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutTriangleIndexes.push_back(*EndCCIterator);
			OutTriangleIndexes.push_back(*IterStart);
			OutTriangleIndexes.push_back(*BeginCCIterator);
#else
			OutTriangleIndexes.push_back(*BeginCCIterator);
			OutTriangleIndexes.push_back(*IterStart);
			OutTriangleIndexes.push_back(*EndCCIterator);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(EndCCIterator, InOutPolygon.VertexIndexes);
		}
	}
	else
	{
		std::vector<unsigned int>::iterator BeginCCIterator(GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(IterStart, InOutPolygon.VertexIndexes));
		std::vector<unsigned int>::iterator EndCCIterator(GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(BeginCCIterator, InOutPolygon.VertexIndexes));

		while (EndCCIterator != IterStart)
		{
			// index list
#ifdef CULLMODE_CCW
			OutTriangleIndexes.push_back(*EndCCIterator);
			OutTriangleIndexes.push_back(*IterStart);
			OutTriangleIndexes.push_back(*BeginCCIterator);
#else
			OutTriangleIndexes.push_back(*BeginCCIterator);
			OutTriangleIndexes.push_back(*IterStart);
			OutTriangleIndexes.push_back(*EndCCIterator);
#endif
			BeginCCIterator = EndCCIterator;
			EndCCIterator	= GetIterNext< unsigned int, std::vector<unsigned int>, std::vector<unsigned int>::iterator >(BeginCCIterator, InOutPolygon.VertexIndexes);
		}
	}
}