#include "coresdkafx.h"
#include "RenderAdjacency.h"

REGISTER_CLASS(Explorer3D, ActorAllocator);

//----------------------------------------------------------------------------------------------
Explorer3D::Explorer3D(const CObjectAbstract * pParent)
: Super(pParent)
, IDrawInterface(pParent)
, ViewportInterface(pParent)
{
	RegisterViewportInterface(this);
	RegisterDrawInterface(this);

	SetTransient(true);
}

//----------------------------------------------------------------------------------------------
Explorer3D::~Explorer3D()
{

}

//----------------------------------------------------------------------------------------------
void Explorer3D::InitializeBrushes()
{
	CActor *pGenerated;
/*
	// add brushes
 	NEW_OBJECT_TRANSIENT_CHILD(pGenerated, Brush_LevelActor, "LevelActor", this);
 	NEW_OBJECT_TRANSIENT_CHILD(pGenerated, Brush_TriggerObject, "TriggerObject", this);
	*/
	NEW_OBJECT_TRANSIENT_CHILD(pGenerated, CCamera, "DefaultCamera", this);
}

//----------------------------------------------------------------------------------------------
void Explorer3D::DoDraw()
{
	RenderSDK::RenderAdjacency &refAdj = m_pRenderSDK->GetRenderAdjacency();
	RenderSDK::SRTVariant_Adjacency &adjacency = refAdj.GetCurrentAdjacency();

	/** setup matrix */
	if (GetAppMain()->GetCameraManager())
	{
		const CCamera *pCamera = GetAppMain()->GetCameraManager()->GetActiveCamera(adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext);

		if (pCamera)
		{
			adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext = m_pRenderSDK->GetRenderDriver()->GetDefaultContext();
			memcpy(&adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewMatrix, pCamera->GetViewMatrix().m, 16 * sizeof(float));
			memcpy(&adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.projMatrix, pCamera->GetProjMatrix().m, 16 * sizeof(float));
			adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fNearPlane = pCamera->GetNearDist();
			adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFarPlane = pCamera->GetFarDist();
			memcpy(&adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewPos, pCamera->GetPosition_().vec_array, 3 * sizeof(float));

			SetViewPoint(pCamera->GetPosition_());
		}
	}

	if (GetAppMain()->GetShowGrid() && GetAppMain()->IsAEditor() && !GetAppMain()->IsAGame())
	{
		DrawGrid();
		GetPhysicsEngine()->DebugRender(GetRenderComponent());
	}

}

//----------------------------------------------------------------------------------------------
bool Explorer3D::DoVisibilityTest_()
{
	return GetVisible();
}

//----------------------------------------------------------------------------------------------
void Explorer3D::DrawGrid()
{
	int MaxIndex = 500;
	int MinIndex = -500;

	for (int IndexX = MinIndex; IndexX <= MaxIndex; IndexX+=50)
	{
		GetRenderComponent()->DrawLine(Vector((float)IndexX, 0.f, -500.f), Vector((float)IndexX, 0.f, 500.f), 
										(IndexX == 0) ? COLOR_BLACK : ((IndexX == MaxIndex || IndexX == MinIndex) ? COLOR_YELLOW : 0xff555555));
		GetRenderComponent()->DrawLine(Vector(-500.f, 0.f, (float)IndexX), Vector(500.f, 0.f, (float)IndexX), 
										(IndexX == 0) ? COLOR_BLACK : ((IndexX == MaxIndex || IndexX == MinIndex) ? COLOR_YELLOW : 0xff555555));
	}
}

//----------------------------------------------------------------------------------------------
void Explorer3D::DrawShadowMaps()
{
	ProjectedLight * sun = static_cast<ProjectedLight*>(FindActor("ProjectedLight"));

	if (sun)
	{
		sun->PreDraw();
	}
}