#include "d3ddriverstdafx.h"

//-----------------------------------------------------------------------
Texture2D::Texture2D(const D3DDriver * Interface)
: Super(Interface)
, m_pTextureNode(0)
, Reference("")
, Color(0xffffffff)
//, m_Texcoord(0.f, 0.f, 1.f, 1.f)
{
	;//
}

//-----------------------------------------------------------------------
Texture2D::Texture2D(const Texture2D &Sender)
: Super(Sender)
, m_pTextureNode(NULL)
{
	if (this != &Sender)
	{
		Color		= Sender.Color;
		Reference	= Sender.Reference;
		m_Texcoord	= Sender.m_Texcoord;
	}
}

//-----------------------------------------------------------------------
Texture2D::~Texture2D()
{
	Release();
}

//-----------------------------------------------------------------------
bool Texture2D::Load(const char *filepath, bool ImmidiateLoad /*= true*/)
{  
	bool bResult = false;

	Release(); // release previously

	if (filepath != "")
	{
		m_pTextureNode = m_pD3DInterface->LoadTexture(filepath);
		bResult = (m_pTextureNode != 0);
	}

	return bResult;
}

//-----------------------------------------------------------------------
bool Texture2D::LoadW(const wchar_t *filepath)
{  
	bool bResult = false;

	Release(); // release previously

	if (filepath != L"")
	{
		m_pTextureNode = m_pD3DInterface->LoadTextureW(filepath);
		bResult = (m_pTextureNode != 0);
	}

	return bResult;
}

//-----------------------------------------------------------------------
void Texture2D::Release()
{
	if (m_pTextureNode)
	{
		m_pD3DInterface->UnregisterTexture(m_pTextureNode);
		m_pTextureNode = nullptr;
	}
}


//----------------------------------------------------------------------------------------------
GLuint Texture2D::GetTextureGL(bool NoNull /*= true*/) const
{
	GLuint OutID = (m_pTextureNode != NULL) ? m_pTextureNode->m_pSTextureBitmap.texID : 0;

	return (OutID != 0) ? OutID : ((!NoNull) ? 0 : m_pD3DInterface->m_pTextureDef->m_pSTextureBitmap.texID);
}

//----------------------------------------------------------------------------------------------
void Texture2D::SetTexcoords(float U, float V, float UL, float VL)
{ 
	m_Texcoord.x = U > 1.f ? (U / m_pTextureNode->m_TGADesc.Width) : U;
	m_Texcoord.y = V > 1.f ? (V / m_pTextureNode->m_TGADesc.Height) : V;
	m_Texcoord.z = UL > 1.f ? (UL / m_pTextureNode->m_TGADesc.Width) : UL;
	m_Texcoord.w = VL > 1.f ? (VL / m_pTextureNode->m_TGADesc.Height) : VL;
}

//----------------------------------------------------------------------------------------------
bool Texture2D::GetInfo(unsigned int &Width, unsigned int &Height) const
{
	return (m_pTextureNode != NULL) ? m_pTextureNode->GetInfo(Width, Height) : false; 
}

//----------------------------------------------------------------------------------------------
bool Texture2D::GetDIB(BYTE** pData, bool bResample /*= false*/, unsigned int NewWidth /*= 0*/, unsigned int NewHeight /*= 0*/) const
{
	return (m_pTextureNode != NULL) ? m_pTextureNode->GetDIB(pData, bResample, NewWidth, NewHeight) : false; 
}
