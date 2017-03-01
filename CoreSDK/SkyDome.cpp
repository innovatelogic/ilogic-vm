#include "coresdkafx.h"
#include "Comp_StaticMesh.h"
#include "Comp_MeshNode.h"
#include "../D3DDrv/D3DMesh.h"
#include "../D3DDrv/SubMeshNode.h"


REGISTER_CLASS_A(SkyDome, ActorAllocator)
	new oes::rflex::PropertyUINT("Detail", (BYTE*)&((SkyDome*)NULL)->Detail - (BYTE*)NULL, "SkyDome", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyString("DiffuseTex", (BYTE*)&((SkyDome*)NULL)->DiffuseTex - (BYTE*)NULL, "SkyDome",	"Value", READ_WRITE, CTRL_EDIT_RESOURCE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyUINT("Color", (BYTE*)&((SkyDome*)NULL)->Color - (BYTE*)NULL, "SkyDome",	"Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
ASSEMBLE_PROPS(SkyDome)
BEGIN_INTERFACE_DECLARATION(SkyDome)
		new oes::rflex::SInterfaceDecl("IDrawInterface", (BYTE*)&((SkyDome*)NULL)->___startObjectMarkerIDrawInterface - (BYTE*)NULL),
ASSEMBLE_INTERFACES(SkyDome)
CLASS_ENDL(SkyDome)
CLASS_INSTANCE_EX(SkyDome, ActorAllocator);

//----------------------------------------------------------------------------------------------
SkyDome::SkyDome(const CObjectAbstract *pParent)
: Super(pParent)
, IDrawInterface(pParent)
, Detail(16)
, DiffuseTex("")
, Color(0xffffffff)
, m_pMeshComponent(0)
{
	RegisterDrawInterface(this);

	NEW_OBJECT_TRANSIENT_CHILD(m_pMeshComponent, Comp_StaticMesh, "MeshComponent", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pMaterialEffect, Comp_MaterialEffect, "MaterialEffect", this);	
	
	RegisterRenderEntity(m_pMeshComponent);
}

//----------------------------------------------------------------------------------------------
SkyDome::~SkyDome()
{

}

//----------------------------------------------------------------------------------------------
void SkyDome::Initialize()
{
	Super::Initialize();

	DoRebuildMesh();
}

//----------------------------------------------------------------------------------------------
void SkyDome::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "Detail") ||
		!strcmp(PropertyName, "DiffuseTex") ||
		!strcmp(PropertyName, "Color"))
	{
		DoRebuildMesh();
	}
}

//----------------------------------------------------------------------------------------------
void SkyDome::DoRebuildMesh()
{
	unsigned int NumVertices = (unsigned int) Detail * (Detail - 1) + 1;
	unsigned int NumFaces = (unsigned int) (Detail * (Detail - 1)) * 2 + Detail;

	Vector * vertices = new Vector[NumVertices];	// array of vertices
	Vector * normals = new Vector[NumVertices];		// array of normals
	Vector4f * colors = new Vector4f[NumVertices];	// array of colors (vertex colors)
	Vector2f * texcoords = new Vector2f[NumVertices];
	unsigned int * Indices = new unsigned int[NumFaces * 3];

	assert(vertices && normals && colors && texcoords && Indices);

	byte a = (Color & 0xFF000000) >> 24;
	byte r = (Color & 0x00FF0000) >> 16;
	byte g = (Color & 0x0000FF00) >> 8;
	byte b = (Color & 0x000000FF);

	for (size_t Index1 = 0; Index1 < Detail; ++Index1) // vertical
	{
		for (size_t Index2 = 0; Index2 < Detail; ++Index2) // horizontal
		{
			float th = (float(Index1) / (Detail - 1)) * (0.5f * PI_CONST) - (0.5f * PI_CONST);
			float al = (float(Index2) / Detail) * (2.f * PI_CONST);
				
			vertices[Index1 * Detail + Index2] = Vector(sin(th) * sin(al), cos(th), sin(th) * cos(al));
			normals[Index1 * Detail + Index2] = -normalize(vertices[Index1 * Detail + Index2]) * 0.01f;
	
			texcoords[Index1 * Detail + Index2].x = (vertices[Index1 * Detail + Index2].x / 2.f) + 0.5f;
			texcoords[Index1 * Detail + Index2].y = (vertices[Index1 * Detail + Index2].z / 2.f) + 0.5f;
	
			colors[Index1 * Detail + Index2] = Vector4f(r / 255.f, g / 255.f, b / 255.f, 1.f);
	
			if (Index1 == Detail - 1){ // last
				break;
			}
		}
	}

	for (unsigned int Index = 0; Index < NumFaces; Index += 2)
	{
		int row = (int)(float(Index) / (Detail * 2));
		int baseIdx = Index / 2  - (row * Detail);
	
		if (row == (Detail - 2))
		{
			if (baseIdx == (Detail - 1)) // close cycle
			{
				// face 1 |/
				Indices[Index * 3 + 0] = baseIdx + (row * Detail);
				Indices[Index * 3 + 1] = NumVertices - 1;
				Indices[Index * 3 + 2] = (row * Detail);
				break;
			}
	
			// face 1 |/
			Indices[Index * 3 + 0] = baseIdx + (row * Detail);
			Indices[Index * 3 + 1] = NumVertices - 1;
			Indices[Index * 3 + 2] = (baseIdx + 1) + (row * Detail);
		}
		else
		{
			if (baseIdx == (Detail - 1)) // close cycle
			{
				// face 1 |/
				Indices[Index * 3 + 0] = baseIdx + (row * Detail);
				Indices[Index * 3 + 1] = baseIdx + ((row + 1) * Detail);
				Indices[Index * 3 + 2] = ((row + 1) * Detail);
		
				// face 2 /|
				Indices[Index * 3 + 3] = baseIdx + (row * Detail);
				Indices[Index * 3 + 4] = ((row + 1) * Detail);
				Indices[Index * 3 + 5] = (row * Detail);
		
				continue;
			}
		
			// face 1 |/
			Indices[Index * 3 + 0] = baseIdx + (row * Detail);
			Indices[Index * 3 + 1] = baseIdx + ((row + 1) * Detail);
			Indices[Index * 3 + 2] = (baseIdx + 1) + ((row + 1) * Detail);
		
			Indices[Index * 3 + 3] = baseIdx + (row * Detail);
			Indices[Index * 3 + 4] = (baseIdx + 1) + ((row + 1) * Detail);
			Indices[Index * 3 + 5] = (baseIdx + 1) + ((row) * Detail);
		}
	}

	unsigned int numDistinctBones = 0;
	std::vector<int> VecBoneRemapper;

	SubMeshNode *pSubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());

	pSubMesh->InitializeVertexData(	GetRenderComponent()->GetRenderDriver(),
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
	if (FindFile(DiffuseTex.c_str(), &sFilename))
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
	delete [] Indices;
}

//----------------------------------------------------------------------------------------------
void SkyDome::DoDraw()
{
	m_pRenderSDK->DrawObject(m_pMeshComponent);
}