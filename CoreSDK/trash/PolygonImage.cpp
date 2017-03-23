#include "coresdkafx.h"
#include "Comp_StaticMesh.h"
#include "Comp_MeshNode.h"
#include "../D3DDrv/D3DMesh.h"
#include "../D3DDrv/SubMeshNode.h"

REGISTER_CLASS_A(CPolygonImage, ActorAllocator)
	new oes::rflex::PropertyString("m_sDiffuseTex", (BYTE*)&((CPolygonImage*)NULL)->m_sDiffuseTex - (BYTE*)NULL, "CPolygonImage", "Value", READ_WRITE, CTRL_EDIT_RESOURCE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(CPolygonImage, ActorAllocator);

//----------------------------------------------------------------------------------------------
CPolygonImage::CPolygonImage(const CObjectAbstract * Parent)
: Super(Parent)
, IDrawInterface(Parent)
{
	RegisterDrawInterface(this);

	NEW_OBJECT_TRANSIENT_CHILD(m_pMeshComponent, Comp_StaticMesh, "MeshComponent", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pMaterialEffect, Comp_MaterialEffect, "UICompDrawImage_MaterialEffect", this);
}

//----------------------------------------------------------------------------------------------
CPolygonImage::~CPolygonImage()
{

}

//----------------------------------------------------------------------------------------------
void CPolygonImage::Initialize()
{
	Super::Initialize();

	DoRebuildMesh();
}

//----------------------------------------------------------------------------------------------
void CPolygonImage::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "m_sDiffuseTex"))
	{
		DoRebuildMesh();
	}
}

//----------------------------------------------------------------------------------------------
void CPolygonImage::DoDraw()
{
	m_pMeshComponent->SetRWTM(GetTransformedWTM_());
	m_pRenderSDK->DrawObject(m_pMeshComponent);
}

//----------------------------------------------------------------------------------------------
bool CPolygonImage::ProcessController(const MouseMoveInputData &InputData)
{
    return false; //IDrawInterface::ProcessController(InputData);
}

//----------------------------------------------------------------------------------------------
void CPolygonImage::DoRebuildMesh()
{
	unsigned int NumVertices = 4;
	unsigned int NumFaces = 2;

	Vector * vertices = new Vector[NumVertices];	// array of vertices
	Vector * normals = new Vector[NumVertices];		// array of normals
	Vector4f * colors = new Vector4f[NumVertices];	// array of colors (vertex colors)
	Vector2f * texcoords = new Vector2f[NumVertices];
	unsigned int * Indices = new unsigned int[NumFaces * 3];


	vertices[0] = Vector(-0.5f, -0.5f, 0.f);
	vertices[1] = Vector(-0.5f, 0.5f, 0.f);
	vertices[2] = Vector(0.5f, 0.5f, 0.f);
	vertices[3] = Vector(0.5f, -0.5f, 0.f);

	normals[0] = Vector(0.f, 0.f, 1.f);
	normals[1] = Vector(0.f, 0.f, 1.f);
	normals[2] = Vector(0.f, 0.f, 1.f);
	normals[3] = Vector(0.f, 0.f, 1.f);

	texcoords[0] = Vector2f(0.f, 0.f);
	texcoords[1] = Vector2f(0.f, 1.f);
	texcoords[2] = Vector2f(1.f, 1.f);
	texcoords[3] = Vector2f(1.f, 0.f);

	colors[0] = Vector4f(1.f, 1.f, 1.f, 1.f);
	colors[1] = Vector4f(1.f, 1.f, 1.f, 1.f);
	colors[2] = Vector4f(1.f, 1.f, 1.f, 1.f);
	colors[3] = Vector4f(1.f, 1.f, 1.f, 1.f);

	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;

	Indices[3] = 0;
	Indices[4] = 2;
	Indices[5] = 3;

	unsigned int numDistinctBones = 0;
	std::vector<int> VecBoneRemapper;

	SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());

	SubMesh->InitializeVertexData(GetRenderComponent()->GetRenderDriver(),
		(float*)vertices,
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

	std::string sFilename;
	if (FindFile(m_sDiffuseTex.c_str(), &sFilename)){
		m_pMaterialEffect->LoadDiffuseMap(sFilename.c_str(), false);
	}

	D3DMesh * dx_mesh = m_pMeshComponent->GetD3DMesh();

	dx_mesh->Clear(true);
	dx_mesh->AddMesh(SMeshEffectPair(SubMesh, m_pMaterialEffect->pMaterialEffect));

	// finally set mesh parameters
	// UGLY need research for correct parameters set
	dx_mesh->SetCastShadows(m_pMeshComponent->GetCastShadows());
	dx_mesh->SetReciveShadows(m_pMeshComponent->GetReciveShadows());

	delete [] texcoords;
	delete [] vertices;
	delete [] normals;
	delete [] colors;
	delete [] Indices;
}
