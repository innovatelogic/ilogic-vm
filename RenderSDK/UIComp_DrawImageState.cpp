#include "StdafxRender.h"

REGISTER_CLASS(UIComp_DrawImageState, CRenderObject);

namespace 
{
	// register child type
	volatile static NObjectFactory::CAuto<ImageState, UIComp_DrawImageState>
		ClassRegistrationImageState("ImageState", 0,  0, "ImageState", "UIComp_DrawImageState");
}

class AutoRegisterPropsUIComp_DrawImageState
{
public:
	AutoRegisterPropsUIComp_DrawImageState()
	{
		const Property_Base* Arr[] =
		{
			new PropertyString("XRef",
				(BYTE*)&((ImageState*)NULL)->XRef - (BYTE*)NULL,
				"ImageState",
				"Value",
				READ_WRITE,
				CTRL_EDIT,
				TRUE,
				false,
				0,
					new PropertyVector4f("UV",
						(BYTE*)&((ImageState*)NULL)->UV - (BYTE*)NULL,
						"ImageState",
						"Value",
						READ_WRITE,
						CTRL_EDIT,
						TRUE,
						false)
				)
			,
		};

		if (ARRAY_COUNT_SIZE(Arr))
		{
			unsigned int nSize = ARRAY_COUNT_SIZE(Arr);
			const Property_Base **pPropsBase = new const Property_Base*[nSize];
			for (unsigned int Index = 0; Index < nSize; ++Index)
			{
				*(pPropsBase + Index) = *(Arr + Index);
			}
			
			// fill registered type with sub childs
			volatile static NObjectFactory::CAuto<ImageState, void> Type_ImageState("ImageState", pPropsBase, nSize);

			// Scope Init2
			{
			const Property_Base *PropArray[] = {

				new PropertyArrayStatic("ImageStates",
					(BYTE*)&((UIComp_DrawImageState*)NULL)->ImageStates - (BYTE*)NULL,
					"UIComp_DrawImageState",
					"Value",
					const_cast<Property_Base*>(*pPropsBase),
					sizeof(ImageState),
					ActiveState_MAX,
					READ_WRITE,
					CTRL_ARRAY,
					TRUE,
					false,
					0, 
					const_cast<Property_Base*>(*pPropsBase)),
				};

				unsigned int nSize = ARRAY_COUNT_SIZE(PropArray);
				const Property_Base **pProps = new const Property_Base*[nSize];
				for (unsigned int Index = 0; Index < nSize; ++Index)
				{
					*(pProps + Index) = *(PropArray + Index);
				}
		
				volatile static NObjectFactory::CAuto<UIComp_DrawImageState, CRenderObject> Type_Array("UIComp_DrawImageState", pProps, nSize);
			}
		}
	}
}AutoRegAutoProps_UIComp_DrawImageState;

//----------------------------------------------------------------------------------------------
bool ImageState::IsInitialized() const 
{
	return (m_pOwner) ? m_pOwner->IsInitialized() : 0; 
}

//----------------------------------------------------------------------------------------------
bool ImageState::OnPrePropertyChange(const char* PropertyName) 
{ 
	bool bResult = true;
	if (m_pOwner) {
		bResult = m_pOwner->OnPrePropertyChange(PropertyName);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
void ImageState::OnPropertyChanged(const char* PropertyName)
{
	if (m_pOwner)
	{
		m_pOwner->OnPropertyChanged(PropertyName);
	}
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
UIComp_DrawImageState::UIComp_DrawImageState(const CObjectAbstract* Parent)
: Super(Parent)
, pMeshNode(NULL)
{
	NEW_OBJECT_TRANSIENT_CHILD(MeshComponent, Comp_StaticMesh, "UICompDrawImage_StaticMesh", this);
	NEW_OBJECT_TRANSIENT_CHILD(MaterialEffect, Comp_MaterialButton, "UICompDrawImage_MaterialButton", this);

	ImageStates.Elements[ActiveState_Normal].m_pOwner = this;
	ImageStates.Elements[ActiveState_Active].m_pOwner = this;
	ImageStates.Elements[ActiveState_Focused].m_pOwner = this;
	ImageStates.Elements[ActiveState_Pressed].m_pOwner = this;
	ImageStates.Elements[ActiveState_Disabled].m_pOwner = this;
}

//----------------------------------------------------------------------------------------------
UIComp_DrawImageState::UIComp_DrawImageState(const UIComp_DrawImageState &Source)
: Super(Source)
, pMeshNode(NULL)
{
	if (this != &Source)
	{
		MeshComponent = CLONE_OBJECT(Comp_StaticMesh, Source.MeshComponent, "UICompDrawImage_StaticMesh", this);
		MaterialEffect = CLONE_OBJECT(Comp_MaterialButton, Source.MaterialEffect, "UICompDrawImage_MaterialButton", this);
	}
}

//----------------------------------------------------------------------------------------------
UIComp_DrawImageState::~UIComp_DrawImageState()
{
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImageState::OnPropertyChanged(const char *PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "UV"))
	{
		MaterialEffect->SetUV(ImageStates.Elements[ActiveState_Normal].UV, ActiveState_Normal);
		MaterialEffect->SetUV(ImageStates.Elements[ActiveState_Active].UV, ActiveState_Active);
		MaterialEffect->SetUV(ImageStates.Elements[ActiveState_Focused].UV, ActiveState_Focused);
		MaterialEffect->SetUV(ImageStates.Elements[ActiveState_Pressed].UV, ActiveState_Pressed);
		MaterialEffect->SetUV(ImageStates.Elements[ActiveState_Disabled].UV, ActiveState_Disabled);

		RebuildMesh();
	}
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImageState::Initialize()
{
	Super::Initialize();

	RebuildMesh();
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImageState::DrawImage()
{
	GetRenderComponent()->DrawObject(MeshComponent);
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImageState::DoRebuildMesh()
{
	TVecIndexedPoint2f VertexList;
	std::vector<unsigned int> IndexList;

	VertexList.reserve(4);
	IndexList.reserve(6);

	IndexedPoint2f P0, P1, P2, P3;
	Vector4f UVs = Vector4f(0.f, 0.f, 1.f, 1.f);

	P0.Point = Vector2f(0.f, 0.f);
	P1.Point = Vector2f(0.f, 1.f);
	P2.Point = Vector2f(1.f, 1.f);
	P3.Point = Vector2f(1.f, 0.f);
 
	P3.UV = Vector2f(UVs.UL, UVs.VL);
	P2.UV = Vector2f(UVs.UL, UVs.V);
	P1.UV = Vector2f(UVs.U,	 UVs.V);
	P0.UV = Vector2f(UVs.U,	 UVs.VL);

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

	byte a = (0xffffffff & 0xFF000000) >> 24;
	byte r = (0xffffffff & 0x00FF0000) >> 16;
	byte g = (0xffffffff & 0x0000FF00) >> 8;
	byte b = (0xffffffff & 0x000000FF);

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

	// generate unique
	unsigned int numDistinctBones = 0;
	std::vector<int> VecBoneRemapper;

	SubMeshNode * SubMesh = new SubMeshNode(GetRenderComponent()->GetRenderDriver());
	//SubMesh->AddRef();

	SubMesh->InitializeVertexData(
		GetRenderComponent()->GetRenderDriver(),
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

	MaterialEffect->LoadDiffuseMap(ImageStates.Elements[ActiveState_Normal].XRef.c_str(), ActiveState_Normal);
	MaterialEffect->LoadDiffuseMap(ImageStates.Elements[ActiveState_Active].XRef.c_str(), ActiveState_Active);
	MaterialEffect->LoadDiffuseMap(ImageStates.Elements[ActiveState_Focused].XRef.c_str(), ActiveState_Focused);
	MaterialEffect->LoadDiffuseMap(ImageStates.Elements[ActiveState_Pressed].XRef.c_str(), ActiveState_Pressed);
	MaterialEffect->LoadDiffuseMap(ImageStates.Elements[ActiveState_Disabled].XRef.c_str(), ActiveState_Disabled);

	D3DMesh * dx_mesh = MeshComponent->GetD3DMesh();

	dx_mesh->Clear(true);
	dx_mesh->AddMesh(SMeshEffectPair(SubMesh, MaterialEffect->m_pMaterialEffect));

	delete [] texcoords;
	delete [] vertices;
	delete [] normals;
	delete [] colors;
	delete [] Indices;
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawImageState::SetActiveState(unsigned int StateIdx)
{
	MaterialEffect->SetActiveState(StateIdx);
}