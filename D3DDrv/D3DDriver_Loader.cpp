#include "d3ddriverstdafx.h"
#include <locale>
#include <codecvt>

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

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        TextureNode* D3DDriver::LoadTexture(const char *URL)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wide = converter.from_bytes(URL);

            try
            {
                SRenderContext *pContext = m_stackContext.top();
                return pContext->LoadTextureW(wide.c_str());
            }
            catch (...)
            {
                return nullptr;
            }
        }

        //----------------------------------------------------------------------------------------------
        TextureNode* D3DDriver::LoadTextureW(const wchar_t *URL)
        {
            try
            {
                SRenderContext *pContext = m_stackContext.top();
                return pContext->LoadTextureW(URL);
            }
            catch (...)
            {
                return nullptr;
            }  
        }

        //----------------------------------------------------------------------------------------------
        TextureNode* D3DDriver::LoadTextureFromPixels32(
            unsigned int *pixels,
            unsigned int imgWidth,
            unsigned int imgHeight,
            unsigned int texWidth,
            unsigned int texHeight)
        {
            try
            {
                SRenderContext *pContext = m_stackContext.top();
                return pContext->LoadTextureFromPixels32(pixels,
                    imgWidth,
                    imgHeight,
                    texWidth,
                    texHeight);
            }
            catch (...)
            {
                return nullptr;
            }
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
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wide = converter.from_bytes(URL);

            SRenderContext *pContext = m_stackContext.top();
            assert(pContext);

            return pContext->LoadMaterialEffectW(wide.c_str());
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
        CSceneMeshNode*	D3DDriver::GetSceneMeshNode(const char *URL /*= 0*/)
        {
            // TODO: remove to separate func.
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wide = converter.from_bytes(URL);

            SRenderContext *pContext = m_stackContext.top();
            assert(pContext);

            return pContext->GetSceneMeshNode(wide.c_str());
        }

        //----------------------------------------------------------------------------------------------
        CSceneMeshNode*	D3DDriver::GetSceneMeshNodeW(const wchar_t *pURL /*= 0*/)
        {
            SRenderContext *pContext = m_stackContext.top();
            assert(pContext);

            return pContext->GetSceneMeshNode(pURL);
        }

        //----------------------------------------------------------------------------------------------
        CSceneMeshNode* D3DDriver::AllocSceneMeshNode(const char *URL /*= 0*/)
        {
            // TODO: remove to separate func.
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wide = converter.from_bytes(URL);

            return AllocSceneMeshNodeW(wide.c_str());
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

            if (Iter != SubMeshAgregatorNodes.end()){
                pAgregator = Iter->second;
            }else{
                pAgregator = new SubMeshAgregator(this);
            }

            return pAgregator;
        }
    }
}