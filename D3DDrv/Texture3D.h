#ifndef __texture3d_h__
#define __texture3d_h__

#ifdef WIN32
#pragma once
#pragma warning(disable: 4251)
#endif

#include "D3DRenderInterface.h"

class D3DDRIVER_API Texture3D : public D3DRenderInterface
{
	DECL_CLASS_SIMPLE(Texture3D, D3DRenderInterface);

public:
	Texture3D(const class D3DDriver * Interface);
	Texture3D(const Texture3D &Sender);
	virtual ~Texture3D();

	virtual bool	Load(const std::string & URL, unsigned int width, unsigned int height, unsigned int depth, bool ImmidiateLoad = true);
	virtual void	Release();

	//LPDIRECT3DVOLUMETEXTURE9	GetTexture(bool NoNull = true) const;

protected:
private:
	class VolumeTextureNode		*pVolumeTextureNode;
	std::string					Reference;
};

#endif//__texture3d_h__