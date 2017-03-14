#include "coresdkafx.h"

REGISTER_CLASS_A(ProjectedLight, CCamera)
	new oes::rflex::PropertyUINT("Color", (BYTE*)&((ProjectedLight*)NULL)->Color - (BYTE*)NULL, "ProjectedLight", "Value",	READ_WRITE,	CTRL_COLOR,	SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyBOOL("bTestAdapt", (BYTE*)&((ProjectedLight*)NULL)->bTestAdapt - (BYTE*)NULL, "ProjectedLight", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyUINT("IndexTest", (BYTE*)&((ProjectedLight*)NULL)->IndexTest - (BYTE*)NULL,	"ProjectedLight", "Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyFLOAT("DepthBias", (BYTE*)&((ProjectedLight*)NULL)->DepthBias - (BYTE*)NULL, "ProjectedLight", "Value",	READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyFLOAT("SlopeBias", (BYTE*)&((ProjectedLight*)NULL)->SlopeBias - (BYTE*)NULL, "ProjectedLight", "Value",	READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(ProjectedLight, CCamera);

//----------------------------------------------------------------------------------------------
ProjectedLight::ProjectedLight(const CObjectAbstract *Parent)
: Super(Parent)
, CompRenderTarget(NULL)
, Color(0xffffffff)
, bTestAdapt(false)
, IndexTest(0)
, DepthBias(4.f)
, SlopeBias(1.f)
{
	NEW_OBJECT_TRANSIENT_CHILD(CompRenderTarget, Comp_RenderTarget, "RenderTarget", this);
}

//----------------------------------------------------------------------------------------------
ProjectedLight::~ProjectedLight()
{

}

//----------------------------------------------------------------------------------------------
void ProjectedLight::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void ProjectedLight::Initialize()
{
	Super::Initialize();

	CompRenderTarget->CreateRenderTarget(1024, 1024 * 3);
}

//----------------------------------------------------------------------------------------------
void ProjectedLight::DoDraw()
{
#ifndef FINAL_RELEASE
/*	if (GetAppMain()->IsAEditor() && !GetAppMain()->IsAGame())
	{
		if (!IsFocused())
		{ 
			// to avoid collisions
			//GetRenderComponent()->SetTransform(WorldMatrixTransform);
			Super::DoDraw();
		}
	}*/
#endif//FINAL_RELEASE
}

//----------------------------------------------------------------------------------------------
void ProjectedLight::PreDraw()
{
	/*core_sdk_api::ViewportInterface * VInterface = ActorAllocator::GetViewportInterface(this);
	const CActor * VRoot = dynamic_cast<const CActor*>(VInterface);

	CameraManager * Mgr = GetAppMain()->GetCameraManager();
	assert(Mgr != NULL);

	const CCamera * BuildCamera = Mgr->GetCamera(0); //Mgr->GetActiveCamera();

	if (BuildCamera && BuildCamera != this)
	{
		// save old camera and set light view as origin
		const CCamera * ActiveCamera_Old = Mgr->GetActiveCamera();

		// save matrixes
		Matrix ProjMatrix_Old = GetRenderComponent()->GetProjMatrix();
		Matrix ViewMatrix_Old = GetRenderComponent()->GetViewMatrix();
		UniCullinfo	Cullinfo_Old = m_UniCullinfo;

		CalcProjMatrix();

		Matrix SourceProjMatrix = GetProjMatrix();

		Bounds3f CasterBB, RecieverBB;
		// activate self
		Mgr->ActivateCamera(this);

		GetRenderComponent()->PushRenderQuevueAdjaency();

		GetRenderComponent()->SetRenderTarget(CompRenderTarget, true);
		GetRenderComponent()->SetBias(0.f, 0.f);

		GetRenderComponent()->PopRenderQuevueAdjaency();

		for (int Index = 0; Index < NUM_SPLITS; ++Index)
		{
			// build light view frustrum to first subfrustra
			BuildLightViewFrustrums(m_UniCullinfo, BuildCamera ,Index);

			GetRenderComponent()->SetFakeRender(true);
			GetRenderComponent()->ResetRenderInfoData();
			GetRenderComponent()->SetRenderInfoMask(RI_RENDER_BOUNDS_SHADOW_CASTER | RI_RENDER_BOUNDS_SHADOW_RECIEVERS);

			// test render pass
			for_each(VRoot->ChildNodes.begin(), VRoot->ChildNodes.end(), std::mem_fun(&CObjectAbstract::Draw));

			GetRenderComponent()->SetRenderInfoMask(RI_NONE);
			GetRenderComponent()->SetFakeRender(false);
			
			CasterBB = GetRenderComponent()->GetRI_GetObjectsShadowCasterBound();
			RecieverBB = GetRenderComponent()->GetRI_GetObjectsShadowReceiversBound();
			
			if (CasterBB.IsValid() && RecieverBB.IsValid())
			{
				GetRenderComponent()->PushRenderQuevueAdjaency();

				// Build crop matrix
				// array size must be equal to number of splits
				CropMatrixes[Index] = BuildCropMatrix(BuildCamera->Subfrustras[Index], SourceProjMatrix, CasterBB, RecieverBB);

				GetRenderComponent()->SetViewMatrix(GetViewMatrix());
				GetRenderComponent()->SetProjMatrix(SourceProjMatrix);
				GetRenderComponent()->SetProjCropMatrix(CropMatrixes[Index]);
			
				GetRenderComponent()->SetViewport(0, Index * 1024, 1024, 1024, 0.f, 1.f);

				const Matrix I;
				GetRenderComponent()->SetTransform(I);

				// render to shadow map
				for_each(VRoot->ChildNodes.begin(), VRoot->ChildNodes.end(), std::mem_fun(&CObjectAbstract::Draw));

				GetRenderComponent()->PopRenderQuevueAdjaency();
			}
		}

		GetRenderComponent()->PushRenderQuevueAdjaency();

		GetRenderComponent()->EndRenderTarget();
		GetRenderComponent()->SetBias(DepthBias, SlopeBias);

		// reactivate old view
		Mgr->ActivateCamera(ActiveCamera_Old);

		if (GetAdapt())
		{
			m_projmatrix = SourceProjMatrix * CropMatrixes[IndexTest];
			BuildLightViewFrustrums(m_UniCullinfo, BuildCamera, IndexTest);
		}
		else
		{
			m_projmatrix = SourceProjMatrix;
			m_UniCullinfo = Cullinfo_Old;
		}

		Vector4f SplitDistances(Subfrustras[0].DistFar,
								Subfrustras[1].DistFar,
								FarDist,
								0.f);

		GetRenderComponent()->SetProjectedLight(GetTransformedWTM().t,
			GetDirection(),
			GetViewMatrix(),
			GetProjMatrix(),
			Color,
			SplitDistances,
			CropMatrixes,
			NUM_SPLITS);

		if (GetFocused() && GetAdapt())
		{
			// debug info
			const Matrix I;
			GetRenderComponent()->DrawBounds(I, CasterBB, 0xfffd9b01);
			GetRenderComponent()->DrawBounds(I, RecieverBB, 0xffff01fc);
		}

		GetRenderComponent()->PopRenderQuevueAdjaency();
	}
	*/
}

//----------------------------------------------------------------------------------------------
bool ProjectedLight::BuildLightViewFrustrums(UniCullinfo &OutCullinfo, const CCamera * BuildCamera, size_t Index) const
{
	bool bResult = false;
/*
	const CCamera::Subfrustra & InSubfrustra = BuildCamera->Subfrustras[Index];

	std::vector<UniEdge> Edges;
	std::vector<UniEdge> FrustrumEdges;
	std::vector<UniTriangle> FrustrumTriangles;

	const Vector2f ViewportSize((float)GetRenderComponent()->GetViewportWidth(),
								(float)GetRenderComponent()->GetViewportHeight());

	OutCullinfo.Planes.clear();

	// side 0
	FrustrumTriangles.push_back(UniTriangle(0, 4, 2, 0));
	FrustrumTriangles.push_back(UniTriangle(6, 2, 4, 0));

	// side 1
	FrustrumTriangles.push_back(UniTriangle(0, 2, 1, 1));
	FrustrumTriangles.push_back(UniTriangle(3, 1, 2, 1));

	// side 2
	FrustrumTriangles.push_back(UniTriangle(0, 4, 1, 2));
	FrustrumTriangles.push_back(UniTriangle(5, 1, 4, 2));

	// side 3
	FrustrumTriangles.push_back(UniTriangle(5, 1, 7, 3));
	FrustrumTriangles.push_back(UniTriangle(3, 7, 1, 3));

	// side 4
	FrustrumTriangles.push_back(UniTriangle(7, 6, 5, 4));
	FrustrumTriangles.push_back(UniTriangle(4, 5, 6, 4));

	// side 5
	FrustrumTriangles.push_back(UniTriangle(7, 3, 6, 5));
	FrustrumTriangles.push_back(UniTriangle(2, 6, 3, 5));

	// build adjacency data
	BuildEdgeList(Edges, FrustrumTriangles);

	// iterate throw all adjacency data
	std::vector<UniEdge>::iterator Iter = Edges.begin();

	while (Iter != Edges.end())
	{
		unsigned int index0 = Iter->Index0;
		unsigned int index1 = Iter->Index1;

		UniTriangle & triangle0 = Iter->triangle0;
		UniTriangle & triangle1 = Iter->triangle1;

		Vector edge0 = InSubfrustra.vecSubFrustum[index0];
		Vector edge1 = InSubfrustra.vecSubFrustum[index1];

		Vector triangle_normal0 = InSubfrustra.SubUniCullinfo.Planes[Iter->triangle0.IdxNormal].Normal;
		Vector triangle_normal1 = InSubfrustra.SubUniCullinfo.Planes[Iter->triangle1.IdxNormal].Normal;

		Vector vec_source = !bOrtho ? GetTransformedWTM_().t - edge0 : -GetDirection();

		float dot0 = vec_source.Dot(triangle_normal0);
		float dot1 = vec_source.Dot(triangle_normal1);

		++Iter;

		if (( dot0 < 0.f && dot1 < 0.f) || (dot0 > 0.f && dot1 > 0.f) ){
			continue;    // not edge  
		}

		Vector SumTriNorm = triangle_normal0 + triangle_normal1;
		SumTriNorm.normalize();

		Vector N;
		cross(N, edge1 - edge0, !bOrtho ? GetTransformedWTM_().t - edge0 : -GetDirection());
		N.normalize();

		if (GetHalfSpace(N, edge0, edge0 + SumTriNorm) < 0.f){
			N *= -1.f;
		}

		OutCullinfo.Planes.push_back(Plane(N, edge0));
	}*/
	return bResult;
}

//----------------------------------------------------------------------------------------------
Matrix ProjectedLight::BuildCropMatrix(const CCamera::Subfrustra & InSubfrustra, const Matrix & SourceProjMatrix, const Bounds3f & CastersBB, const Bounds3f & RecieversBB)
{
	Matrix LightViewProjMatrix = m_viewmatrix * SourceProjMatrix;

	Bounds3f splitBB, castersBB, recieversBB;

	Vector  PointsFrustrum[8], PointsCaster[8], PointsReceiver[8];    // corners of the box

	PointsFrustrum[0] = Vector(InSubfrustra.Bounds.bound_min.x, InSubfrustra.Bounds.bound_min.y, InSubfrustra.Bounds.bound_min.z); // xyz
	PointsFrustrum[1] = Vector(InSubfrustra.Bounds.bound_max.x, InSubfrustra.Bounds.bound_min.y, InSubfrustra.Bounds.bound_min.z); // Xyz
	PointsFrustrum[2] = Vector(InSubfrustra.Bounds.bound_min.x, InSubfrustra.Bounds.bound_max.y, InSubfrustra.Bounds.bound_min.z); // xYz
	PointsFrustrum[3] = Vector(InSubfrustra.Bounds.bound_max.x, InSubfrustra.Bounds.bound_max.y, InSubfrustra.Bounds.bound_min.z); // XYz
	PointsFrustrum[4] = Vector(InSubfrustra.Bounds.bound_min.x, InSubfrustra.Bounds.bound_min.y, InSubfrustra.Bounds.bound_max.z); // xyZ
	PointsFrustrum[5] = Vector(InSubfrustra.Bounds.bound_max.x, InSubfrustra.Bounds.bound_min.y, InSubfrustra.Bounds.bound_max.z); // XyZ
	PointsFrustrum[6] = Vector(InSubfrustra.Bounds.bound_min.x, InSubfrustra.Bounds.bound_max.y, InSubfrustra.Bounds.bound_max.z); // xYZ
	PointsFrustrum[7] = Vector(InSubfrustra.Bounds.bound_max.x, InSubfrustra.Bounds.bound_max.y, InSubfrustra.Bounds.bound_max.z); // XYZ

	PointsCaster[0] = Vector(CastersBB.bound_min.x,  CastersBB.bound_min.y,  CastersBB.bound_min.z); // xyz
	PointsCaster[1] = Vector(CastersBB.bound_max.x,  CastersBB.bound_min.y,  CastersBB.bound_min.z); // Xyz
	PointsCaster[2] = Vector(CastersBB.bound_min.x,  CastersBB.bound_max.y,  CastersBB.bound_min.z); // xYz
	PointsCaster[3] = Vector(CastersBB.bound_max.x,  CastersBB.bound_max.y,  CastersBB.bound_min.z); // XYz
	PointsCaster[4] = Vector(CastersBB.bound_min.x,  CastersBB.bound_min.y,  CastersBB.bound_max.z); // xyZ
	PointsCaster[5] = Vector(CastersBB.bound_max.x,  CastersBB.bound_min.y,  CastersBB.bound_max.z); // XyZ
	PointsCaster[6] = Vector(CastersBB.bound_min.x,  CastersBB.bound_max.y,  CastersBB.bound_max.z); // xYZ
	PointsCaster[7] = Vector(CastersBB.bound_max.x,  CastersBB.bound_max.y,  CastersBB.bound_max.z); // XYZ

	PointsReceiver[0] = Vector(RecieversBB.bound_min.x,  RecieversBB.bound_min.y,  RecieversBB.bound_min.z); // xyz
	PointsReceiver[1] = Vector(RecieversBB.bound_max.x,  RecieversBB.bound_min.y,  RecieversBB.bound_min.z); // Xyz
	PointsReceiver[2] = Vector(RecieversBB.bound_min.x,  RecieversBB.bound_max.y,  RecieversBB.bound_min.z); // xYz
	PointsReceiver[3] = Vector(RecieversBB.bound_max.x,  RecieversBB.bound_max.y,  RecieversBB.bound_min.z); // XYz
	PointsReceiver[4] = Vector(RecieversBB.bound_min.x,  RecieversBB.bound_min.y,  RecieversBB.bound_max.z); // xyZ
	PointsReceiver[5] = Vector(RecieversBB.bound_max.x,  RecieversBB.bound_min.y,  RecieversBB.bound_max.z); // XyZ
	PointsReceiver[6] = Vector(RecieversBB.bound_min.x,  RecieversBB.bound_max.y,  RecieversBB.bound_max.z); // xYZ
	PointsReceiver[7] = Vector(RecieversBB.bound_max.x,  RecieversBB.bound_max.y,  RecieversBB.bound_max.z); // XYZ

	Vector SF_Min(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);
	Vector SF_Max(-MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT);

	Vector Cs_Min(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);
	Vector Cs_Max(-MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT);

	Vector Rc_Min(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);
	Vector Rc_Max(-MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT);

	for (int Index = 0; Index < 8; ++Index)
	{
		transform_coord(PointsFrustrum[Index], PointsFrustrum[Index], LightViewProjMatrix);
		transform_coord(PointsCaster[Index], PointsCaster[Index], LightViewProjMatrix);
		transform_coord(PointsReceiver[Index], PointsReceiver[Index], LightViewProjMatrix);

		// subfrustra
		if (PointsFrustrum[Index].x < SF_Min.x){
			SF_Min.x = PointsFrustrum[Index].x;
		} 
		else if (PointsFrustrum[Index].x > SF_Max.x){
			SF_Max.x = PointsFrustrum[Index].x;
		}
		if (PointsFrustrum[Index].y < SF_Min.y){
			SF_Min.y = PointsFrustrum[Index].y;
		}
		else if (PointsFrustrum[Index].y > SF_Max.y){
			SF_Max.y = PointsFrustrum[Index].y;
		}
		if (PointsFrustrum[Index].z < SF_Min.z){
			SF_Min.z = PointsFrustrum[Index].z;
		}
		else if (PointsFrustrum[Index].z > SF_Max.z){
			SF_Max.z = PointsFrustrum[Index].z;
		}

		// casters
		if (PointsCaster[Index].x < Cs_Min.x){
			Cs_Min.x = PointsCaster[Index].x;
		} 
		else if (PointsCaster[Index].x > Cs_Max.x){
			Cs_Max.x = PointsCaster[Index].x;
		}
		if (PointsCaster[Index].y < Cs_Min.y){
			Cs_Min.y = PointsCaster[Index].y;
		}
		else if (PointsCaster[Index].y > Cs_Max.y){
			Cs_Max.y = PointsCaster[Index].y;
		}
		if (PointsCaster[Index].z < Cs_Min.z){
			Cs_Min.z = PointsCaster[Index].z;
		}
		else if (PointsCaster[Index].z > Cs_Max.z){
			Cs_Max.z = PointsCaster[Index].z;
		}

		// receivers
		if (PointsReceiver[Index].x < Rc_Min.x){
			Rc_Min.x = PointsReceiver[Index].x;
		} 
		else if (PointsReceiver[Index].x > Rc_Max.x){
			Rc_Max.x = PointsReceiver[Index].x;
		}
		if (PointsReceiver[Index].y < Rc_Min.y){
			Rc_Min.y = PointsReceiver[Index].y;
		}
		else if (PointsReceiver[Index].y > Rc_Max.y){
			Rc_Max.y = PointsReceiver[Index].y;
		}
		if (PointsReceiver[Index].z < Rc_Min.z){
			Rc_Min.z = PointsReceiver[Index].z;
		}
		else if (PointsReceiver[Index].z > Rc_Max.z){
			Rc_Max.z = PointsReceiver[Index].z;
		}
	}

	Bounds3f cropBB = splitBB;
	cropBB.bound_min.x = FMAX(FMAX(Cs_Min.x, Rc_Min.x),	SF_Min.x);
	cropBB.bound_max.x = FMIN(FMIN(Cs_Max.x, Rc_Max.x), SF_Max.x);

	cropBB.bound_min.y = FMAX(FMAX(Cs_Min.y, Rc_Min.y), SF_Min.y);
	cropBB.bound_max.y = FMIN(FMIN(Cs_Max.y, Rc_Max.y), SF_Max.y);
  
	cropBB.bound_min.z = FMIN(Cs_Min.z, SF_Min.z);
	cropBB.bound_max.z = FMIN(Rc_Max.z, SF_Max.z);

	// create crop matrix
	float scaleX, scaleY, scaleZ;
	float offsetX, offsetY, offsetZ;

	scaleX = 2.f / (cropBB.bound_max.x - cropBB.bound_min.x);
	scaleY = 2.f / (cropBB.bound_max.y - cropBB.bound_min.y);
	offsetX = -0.5f * (cropBB.bound_max.x + cropBB.bound_min.x) * scaleX;
	offsetY = -0.5f * (cropBB.bound_max.y + cropBB.bound_min.y) * scaleY;
	scaleZ = 1.f / (cropBB.bound_max.z - cropBB.bound_min.z);
	offsetZ = -cropBB.bound_min.z * scaleZ;

	return Matrix(scaleX, 0.f, 0.f, 0.f,
					0.f, scaleY, 0.f, 0.f,
					0.f, 0.f, scaleZ, 0.f,
					offsetX, offsetY, offsetZ, 1.f);
}

//----------------------------------------------------------------------------------------------
void ProjectedLight::CalcProjMatrix()
{
	if (!m_bOrtho)
	{
		Super::CalcProjMatrix();	
	}
	else
	{
		float w = 1024.f;
		float h = 1024.f;

		ortho(m_projmatrix, -w, w, -h, h, NearDist, FarDist);

		ortho(Subfrustras[0].SubProjMatrix, -w, w, -h, h, Subfrustras[0].DistNear, Subfrustras[0].DistFar);
		ortho(Subfrustras[1].SubProjMatrix, -w, w, -h, h, Subfrustras[1].DistNear, Subfrustras[1].DistFar);
		ortho(Subfrustras[2].SubProjMatrix, -w, w, -h, h, Subfrustras[2].DistNear, Subfrustras[2].DistFar);
	}
}