#ifndef __font2d_h__
#define __font2d_h__

#ifdef _WIN32
#pragma once
#endif

#include "D3DRenderInterface.h"

class D3DDRIVER_API CFont2D : public D3DRenderInterface
{
	DECL_CLASS_SIMPLE(CFont2D, D3DRenderInterface);

public:
	CFont2D(const class D3DDriver *Interface);
	CFont2D(const CFont2D &Source);
	virtual ~CFont2D();

	virtual bool Load(const char *StrRef, int Height, BaseMaterial *pEffect = NULL);

	virtual void Release();
	virtual void Draw(const wchar_t *text, unsigned int color);

	class CFontNode* GetFontNode() const { return m_pFontNode; }

protected:
private:
	class CFontNode		*m_pFontNode;
	class BaseMaterial	*m_pEffect;
};

#endif//__font2d_h__