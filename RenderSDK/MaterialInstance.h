#ifndef __matinstance_h__
#define __matinstance_h__

#ifdef WIN32
#pragma once
#endif

class RENDERSDK_API CMaterialInstance
{
public:
	class MaterialEffect* GetMat() const { return m_pMaterialInstance; }

protected:
private:
	class MaterialEffect *m_pMaterialInstance;
};

#endif//__matinstance_h__