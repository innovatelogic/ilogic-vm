
#pragma once

#ifdef _WIN32
#pragma warning (disable:4251) 
#endif

#include "dxstdafx.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;
        class TextureNode;
        class MaterialEffectNode;
        class CFontNode;
        class CSceneMeshNode;

        namespace RenderDriver {
            class RenderTargetNode;
        }

        typedef std::map<std::wstring, CSceneMeshNode*> TSceneMeshNodesW;
        typedef std::map<std::wstring, MaterialEffectNode*> TMapMaterialEffectsW;
        typedef std::map<std::wstring, TextureNode*> TMapTextureNodeW;
        typedef std::vector<CFontNode*> TVecFontNodes;

        class D3DDRIVER_API SRenderContext final
        {
        public:
            SRenderContext(D3DDriver *pDriver);
            ~SRenderContext();

            TextureNode*    LoadTextureW(const wchar_t *url);
            TextureNode*    LoadTextureFromPixels32(
                unsigned int *pixels,
                unsigned int imgWidth,
                unsigned int imgHeight,
                unsigned int texWidth,
                unsigned int texHeight);
            
            bool    UnregisterTexture(TextureNode *node);

            MaterialEffectNode*		LoadMaterialEffectW(const wchar_t *URL);
            bool					UnregisterMaterialEffect(MaterialEffectNode *Node);

            CFontNode*				LoadFontW(const char *Name, int Size);
            bool					UnregisterFont(const CFontNode* pFont);

            CSceneMeshNode*			GetSceneMeshNode(const wchar_t *pURL);
            CSceneMeshNode*			AllocSceneMeshNode(const wchar_t *pURL);
            bool					UnregisterSceneMeshNode(const CSceneMeshNode *pNode);

            RenderDriver::RenderTargetNode* AllocRenderTarget(unsigned int width, unsigned int height);
            void							FreeRenderTarget(RenderDriver::RenderTargetNode *rt);

            bool    GetWireframeMode() const { return m_bWireframe; }
            void    SetWireframeMode(bool flag) { m_bWireframe = flag; }

        private:
            bool LoadTGAW(TextureNode *pNode, const wchar_t *filename);
            //void make_dlist(FT_Face face, char ch, GLuint list_base, GLuint *tex_base);
            //void fill_texture_data(uint32_t ch, class CFontNode::font_info_t *font, uint32_t texture_width, uint8_t *texture_data);

            bool LoadUncompressedTGA(TextureNode *pNode, FILE *fTGA);
            bool LoadCompressedTGA(TextureNode *pNode, FILE *fTGA);

            bool LoadFile(const wchar_t *fileName, bool binary, uint8_t **buffer, uint32_t *size);
            GLint ShaderStatus(GLuint shader, GLenum param);

        public:
            HWND	m_hWnd;
            HDC		m_hDC;				// Private GDI Device Context
            size_t	m_displayModeWidth;
            size_t	m_displayModeHeight;

            HGLRC	m_hRC;				// Permanent Rendering Context
            GLuint	m_PixelFormat;		// Holds The Results After Searching For A Match

            // render settings
            bool    m_bWireframe;

        private:
            D3DDriver					    *m_pDriver;

            TMapTextureNodeW				m_MapTextureNodesW;
            TMapMaterialEffectsW			m_MapMaterialEffects;
            TVecFontNodes					m_VecFontNodes;
            TSceneMeshNodesW				m_MapSceneMeshNodes;

            std::vector<RenderDriver::RenderTargetNode*> m_renderTargets;
        };
    }
}