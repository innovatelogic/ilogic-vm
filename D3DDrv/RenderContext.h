#ifndef __rendercontext_h__
#define __rendercontext_h__

#pragma once

#ifdef _WIN32
#pragma once
#pragma warning (disable:4251) 
#endif

#include "dxstdafx.h"

class D3DDRIVER_API SRenderContext
{
public:
	SRenderContext(class D3DDriver *pDriver);

	class TextureNode*			LoadTextureW(const wchar_t *URL);
	bool						UnregisterTexture(TextureNode *pNode);

	class MaterialEffectNode*	LoadMaterialEffectW(const wchar_t *URL);
	bool						UnregisterMaterialEffect(MaterialEffectNode *Node);

	class CFontNode*			LoadFontW(const char *Name, int Size);
	bool						UnregisterFont(const CFontNode* pFont);

	class CSceneMeshNode*		GetSceneMeshNode(const wchar_t *pURL);
	class CSceneMeshNode*		AllocSceneMeshNode(const wchar_t *pURL);
	bool						UnregisterSceneMeshNode(const CSceneMeshNode *pNode);

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
	UINT	m_displayModeWidth;
	UINT	m_displayModeHeight;

	HGLRC	m_hRC;				// Permanent Rendering Context
	GLuint	m_PixelFormat;		// Holds The Results After Searching For A Match

private:
	class D3DDriver					*m_pDriver;

	TMapTextureNodeW				m_MapTextureNodesW;
	TMapMaterialEffectsW			m_MapMaterialEffects;
	TVecFontNodes					m_VecFontNodes;
	TSceneMeshNodesW				m_MapSceneMeshNodes;
};

#endif//__rendercontext_h__