#ifndef __materialeffect_h__
#define __materialeffect_h__

#ifdef _WIN32
#pragma once
#pragma warning(disable: 4251)
#endif

#include "BaseMaterial.h"

class D3DDRIVER_API MaterialEffect : public BaseMaterial
{
public:
	MaterialEffect(const class D3DDriver *Interface);
	MaterialEffect(const MaterialEffect &Source);
	virtual ~MaterialEffect();
	
	virtual bool			Release();
	virtual void			Affect();

	/*const*/ class Texture2D*	GetDiffuseTexture() const;
	const class Texture2D*	GetNormalTexture() const;
	const class Texture2D*	GetHeightTexure() const;

	virtual bool			LoadDiffuseMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool			LoadNormalMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool			LoadHeightMap(const char *filename, bool ImmidiateLoad = true);

	float GetDiffuseU() const;
	float GetDiffuseV() const;
	float GetDiffuseUL() const;
	float GetDiffuseVL() const;

protected:
	class Texture2D			*DiffuseMap; // diffuse texture
	class Texture2D			*HeightMap;  // height texture
	class Texture2D			*NormalMap;  // normal map 
};

#endif//__materialeffect_h__