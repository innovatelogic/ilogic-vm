#include "d3ddriverstdafx.h"

struct SD3D_PixelFormat // DDPIXELFORMAT
{
	int dwSize;
	int dwFlags;
	int dwFourCC;
	int dwRGBBitCount;
	int dwRBitMask, dwGBitMask, dwBBitMask;
	int dwRGBAlphaBitMask;
};

struct SD3D_Caps2
{
	int dwCaps1;
	int dwCaps2;
	int Reserved[2];
};

struct SDDSURFACEDESC2
{
	int dwSize;
	int dwFlags;
	int dwHeight;
	int dwWidth;
	int dwPitchOrLinearSize;
	int dwDepth;
	int dwMipMapCount;
	int dwReserved1[11];
	SD3D_PixelFormat ddpfPixelFormat;
	SD3D_Caps2 ddsCaps;
	int dwReserved2;
};

//----------------------------------------------------------------------------------------------
TextureNode* D3DDriver::LoadTexture(const char *URL)
{
	// TODO: remove to separate func.
	int Length;
	int slength = (int)strlen(URL) + 1;
	Length = MultiByteToWideChar(CP_ACP, 0, URL, slength, 0, 0);
	wchar_t* buf = new wchar_t[Length];
	MultiByteToWideChar(CP_ACP, 0, URL, -1, buf, Length);
	std::wstring wstrUrl(buf);
	delete[] buf;

	SRenderContext *pContext = m_stackContext.top();

	assert(pContext);

	return pContext->LoadTextureW(wstrUrl.c_str());
}

//----------------------------------------------------------------------------------------------
TextureNode* D3DDriver::LoadTextureW(const wchar_t *URL)
{
	SRenderContext *pContext = m_stackContext.top();

	assert(pContext);

	return pContext->LoadTextureW(URL);
}

//----------------------------------------------------------------------------------------------
bool D3DDriver::UnregisterTexture(TextureNode *pNode)
{
	SRenderContext *pContext = pNode->GetOwnerContext();

	assert(pContext);
	
	return pContext->UnregisterTexture(pNode);
}

//----------------------------------------------------------------------------------------------
CFontNode* D3DDriver::LoadFont(const char *FaceName, int Height)
{
	SRenderContext *pContext = m_stackContext.top();

	assert(pContext);

	return pContext->LoadFontW(FaceName, Height);
}

//----------------------------------------------------------------------------------------------
bool D3DDriver::UnregisterFont(const CFontNode *pFont)
{
	SRenderContext *pContext = pFont->GetOwnerContext();
	
	assert(pContext);

	return pContext->UnregisterFont(pFont);
}

//----------------------------------------------------------------------------------------------
MaterialEffectNode*	D3DDriver::LoadMaterialEffect(const char *URL)
{
	// TODO: remove to separate func.
	int Length;
	int slength = (int)strlen(URL) + 1;
	Length = MultiByteToWideChar(CP_ACP, 0, URL, slength, 0, 0);
	wchar_t* buf = new wchar_t[Length];
	MultiByteToWideChar(CP_ACP, 0, URL, -1, buf, Length);
	std::wstring wstrUrl(buf);
	delete[] buf;

	SRenderContext *pContext = m_stackContext.top();
	assert(pContext);

	return pContext->LoadMaterialEffectW(wstrUrl.c_str());
}

//----------------------------------------------------------------------------------------------
MaterialEffectNode*	D3DDriver::LoadMaterialEffectW(const wchar_t * URL)
{
	SRenderContext *pContext = m_stackContext.top();
	assert(pContext);

	return pContext->LoadMaterialEffectW(URL);
}

//----------------------------------------------------------------------------------------------
bool D3DDriver::UnregisterMaterialEffect(MaterialEffectNode *pNode)
{
	SRenderContext *pContext = pNode->GetOwnerContext();

	assert(pContext);

	return pContext->UnregisterMaterialEffect(pNode);
}

//----------------------------------------------------------------------------------------------
CSceneMeshNode*	D3DDriver::GetSceneMeshNode(const char *pURL /*= 0*/)
{
	// TODO: remove to separate func.
	int Length = 0;
	int slength = (int)strlen(pURL) + 1;
	Length = MultiByteToWideChar(CP_ACP, 0, pURL, slength, 0, 0);
	wchar_t* buf = new wchar_t[Length];
	MultiByteToWideChar(CP_ACP, 0, pURL, -1, buf, Length);
	std::wstring wstrUrl(buf);
	delete[] buf;

	SRenderContext *pContext = m_stackContext.top();
	assert(pContext);

	return pContext->GetSceneMeshNode(wstrUrl.c_str());
}

//----------------------------------------------------------------------------------------------
CSceneMeshNode*	D3DDriver::GetSceneMeshNodeW(const wchar_t *pURL /*= 0*/)
{
	SRenderContext *pContext = m_stackContext.top();
	assert(pContext);

	return pContext->GetSceneMeshNode(pURL);
}

//----------------------------------------------------------------------------------------------
CSceneMeshNode* D3DDriver::AllocSceneMeshNode(const char *pURL /*= 0*/)
{
	// TODO: remove to separate func.
	int Length = 0;
	int slength = (int)strlen(pURL) + 1;
	Length = MultiByteToWideChar(CP_ACP, 0, pURL, slength, 0, 0);
	wchar_t* buf = new wchar_t[Length];
	MultiByteToWideChar(CP_ACP, 0, pURL, -1, buf, Length);
	std::wstring wstrUrl(buf);
	delete[] buf;

	return AllocSceneMeshNodeW(wstrUrl.c_str());
}

//----------------------------------------------------------------------------------------------
CSceneMeshNode* D3DDriver::AllocSceneMeshNodeW(const wchar_t *pURL /*= 0*/)
{
	SRenderContext *pContext = m_stackContext.top();
	
	assert(pContext);

	return pContext->AllocSceneMeshNode(pURL);
}

//----------------------------------------------------------------------------------------------
bool D3DDriver::UnregisterSceneMeshNode(const CSceneMeshNode *pNode)
{
	SRenderContext *pContext = pNode->GetOwnerContext();
	
	assert(pContext);

	return pContext->UnregisterSceneMeshNode(pNode);
}

//----------------------------------------------------------------------------------------------
SkeletonNode* D3DDriver::GetSkeletonNode(const char *filename)
{
	SkeletonNode * Node = NULL;

	TSkeletonNodes::const_iterator Iter = SkeletonNodes.find(filename);

	if (Iter != SkeletonNodes.end())
	{
		if (Iter->second->IsClear())
		{
			Node = Iter->second;
			Node->AddRef();
		}
		else
		{
			Iter->second->AddRef();
			return Iter->second;
		}
	}
	else
	{
		Node = new SkeletonNode();
		Node->AddRef();
	}

	if (Node->ReadMarkers(std::string(filename).substr(0, std::string(filename).rfind('.')) + ".mrk"))
	{
		std::ifstream stream;
		stream.open(filename, std::ios::in);

		if (stream.is_open())
		{
			Node->ReadFromStream(stream);
			stream.close();
		}
	}

	SkeletonNodes.insert(std::make_pair(filename, Node));
	return Node;
}

//----------------------------------------------------------------------------------------------
RenderDriver::RenderTargetNode* D3DDriver::CreateRenderTarget(unsigned int width, unsigned height)
{
	SRenderContext *pContext = m_stackContext.top();

	assert(pContext);

	return pContext->AllocRenderTarget(width, height);
}
//----------------------------------------------------------------------------------------------
void D3DDriver::ReleaseRenderTarget(RenderDriver::RenderTargetNode *rt)
{
	SRenderContext * const pContext = rt->GetOwnerContext();

	assert(pContext);

	pContext->FreeRenderTarget(rt);
}

//----------------------------------------------------------------------------------------------
bool D3DDriver::LoadUncompressedTGA(TextureNode * pOut, const wchar_t *URL, FILE *file)
{
	bool bResult = false;

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool D3DDriver::LoadCompressedTGA(TextureNode * pOut, const wchar_t *URL, FILE *file)
{
	bool bResult = false;

	return bResult;
}

//----------------------------------------------------------------------------------------------
SubMeshAgregator* D3DDriver::GetD3DMeshNode(const char * filename)
{
	SubMeshAgregator * pAgregator = NULL;

	TSubMeshAgregatorNodes::iterator Iter = SubMeshAgregatorNodes.find(filename);

	if (Iter != SubMeshAgregatorNodes.end())
	{
		pAgregator = Iter->second;
	}
	else
	{		
		pAgregator = new SubMeshAgregator(this);
	}

	return pAgregator;
}