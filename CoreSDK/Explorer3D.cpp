#include "coresdkafx.h"

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
	RenderQuevueAdjacency &Queve = m_pRenderSDK->GetCurrQuevueAdjaency();

	/** setup matrix */
	if (GetAppMain()->GetCameraManager())
	{
		const CCamera *pCamera = GetAppMain()->GetCameraManager()->GetActiveCamera(Queve.pRenderContext);

		if (pCamera)
		{
			Queve.pRenderContext = m_pRenderSDK->GetRenderDriver()->GetDefaultContext();
			Queve.ViewMatrix = pCamera->GetViewMatrix();
			Queve.ProjMatrix = pCamera->GetProjMatrix();
			Queve.fNearPlane = pCamera->GetNearDist();
			Queve.fFarPlane  = pCamera->GetFarDist();
			Queve.ViewPos	 = pCamera->GetPosition_();
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
/*bool Explorer3D::OnEventPressed(const MouseInputData& InputData)
{
	bool bResult = false;

	if (GetAppMain()->GetEditorInputFlag() & EF_EDIT_3D)
	{
		bResult = Super::OnEventPressed(InputData);
	}
	return bResult;
}*/

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