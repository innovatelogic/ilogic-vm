#include "StdafxRender.h"

REGISTER_CLASS_A(UIComp_DrawImage, CRenderObject)
	new oes::rflex::PropertyString("ResourceTag", (BYTE*)&((UIComp_DrawImage*)NULL)->ResourceTag - (BYTE*)NULL, "UIComp_DrawImage",	"Value", READ_WRITE, CTRL_EDIT_RESOURCE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyVector4f("UV", (BYTE*)&((UIComp_DrawImage*)NULL)->UV - (BYTE*)NULL,	"UIComp_DrawImage",	"Value", READ_WRITE, CTRL_EDIT,	SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyUINT("Color", (BYTE*)&((UIComp_DrawImage*)NULL)->Color - (BYTE*)NULL, "UIComp_DrawImage", "Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP)
END_REGISTER_CLASS(UIComp_DrawImage, CRenderObject);

//----------------------------------------------------------------------------------------------
UIComp_DrawImage::UIComp_DrawImage(const CObjectAbstract * Parent)
: Super(Parent)
, Color(0xffffffff)
, ResourceTag("")
, UV(0.f, 0.f, 1.f, 1.f)
{
	NEW_OBJECT_TRANSIENT_CHILD(m_pMeshComponent, Comp_StaticMesh, "UICompDrawImage_StaticMesh", this);
	NEW_OBJECT_TRANSIENT_CHILD(MaterialEffect, Comp_MaterialEffectUI, "UICompDrawImage_MaterialEffect", this);
}

//----------------------------------------------------------------------------------------------
UIComp_DrawImage::UIComp_DrawImage(const UIComp_DrawImage &Source)
: Super(Source)
{
}

//----------------------------------------------------------------------------------------------
UIComp_DrawImage::~UIComp_DrawImage()
{
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImage::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "ResourceTag") ||
		!strcmp(PropertyName, "Color"))
	{
		RebuildMesh();
	}
	else if (!strcmp(PropertyName, "UV")){
		MaterialEffect->SetUV(UV);
	}
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImage::Initialize()
{
	Super::Initialize();

	RebuildMesh();
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImage::LoadImage(const wchar_t *filename, IListener *pListener /*= 0*/)
{
	DWORD ThreadId = ::GetCurrentThreadId();

	if (ThreadId != GetRenderComponent()->ThreadRenderID)
	{
		GetRenderComponent()->PushToRenderStack(new CommandL<UIComp_DrawImage>(&UIComp_DrawImage::LoadImage, filename, this, pListener), true);
	}
	else
	{
		bool bResult = MaterialEffect->LoadDiffuseMapW(filename);
		if (pListener)
		{
			if (bResult){
				pListener->OnOperationComplete(GetParent());
			}else{
				pListener->OnOperationFailed(GetParent());
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
bool UIComp_DrawImage::GetInfo(unsigned int &Width, unsigned int &Height) const 
{
	return MaterialEffect->GetInfo(Width, Height); 
}

//----------------------------------------------------------------------------------------------
bool UIComp_DrawImage::GetDIB(BYTE** pData, bool bResample /*= false*/, unsigned int NewWidth /*= 0*/, unsigned int NewHeight /*= 0*/) const
{
	return MaterialEffect->GetDIB(pData, bResample, NewWidth, NewHeight); 
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImage::SetStateUV(Vector4f &UV)
{
	MaterialEffect->SetUV(UV);
}

//----------------------------------------------------------------------------------------------
Vector4f UIComp_DrawImage::GetStateUV()
{
	return MaterialEffect->GetUV();
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImage::SetStateTag(std::string& ResourceTag)
{
	MaterialEffect->SetDiffuseReference(ResourceTag);
}

//----------------------------------------------------------------------------------------------
std::string UIComp_DrawImage::GetStateTag()
{
	return MaterialEffect->GetDiffuseReference();
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImage::DrawImage()
{
	m_pRenderSDK->DrawObject(m_pMeshComponent);
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImage::DoRebuildMesh()
{
	TVecIndexedPoint2f VertexList;
	std::vector<unsigned int> IndexList;
 
	IndexedPoint2f P0, P1, P2, P3;
 
	P0.Point = Vector2f(0.f, 0.f);
	P1.Point = Vector2f(0.f, 1.f);
	P2.Point = Vector2f(1.f, 1.f);
	P3.Point = Vector2f(1.f, 0.f);
 
	P3.UV = Vector2f(1.f, 1.f);
	P2.UV = Vector2f(1.f, 0.f);
	P1.UV = Vector2f(0.f, 0.f);
	P0.UV = Vector2f(0.f, 1.f);
 
	VertexList.push_back(P0);
	VertexList.push_back(P1);
	VertexList.push_back(P2);
	VertexList.push_back(P3);
 
	IndexList.push_back(2);
	IndexList.push_back(1);
	IndexList.push_back(0);
 
	IndexList.push_back(0);
	IndexList.push_back(3);
	IndexList.push_back(2);
	
	unsigned int NumVertices = (unsigned int)VertexList.size();
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
		vertices[Index] = Vector(VertexList[Index].Point.x, VertexList[Index].Point.y, 0.f);
		texcoords[Index].x = VertexList[Index].UV.x;
		texcoords[Index].y = VertexList[Index].UV.y;
		colors[Index] = Vector4f(r / 255.f, g / 255.f, b / 255.f, 1.f);
	}

	for (unsigned int Index = 0; Index < IndexList.size(); ++Index)
	{
		Indices[Index] = IndexList[Index];
	}

	unsigned int numDistinctBones = 0;
	std::vector<int> VecBoneRemapper;

	CSceneMeshNode *pSceneMesh = new CSceneMeshNode(GetRenderComponent()->GetRenderDriver()); //AllocSceneMeshNode(sFilename.c_str());
	pSceneMesh->AddRef();

	SubMeshNode	*SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());

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
	if (FindFile(ResourceTag.c_str(), &sFilename)){
		MaterialEffect->LoadDiffuseMap(sFilename.c_str(), false);
	}

	MaterialEffect->SetUV(UV);

	D3DMesh * dx_mesh = m_pMeshComponent->GetD3DMesh();

	dx_mesh->Clear(true);
	//dx_mesh->AddMesh(SMeshEffectPair(SubMesh, MaterialEffect->pMaterialEffectUI));

	pSceneMesh->GetSubMeshNodes().push_back(SubMesh);

	dx_mesh->SetSceneMeshNode(pSceneMesh);

	dx_mesh->AddMesh(SMeshEffectPair(SubMesh, MaterialEffect->pMaterialEffectUI));

	delete [] texcoords;
	delete [] vertices;
	delete [] normals;
	delete [] colors;
	delete [] Indices;
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImage::SetRenderTarget(Comp_RenderTarget *pRT)
{
	MaterialEffect->SetRenderTarget(pRT);
}