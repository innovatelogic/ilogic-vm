#ifndef __materialeffectlava_h__
#define __materialeffectlava_h__

#ifdef WIN32
#pragma once
#endif

#include "MaterialEffect.h"

class D3DDRIVER_API MaterialEffectLava : public BaseMaterial
{
public:
	MaterialEffectLava(const class D3DDriver * Interface);
	virtual ~MaterialEffectLava();

	virtual void			Affect();

	virtual bool			LoadDiffuseMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool			LoadNormalMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool			LoadHeightMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool			LoadNoiseMap(const char *filename, bool ImmidiateLoad = true);

protected:
	class Texture2D			*DiffuseMap; // diffuse texture
	class Texture2D			*HeightMap;  // height texture
	class Texture2D			*NormalMap;  // normal map
	class Texture3D			*NoiseMap;

private:
	float	noiseScale;
	float	morphRate;
	float	bubbleRate;
	float	bubbleScale;
	float	scale;
	float	bias;
};

#endif//__materialeffectlava_h__