#ifndef __uicomp_drawimagestate_h__
#define __uicomp_drawimagestate_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

struct ImageState
{
	std::string		XRef;
	Vector4f		UV;
	class UIComp_DrawImageState * m_pOwner;

	bool IsInitialized() const;
	bool OnPrePropertyChange(const char* PropertyName);
	void OnPropertyChanged(const char* PropertyName);

	ImageState()
		: XRef("")
		, UV(0.f, 0.f, 1.f, 1.f)
		, m_pOwner(0)
	{
	}
};

class RENDERSDK_API UIComp_DrawImageState : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(UIComp_DrawImageState, CRenderObject);

	// 	ActiveState_Normal		= 0,
	// 	ActiveState_Active		= 1,
	// 	ActiveState_Focused		= 2,
	// 	ActiveState_Pressed		= 3,
	// 	ActiveState_Disabled	= 4,

public:
	UIComp_DrawImageState(const CObjectAbstract* Parent);
	UIComp_DrawImageState(const UIComp_DrawImageState &Source);
	virtual ~UIComp_DrawImageState();

	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual void			Initialize();

	virtual void			DrawImage();
	virtual void			SetActiveState(unsigned int StateIdx);

	Comp_StaticMesh*		GetStaticMesh() const { return MeshComponent; }

protected:
	virtual void			DoRebuildMesh();

private:
protected:
	class Comp_StaticMesh		*MeshComponent;
	class Comp_MaterialButton	*MaterialEffect;

	Comp_MeshNode				*pMeshNode;

	TStaticArray<ImageState, ActiveState_MAX> ImageStates;
};

#endif//__uicomp_drawimagestate_h__