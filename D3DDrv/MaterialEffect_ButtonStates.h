#ifndef __materialeffect_buttonstates_h__
#define __materialeffect_buttonstates_h__

#ifdef WIN32
#pragma once
#endif

#include "BaseMaterial.h"

class D3DDRIVER_API MaterialEffect_ButtonStates : public BaseMaterial
{
public:
	MaterialEffect_ButtonStates(const class D3DDriver * Interface);
	virtual ~MaterialEffect_ButtonStates();

	virtual bool			Release();
	virtual void			Affect();

	virtual bool			LoadDiffuseMap(const char *filename, unsigned int StateIdx);

	/*const*/ class Texture2D*	GetDiffuseTexture(unsigned int StateIdx) const;

	virtual void			SetActiveIdx(unsigned int Idx);

protected:
private:
	std::vector<class Texture2D*> DiffuseMaps; // diffuse texture
	unsigned int StateIndex;
};

#endif//__materialeffect_buttonstates_h__