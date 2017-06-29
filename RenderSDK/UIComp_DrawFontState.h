#ifndef __uicomp_drawfontstate_h__
#define __uicomp_drawfontstate_h__

#ifdef WIN32
#pragma once
#endif

#include "RenderObject.h"

struct SFontState
{
	std::string			FontFamily;
	size_t				Height;
	unsigned int		Color;
    oes::d3d::CFont2D		*FontComponent;
    oes::d3d::CD3DFontMesh	*pMesh;
	class UIComp_DrawFontState * m_pOwner;

	bool	IsInitialized() const;
	bool	OnPrePropertyChange(const char *PropertyName);
	void	OnPropertyChanged(const char *PropertyName);

	SFontState()
		: FontFamily("Arial.ttf")
		, Height(12)
		, Color(0xff000000)
		, FontComponent(NULL)
		, pMesh(0)
		, m_pOwner(0)
	{
	}
};

class RENDERSDK_API UIComp_DrawFontState : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(UIComp_DrawFontState, CRenderObject);

	// 	ActiveState_Normal		= 0,
	// 	ActiveState_Active		= 1,
	// 	ActiveState_Focused		= 2,
	// 	ActiveState_Pressed		= 3,
	// 	ActiveState_Disabled	= 4,

public:
	UIComp_DrawFontState(const CObjectAbstract *pParent);
	UIComp_DrawFontState(const UIComp_DrawFontState &Source);
	virtual ~UIComp_DrawFontState();

	virtual void			OnPropertyChanged(const char *PropertyName);

	virtual void			Initialize();

	virtual void			Render() const;

	virtual void			SetActiveState(EActiveState State) { m_ActiveState = State; }
	virtual EActiveState	GetActiveState() const { return m_ActiveState; }

	virtual void			SetTextPositionX(float x) { TextPositionX = x; }
	virtual float			GetTextPositionX() const { return TextPositionX; }

	virtual void			SetTextPositionY(float y) { TextPositionY = y; }
	virtual float			GetTextPositionY() const { return TextPositionY; }

	class CComp_MaterialEffectFont* GetMaterialEffect() const { return m_pMaterialEffect; } 
	
	virtual void			ReinitStyles();
	virtual void			RebuildMesh(const std::wstring &text);

private:
	EActiveState m_ActiveState;

	TStaticArray<SFontState, ActiveState_MAX> ArrFontStates;

	class CComp_MaterialEffectFont	*m_pMaterialEffect;

	// test values
	float TextPositionX;
	float TextPositionY;
};

#endif//__uicomp_drawfontstate_h__