#ifndef __uicomp_drawfont_h__
#define __uicomp_drawfont_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API UIComp_DrawFont : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(UIComp_DrawFont, CRenderObject);

public:
	UIComp_DrawFont(const CObjectAbstract *pParent);
	UIComp_DrawFont(const UIComp_DrawFont &Source);
	virtual ~UIComp_DrawFont();

	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual void			SetStyle(const std::string& strStyleInfo);
	virtual void			Initialize();

	virtual void			Render() const;

	virtual void			SetColor(unsigned int color) { m_Color = color; }
	virtual unsigned int	GetColor() const { return m_Color; }

	virtual void				 SetText(const std::wstring &text) { m_Value = text; }
	virtual const std::wstring&	 GetText() const { return m_Value; }

protected:
	virtual void			ReinitStyles();

private:
	std::string  m_FontFamily;
	size_t		 m_Height;
	unsigned int m_Color;

	std::wstring m_Value;

	class CFont2D* m_pFontComponent;
};

#endif//__uicomp_drawfont_h__