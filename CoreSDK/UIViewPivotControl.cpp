#include "coresdkafx.h"

REGISTER_CLASS(UIViewPivotControl, ActorAllocator);

const Vector Offset(0.5f, 0.5f, 0.5f);

static const char *CUBE_TEXTURE = "dev_measuregeneric01b.tga";

static const Vector vBuffBottom [] = 
{
	Vector(0.f, 0.f, 0.f) - Offset,
	Vector(0.25f, 0.f, 0.f) - Offset,
	Vector(0.75f, 0.f, 0.f) - Offset,
	Vector(1.f,	0.f, 0.f) - Offset,

	Vector(0.f,	0.f, 0.25f) - Offset,
	Vector(0.25f, 0.f, 0.25f) - Offset,
	Vector(0.75f, 0.f, 0.25f) - Offset,
	Vector(1.f,	0.f, 0.25f) - Offset,

	Vector(0.f,	  0.f, 0.75f) - Offset,
	Vector(0.25f, 0.f, 0.75f) - Offset,
	Vector(0.75f, 0.f, 0.75f) - Offset,
	Vector(1.f,	  0.f, 0.75f) - Offset,

	Vector(0.f,	  0.f, 1.f) - Offset,
	Vector(0.25f, 0.f, 1.f) - Offset,
	Vector(0.75f, 0.f, 1.f) - Offset,
	Vector(1.f,	  0.f, 1.f) - Offset,
};

static const Vector vBuffTop [] = 
{
	Vector(0.f,	1.f, 0.f) - Offset,
	Vector(0.25f, 1.f, 0.f) - Offset,
	Vector(0.75f, 1.f, 0.f) - Offset,
	Vector(1.f,	1.f, 0.f) - Offset,
 
	Vector(0.f,	1.f, 0.25f) - Offset,
	Vector(0.25f, 1.f, 0.25f) - Offset,
	Vector(0.75f, 1.f, 0.25f) - Offset,
	Vector(1.f,	1.f, 0.25f) - Offset,
 
	Vector(0.f,	  1.f, 0.75f) - Offset,
	Vector(0.25f, 1.f, 0.75f) - Offset,
	Vector(0.75f, 1.f, 0.75f) - Offset,
	Vector(1.f,	  1.f, 0.75f) - Offset,
 
	Vector(0.f,	  1.f, 1.f) - Offset,
	Vector(0.25f, 1.f, 1.f) - Offset,
	Vector(0.75f, 1.f, 1.f) - Offset,
	Vector(1.f,	  1.f, 1.f) - Offset,
};

static const Vector vBuffFront [] = 
{
	// 	// front
	Vector(0.f,	 0.f, 0.f) - Offset,
	Vector(0.25f, 0.f, 0.f) - Offset,
	Vector(0.75f, 0.f, 0.f) - Offset,
	Vector(1.f,	 0.f, 0.f) - Offset,
	
	Vector(0.f,	 0.25f, 0.f) - Offset,
	Vector(0.25f, 0.25f, 0.f) - Offset,
	Vector(0.75f, 0.25f, 0.f) - Offset,
	Vector(1.f,	 0.25f, 0.f) - Offset,
	
	Vector(0.f,	 0.75f, 0.f) - Offset,
	Vector(0.25f, 0.75f, 0.f) - Offset,
	Vector(0.75f, 0.75f, 0.f) - Offset,
	Vector(1.f,	 0.75f, 0.f) - Offset,
	
	Vector(0.f,	 1.f, 0.f) - Offset,
	Vector(0.25f, 1.f, 0.f) - Offset,
	Vector(0.75f, 1.f, 0.f) - Offset,
	Vector(1.f,	 1.f, 0.f) - Offset,
};

static const Vector vBuffBack [] = 
{
	Vector(0.f,	 0.f, 1.f) - Offset,
	Vector(0.25f, 0.f, 1.f) - Offset,
	Vector(0.75f, 0.f, 1.f) - Offset,
	Vector(1.f,	 0.f, 1.f) - Offset,
	
	Vector(0.f,	 0.25f, 1.f) - Offset,
	Vector(0.25f, 0.25f, 1.f) - Offset,
	Vector(0.75f, 0.25f, 1.f) - Offset,
	Vector(1.f,	 0.25f, 1.f) - Offset,
	
	Vector(0.f,	 0.75f, 1.f) - Offset,
	Vector(0.25f, 0.75f, 1.f) - Offset,
	Vector(0.75f, 0.75f, 1.f) - Offset,
	Vector(1.f,	 0.75f, 1.f) - Offset,
	
	Vector(0.f,	 1.f, 1.f) - Offset,
	Vector(0.25f, 1.f, 1.f) - Offset,
	Vector(0.75f, 1.f, 1.f) - Offset,
	Vector(1.f,	 1.f, 1.f) - Offset,
};

static const Vector vBuffLeft [] = 
{
	Vector(0.f,	0.f, 0.f) - Offset,
	Vector(0.f,	0.f, 0.25f) - Offset,
	Vector(0.f,	0.f, 0.75f) - Offset,
	Vector(0.f,	0.f, 1.f) - Offset,

	Vector(0.f, 0.25f, 0.f) - Offset,
	Vector(0.f, 0.25f, 0.25f) - Offset,
	Vector(0.f, 0.25f, 0.75f) - Offset,
	Vector(0.f, 0.25f, 1.f) - Offset,

	Vector(0.f, 0.75f, 0.f) - Offset,
	Vector(0.f, 0.75f, 0.25f) - Offset,
	Vector(0.f, 0.75f, 0.75f) - Offset,
	Vector(0.f, 0.75f, 1.f) - Offset,

	Vector(0.f, 1.f, 0.f) - Offset,
	Vector(0.f, 1.f, 0.25f) - Offset,
	Vector(0.f, 1.f, 0.75f) - Offset,
	Vector(0.f, 1.f, 1.f) - Offset,
};

static const Vector vBuffRight [] = 
{
	Vector(1.f,	0.f, 0.f) - Offset,
	Vector(1.f,	0.f, 0.25f) - Offset,
	Vector(1.f,	0.f, 0.75f) - Offset,
	Vector(1.f,	0.f, 1.f) - Offset,

	Vector(1.f, 0.25f, 0.f) - Offset,
	Vector(1.f, 0.25f, 0.25f) - Offset,
	Vector(1.f, 0.25f, 0.75f) - Offset,
	Vector(1.f, 0.25f, 1.f) - Offset,

	Vector(1.f, 0.75f, 0.f) - Offset,
	Vector(1.f, 0.75f, 0.25f) - Offset,
	Vector(1.f, 0.75f, 0.75f) - Offset,
	Vector(1.f, 0.75f, 1.f) - Offset,

	Vector(1.f, 1.f, 0.f) - Offset,
	Vector(1.f, 1.f, 0.25f) - Offset,
	Vector(1.f, 1.f, 0.75f) - Offset,
	Vector(1.f, 1.f, 1.f) - Offset,
};

unsigned int iBuffIndexesL [] =
{
	// r0
	0, 5, 4, // 0 - 5
	0, 1, 5,

	1, 6, 5, // 6 - 11
	1, 2, 6,

	2, 7, 6, // 12 - 17
	2, 3, 7,

	// r1
	4, 9, 8, // 18 - 23
	4, 5, 9,

	5, 10, 9, // 24 - 29
	5, 6, 10,

	6, 11, 10, // 30 - 35
	6, 7, 11,

	// r2
	8, 13, 12, // 36 - 41
	8, 9,  13,

	9, 14, 13, // 42 - 47
	9, 10, 14,

	10, 15, 14, // 48 - 53
	10, 11, 15,
};

unsigned int iBuffIndexesR [] =
{
	0, 4, 5, // 0 - 5
	5, 1, 0,

	5, 6, 1, // 6 - 11
	6, 2, 1,

	6, 7, 2, // 12 - 17
	7, 3, 2,

	8, 9, 4, // 18 - 23
	9, 5, 4,

	9, 10, 5, // 24 - 29
	10,  6, 5,

	10, 11, 6, // 30 - 35
	11, 7, 6,

	12, 13, 8, // 36 - 41
	13, 9, 8,

	13, 14, 9, // 42 - 47
	14, 10, 9,

	14, 15, 10, // 48 - 53
	15, 11, 10,
};
 
//----------------------------------------------------------------------------------------------
UIViewPivotControl::UIViewPivotControl(const CObjectAbstract *pParent)
: Super(pParent)
, IDrawInterface(pParent)
{
	IDrawInterface::RegisterDrawInterface(this, 0);

	NEW_OBJECT_TRANSIENT_CHILD(CompRenderTarget, Comp_RenderTarget, "RenderTarget", this);
	NEW_OBJECT_TRANSIENT_CHILD(MeshComponent, Comp_StaticMesh, "MeshComponent", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pMaterialEffect, Comp_MaterialEffect, "UICompDrawImage_MaterialEffect", this);
}

//----------------------------------------------------------------------------------------------
UIViewPivotControl::~UIViewPivotControl()
{

}

//----------------------------------------------------------------------------------------------
void UIViewPivotControl::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void UIViewPivotControl::Initialize()
{
	Super::Initialize();

	CompRenderTarget->CreateRenderTarget(256, 256);

	RebuildMesh();
}

//----------------------------------------------------------------------------------------------
void UIViewPivotControl::DoDraw()
{
	RenderSDK::RenderAdjacency &refAdj = m_pRenderSDK->GetRenderAdjacency();
	RenderSDK::SRTVariant_Adjacency &adjacency = refAdj.GetCurrentAdjacency();

	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext = m_pRenderSDK->GetRenderDriver()->GetDefaultContext();
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bClearTarget = true;
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.clearColor = 0x11000000;
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target = CompRenderTarget->GetRenderTarget();
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_drawn = false;
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bFog = false;

	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.glAlphaTest = true;
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.glBlend = false;

	const CameraManager *pMgr = GetAppMain()->GetCameraManager();
	assert(pMgr);
    
	const CameraManager::T *buildCamera = pMgr->GetActiveCamera();
	assert(buildCamera);

	Matrix viewmatrix;
    Matrix projmatrix;

	Quaternion rot = buildCamera->GetRot();

	rot.Normalize();
	rot.ToMatrix(&viewmatrix);

	Vector axisX(viewmatrix._11, viewmatrix._21, viewmatrix._31);
	Vector axisY(viewmatrix._12, viewmatrix._22, viewmatrix._32);
	Vector axisZ(viewmatrix._13, viewmatrix._23, viewmatrix._33);

	Vector eye = buildCamera->GetPosition_();
	eye.normalize();
	eye *= 2.f;

	viewmatrix.a03 = -axisX.Dot(eye);
	viewmatrix.a13 = -axisY.Dot(eye);
	viewmatrix.a23 = -axisZ.Dot(eye);

    const float fov = 60.f;
    const float w = (float)CompRenderTarget->GetRenderTarget()->GetWidth();
    const float h = (float)CompRenderTarget->GetRenderTarget()->GetHeight();

    float fAspect = w / h;
    perspective(projmatrix, fov, fAspect, 1.f, 200.f);

	memcpy(&adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewMatrix, viewmatrix.m, 16 * sizeof(float));
	memcpy(&adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.projMatrix, projmatrix.m, 16 * sizeof(float));

	if (buildCamera->IsArcball())
	{
		GetRenderComponent()->DrawObject(MeshComponent);
	}

	const Vector originX(1.f, 0.f, 0.f);
	const Vector originY(0.f, 1.f, 0.f);
	const Vector originZ(0.f, 0.f, 1.f);
	const Vector base = eye + buildCamera->GetDirection() * 2.f;

	m_pCoreSDK->GetRenderSDK()->DrawLine(base, base + originX, COLOR_RED, false);
	m_pCoreSDK->GetRenderSDK()->DrawLine(base, base + originY, COLOR_BLUE, false);
	m_pCoreSDK->GetRenderSDK()->DrawLine(base, base + originZ, COLOR_GREEN, false);

}

//----------------------------------------------------------------------------------------------
void UIViewPivotControl::RebuildMesh()
{
	const unsigned int NumVertices = 16;
	
	// initialize
	std::string sFilename;
	if (FindFile(CUBE_TEXTURE, &sFilename)){
		m_pMaterialEffect->LoadDiffuseMap(sFilename.c_str(), false);
	}

	m_pMaterialEffect->SetAlphaTestEnable(true);
	m_pMaterialEffect->SetTransparent(0.f);

	D3DMesh * dx_mesh = MeshComponent->GetD3DMesh();

	dx_mesh->Clear(true);

	// bottom
	{
		SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());
//		SubMesh->AddRef();

		Vector2f * texcoords = new Vector2f[NumVertices];
		Vector * normals = new Vector[NumVertices];		// array of normals
		Vector4f * colors = new Vector4f[NumVertices];	// array of colors (vertex colors)

		for (int Index = 0; Index < NumVertices; ++ Index)
		{
			texcoords[Index] = Vector2f((vBuffBottom[Index].x + 0.5f) * 0.166666f, (vBuffBottom[Index].z * (-1.f)) + 0.5f);
			normals[Index] = Vector(0.f, -1.f, 0.f);
			colors[Index] = Vector4f(255.f, 255.f, 255.f, 1.f);
		}

		unsigned int numDistinctBones = 0;
		std::vector<int> VecBoneRemapper;
		SubMesh->InitializeVertexData(GetRenderComponent()->GetRenderDriver(),
			(float*)vBuffBottom,
			(float*)normals,
			(float*)colors,
			NULL,
			NULL,
			iBuffIndexesL,
			NumVertices,
			18,
			(float*)texcoords,
			1,
			false,
			NULL,
			0,
			VecBoneRemapper,
			numDistinctBones);

		dx_mesh->AddMesh(SMeshEffectPair(SubMesh, m_pMaterialEffect->pMaterialEffect));

		delete [] texcoords;
		delete [] normals;
		delete [] colors;
	}
	// top
	{
		SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());
//		SubMesh->AddRef();

		Vector2f *texcoords = new Vector2f[NumVertices];
		Vector *normals = new Vector[NumVertices];		// array of normals
		Vector4f *colors = new Vector4f[NumVertices];	// array of colors (vertex colors)

		for (int Index = 0; Index < NumVertices; ++ Index)
		{
			texcoords[Index] = Vector2f((vBuffBottom[Index].x + 0.5f) * 0.166666f + 0.166666f, vBuffBottom[Index].z + 0.5f);
			normals[Index] = Vector(0.f, -1.f, 0.f);
			colors[Index] = Vector4f(255.f, 0.f, 0.f, 1.f);
		}

		unsigned int numDistinctBones = 0;
		std::vector<int> VecBoneRemapper;
		SubMesh->InitializeVertexData(GetRenderComponent()->GetRenderDriver(),
			(float*)vBuffTop,
			(float*)normals,
			(float*)colors,
			NULL,
			NULL,
			iBuffIndexesR,
			NumVertices,
			18,
			(float*)texcoords,
			1,
			false,
			NULL,
			0,
			VecBoneRemapper,
			numDistinctBones);

		dx_mesh->AddMesh(SMeshEffectPair(SubMesh, m_pMaterialEffect->pMaterialEffect));

		delete [] texcoords;
		delete [] normals;
		delete [] colors;
	}
	// front
	{
		SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());
//		SubMesh->AddRef();

		Vector2f *texcoords = new Vector2f[NumVertices];
		Vector *normals = new Vector[NumVertices];		// array of normals
		Vector4f *colors = new Vector4f[NumVertices];	// array of colors (vertex colors)

		for (int Index = 0; Index < NumVertices; ++ Index)
		{
			texcoords[Index] = Vector2f((vBuffFront[Index].x + 0.5f) * 0.166666f + 2 * 0.166666f, vBuffFront[Index].y + 0.5f);
			normals[Index] = Vector(0.f, -1.f, 0.f);
			colors[Index] = Vector4f(255.f, 255.f, 255.f, 1.f);
		}

		unsigned int numDistinctBones = 0;
		std::vector<int> VecBoneRemapper;
		SubMesh->InitializeVertexData(GetRenderComponent()->GetRenderDriver(),
			(float*)vBuffFront,
			(float*)normals,
			(float*)colors,
			NULL,
			NULL,
			iBuffIndexesR,
			NumVertices,
			18,
			(float*)texcoords,
			1,
			false,
			NULL,
			0,
			VecBoneRemapper,
			numDistinctBones);

		dx_mesh->AddMesh(SMeshEffectPair(SubMesh, m_pMaterialEffect->pMaterialEffect));

		delete [] texcoords;
		delete [] normals;
		delete [] colors;
	}
	// back
	{
		SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());
//		SubMesh->AddRef();

		Vector2f *texcoords = new Vector2f[NumVertices];
		Vector *normals = new Vector[NumVertices];		// array of normals
		Vector4f *colors = new Vector4f[NumVertices];	// array of colors (vertex colors)

		for (int Index = 0; Index < NumVertices; ++ Index)
		{
			texcoords[Index] = Vector2f(((vBuffBack[Index].x * -1.f) + 0.5f) * 0.166666f + 3 * 0.166666f, vBuffBack[Index].y + 0.5f);
			normals[Index] = Vector(0.f, -1.f, 0.f);
			colors[Index] = Vector4f(255.f, 255.f, 255.f, 1.f);
		}

		unsigned int numDistinctBones = 0;
		std::vector<int> VecBoneRemapper;
		SubMesh->InitializeVertexData(GetRenderComponent()->GetRenderDriver(),
			(float*)vBuffBack,
			(float*)normals,
			(float*)colors,
			NULL,
			NULL,
			iBuffIndexesL,
			NumVertices,
			18,
			(float*)texcoords,
			1,
			false,
			NULL,
			0,
			VecBoneRemapper,
			numDistinctBones);

		dx_mesh->AddMesh(SMeshEffectPair(SubMesh, m_pMaterialEffect->pMaterialEffect));

		delete [] texcoords;
		delete [] normals;
		delete [] colors;
	}
	// left
	{
		SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());
//		SubMesh->AddRef();

		Vector2f * texcoords = new Vector2f[NumVertices];
		Vector * normals = new Vector[NumVertices];		// array of normals
		Vector4f * colors = new Vector4f[NumVertices];	// array of colors (vertex colors)

		for (int Index = 0; Index < NumVertices; ++ Index)
		{
			texcoords[Index] = Vector2f(((vBuffLeft[Index].z * -1.f) + 0.5f) * 0.166666f + 4 * 0.166666f, vBuffLeft[Index].y + 0.5f);
			normals[Index] = Vector(0.f, -1.f, 0.f);
			colors[Index] = Vector4f(255.f, 255.f, 255.f, 1.f);
		}

		unsigned int numDistinctBones = 0;
		std::vector<int> VecBoneRemapper;
		SubMesh->InitializeVertexData(GetRenderComponent()->GetRenderDriver(),
			(float*)vBuffLeft,
			(float*)normals,
			(float*)colors,
			NULL,
			NULL,
			iBuffIndexesL,
			NumVertices,
			18,
			(float*)texcoords,
			1,
			false,
			NULL,
			0,
			VecBoneRemapper,
			numDistinctBones);

		dx_mesh->AddMesh(SMeshEffectPair(SubMesh, m_pMaterialEffect->pMaterialEffect));

		delete [] texcoords;
		delete [] normals;
		delete [] colors;
	}

	// right
	{
		SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());
//		SubMesh->AddRef();

		Vector2f * texcoords = new Vector2f[NumVertices];
		Vector * normals = new Vector[NumVertices];		// array of normals
		Vector4f * colors = new Vector4f[NumVertices];	// array of colors (vertex colors)

		for (int Index = 0; Index < NumVertices; ++ Index)
		{
			texcoords[Index] = Vector2f((vBuffRight[Index].z + 0.5f) * 0.166666f + 5 * 0.166666f, vBuffRight[Index].y + 0.5f);
			normals[Index] = Vector(0.f, -1.f, 0.f);
			colors[Index] = Vector4f(255.f, 255.f, 255.f, 1.f);
		}

		unsigned int numDistinctBones = 0;
		std::vector<int> VecBoneRemapper;
		SubMesh->InitializeVertexData(GetRenderComponent()->GetRenderDriver(),
			(float*)vBuffRight,
			(float*)normals,
			(float*)colors,
			NULL,
			NULL,
			iBuffIndexesR,
			NumVertices,
			18,
			(float*)texcoords,
			1,
			false,
			NULL,
			0,
			VecBoneRemapper,
			numDistinctBones);

		dx_mesh->AddMesh(SMeshEffectPair(SubMesh, m_pMaterialEffect->pMaterialEffect));

		delete [] texcoords;
		delete [] normals;
		delete [] colors;
	}
}

//----------------------------------------------------------------------------------------------
bool UIViewPivotControl::ProcessClick(const MouseInputData& InputData, const Vector2f& ViewportSize)
{
	Matrix ViewMatrix, ProjMatrix;

	if (GetAppMain()->GetCameraManager())
	{
        CameraManager::T * Camera = const_cast<CameraManager::T*>(GetAppMain()->GetCameraManager()->GetActiveCamera(InputData.pRenderContext));

		if (Camera)
		{
			ProjMatrix = Camera->GetProjMatrix();

			Quaternion Rot = Camera->GetRot();

			Rot.Normalize();
			Rot.ToMatrix(&ViewMatrix);

			Vector eye = Camera->GetPosition_();
			eye.normalize();
			eye *= 2.f;

			Vector axisX(ViewMatrix._11, ViewMatrix._21, ViewMatrix._31);
			Vector axisY(ViewMatrix._12, ViewMatrix._22, ViewMatrix._32);
			Vector axisZ(ViewMatrix._13, ViewMatrix._23, ViewMatrix._33);

			ViewMatrix.a03 = -axisX.Dot(eye);
			ViewMatrix.a13 = -axisY.Dot(eye);
			ViewMatrix.a23 = -axisZ.Dot(eye);

			Vector ViewDirection = UnprojectViewport(ViewDirection, 
				Camera->GetProjMatrix(), 
				ViewMatrix,
				InputData.MousePos,
				ViewportSize);

			bool bHit = false;
			int iHitIndex = 0;
			float iMinT = 0.f;
			float t = 0.f;

			Quaternion RotPresets [] =
			{
				Quaternion(-PI_CONST * 0.25f, PI_CONST * 0.25f, 0.f, 0.f), // 0
				Quaternion(0.f, PI_CONST * 0.25f, 0.f, 0.f),				// 1
				Quaternion(PI_CONST * 0.25f, PI_CONST * 0.25f, 0.f, 0.f),	// 2
				Quaternion(PI_CONST * 0.5f, PI_CONST * 0.25f, 0.f, 0.f),	// 3
				Quaternion(PI_CONST * 0.75f, PI_CONST * 0.25f, 0.f, 0.f),	// 4
				Quaternion(PI_CONST, PI_CONST * 0.25f, 0.f, 0.f),			// 5
				Quaternion(PI_CONST * 1.25f, PI_CONST * 0.25f, 0.f, 0.f),	// 6
				Quaternion(PI_CONST * 1.5f, PI_CONST * 0.25f, 0.f, 0.f),	// 7
				Quaternion(0.f, PI_CONST * 0.5f, 0.f, 0.f),					// 8

				Quaternion(-PI_CONST * 0.25f, 0.f, 0.f, 0.f),				// 9
				Quaternion(0.f, 0.f, 0.f, 0.f),								// 10
				Quaternion(PI_CONST * 0.25f, 0.f, 0.f, 0.f),				// 11
				Quaternion(PI_CONST * 0.5f, 0.f, 0.f, 0.f),					// 12
				Quaternion(PI_CONST * 0.75f, 0.f, 0.f, 0.f),				// 13
				Quaternion(PI_CONST, 0.f, 0.f, 0.f),						// 14		
				Quaternion(PI_CONST * 1.25f, 0.f, 0.f, 0.f),				// 15
				Quaternion(PI_CONST * 1.5f, 0.f, 0.f, 0.f),					// 16

				Quaternion(-PI_CONST * 0.25f, -PI_CONST * 0.25f, 0.f, 0.f), // 17
				Quaternion(0.f, -PI_CONST * 0.25f, 0.f, 0.f), // 18
				Quaternion(PI_CONST * 0.25f, -PI_CONST * 0.25f, 0.f, 0.f), // 19
				Quaternion(PI_CONST * 0.5f, -PI_CONST * 0.25f, 0.f, 0.f), // 20
				Quaternion(PI_CONST * 0.75f, -PI_CONST * 0.25f, 0.f, 0.f), // 21
				Quaternion(PI_CONST, -PI_CONST * 0.25f, 0.f, 0.f), // 22
				Quaternion(PI_CONST * 1.25f, -PI_CONST * 0.25f, 0.f, 0.f), // 23
				Quaternion(PI_CONST * 1.5f, -PI_CONST * 0.25f, 0.f, 0.f), // 24
				Quaternion(0.f, -PI_CONST * 0.5f, 0.f, 0.f), // 25

			};
			
			//		-----------------------------
			//		|		|			|		|
			//		|		|			|		|
			//		-----------------------------
			//		|		|			|		|
			//		|		|			|		|
			//		-----------------------------
			//		|		|			|		|
			//		|		|			|		|
			//		-----------------------------
			//		

			// bottom
			// -45 45 0
			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[0]], vBuffBottom[iBuffIndexesL[1]], vBuffBottom[iBuffIndexesL[2]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[3]], vBuffBottom[iBuffIndexesL[4]], vBuffBottom[iBuffIndexesL[5]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[0]], vBuffFront[iBuffIndexesR[1]], vBuffFront[iBuffIndexesR[2]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[3]], vBuffFront[iBuffIndexesR[4]], vBuffFront[iBuffIndexesR[5]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[0]], vBuffLeft[iBuffIndexesL[1]], vBuffLeft[iBuffIndexesL[2]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[3]], vBuffLeft[iBuffIndexesL[4]], vBuffLeft[iBuffIndexesL[5]], &t) == 1))
			{
 				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 0;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[6]], vBuffBottom[iBuffIndexesL[7]], vBuffBottom[iBuffIndexesL[8]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[9]], vBuffBottom[iBuffIndexesL[10]], vBuffBottom[iBuffIndexesL[11]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesL[6]], vBuffFront[iBuffIndexesL[7]], vBuffFront[iBuffIndexesL[8]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesL[9]], vBuffFront[iBuffIndexesL[10]], vBuffFront[iBuffIndexesL[11]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 1;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[12]], vBuffBottom[iBuffIndexesL[13]], vBuffBottom[iBuffIndexesL[14]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[15]], vBuffBottom[iBuffIndexesL[16]], vBuffBottom[iBuffIndexesL[17]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[12]], vBuffFront[iBuffIndexesR[13]], vBuffFront[iBuffIndexesR[14]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[15]], vBuffFront[iBuffIndexesR[16]], vBuffFront[iBuffIndexesR[17]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[0]], vBuffRight[iBuffIndexesR[1]], vBuffRight[iBuffIndexesR[2]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[3]], vBuffRight[iBuffIndexesR[4]], vBuffRight[iBuffIndexesR[5]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 2;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[30]], vBuffBottom[iBuffIndexesL[31]], vBuffBottom[iBuffIndexesL[32]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[33]], vBuffBottom[iBuffIndexesL[34]], vBuffBottom[iBuffIndexesL[35]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[6]], vBuffRight[iBuffIndexesR[7]], vBuffRight[iBuffIndexesR[8]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[9]], vBuffRight[iBuffIndexesR[10]], vBuffRight[iBuffIndexesR[11]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 3;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[48]], vBuffBottom[iBuffIndexesL[49]], vBuffBottom[iBuffIndexesL[50]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[51]], vBuffBottom[iBuffIndexesL[52]], vBuffBottom[iBuffIndexesL[53]], &t) == 1) ||
 				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[12]], vBuffRight[iBuffIndexesR[13]], vBuffRight[iBuffIndexesR[14]], &t) == 1) ||
 				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[15]], vBuffRight[iBuffIndexesR[16]], vBuffRight[iBuffIndexesR[17]], &t) == 1) ||
 				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[12]], vBuffBack[iBuffIndexesL[13]], vBuffBack[iBuffIndexesL[14]], &t) == 1) ||
 				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[15]], vBuffBack[iBuffIndexesL[16]], vBuffBack[iBuffIndexesL[17]], &t) == 1)
				)
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 4;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[42]], vBuffBottom[iBuffIndexesL[43]], vBuffBottom[iBuffIndexesL[44]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[45]], vBuffBottom[iBuffIndexesL[46]], vBuffBottom[iBuffIndexesL[47]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[6]], vBuffBack[iBuffIndexesL[7]], vBuffBack[iBuffIndexesL[8]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[9]], vBuffBack[iBuffIndexesL[10]], vBuffBack[iBuffIndexesL[11]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 5;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[36]], vBuffBottom[iBuffIndexesL[37]], vBuffBottom[iBuffIndexesL[38]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[39]], vBuffBottom[iBuffIndexesL[40]], vBuffBottom[iBuffIndexesL[41]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[12]], vBuffLeft[iBuffIndexesL[13]], vBuffLeft[iBuffIndexesL[14]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[15]], vBuffLeft[iBuffIndexesL[16]], vBuffLeft[iBuffIndexesL[17]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[0]], vBuffBack[iBuffIndexesL[1]], vBuffBack[iBuffIndexesL[2]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[3]], vBuffBack[iBuffIndexesL[4]], vBuffBack[iBuffIndexesL[5]], &t) == 1)
				)
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 6;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[18]], vBuffBottom[iBuffIndexesL[19]], vBuffBottom[iBuffIndexesL[20]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[21]], vBuffBottom[iBuffIndexesL[22]], vBuffBottom[iBuffIndexesL[23]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[6]], vBuffLeft[iBuffIndexesL[7]], vBuffLeft[iBuffIndexesL[8]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[9]], vBuffLeft[iBuffIndexesL[10]], vBuffLeft[iBuffIndexesL[11]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 7;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[24]], vBuffBottom[iBuffIndexesL[25]], vBuffBottom[iBuffIndexesL[26]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBottom[iBuffIndexesL[27]], vBuffBottom[iBuffIndexesL[28]], vBuffBottom[iBuffIndexesL[29]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 8;
					iMinT = t;
				}
			}

			// center
			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[18]], vBuffFront[iBuffIndexesR[19]], vBuffFront[iBuffIndexesR[20]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[21]], vBuffFront[iBuffIndexesR[22]], vBuffFront[iBuffIndexesR[23]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesR[18]], vBuffLeft[iBuffIndexesR[19]], vBuffLeft[iBuffIndexesR[20]], &t) == 1)||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesR[21]], vBuffLeft[iBuffIndexesR[22]], vBuffLeft[iBuffIndexesR[23]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 9;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[24]], vBuffFront[iBuffIndexesR[25]], vBuffFront[iBuffIndexesR[26]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[27]], vBuffFront[iBuffIndexesR[28]], vBuffFront[iBuffIndexesR[29]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 10;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[30]], vBuffFront[iBuffIndexesR[31]], vBuffFront[iBuffIndexesR[32]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[33]], vBuffFront[iBuffIndexesR[34]], vBuffFront[iBuffIndexesR[35]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[18]], vBuffRight[iBuffIndexesR[19]], vBuffRight[iBuffIndexesR[20]], &t) == 1)||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[21]], vBuffRight[iBuffIndexesR[22]], vBuffRight[iBuffIndexesR[23]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 11;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[24]], vBuffRight[iBuffIndexesR[25]], vBuffRight[iBuffIndexesR[26]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[27]], vBuffRight[iBuffIndexesR[28]], vBuffRight[iBuffIndexesR[29]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 12;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[30]], vBuffRight[iBuffIndexesR[31]], vBuffRight[iBuffIndexesR[32]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[33]], vBuffRight[iBuffIndexesR[34]], vBuffRight[iBuffIndexesR[35]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[30]], vBuffBack[iBuffIndexesL[31]], vBuffBack[iBuffIndexesL[32]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[33]], vBuffBack[iBuffIndexesL[34]], vBuffBack[iBuffIndexesL[35]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 13;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[24]], vBuffBack[iBuffIndexesL[25]], vBuffBack[iBuffIndexesL[26]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[27]], vBuffBack[iBuffIndexesL[28]], vBuffBack[iBuffIndexesL[29]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 14;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[18]], vBuffBack[iBuffIndexesL[19]], vBuffBack[iBuffIndexesL[20]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[21]], vBuffBack[iBuffIndexesL[22]], vBuffBack[iBuffIndexesL[23]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[30]], vBuffLeft[iBuffIndexesL[31]], vBuffLeft[iBuffIndexesL[32]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[33]], vBuffLeft[iBuffIndexesL[34]], vBuffLeft[iBuffIndexesL[35]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 15;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[24]], vBuffLeft[iBuffIndexesL[25]], vBuffLeft[iBuffIndexesL[26]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[27]], vBuffLeft[iBuffIndexesL[28]], vBuffLeft[iBuffIndexesL[29]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 16;
					iMinT = t;
				}
			}

			// top
			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[0]], vBuffTop[iBuffIndexesL[1]], vBuffTop[iBuffIndexesL[2]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[3]], vBuffTop[iBuffIndexesL[4]], vBuffTop[iBuffIndexesL[5]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[36]], vBuffFront[iBuffIndexesR[37]], vBuffFront[iBuffIndexesR[38]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[39]], vBuffFront[iBuffIndexesR[40]], vBuffFront[iBuffIndexesR[41]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[36]], vBuffLeft[iBuffIndexesL[37]], vBuffLeft[iBuffIndexesL[38]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[39]], vBuffLeft[iBuffIndexesL[40]], vBuffLeft[iBuffIndexesL[41]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 17;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[6]], vBuffTop[iBuffIndexesL[7]], vBuffTop[iBuffIndexesL[8]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[9]], vBuffTop[iBuffIndexesL[10]], vBuffTop[iBuffIndexesL[11]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[42]], vBuffFront[iBuffIndexesR[43]], vBuffFront[iBuffIndexesR[44]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[45]], vBuffFront[iBuffIndexesR[46]], vBuffFront[iBuffIndexesR[47]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 18;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[12]], vBuffTop[iBuffIndexesL[13]], vBuffTop[iBuffIndexesL[14]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[15]], vBuffTop[iBuffIndexesL[16]], vBuffTop[iBuffIndexesL[17]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[48]], vBuffFront[iBuffIndexesR[49]], vBuffFront[iBuffIndexesR[50]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffFront[iBuffIndexesR[51]], vBuffFront[iBuffIndexesR[52]], vBuffFront[iBuffIndexesR[53]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[36]], vBuffRight[iBuffIndexesR[37]], vBuffRight[iBuffIndexesR[38]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[39]], vBuffRight[iBuffIndexesR[40]], vBuffRight[iBuffIndexesR[41]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 19;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[42]], vBuffBack[iBuffIndexesL[43]], vBuffBack[iBuffIndexesL[44]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[45]], vBuffBack[iBuffIndexesL[46]], vBuffBack[iBuffIndexesL[47]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[30]], vBuffTop[iBuffIndexesL[31]], vBuffTop[iBuffIndexesL[32]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[33]], vBuffTop[iBuffIndexesL[34]], vBuffTop[iBuffIndexesL[35]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 20;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[48]], vBuffTop[iBuffIndexesL[49]], vBuffTop[iBuffIndexesL[50]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[51]], vBuffTop[iBuffIndexesL[52]], vBuffTop[iBuffIndexesL[53]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[48]], vBuffBack[iBuffIndexesL[49]], vBuffBack[iBuffIndexesL[50]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[51]], vBuffBack[iBuffIndexesL[52]], vBuffBack[iBuffIndexesL[53]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[48]], vBuffRight[iBuffIndexesR[49]], vBuffRight[iBuffIndexesR[50]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffRight[iBuffIndexesR[51]], vBuffRight[iBuffIndexesR[52]], vBuffRight[iBuffIndexesR[53]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 21;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[42]], vBuffTop[iBuffIndexesL[43]], vBuffTop[iBuffIndexesL[44]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[45]], vBuffTop[iBuffIndexesL[46]], vBuffTop[iBuffIndexesL[47]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[30]], vBuffBack[iBuffIndexesL[31]], vBuffBack[iBuffIndexesL[32]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[33]], vBuffBack[iBuffIndexesL[34]], vBuffBack[iBuffIndexesL[35]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 22;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[36]], vBuffTop[iBuffIndexesL[37]], vBuffTop[iBuffIndexesL[38]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[39]], vBuffTop[iBuffIndexesL[40]], vBuffTop[iBuffIndexesL[41]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[36]], vBuffBack[iBuffIndexesL[37]], vBuffBack[iBuffIndexesL[38]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffBack[iBuffIndexesL[39]], vBuffBack[iBuffIndexesL[40]], vBuffBack[iBuffIndexesL[41]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[48]], vBuffLeft[iBuffIndexesL[49]], vBuffLeft[iBuffIndexesL[50]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[51]], vBuffLeft[iBuffIndexesL[52]], vBuffLeft[iBuffIndexesL[53]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 23;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[18]], vBuffTop[iBuffIndexesL[19]], vBuffTop[iBuffIndexesL[20]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[21]], vBuffTop[iBuffIndexesL[22]], vBuffTop[iBuffIndexesL[23]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[42]], vBuffLeft[iBuffIndexesL[43]], vBuffLeft[iBuffIndexesL[44]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffLeft[iBuffIndexesL[45]], vBuffLeft[iBuffIndexesL[46]], vBuffLeft[iBuffIndexesL[47]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 24;
					iMinT = t;
				}
			}

			if ((IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[24]], vBuffTop[iBuffIndexesL[25]], vBuffTop[iBuffIndexesL[26]], &t) == 1) ||
				(IntersectRayWithTriangle(eye, eye + ViewDirection, vBuffTop[iBuffIndexesL[27]], vBuffTop[iBuffIndexesL[28]], vBuffTop[iBuffIndexesL[29]], &t) == 1))
			{
				if (!bHit || t < iMinT)
				{
					bHit = true;
					iHitIndex = 25;
					iMinT = t;
				}
			}

			if (bHit){
				Camera->SetInterp(RotPresets[iHitIndex]);
			}
		}
	}
	return true;
}