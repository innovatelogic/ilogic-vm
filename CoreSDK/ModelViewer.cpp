#include "coresdkafx.h"
#include "ObjectFactory.h"

REGISTER_CLASS(CModelViewer, ActorAllocator);

//----------------------------------------------------------------------------------------------
CModelViewer::CModelViewer(const CObjectAbstract *pParent)
: Super(pParent)
, IDrawInterface(pParent)
, core_sdk_api::TIViewport(pParent)
, m_pRenderContext(nullptr)
, m_pGenerated(nullptr)
, m_pMeshComponent(nullptr)
{
    core_sdk_api::TIViewport::RegisterViewportInterface(this);
	RegisterDrawInterface(this);

	SetTransient(true);

	NEW_OBJECT_TRANSIENT_CHILD(m_pEnvSceneInfo, CComp_EnvSceneInfo, "EnvSceneInfo", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pCamera, CCamera, "DefaultCamera", this);
}

//----------------------------------------------------------------------------------------------
CModelViewer::~CModelViewer()
{
	SAFE_DELETE(m_pRenderContext);
}

//----------------------------------------------------------------------------------------------
void CModelViewer::Initialize()
{
	CCoreSDK *pApp = GetAppMain();
	assert(pApp);

	CRenderSDK *pRenderSDK = pApp->GetRenderSDK();
	assert(pRenderSDK);

	D3DDriver *pDriver = pRenderSDK->GetRenderDriver();
	assert(pDriver);

	m_pRenderContext = new SRenderContext(pDriver);

	m_pCamera->SetRenderContext(m_pRenderContext);

	// custom camera registration
	//GetAppMain()->GetCameraManager()->RegisterCamera(m_pCamera, m_pRenderContext);

	Super::Initialize();
}

//----------------------------------------------------------------------------------------------
void CModelViewer::DoDraw()
{
	RenderSDK::SRTVariant_Adjacency &Queve = m_pRenderSDK->GetCurrQuevueAdjaency();

	// setup env params
	Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bFog = m_pEnvSceneInfo->m_bFog;
	Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMin = m_pEnvSceneInfo->m_fFogMin;
	Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMax = m_pEnvSceneInfo->m_fFogMax;
	Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogDensity =	m_pEnvSceneInfo->m_fFogDensity;
	Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nFogColor = m_pEnvSceneInfo->m_FogColor;
	Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.glAlphaTest = false;
	Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.glBlend = false;

	/** setup matrix */
	if (GetAppMain()->GetCameraManager())
	{
		if (m_pMeshComponent) // 2d mode preview
		{
			Matrix m(1.0, 0.f, 0.f, 0.f,
					 0.0, 0.f, 1.f, 0.f,
					 0.0, 1.f, 0.f, 0.f,
					 0.0, 0.f, 0.f, 1.f);

			float w = 0.65f;
			float h = 0.65f;

			Matrix projmatrix;

			ortho(projmatrix, -w, w, -h, h, 1.f, 100.f);

			memcpy(&Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewMatrix, m.m, 16 * sizeof(float));
			memcpy(&Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.projMatrix, projmatrix.m, 16 * sizeof(float));
		}
		else
		{
			memcpy(&Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewMatrix, m_pCamera->GetViewMatrix().m, 16 * sizeof(float));
			memcpy(&Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.projMatrix, m_pCamera->GetProjMatrix().m, 16 * sizeof(float));
			memcpy(&Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewPos, m_pCamera->GetPosition_().vec_array, 3 * sizeof(float));

			SetViewPoint(m_pCamera->GetPosition_());
		}

		Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext = m_pRenderContext;
		Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fNearPlane = m_pCamera->GetNearDist();
		Queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFarPlane  = m_pCamera->GetFarDist();
	}

	if (m_pMeshComponent){
		m_pRenderSDK->DrawObject(m_pMeshComponent);
	}
	else
	{
		DrawGrid();
	}
}

//----------------------------------------------------------------------------------------------
bool CModelViewer::DoVisibilityTest_()
{
	return GetVisible();
}

//----------------------------------------------------------------------------------------------
bool CModelViewer::GenerateObjectView(const wchar_t *pFilename)
{
	bool bResult = false;

	if (m_pGenerated)
	{
		m_pGenerated->Release();
		m_pGenerated = nullptr;
	}

	if (m_pMeshComponent)
	{
		m_pMeshComponent->Release();
		m_pMeshComponent = nullptr;

		m_pMaterialEffect->Release();
		m_pMaterialEffect = nullptr;
	}

    oes::rflex::TClassFactory *classFactory = oes::rflex::GetClassFactoryA();

	std::string sFilename;

	std::wstring strFileNameW = getPathFileName(std::wstring(pFilename)).c_str();
	std::string  strFileNameA = ConvertWideStringToString(strFileNameW);

	GetAppMain()->GetRenderSDK()->GetRenderDriver()->PushContext(m_pRenderContext);

	if (FindFile(strFileNameA.c_str(), &sFilename))
	{
		std::wstring mat_ext(L".mat");
		std::wstring dds_ext(L".dds");
		std::wstring tga_ext(L".tga");

		if (!strFileNameW.compare(strFileNameW.length() - dds_ext.length(), dds_ext.length(), dds_ext) ||
			!strFileNameW.compare(strFileNameW.length() - tga_ext.length(), tga_ext.length(), tga_ext))
		{
			m_pMeshComponent = static_cast<Comp_StaticMesh*>(GENERATE_OBJECT_SDK("Comp_StaticMesh", "Auto", this));
			m_pMaterialEffect = static_cast<Comp_MaterialEffect*>(GENERATE_OBJECT_SDK("Comp_MaterialEffect", "Auto", this));

			AddChildNode(m_pMeshComponent);
			AddChildNode(m_pMaterialEffect);

			m_pMeshComponent->Initialize();
			m_pMaterialEffect->Initialize();

			RebuildMesh(strFileNameA.c_str());
		}
		else if (0 == strFileNameW.compare(strFileNameW.length() - mat_ext.length(), mat_ext.length(), mat_ext))
		{
			m_pGenerated = static_cast<CActor*>(GENERATE_OBJECT_SDK("CLevelActor", "Auto", this));
			if (m_pGenerated)
			{
				AddChildNode(m_pGenerated);

				static_cast<CLevelActor*>(m_pGenerated)->SetNameMesh(strFileNameA.c_str());
			}

			m_pCamera->SetOrtho(false);
		}
		else // try to load compound xml
		{
            oes::rflex::TClassFactory *classFactory = oes::rflex::GetClassFactoryA();

			std::string inFilename = ConvertWideStringToString(pFilename);
			std::string sFilename;

			if (FindFile(inFilename.c_str(), &sFilename))
			{
				XML_PARSE_FILE(sFilename.c_str())
				{
					std::string V = std::string(xml_current_tree->Value());

					oes::rflex::CClassFactory<
						IObjectAbstract,
                        oes::rflex::TGenFunction,
                        oes::rflex::TCloneFunction>::TMapGenerator::const_iterator Iter = classFactory->m_MapGenerator.begin();

					for (;Iter != classFactory->m_MapGenerator.end(); ++Iter)
					{
						XML_CHECK_NODE(Iter->first)
						{
							XML_DEF_ATTRIBUTES_NODE(Value);

							ValueParser ValueStore(Value);

							m_pGenerated = static_cast<CActor*>(GENERATE_OBJECT_SDK(xml_current_tree->Value(), ValueStore.GetValueString("Name").c_str(), this));
							if (m_pGenerated)
							{
								AddChildNode(m_pGenerated);
								m_pGenerated->SuperDeserializer(XML_CURRENT_NODE);
							}
							break;
						}
					}
				}
			}
		}
		bResult = true;
		m_pCamera->SetOrtho(false);
	}

	if (m_pGenerated)
	{
		m_pGenerated->Initialize();
	}

	GetAppMain()->GetViewportManager()->RebuildTransform(this);

	GetAppMain()->GetRenderSDK()->GetRenderDriver()->PopContext();
	
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CModelViewer::Save(const wchar_t *pFilename)
{
	bool bResult = false;

	if (m_pGenerated)
	{
		CHAR chFileName[MAX_PATH] = "";
		ConvertWideStringToAnsiCch(chFileName, pFilename, MAX_PATH);

		GetAppMain()->SerializeActor(chFileName, m_pGenerated, true);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
void CModelViewer::DrawGrid()
{
	int MaxIndex = 500;
	int MinIndex = -500;

	for (int IndexX = MinIndex; IndexX <= MaxIndex; IndexX+=10)
	{
		GetRenderComponent()->DrawLine(Vector((float)IndexX, 0.f, -500.f), Vector((float)IndexX, 0.f, 500.f), 
										(IndexX == 0) ? COLOR_BLACK : ((IndexX == MaxIndex || IndexX == MinIndex) ? COLOR_YELLOW : 0xff555555));
		GetRenderComponent()->DrawLine(Vector(-500.f, 0.f, (float)IndexX), Vector(500.f, 0.f, (float)IndexX), 
										(IndexX == 0) ? COLOR_BLACK : ((IndexX == MaxIndex || IndexX == MinIndex) ? COLOR_YELLOW : 0xff555555));
	}
}

//----------------------------------------------------------------------------------------------
void CModelViewer::RebuildMesh(const char *diffuseTexture)
{
	unsigned int NumVertices = 4;
	unsigned int NumFaces = 2;

	Vector *vertices = new Vector[NumVertices];	// array of vertices
	Vector *normals = new Vector[NumVertices];		// array of normals
	Vector4f *colors = new Vector4f[NumVertices];	// array of colors (vertex colors)
	Vector2f *texcoords = new Vector2f[NumVertices];
	unsigned int *indices = new unsigned int[NumFaces * 3];

	assert(vertices && normals && colors && texcoords && indices);

	unsigned int Color = 0xff00ffff;

	byte a = (Color & 0xFF000000) >> 24;
	byte r = (Color & 0x00FF0000) >> 16;
	byte g = (Color & 0x0000FF00) >> 8;
	byte b = (Color & 0x000000FF);

	Vector vshift(0.5f, 0.f, 0.5f);
	vertices[0] = Vector(0.f, 0.f, 0.f) - vshift;
	vertices[1] = Vector(0.f, 0.f, 1.f) - vshift;
	vertices[2] = Vector(1.f, 0.f, 1.f) - vshift;
	vertices[3] = Vector(1.f, 0.f, 0.f) - vshift;

	normals[0] = Vector(0.f, 1.f, 0.f);
	normals[1] = Vector(0.f, 1.f, 0.f);
	normals[2] = Vector(0.f, 1.f, 0.f);
	normals[3] = Vector(0.f, 1.f, 0.f);

	texcoords[0] = Vector2f(0, 0);
	texcoords[1] = Vector2f(0, 1);
	texcoords[2] = Vector2f(1, 1);
	texcoords[3] = Vector2f(1, 0);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	unsigned int numDistinctBones = 0;
	std::vector<int> VecBoneRemapper;

	SubMeshNode *pSubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());

	pSubMesh->InitializeVertexData(	GetRenderComponent()->GetRenderDriver(),
		(float*)vertices,
		(float*)normals,
		(float*)colors,
		NULL,
		NULL,
		indices,
		NumVertices,
		NumFaces,
		(float*)texcoords,
		1,
		false,
		NULL,
		0,
		VecBoneRemapper,
		numDistinctBones);

	std::string sFilename;
	if (FindFile(diffuseTexture, &sFilename))
	{
		m_pMaterialEffect->LoadDiffuseMap(sFilename.c_str(), false);
		m_pMaterialEffect->pMaterialEffect->fog = false;
	}

	D3DMesh *dx_mesh = m_pMeshComponent->GetD3DMesh();

	dx_mesh->Clear(true);

	CSceneMeshNode *pSceneMesh = new CSceneMeshNode(GetRenderComponent()->GetRenderDriver()); //AllocSceneMeshNode(sFilename.c_str());
	pSceneMesh->AddRef();

	pSceneMesh->GetSubMeshNodes().push_back(pSubMesh);

	dx_mesh->SetSceneMeshNode(pSceneMesh);

	dx_mesh->AddMesh(SMeshEffectPair(pSubMesh, m_pMaterialEffect->pMaterialEffect));

	// finally set mesh parameters
	// UGLY need research for correct parameters set
	dx_mesh->SetCastShadows(m_pMeshComponent->GetCastShadows());
	dx_mesh->SetReciveShadows(m_pMeshComponent->GetReciveShadows());

	delete [] texcoords;
	delete [] vertices;
	delete [] normals;
	delete [] colors;
	delete [] indices;
}