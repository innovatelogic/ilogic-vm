#ifndef __modelviewer_h__
#define __modelviewer_h__

#pragma once

#include "D3DDriver.h"
#include "ActorAllocator.h"
#include "ViewportInterface.h"
#include "IDrawInterface.h"
#include "src/Camera.h"
#include "include/PxCamera.h"


class CORESDK_API CModelViewer : public ActorAllocator, 
	public core_sdk_api::TIViewport,
	public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(CModelViewer, ActorAllocator);

public:
	//Constructor/Destructor
	CModelViewer(const CObjectAbstract *parent);
	virtual ~CModelViewer();

	virtual void	Initialize();

	virtual void	DoDraw();
	virtual bool	DoVisibilityTest_();

	oes::d3d::SRenderContext* GetRenderContext() const { return  m_pRenderContext; }

	bool GenerateObjectView(const wchar_t *pFilename);
	bool Save(const wchar_t *pFilename);

    class CCamera* GetCamera() const { return m_pCamera; }

protected:
private:
	virtual void	DrawGrid();

	void RebuildMesh(const char *diffuseTexture);

private:
    oes::d3d::SRenderContext		*m_pRenderContext;

	class CComp_EnvSceneInfo	*m_pEnvSceneInfo;
	CCamera                     *m_pCamera;

	class CActor				*m_pGenerated;

	// texture viewer components
	class Comp_StaticMesh		*m_pMeshComponent;
	class Comp_MaterialEffect	*m_pMaterialEffect;
};

#endif//__modelviewer_h__