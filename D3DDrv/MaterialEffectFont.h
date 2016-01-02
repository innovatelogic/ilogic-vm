#ifndef __materialeffectfont_h__
#define __materialeffectfont_h__

#ifdef WIN32
#pragma once
#endif

#include "BaseMaterial.h"

class D3DDRIVER_API CMaterialEffectFont : public BaseMaterial
{
public:
	CMaterialEffectFont(const class D3DDriver *pInterface);
	virtual ~CMaterialEffectFont();

	virtual	bool	Release();
	virtual void	Affect();

protected:
private:
};

#endif//__materialeffectui_h__