#ifndef __uicomp_drawimage_h__
#define __uicomp_drawimage_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API UIComp_DrawImage : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(UIComp_DrawImage, CRenderObject);

public:
	UIComp_DrawImage(const CObjectAbstract *pParent);
	UIComp_DrawImage(const UIComp_DrawImage &Source);
	virtual ~UIComp_DrawImage();

	/** 
	 * Calls in main thread every time some property was changed.
	 * 
	 * @param PropertyName [in] current property name have change.
	 */
	virtual void	OnPropertyChanged(const char* PropertyName);
	virtual void	Initialize();

	/** 
	 * Calls in main thread every time some property was changed.
	 * 
	 * @param filename [in] filename path.
	 * @param pListener [in] ptr to listener will receive events.
	 */
	void LoadImage(const wchar_t *filename, IListener *pListener = 0);

	bool GetInfo(unsigned int &Width, unsigned int &Height) const;
	bool GetDIB(BYTE** pData, bool bResample = false, unsigned int NewWidth = 0, unsigned int NewHeight = 0) const;

	/** properties*/
	void			SetStateUV(Vector4f &UV);
	Vector4f		GetStateUV();
	void			SetStateTag(std::string& ResourceTag);
	std::string		GetStateTag();
	void			DrawImage();
	
	void			SetResourceTag(std::string& Tag) { ResourceTag = Tag; }
	std::string		GetResourceTag() const { return ResourceTag; }

	void			SetUV(Vector4f &uv) { UV = uv; }
	Vector4f		GetUV() const { return UV; }

	void			SetColor(unsigned int color) { Color = color; }
	unsigned int	GetColor() { return Color; }

	Comp_StaticMesh*		GetStaticMesh() const { return m_pMeshComponent; }
	Comp_MaterialEffectUI*	GetMaterialEffect() const { return MaterialEffect; } 

	void			SetRenderTarget(Comp_RenderTarget *pRT);

protected:
	virtual void	DoRebuildMesh();

protected:
	/** mesh data*/
	class Comp_StaticMesh		*m_pMeshComponent;
	class Comp_MaterialEffectUI	*MaterialEffect;

	unsigned int				Color;
	std::string					ResourceTag;
	Vector4f					UV;
};

#endif//__uicomp_drawimage_h__

