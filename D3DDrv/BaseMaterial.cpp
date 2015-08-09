#include "d3ddriverstdafx.h"

#undef D3DX_API
//----------------------------------------------------------------------------------------------
BaseMaterial::BaseMaterial(const class D3DDriver * Interface)
: D3DRenderInterface(Interface)
, m_pEffectNode(NULL)
, m_Reference("ogl_diffuse.vs")
, m_TechniqueName("Technique_Diffuse")
{
}

//----------------------------------------------------------------------------------------------
BaseMaterial::BaseMaterial(const class BaseMaterial& Source)
: D3DRenderInterface(Source) 
{
	if (this != &Source)
	{
		m_pEffectNode = NULL;
		m_Reference = Source.m_Reference;
		m_TechniqueName = Source.m_TechniqueName;
	}
}

//----------------------------------------------------------------------------------------------
BaseMaterial::~BaseMaterial()
{
}

//----------------------------------------------------------------------------------------------
// todo: specify NVI interface!
bool BaseMaterial::Load(const char *filepath)
{
	bool bResult = false;

	Release();

	if (filepath != "")
	{
		m_pEffectNode = m_pD3DInterface->LoadMaterialEffect(filepath);
		bResult = (m_pEffectNode != 0);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool BaseMaterial::LoadW(const wchar_t *filepath)
{
	bool bResult = false;

	Release();

	if (filepath != L"")
	{
		m_pEffectNode = m_pD3DInterface->LoadMaterialEffectW(filepath);
		bResult = (m_pEffectNode != 0);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool BaseMaterial::Release()
{
	long count = 0;

	if (m_pEffectNode)
	{
		m_pD3DInterface->UnregisterMaterialEffect(m_pEffectNode);
		m_pEffectNode = nullptr;
	}

	return (count == 0);
}

//----------------------------------------------------------------------------------------------
unsigned int BaseMaterial::BeginEffect(const char *Tehnique) const
{
	GLuint program = GetNode()->GetShaderProgram();

	glUseProgram(program);

	if (transperent)
	{
		glAlphaFunc(GL_GREATER, 0.2f);	// Set Alpha Testing     (disable blending)
		glEnable(GL_ALPHA_TEST);		// Enable Alpha Testing  (disable blending)
	}
	
	return 0;
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::EndEffect()
{
	if (transperent)
	{
		glDisable(GL_ALPHA_TEST);
	}

	GLuint program = GetNode()->GetShaderProgram();
	glUseProgram(0);
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::BeginPass(unsigned int Pass) const
{
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::EndPass() const
{
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::SetBoolParam(const char *ParamName, bool Value) const
{
	assert(m_pEffectNode);

	if (m_pEffectNode->IsParam_bool(ParamName))
	{
		GLint program = m_pEffectNode->GetShaderProgram();
		GLint InFog = glGetUniformLocation(program, ParamName);
		if (InFog != -1){
			glUniform1i(InFog, false);
		}
	}
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::SetMatrixParam(const char *ParamName, const float* Value, int index /*= -1*/) const
{
	assert(m_pEffectNode);

	if (m_pEffectNode->IsParam_float4x4(ParamName) ||
		m_pEffectNode->IsParam_float4x3(ParamName))
	{
		char buf[64];
		strcpy(buf, ParamName);

		if (index != -1)
		{
			char idxbuf[64];
			sprintf(idxbuf, "[%d]", index);
			strcat(buf, idxbuf);
		}
	}
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::SetVector3Param(const char *ParamName, const float *Value) const
{
	assert(m_pEffectNode);

	if (m_pEffectNode->IsParam_float3(ParamName))
	{
		GLint program = m_pEffectNode->GetShaderProgram();
		if (program != -1)
		{
			GLint id = glGetUniformLocation(program, ParamName);
			if (id != -1)
			{
				glUniform4f(id, *Value, *(Value+1), *(Value+2), 0.f);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::SetVector4Param(const char *ParamName, const float* Value) const
{
	assert(m_pEffectNode);

	if (m_pEffectNode->IsParam_float4(ParamName))
	{
		GLint program = m_pEffectNode->GetShaderProgram();
		if (program != -1)
		{
			GLint id = glGetUniformLocation(program, ParamName);
			if (id != -1)
			{
				glUniform4f(id, *Value, *(Value+1), *(Value+2), *(Value+3));
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::SetSampleTexture(const char *ParamName, const Texture2D *Value) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Value->GetTextureGL());

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::SetSampleTextureID(short Index, const class Texture2D * Value) const
{
	glActiveTexture(GL_TEXTURE0 + Index);
	glBindTexture(GL_TEXTURE_2D, Value->GetTextureGL());
}

//----------------------------------------------------------------------------------------------
void BaseMaterial::SetSampleTexture(const char *ParamName, const Texture3D *Value) const
{
	assert(m_pEffectNode);

	if (m_pEffectNode->IsParam_Texture(ParamName))
	{
	}
}

//----------------------------------------------------------------------------------------------
//void BaseMaterial::SetSampleTextureDirect(const char *ParamName, LPDIRECT3DTEXTURE9 Value) const
//{
//	assert(m_pEffectNode);
//	if (m_pEffectNode->IsParam_Texture(ParamName))
//	{
//	}
//}

//----------------------------------------------------------------------------------------------
void BaseMaterial::SetScalarValue(const char *ParamName, float Value) const
{
	assert(m_pEffectNode);

	if (m_pEffectNode->IsParam_float(ParamName))
	{
		GLint program = m_pEffectNode->GetShaderProgram();
		if (program != -1)
		{
			GLint InFogDensity = glGetUniformLocation(program, ParamName);
			if (InFogDensity != -1)
			{
				glUniform1f(InFogDensity, Value);
			}
		}
	}
}