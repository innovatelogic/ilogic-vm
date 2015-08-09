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
SkeletonNode* D3DDriver::GetSkeletonNode(const char * filename)
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
RenderTargetNode* D3DDriver::CreateRenderTarget(unsigned int Width, unsigned Height)
{
	RenderTargetNode * Node = new RenderTargetNode(this);

	// Create depth-stencil buffer
// 	if (FAILED(m_pd3dDevice->CreateDepthStencilSurface( Width,
// 														Height,
// 														D3DFMT_D24S8/*(D3DFORMAT*)m_d3dSettings->PDeviceCombo()->pDepthStencilFormatList->GetPtr(0)*/,
// 														D3DMULTISAMPLE_NONE,
// 														0,
// 														TRUE,
// 														&Node->pSurfaceBuffer,
// 														NULL))) 

/*
	
	// create render target texture
	if (FAILED(m_pd3dDevice->CreateTexture( Width,
											Height,
											1,
											D3DUSAGE_RENDERTARGET,
											D3DFMT_A8R8G8B8,
											D3DPOOL_DEFAULT,
											&Node->pTextureMap,
											NULL))) 
	{
		Node->pTextureMap;
		MessageBox(NULL, TEXT("shadow map"), TEXT("ERROR: Create texture"), MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
	}

	D3DSURFACE_DESC desc;
	Node->pTextureMap->GetSurfaceLevel(0, &Node->pSurfaceBuffer);
	Node->pSurfaceBuffer->GetDesc(&desc);

	if (FAILED(D3DXCreateRenderToSurface(m_pd3dDevice, 
											desc.Width, 
											desc.Height, 
											desc.Format, 
											TRUE,
											D3DFMT_D16, 
											&Node->m_pRenderToSurface)))
	{
		Node->pSurfaceBuffer = NULL;
		MessageBox(NULL, TEXT("DepthStencil buffer"), TEXT("ERROR: Create DepthStencil buffer"), MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
	}

	(Node->pSurfaceBuffer && Node->pTextureMap) ? Node->AddRef() : Node->Release();
	RenderTargets.push_back(Node);
	*/
	return Node;
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