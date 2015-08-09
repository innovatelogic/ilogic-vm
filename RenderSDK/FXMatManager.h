#ifndef __fxmatmanager_h__
#define __fxmatmanager_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API CFXMatManager
{
public:
	CFXMatManager(class CRenderSDK *pRenderSDK);
	virtual ~CFXMatManager();

	void Release();

	class MaterialEffect*	GetMaterialInstance(const char *Name);
	void					ReleaseMaterial(class MaterialEffect *Inst);

protected:
	bool Load(const char *Path, m_material &OutMaterial);

private:
	std::map<std::string, m_material> m_MapMatPrototypes;

	class CRenderSDK *m_pRenderSDK;
};

#endif//__fxmatmanager_h__