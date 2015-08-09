#include "coresdkafx.h"
#include "../D3DDrv/D3DMesh.h"
#include "Comp_MeshNode.h"

REGISTER_CLASS_A(SplineAgregator, UIScreenObject)
	new PropertyUINT("Detail", (BYTE*)&((SplineAgregator*)NULL)->Detail - (BYTE*)NULL, "SplineAgregator", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyUINT("Color", (BYTE*)&((SplineAgregator*)NULL)->Color - (BYTE*)NULL, "SplineAgregator", "Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyFLOAT("BorderSize",	(BYTE*)&((SplineAgregator*)NULL)->BorderSize - (BYTE*)NULL,	"SplineAgregator", "Value",	READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyString("DiffuseTex", (BYTE*)&((SplineAgregator*)NULL)->DiffuseTex - (BYTE*)NULL, "SplineAgregator",	"Value", READ_WRITE, CTRL_EDIT,	SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyBOOL("bBorder",	(BYTE*)&((SplineAgregator*)NULL)->bBorder - (BYTE*)NULL, "SplineAgregator",	"Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyUINT("BorderColor",	(BYTE*)&((SplineAgregator*)NULL)->BorderColor - (BYTE*)NULL, "SplineAgregator",	"Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(SplineAgregator, UIScreenObject)	

//----------------------------------------------------------------------------------------------
SplineAgregator::SplineAgregator(const CObjectAbstract * Parent)
: Super(Parent)
, AdaptiveDetail(false)
, Detail(1)	
, Color(0xffffffff)
, SplineClosed(true)
, bBorder(false)
, BorderSize(5.f)
, BorderColor(0xffffffff)
, InternalPreUpdateShiftTranslate(0.f, 0.f)
, MeshComponent(0)
, DiffuseTex("")
{
	 NEW_OBJECT_TRANSIENT_CHILD(MeshComponent, Comp_StaticMesh, "SplineAgregator_MeshComponent", this);
	 NEW_OBJECT_TRANSIENT_CHILD(BorderMeshComponent, Comp_StaticMesh, "SplineAgregator_MeshComponent", this);

	 NEW_OBJECT_TRANSIENT_CHILD(MaterialEffect, Comp_MaterialEffect, "UICompDrawImage_MaterialEffect", this);
	 NEW_OBJECT_TRANSIENT_CHILD(MaterialEffectBorder, Comp_MaterialEffect, "UICompDrawImage_MaterialEffect", this);
}

//----------------------------------------------------------------------------------------------
SplineAgregator::SplineAgregator(const SplineAgregator& Source)
: Super(Source)
{
	if (this != &Source)
	{
		AdaptiveDetail	= Source.AdaptiveDetail;
		Detail			= Source.Detail;
		Color			= Source.Color;
		SplineClosed	= Source.SplineClosed;
		bBorder			= Source.bBorder;
		BorderSize		= Source.BorderSize;
		BorderColor		= Source.BorderColor;
		InternalPreUpdateShiftTranslate = Source.InternalPreUpdateShiftTranslate;
		DiffuseTex		= Source.DiffuseTex;

		MeshComponent		= CLONE_OBJECT(Comp_StaticMesh, Source.MeshComponent, "SplineAgregator_MeshComponent", this);
		BorderMeshComponent = CLONE_OBJECT(Comp_StaticMesh, Source.BorderMeshComponent, "SplineAgregator_MeshComponent", this);

		MaterialEffect = CLONE_OBJECT(Comp_MaterialEffect, Source.MaterialEffect, "UICompDrawImage_MaterialEffect", this);
		MaterialEffectBorder = CLONE_OBJECT(Comp_MaterialEffect, Source.MaterialEffectBorder, "UICompDrawImage_MaterialEffect", this);
	}
}

//----------------------------------------------------------------------------------------------
SplineAgregator::~SplineAgregator()
{

}

//----------------------------------------------------------------------------------------------
/*void SplineAgregator::RegisterProperties()
{
	Super::RegisterProperties();

//	REGISTER_PROPERTY_COMMAND("Convert to Editable Mesh", this, SplineAgregator, SplineAgregator::ConvertToEditableMesh);
}*/

//----------------------------------------------------------------------------------------------
void SplineAgregator::Initialize()
{	
	Super::Initialize();
	Rebuild();
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "Detail") || 
		!strcmp(PropertyName, "Color") ||
		!strcmp(PropertyName, "BorderSize") ||
		!strcmp(PropertyName, "DiffuseTex") ||
		!strcmp(PropertyName, "bBorder") ||
		!strcmp(PropertyName, "BorderColor"))
	{
		Rebuild();
		//RebuildTransform();
	}
}

//----------------------------------------------------------------------------------------------
bool SplineAgregator::HitTest(const Vector2f &Position)
{
/*	Vector2f LocalPosition = Position - WorldMatrixTransform.t2;

	// calc projection to local axis
	Vector2f AxisX = WorldMatrixTransform.__row0;
	Vector2f AxisY = WorldMatrixTransform.__row1;

	AxisX.Normalize();
	AxisY.Normalize();

	Matrix IObjectMatrix;	
	invert(IObjectMatrix, WorldMatrixTransform);
	Vector2f TPosition(LocalPosition.Dot(AxisX) * IObjectMatrix.__row0.Length(),
					   LocalPosition.Dot(AxisY) * IObjectMatrix.__row1.Length());

	size_t num_triangles = IndexList.size() / 3;

	if (num_triangles >= 1)
	{
		std::vector<size_t>::iterator Index0 = IndexList.begin();
		std::vector<size_t>::iterator Index1 = Index0 + 1;
		std::vector<size_t>::iterator Index2 = Index1 + 1;

		while (1)
		{
			TVecIndexedPoint2fIterator Iter0 = SplineList.begin();
			TVecIndexedPoint2fIterator Iter1 = SplineList.begin();
			TVecIndexedPoint2fIterator Iter2 = SplineList.begin();

			std::advance(Iter0, *Index0);
			std::advance(Iter1, *Index1);
			std::advance(Iter2, *Index2);

			if (IntersectRayWithTriangle(TPosition, Iter0->Point, Iter1->Point, Iter2->Point)){
				return true;
			}

			Index0 += 3;
			if (Index0 == IndexList.end()){	
				break;
			}

			Index1 += 3;
			Index2 += 3;
		}
	}*/
	return false;
}

//----------------------------------------------------------------------------------------------
/** review this code */
Vector2f SplineAgregator::GetTransformedSize_() const
{
	Vector2f out_size;

	/*if (GetParent() && static_cast<UIScreenObject*>(GetParent()))
	{
		UIScreenObject * Parent = static_cast<UIScreenObject*>(GetParent());
		out_size = Parent->GetTransformedSize();
	}*/
	return out_size;
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::DoDraw()
{
/*	GetRenderComponent()->PushToRenderStack(new RenderChunk<Comp_StaticMesh, CRenderObject>(&CRenderObject::Render, MeshComponent));

 	if (bBorder && BorderSize != 0.f){
 		GetRenderComponent()->PushToRenderStack(new RenderChunk<Comp_StaticMesh, CRenderObject>(&CRenderObject::Render, BorderMeshComponent));
 	}

 	if (IsFocused() || ExtraSelected){
 		RenderSpline(WorldMatrixTransform);
 	}*/
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::DrawBounds() const
{
/*	if (GetAppMain()->GetObjectBoundsVisible())
	{
		GetRenderComponent()->DrawBounds2f(WorldMatrixTransform, Bounds, 0xff0000ff);
	}
	if (GetAppMain()->GetSparitalSubdivisionVisible())
	{
		Matrix I;
		GetRenderComponent()->DrawBounds2f(I, CompositeBounds, 0xff00ff00);
	}*/
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::RenderSpline(const Matrix &WTM)
{
	Matrix IWTM;
	invert(IWTM, WTM);

	TVecIndexedPoint2fIterator Iter(SplineList.begin());
	while (Iter != SplineList.end())
	{
		TVecIndexedPoint2fIterator IterNext(GetIteratorNext(SplineList, Iter));
		GetRenderComponent()->DrawLine(WTM.t2 + transform_coord(IterNext->Point, IWTM),
									   WTM.t2 + transform_coord(Iter->Point, IWTM), COLOR_WHITE);
		++Iter;
	}
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::ResetSpline()
{
	// empty data
	IndexList.clear();
	SplineList.clear();
	TriangleBorderList.clear();
	SplineBorderList.clear();
	IndexBorderList.clear();
}

//----------------------------------------------------------------------------------------------
bool SplineAgregator::Rebuild()
{
	ResetSpline();

	BuildSplineList(SplineList);

	Tesselate(SplineList, IndexList);

	BuildBorder(SplineList);

	InternalPreUpdateShiftTranslate.x = 
	InternalPreUpdateShiftTranslate.y = 0.f;
	
	Super::ApplyModificators();

	DoRebuildMesh();
	
	return true;
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::DoRebuildMesh()
{
	DoRebuildBorderMesh();

	if (MeshComponent && SplineList.size() > 0 && IndexList.size() > 0)
	{
		Comp_MeshNode * pMeshNode = NULL;
		NEW_OBJECT_TRANSIENT_CHILD(pMeshNode, Comp_MeshNode, "MeshNode", MeshComponent);
		pMeshNode->Effect = MaterialEffect->pMaterialEffect;

		unsigned int NumVertices = (unsigned int)SplineList.size();
		unsigned int NumFaces = (unsigned int)IndexList.size() / 3;

		Vector * vertices = new Vector[NumVertices];	// array of vertices
		Vector * normals = new Vector[NumVertices];		// array of normals
		Vector4f * colors = new Vector4f[NumVertices];	// array of colors (vertex colors)
		Vector2f * texcoords = new Vector2f[NumVertices];
		unsigned int * Indices = new unsigned int[NumFaces * 3];

		byte a = (Color & 0xFF000000) >> 24;
		byte r = (Color & 0x00FF0000) >> 16;
		byte g = (Color & 0x0000FF00) >> 8;
		byte b = (Color & 0x000000FF);

		for (unsigned int Index = 0; Index < NumVertices; ++Index)
		{
			vertices[Index] = Vector(SplineList[Index].Point.x, SplineList[Index].Point.y, 0.f);
			texcoords[Index].x = SplineList[Index].UV.x;
			texcoords[Index].y = SplineList[Index].UV.y;
			colors[Index] = Vector4f(r / 255.f, g / 255.f, b / 255.f, 1.f);
		}

		for (unsigned int Index = 0; Index < IndexList.size(); ++Index){
			Indices[Index] = IndexList[Index];
		}

		D3DMesh & dx_mesh = *pMeshNode->Mesh;

		// generate unique
		//dx_mesh.SetSubMeshNode("##AutoGenerated_SplineInstance", true);

		unsigned int numDistinctBones = 0;
		std::vector<int> VecBoneRemapper;
/*
		dx_mesh.InitializeVertexData((float*)vertices,
									(float*)normals,
									(float*)colors,
									NULL,
									NULL,
									Indices,
									NumVertices,
									NumFaces,
									(float*)texcoords,
									1,
									false,
									NULL,
									0,
									VecBoneRemapper,
									numDistinctBones);
*/
		std::string sFilename;
		if (FindFile(GetDiffuseTex().c_str(), &sFilename)){
			MaterialEffect->LoadDiffuseMap(sFilename.c_str());
		}
		MaterialEffect->SetReference("dx9_hlsl_bumpmapping.fx");
		MaterialEffect->SetTehnique("DiffuseOnly");

		pMeshNode->Initialize();

		delete [] texcoords;
		delete [] vertices;
		delete [] normals;
		delete [] colors;
		delete [] Indices;
	}
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::DoRebuildBorderMesh()
{
	if (BorderMeshComponent && SplineBorderList.size() > 0)
	{
		Comp_MeshNode * pMeshNode = NULL;
		NEW_OBJECT_TRANSIENT_CHILD(pMeshNode, Comp_MeshNode, "MeshNode", BorderMeshComponent);
		pMeshNode->Effect = MaterialEffectBorder->pMaterialEffect;

		unsigned int NumVertices = (unsigned int)SplineBorderList.size();
		unsigned int NumFaces = (unsigned int)SplineBorderList.size() / 3;

		Vector * vertices = new Vector[NumVertices];	// array of vertices
		Vector * normals = new Vector[NumVertices];		// array of normals
		Vector4f * colors = new Vector4f[NumVertices];	// array of colors (vertex colors)
		Vector2f * texcoords = new Vector2f[NumVertices];
		unsigned int * Indices = new unsigned int[NumVertices];

		byte a = (BorderColor & 0xFF000000) >> 24;
		byte r = (BorderColor & 0x00FF0000) >> 16;
		byte g = (BorderColor & 0x0000FF00) >> 8;
		byte b = (BorderColor & 0x000000FF);

		for (unsigned int Index = 0; Index < NumVertices; ++Index)
		{
			vertices[Index] = Vector(SplineBorderList[Index].Point.x, SplineBorderList[Index].Point.y, 0.f);
			texcoords[Index].x = SplineBorderList[Index].UV.x;
			texcoords[Index].y = SplineBorderList[Index].UV.y;
			colors[Index] = Vector4f(r / 255.f, g / 255.f, b / 255.f, 1.f);

			Indices[Index] = (unsigned short)Index;
		}

		D3DMesh & dx_mesh = *pMeshNode->Mesh;

		// generate unique
		//dx_mesh.GetInterface()->GetSubMeshNode("##AutoGenerated_SplineInstance", true);

		unsigned int numDistinctBones = 0;
		std::vector<int> VecBoneRemapper;
/*
		dx_mesh.InitializeVertexData((float*)vertices,
									(float*)normals,
									(float*)colors,
									NULL,
									NULL,
									Indices,
									NumVertices,
									NumFaces,
									(float*)texcoords,
									1,
									false,
									NULL,
									0,
									VecBoneRemapper,
									numDistinctBones);
*/
		//pMeshNode->MaterialEffect->LoadDiffuseMap(GetDiffuseTex());
		MaterialEffectBorder->SetReference("dx9_hlsl_fx_simple.fx");
		MaterialEffectBorder->SetTehnique("Technique_DiffuseVC");
		pMeshNode->Initialize();

		delete [] texcoords;
		delete [] vertices;
		delete [] normals;
		delete [] colors;
		delete [] Indices;
	}
}

//----------------------------------------------------------------------------------------------
bool SplineAgregator::GetPointByIndex(size_t Index, Vector2f & OutValue) const
{
	bool bResult = false; 

	if (Index >= 0 && Index < SplineList.size())
	{
		int LocalCounter = 0;

		TVecIndexedPoint2fConstIterator Iter = SplineList.begin();

		while (Iter != SplineList.end() && !bResult)
		{
			if (LocalCounter == Index)
			{
				OutValue = Iter->Point;
				bResult = true;
			}
			LocalCounter++;
			Iter++;
		}
	}
	return bResult; 
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::BuildSplineList(TVecIndexedPoint2f& OutSpline)
{
/*	TVecActorChildIterator  P0, P1, P2, P3;

	OutSpline.clear();

	P1 = GetChildIteratorNext(ChildNodes.end(), "Point2D");
	P2 = GetChildIteratorNext(P1, "Point2D");
	P3 = GetChildIteratorNext(P2, "Point2D");
	P0 = GetChildIteratorPrev(P1, "Point2D");

	if (P0 == ChildNodes.end() || P1 == ChildNodes.end() ||
		P2 == ChildNodes.end() ||	P3 == ChildNodes.end()){
		return;
	}

	do
	{
		Vector VP0 = (*P0)->m_LTM.t;
		Vector VP1 = (*P1)->m_LTM.t;
		Vector VP2 = (*P2)->m_LTM.t;
		Vector VP3 = (*P3)->m_LTM.t;

		for (size_t Index = 0; Index < Detail; ++Index)
		{
			Vector Result;
			Catmull_Rom_Spline(VP0, VP1, VP2, VP3, (float)Index / (float)Detail, Result);

			IndexedPoint2f Point(Vector2f(Result.x, Result.y), Vector2f(0.f, 0.f), (unsigned int)OutSpline.size());
			OutSpline.push_back(Point);
		}

		P0 = GetChildIteratorNext(P0, "Point2D");
		P1 = GetChildIteratorNext(P1, "Point2D");
		P2 = GetChildIteratorNext(P2, "Point2D");
		P3 = GetChildIteratorNext(P3, "Point2D");

	}while (P1 != GetChildIteratorNext(ChildNodes.end(), "Point2D"));*/
}

//----------------------------------------------------------------------------------------------
/** build border triangle list*/
bool SplineAgregator::BuildBorder(TVecIndexedPoint2f &SourceSpline)
{
	TVecIndexedPoint2fIterator  P0, P1, P2, P3;
	unsigned int Index = 0;

	P0 = GetIteratorPrev(SourceSpline, SourceSpline.begin());
	P1 = SourceSpline.begin();
	P2 = GetIteratorNext(SourceSpline, P1);
	P3 = GetIteratorNext(SourceSpline, P2);

	if (P0 == SourceSpline.end() || P1 == SourceSpline.end() ||
		P2 == SourceSpline.end() ||	P3 == SourceSpline.end()){
		return false;
	}

	do
	{
		Vector2f NormalPrev = GetSegmentNormal(P0->Point, P1->Point) * BorderSize; 
		Vector2f Normal     = GetSegmentNormal(P1->Point, P2->Point) * BorderSize;
		Vector2f NormalNext = GetSegmentNormal(P2->Point, P3->Point) * BorderSize; 

		bool bEar0 = IsEarTest(SourceSpline, P1);
		bool bEar1 = IsEarTest(SourceSpline, P2);

		Vector2f MedianP1 = (Normal + NormalPrev);
		Vector2f MedianP2 = (Normal + NormalNext);

		MedianP1.Normalize();
		MedianP2.Normalize();

		MedianP1 = MedianP1 * BorderSize;
		MedianP2 = MedianP2 * BorderSize;

		if (bEar0 && bEar1)
		{
			AddTriangle(SplineBorderList, P2->Point + Normal,	P1->Point, P2->Point );
			AddTriangle(SplineBorderList, P1->Point + Normal,	P1->Point, P2->Point + Normal);
			AddTriangle(SplineBorderList, P1->Point + MedianP1,	P1->Point, P1->Point + Normal);
			AddTriangle(SplineBorderList, P2->Point + Normal,   P2->Point, P2->Point + MedianP2);
		}
		else if (bEar0 && !bEar1)
		{
			AddTriangle(SplineBorderList, P2->Point + MedianP2, P1->Point, P2->Point);
			AddTriangle(SplineBorderList, P1->Point + Normal, P1->Point, P2->Point + MedianP2);
			AddTriangle(SplineBorderList, P1->Point + MedianP1, P1->Point, P1->Point + Normal);
		}
		else if (!bEar0 && bEar1)
		{
			AddTriangle(SplineBorderList, P2->Point + Normal, P1->Point, P2->Point);
			AddTriangle(SplineBorderList, (*P1).Point + MedianP1, (*P1).Point, P2->Point + Normal);
			AddTriangle(SplineBorderList, P2->Point + Normal, P2->Point, P2->Point + MedianP2);
		}
		else if (!bEar0 && !bEar1)
		{
			AddTriangle(SplineBorderList, P2->Point + MedianP2, P1->Point, P2->Point);
			AddTriangle(SplineBorderList, P1->Point + MedianP1, P1->Point, P2->Point + MedianP2);
		}

		P0 = GetIteratorNext(SourceSpline, P0);
		P1 = GetIteratorNext(SourceSpline, P1);
		P2 = GetIteratorNext(SourceSpline, P2);
		P3 = GetIteratorNext(SourceSpline, P3);

	}while (P1 != SourceSpline.begin());
	return true;
}

//----------------------------------------------------------------------------------------------
void SplineAgregator::AddTriangle(TVecIndexedPoint2f &VB, const Vector2f& P0, const Vector2f& P1, const Vector2f& P2)
{
	IndexedPoint2f IP0, IP1, IP2;

	IP0.Point = P0;
	IP1.Point = P1;
	IP2.Point = P2;

	VB.push_back(IP0);
	VB.push_back(IP2);
	VB.push_back(IP1);
}

//----------------------------------------------------------------------------------------------
/** update UV texture coords */
void SplineAgregator::UpdateCoordsUV(const Matrix &LTM, const Vector2f& SourceSize)
{
/*	float ExtendX = Bounds.bound_max.x - Bounds.bound_min.x;
	float ExtendY = Bounds.bound_max.y - Bounds.bound_min.y;

	TVecIndexedPoint2fIterator Iter(SplineList.begin());
	while (Iter != SplineList.end())
	{
		Vector2f UVP;
		transform_coord(UVP, Iter->Point - LTM.t2, LTM);

		Iter->UV.x = UVP.x / SourceSize.x;
		Iter->UV.y = UVP.y / SourceSize.y;
		Iter++;
	}*/
}

//----------------------------------------------------------------------------------------------
bool SplineAgregator::InsertPoint(const Vector2f& MousePos, const UIScreenObject* Inserter)
{
/*	ViewportInterface * VInterface = ActorAllocator::GetViewportInterface(this);

	if (VInterface)
	{
		Matrix IWTM;
		invert(IWTM, Inserter->GetTransformedWTM_());
		Vector2f ViewAxisX = (IWTM.__row0 * Inserter->GetSize().x) * (1.f / VInterface->GetViewportViewMatrix().__row0.Length());

		// get point in subscriber space
		Matrix ALTM;
		if (CActor::GetObjectAInLocalSpaceB(ALTM, this, Inserter))
		{
			// check intersection
			int Index = 0;
			TVecIndexedPoint2fIterator Iter = SplineList.begin();

			while (Iter != SplineList.end())
			{
				TVecIndexedPoint2fIterator IterNext = GetIteratorNext(SplineList, Iter);
			
				float mu1, mu2;
				Vector2f Position;

				if (SegmentSphereIntersection2D(Iter->Point, IterNext->Point, ALTM.t2, ViewAxisX.x, &mu1, &mu2))
				{
					if (mu1 > mu2){
						std::swap(mu1, mu2);
					}

					Vector2f v = IterNext->Point - Iter->Point;
					float d = mu1 + (mu2 - mu1) * 0.5f;
					v = v * d;

					int ControlPointIndex = Index / GetDetail();

					// find real index of control Point2D
					int InsertIndex  = 0;
					int PointCounter = 0;

					TVecActorChildIterator Iterator = ChildNodes.begin();
					while(Iterator != ChildNodes.end())
					{
						if (dynamic_cast<Point2D*>(*Iterator) != NULL) // Point2D
						{
							PointCounter++;
							if (PointCounter == ControlPointIndex + 1){ // find current point counter
								break;
							}
						}
						++InsertIndex;
						++Iterator;// inc. to next child
					}

					CActor * SubObject = NEW_OBJECT(Point2D, CActor::ResolveName(std::string("Point2D"), this).c_str(), this);

					SubObject->Initialize();
					SubObject->SetPosition2f(Iter->Point + v);
					
					// insert in correct position
					AddChildNode(SubObject, InsertIndex + 1);

					RebuildTransform();
					Rebuild();

					return true;
				}
				++Index;
				++Iter;
			}
		}
	}*/
	return false;
}

//----------------------------------------------------------------------------------------------
/*void SplineAgregator::DoRebuildBounds()
{
	Matrix IWTM;
	invert(IWTM, WorldMatrixTransform);

	Vector2f BMin(FLT_MAX, FLT_MAX);
	Vector2f BMax(-FLT_MAX, -FLT_MAX);

	TVecIndexedPoint2fIterator Iter(SplineList.begin());
	while (Iter != SplineList.end())
	{
		Vector2f TPoint = transform_coord(Iter->Point, IWTM);

		if (TPoint.x < BMin.x) { BMin.x = TPoint.x; }
		if (TPoint.x > BMax.x) { BMax.x = TPoint.x; }

		if (TPoint.y < BMin.y) { BMin.y = TPoint.y; }
		if (TPoint.y > BMax.y) { BMax.y = TPoint.y; }

		++Iter;
	}

	Iter = SplineBorderList.begin();
	while (Iter != SplineBorderList.end())
	{
		Vector2f TPoint = transform_coord(Iter->Point, IWTM);

		if (TPoint.x < BMin.x) { BMin.x = TPoint.x; }
		if (TPoint.x > BMax.x) { BMax.x = TPoint.x; }
		if (TPoint.y < BMin.y) { BMin.y = TPoint.y; }
		if (TPoint.y > BMax.y) { BMax.y = TPoint.y; }

		++Iter;
	}

	Bounds.bound_min = Vector(BMin.x, BMin.y, 0.f);
	Bounds.bound_max = Vector(BMax.x, BMax.y, 0.f);
}*/

//----------------------------------------------------------------------------------------------
void SplineAgregator::ConvertToEditableMesh()
{

}