#include "StdafxRender.h"
#include "D3DFontMesh.h"
#include "FontNode.h"

class AutoRegisterProps
{
public:
	AutoRegisterProps()
	{
		const oes::rflex::Property_Base *Arr[] =
		{
			new oes::rflex::PropertyString("FontFamily",
				(BYTE*)&((SFontState*)NULL)->FontFamily - (BYTE*)NULL,
				"SFontState",
				"Value",
				READ_WRITE,
				CTRL_EDIT,
				TRUE,
				false,
				0,
					new oes::rflex::PropertyUINT("Height",
						(BYTE*)&((SFontState*)NULL)->Height - (BYTE*)NULL,
						"SFontState",
						"Value",
						READ_WRITE,
						CTRL_EDIT,
						TRUE,
						false,
						0,
							new oes::rflex::PropertyUINT("Color",
								(BYTE*)&((SFontState*)NULL)->Color - (BYTE*)NULL,
								"SFontState",
								"Value",
								READ_WRITE,
								CTRL_COLOR,
								TRUE,
								false)
						)
				),
		};

		if (ARRAY_COUNT_SIZE(Arr))
		{
			unsigned int nSize = ARRAY_COUNT_SIZE(Arr);
			const oes::rflex::Property_Base** pPropsBase = new const oes::rflex::Property_Base*[nSize];
			for (unsigned int Index = 0; Index < nSize; ++Index)
			{
				*(pPropsBase + Index) = *(Arr + Index);
			}

			// Register
			volatile static oes::rflex::CAuto<SFontState, void> Type_FontState("SFontState", pPropsBase, nSize);
		}
	}
}AutoRegisterPropsInst;

REGISTER_CLASS(UIComp_DrawFontState, CRenderObject);

class AutoRegisterPropsUIComp_DrawFontState
{
public:
	AutoRegisterPropsUIComp_DrawFontState()
	{
        oes::rflex::AppClassTree &classTree = oes::rflex::GetClassTree();
        oes::rflex::ClassNode* pFontState = classTree.Find("SFontState");

        oes::rflex::Property_Base** raw = pFontState->GetPropsRaw();

		const oes::rflex::Property_Base *PropArray[] = {

			new oes::rflex::PropertyArrayStatic("ArrFontStates",
			(BYTE*)&((UIComp_DrawFontState*)NULL)->ArrFontStates - (BYTE*)NULL,
			"UIComp_DrawFontState",
			"Value",
			0,
			sizeof(SFontState),
			ActiveState_MAX,
			READ_WRITE,
			CTRL_ARRAY,
			TRUE,
			false,
			0,
			*raw),
		};

		unsigned int nSize = ARRAY_COUNT_SIZE(PropArray);
		const oes::rflex::Property_Base** pProps = new const oes::rflex::Property_Base*[nSize];
		for (unsigned int Index = 0; Index < nSize; ++Index)
		{
			*(pProps + Index) = *(PropArray + Index);
		}

		volatile static oes::rflex::CAuto<UIComp_DrawFontState, CRenderObject> Type_Array("UIComp_DrawFontState", pProps, nSize);
	}
}AutoRegAutoProps_UIComp_DrawFontState;

//----------------------------------------------------------------------------------------------
bool SFontState::IsInitialized() const
{
	return (m_pOwner) ? m_pOwner->IsInitialized() : 0;
}

//----------------------------------------------------------------------------------------------
bool SFontState::OnPrePropertyChange(const char* PropertyName) 
{ 
	bool bResult = true;
	if (m_pOwner) {
		bResult = m_pOwner->OnPrePropertyChange(PropertyName);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
void SFontState::OnPropertyChanged(const char *PropertyName)
{
	if (m_pOwner){
		m_pOwner->OnPropertyChanged(PropertyName);
	}
}

//----------------------------------------------------------------------------------------------
UIComp_DrawFontState::UIComp_DrawFontState(const CObjectAbstract *Parent)
: Super(Parent)
, m_ActiveState(ActiveState_Normal)
, TextPositionX(0.f)
, TextPositionY(0.f)
{
	NEW_OBJECT_TRANSIENT_CHILD(m_pMaterialEffect, CComp_MaterialEffectFont, "MaterialEffectFont", this);

	for (unsigned int Index = 0; Index < ActiveState_MAX; ++Index)
	{
		ArrFontStates.Elements[Index].FontComponent = oes::d3d::Alloc_Font2D(GetRenderComponent()->GetRenderDriver());
		ArrFontStates.Elements[Index].pMesh = new oes::d3d::CD3DFontMesh(GetRenderComponent()->GetRenderDriver());
		ArrFontStates.Elements[Index].m_pOwner = this;
	}
}

//----------------------------------------------------------------------------------------------
UIComp_DrawFontState::UIComp_DrawFontState(const UIComp_DrawFontState &Source)
: Super(Source)
, m_ActiveState(ActiveState_Normal)
{
	if (this != &Source)
	{
	}
}

//----------------------------------------------------------------------------------------------
UIComp_DrawFontState::~UIComp_DrawFontState()
{
	// @TODO release
	for (unsigned int Index = 0; Index < ActiveState_MAX; ++Index)
	{
		Release_Font2D(ArrFontStates.Elements[Index].FontComponent);
	}
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawFontState::OnPropertyChanged(const char* PropertyName)
{
	//Super::OnPropertyChanged(PropertyName);
	
// 	if  (!strcmp(PropertyName, "FontFamily") ||
// 		 !strcmp(PropertyName, "Color") ||
// 		 !strcmp(PropertyName, "Height"))
// 	{
// 		ReinitStyles();
// 	}
	
	GetParent()->OnPropertyChanged(PropertyName); // Dirty hack
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawFontState::Initialize()
{
	Super::Initialize();

	ReinitStyles();
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawFontState::Render() const
{
    oes::d3d::D3DDriver* pDrv3D = GetRenderComponent()->GetRenderDriver();

	Matrix M;
	M.t = m_WorldMatrixTransform.t;

	pDrv3D->SetWorldMatrix(M.GetPtr());

    oes::d3d::BaseMaterial *pEffect = m_pMaterialEffect->GetPrtEffect();
	if (pEffect)
	{	
		pEffect->BeginEffect(0);

		GLuint program = pEffect->GetNode()->GetShaderProgram();

		float WVMat[16] = {0.f},
			  WVPMat[16] = {0.f};

		Matrix4Mul(WVMat, pDrv3D->m_world, pDrv3D->m_view);
		Matrix4Mul(WVPMat, WVMat, pDrv3D->m_proj);

		GLint modelViewProjectionMatrixLocation = glGetUniformLocation(program, "mVPMat");
		if (modelViewProjectionMatrixLocation != -1){
			glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, GL_TRUE,  WVPMat);
		}

		GLint modelViewMatrixLocation = glGetUniformLocation(program, "mVMat");
		if (modelViewMatrixLocation != -1){
			glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_TRUE,  WVMat);
		}

		GLint InColor = glGetUniformLocation(program, "InColor");
		if (InColor != -1){

			const unsigned int &Color = ArrFontStates.Elements[m_ActiveState].Color;

			byte r = (Color & 0x00FF0000) >> 16;
			byte g = (Color & 0x0000FF00) >> 8;
			byte b = (Color & 0x000000FF);

			glUniform4f(InColor, r / 255.f, g / 255.f, b / 255.f, 1.f);
		}

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, ArrFontStates.Elements[m_ActiveState].FontComponent->GetFontNode()->m_BitmapTexID);

		ArrFontStates.Elements[m_ActiveState].pMesh->Render();

		pEffect->EndEffect();
	}
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawFontState::ReinitStyles()
{
//	DWORD ThreadId = ::GetCurrentThreadId();
//	if (ThreadId != GetRenderComponent()->ThreadRenderID)
//	{
//		GetRenderComponent()->PushToRenderStack(new Command<UIComp_DrawFontState, UIComp_DrawFontState>(&UIComp_DrawFontState::ReinitStyles, this), true);
//	}
//	else
	{
		for (unsigned int Index = 0; Index < ActiveState_MAX; ++Index)
		{
			std::string sFilename;
			if (FindFile(ArrFontStates.Elements[Index].FontFamily.c_str(), &sFilename))
			{
				ArrFontStates.Elements[Index].FontComponent->Load(sFilename.c_str(), ArrFontStates.Elements[Index].Height);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void UIComp_DrawFontState::RebuildMesh(const std::wstring &text)
{
	size_t size = text.size();

	if (size > 0)
	{
		for (unsigned int Index = 0; Index < ActiveState_MAX; ++Index)
		{
			Vector			*vertices = new Vector[size * 4];			// array of vertices
			Vector2f		*uv  = new Vector2f[size * 4];
			unsigned int	*faces = new unsigned int [size * 6];

			std::wstring::const_iterator c = text.begin();
			std::wstring::const_iterator const temp_end = text.end();

            oes::d3d::CFont2D *pFont = ArrFontStates.Elements[Index].FontComponent;
            oes::d3d::CFontNode *pNode = pFont->GetFontNode();

			int advance = 0;
			int i = 0;
			int idx = 0;

			for (; c != temp_end; ++c)
			{
                oes::d3d::CFontNode::char_info_t *ci = &pNode->m_Info.ch[*c];

				vertices[i + 0].x = ci->left + advance + ci->v[0].x;
				vertices[i + 0].y = ci->v[0].y + (pNode->m_Info.max_height - ci->top);

				vertices[i + 1].x = ci->left + advance + ci->v[1].x;
				vertices[i + 1].y = ci->v[1].y + (pNode->m_Info.max_height - ci->top);

				vertices[i + 2].x = ci->left + advance + ci->v[2].x;
				vertices[i + 2].y = ci->v[2].y + (pNode->m_Info.max_height - ci->top);

				vertices[i + 3].x = ci->left + advance + ci->v[3].x;
				vertices[i + 3].y = ci->v[3].y + (pNode->m_Info.max_height - ci->top);

				faces[idx + 0] = i + 0;
				faces[idx + 1] = i + 3;
				faces[idx + 2] = i + 1;
				faces[idx + 3] = i + 1;
				faces[idx + 4] = i + 3;
				faces[idx + 5] = i + 2;

				advance += ci->advance;
				i += 4;
				idx += 6;
			}

			i = 0;
			c = text.begin();
	
			for (; c != temp_end; ++c)
			{
				const oes::d3d::CFontNode::char_info_t *ci = &pNode->m_Info.ch[*c];

				uv[i + 0].x = ci->uv[0].u;
				uv[i + 0].y = ci->uv[0].v;

				uv[i + 1].x = ci->uv[1].u;
				uv[i + 1].y = ci->uv[1].v;

				uv[i + 2].x = ci->uv[2].u;
				uv[i + 2].y = ci->uv[2].v;

				uv[i + 3].x = ci->uv[3].u;
				uv[i + 3].y = ci->uv[3].v;

				i += 4;
			}

			ArrFontStates.Elements[Index].pMesh->Release();
			ArrFontStates.Elements[Index].pMesh->InitializeVertexData((float*)vertices, (float*)uv, faces, size * 4, size * 6, 0);

			delete [] vertices;
			delete [] uv;
		}
	}
}