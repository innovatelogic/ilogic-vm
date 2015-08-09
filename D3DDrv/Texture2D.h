#ifndef __textuer2d_h__
#define __textuer2d_h__

#ifdef _WIN32
#pragma once
#pragma warning(disable: 4251)
#endif

#include "D3DRenderInterface.h"

class D3DDRIVER_API Texture2D : public D3DRenderInterface
{
	DECL_CLASS_SIMPLE(Texture2D, D3DRenderInterface);

	// defines the different types of textures
	typedef enum TextureType
	{
		CUSTOM              = 0x00000000,
		AMBIENT             = 0x00000001,
		DIFFUSE             = 0x00000002,
		SPECULAR            = 0x00000003, // specular color map
		SPECULAR_POWER      = 0x00000004, // specular highlight map
		GLOSS               = 0x00000005,
		SELF_ILLUMATION     = 0x00000006,
		BUMP                = 0x00000007,
		NORMAL              = 0x00000008,
		OPACITY             = 0x00000009,
		REFLECTION          = 0x0000000A,
		REFRACTION          = 0x0000000B,
		CUBE_MAP            = 0x0000000C
	} TexType;

public:
	Texture2D(const class D3DDriver *Interface);
	Texture2D(const Texture2D &Sender);
	virtual ~Texture2D();

	virtual bool		Load(const char *filepath, bool ImmidiateLoad = true);
	virtual bool		LoadW(const wchar_t *filepath);
	
	virtual void		Release();

	virtual void		SetReference(const std::string &Ref) { Reference = Ref; }
	virtual std::string	GetReference() const { return Reference; }

	virtual void		 SetColor(unsigned int color) { Color = color; }
	virtual unsigned int GetColor() { return Color; }

	GLuint				GetTextureGL(bool NoNull = true) const;

	void SetTexcoords(float U, float V, float UL, float VL);

   float GetU()		{ return m_Texcoord.x; }
   float GetV()		{ return m_Texcoord.y; }
   float GetUL()	{ return m_Texcoord.z; }
   float GetVL()	{ return m_Texcoord.w; }

   bool GetInfo(unsigned int &Width, unsigned int &Height) const;
   bool GetDIB(BYTE** pData, bool bResample = false, unsigned int NewWidth = 0, unsigned int NewHeight = 0) const;

private:

	unsigned int		Color;
    std::string         Reference;
	SVector4f			m_Texcoord;

	class TextureNode * m_pTextureNode;
};

#endif//__textuer2d_h__