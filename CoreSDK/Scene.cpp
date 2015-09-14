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
	RenderSDK::RenderAdjacency &refAdj = m_pRenderSDK->GetRenderAdjacency();
	RenderSDK::SRTVariant_Adjacency &adjacency = refAdj.GetCurrentAdjacency();

	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bFog = m_pEnvSceneInfo->m_bFog;
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMin = m_pEnvSceneInfo->m_fFogMin;
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMax = m_pEnvSceneInfo->m_fFogMax;
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogDensity = m_pEnvSceneInfo->m_fFogDensity;
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nFogColor = m_pEnvSceneInfo->m_FogColor;
}
