#ifndef __volumetexturenode_h__
#define __volumetexturenode_h__

#ifdef WIN32
#pragma once
#endif

#include "Refcount.h"

class D3DDRIVER_API VolumeTextureNode : public Refcount
{

public:
	VolumeTextureNode();
	virtual ~VolumeTextureNode();

protected:
	virtual void DoRelease();

public:
	//LPDIRECT3DVOLUMETEXTURE9 VolumeTexture;
};

#endif//__volumetexturenode_h__