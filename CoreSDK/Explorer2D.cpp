#include "coresdkafx.h"

REGISTER_CLASS(Explorer2D, UIScreenObject);

//----------------------------------------------------------------------------------
Explorer2D::Explorer2D(const CObjectAbstract *pParent)
: Super(pParent)
, core_sdk_api::TIViewport(pParent)
{
    core_sdk_api::TIViewport::RegisterViewportInterface(this);

	SetTransient(true);
}

//----------------------------------------------------------------------------------
Explorer2D::~Explorer2D()
{

}

//----------------------------------------------------------------------------------
void Explorer2D::Initialize()
{
	assert(!IsInitialized());

	Super::Initialize();

	const float width = (float)GetRenderComponent()->GetViewportWidth();
	const float height = (float)GetRenderComponent()->GetViewportHeight();

	SetTransient(true);
	//	SetSize(Vector2f(VWidth, VHeight));

	Matrix M;
	ortho(M, 0.f, width, height, 0.f, 0.1f, MAX_FLOAT);

	SetViewportProjMatrix(M);

	//GetAppMain()->GetViewportManager()->RebuildTransform(this);
}

//----------------------------------------------------------------------------------
void Explorer2D::InitializeBrushes()
{
}

//----------------------------------------------------------------------------------
void Explorer2D::DoDraw()
{
	RenderSDK::RenderAdjacency &refAdj = m_pRenderSDK->GetRenderAdjacency();
	RenderSDK::SRTVariant_Adjacency &adjacency = refAdj.GetCurrentAdjacency();

	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext = m_pRenderSDK->GetRenderDriver()->GetDefaultContext();
	memcpy(&adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewMatrix, GetViewportViewMatrix().m, 16 * sizeof(float));
	memcpy(&adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.projMatrix, GetViewportProjMatrix().m, 16 * sizeof(float));
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fNearPlane = GetNearPlane();
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFarPlane = GetFarPlane();
	adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bFog = false;
}

//----------------------------------------------------------------------------------------------
Vector2f Explorer2D::GetTransformedSize_() const
{
	if (GetRenderComponent()->GetRenderDriver())
	{
		return Vector2f((float)GetRenderComponent()->GetRenderDriver()->GetWidth(), 
						(float)GetRenderComponent()->GetRenderDriver()->GetHeight());
	}
	return Vector2f(800, 600);
}