#include "coresdkafx.h"

REGISTER_CLASS(Explorer2D, UIScreenObject);

//----------------------------------------------------------------------------------
Explorer2D::Explorer2D(const CObjectAbstract *pParent)
: Super(pParent)
, ViewportInterface(pParent)
{
	RegisterViewportInterface(this);

	const float VWidth = (float)GetRenderComponent()->GetViewportWidth();
	const float VHeight = (float)GetRenderComponent()->GetViewportHeight();

	SetTransient(true);
//	SetSize(Vector2f(VWidth, VHeight));

	Matrix M;
	ortho(M, 0.f, VWidth, VHeight, 0.f, 0.1f, MAX_FLOAT);

	SetViewportProjMatrix(M);
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

	//GetAppMain()->GetViewportManager()->RebuildTransform(this);
}

//----------------------------------------------------------------------------------
void Explorer2D::InitializeBrushes()
{
#ifndef FINAL_RELEASE
/*	CActor *Generated;
	// add brushes
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, UIImage_Brush, "Image", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, UISprite_Brush, "Sprite", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, Bezier_SplineAgregator_Brush, "BezierSpline", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, SplineAgregator_Brush, "Spline", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, SplineAgregator_PointInserter_Brush, "PointSpline", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, UIFilter_Brush, "Filter", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, UVModificator_Brush, "UVModificator", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, SplineCollision2DModificator_Brush, "SplineCollision2D", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, BoxCollisionController_Brush, "BoxController2D", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, SphereCollisionController_Brush, "SphereController2D", this);
 	NEW_OBJECT_TRANSIENT_CHILD(Generated, LabelButton_Brush, "LabelButton", this);*/
#endif//FINAL_RELEASE
}

//----------------------------------------------------------------------------------
void Explorer2D::DoDraw()
{
	RenderQuevueAdjacency &Queve = m_pRenderSDK->GetCurrQuevueAdjaency();

	Queve.pRenderContext = m_pRenderSDK->GetRenderDriver()->GetDefaultContext();
	Queve.ViewMatrix = GetViewportViewMatrix();
	Queve.ProjMatrix = GetViewportProjMatrix();
	Queve.fNearPlane = GetNearPlane();
	Queve.fFarPlane  = GetFarPlane();
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