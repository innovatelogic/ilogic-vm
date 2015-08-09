#include "coresdkafx.h"

REGISTER_CLASS(CScene, ActorAllocator);

//----------------------------------------------------------------------------------------------
CScene::CScene(const CObjectAbstract *Parent)
: Super(Parent)
, IDrawInterface(Parent)
{
	RegisterDrawInterface(this);

	NEW_OBJECT_TRANSIENT_CHILD(m_pEnvSceneInfo, CComp_EnvSceneInfo, "EnvSceneInfo", this);
}

//----------------------------------------------------------------------------------------------
CScene::CScene(const CScene &Source)
: Super(Source)
, IDrawInterface(Source)
{
	if (this != &Source)
	{
	}
}

//----------------------------------------------------------------------------------------------
CScene::~CScene()
{

}

//----------------------------------------------------------------------------------------------
void CScene::Initialize()
{
	Super::Initialize();

	GetAppMain()->GetViewportManager()->RebuildTransform(this);
}

//------------------------------------------------------------------------
void CScene::DoDraw()
{
	RenderQuevueAdjacency& Queve = m_pRenderSDK->GetCurrQuevueAdjaency();

	// setup fog params
	Queve.bFog = m_pEnvSceneInfo->m_bFog;
	Queve.fFogMin = m_pEnvSceneInfo->m_fFogMin;
	Queve.fFogMax = m_pEnvSceneInfo->m_fFogMax;
	Queve.fFogDensity =	m_pEnvSceneInfo->m_fFogDensity;
	Queve.FogColor = m_pEnvSceneInfo->m_FogColor;
}
