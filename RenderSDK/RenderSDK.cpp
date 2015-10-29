#include "StdafxRender.h"

#define RENDER_STACK_SIZE 65535

//----------------------------------------------------------------------------------------------
// D3DDriver * CRenderSDK::GetRenderDriver() const
// {
// 	return m_pRenderDriver; 
// }

//----------------------------------------------------------------------------------------------
CRenderSDK::CRenderSDK(const CObjectAbstract *pParent)
: Super(pParent)
, m_pRenderDriver(new D3DDriver())
, ActiveStack(0)
, bFlagSwapStack(false)
, RenderCPS(0)
, bFakeDraw(false)
, RenderInfoMask(RI_NONE)
, RI_NumObjects(0)
, bFreezeRender(false)
, bDrawHelpers(true)
{
	m_pRenderAdjacency = new RenderSDK::RenderAdjacency(m_pRenderDriver);

	RenderStack[0].reserve(RENDER_STACK_SIZE);
	RenderStack[1].reserve(RENDER_STACK_SIZE);

	RenderStackPost[0].reserve(RENDER_STACK_SIZE);
	RenderStackPost[1].reserve(RENDER_STACK_SIZE);

	ResetRenderInfoData();
}

//----------------------------------------------------------------------------------------------
CRenderSDK::~CRenderSDK()
{
	for_each( RenderStack[0].begin(), RenderStack[0].end(),	DeleteVectorFntor());
	for_each( RenderStack[1].begin(), RenderStack[1].end(),	DeleteVectorFntor());
	
	RenderStack[0].clear();
	RenderStack[1].clear();

	for_each( RenderStackPost[0].begin(), RenderStackPost[0].end(),	DeleteVectorFntor());
	for_each( RenderStackPost[1].begin(), RenderStackPost[1].end(),	DeleteVectorFntor());

	RenderStackPost[0].clear();
	RenderStackPost[1].clear();

	delete m_pFXManager;

	m_pRenderDriver->ReleaseDriver();
	m_pRenderDriver = NULL;

	delete m_pRenderAdjacency;
}

//----------------------------------------------------------------------------------------------
NObjectFactory::TClassFactory * CRenderSDK::GetClassFactory()
{
    return NObjectFactory::TGlobalClassFactory::GetInstance();
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::Initialize(HWND hWnd, unsigned int width, unsigned int height, void *driver/*=NULL*/)
{
	if (driver == NULL)
	{
		m_pFXManager = new CFXMatManager(this);
		m_pRenderDriver->InitRenderDriver(hWnd, width, height);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::ProcessStreaming()
{
	if (m_pRenderDriver->GetNumCommands() > 0)
	{
		EnterCS();

		m_pRenderDriver->ProcessCommands();

		LeaveCS();
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetViewport(float x, float y, float ext_x, float ext_y)
{
	if (m_pRenderDriver){
		m_pRenderDriver->SetViewport(x, y, ext_x, ext_y);
	}
}

//----------------------------------------------------------------------------------------------
bool CRenderSDK::ResizeWindow(unsigned int width, unsigned int height, SRenderContext *pContext /*= 0*/)
{
	bool bResult = false;
	if (m_pRenderDriver){
		bResult = m_pRenderDriver->ResizeWindow(width, height, pContext);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CRenderSDK::GetWireframeMode() const
{
	bool bResult = false;
	if (m_pRenderDriver){
		bResult = m_pRenderDriver->GetWireframeMode();
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetWireframeMode(bool flag)
{
	m_pRenderDriver->SetWireframeMode(flag);
}

//----------------------------------------------------------------------------------------------
unsigned int CRenderSDK::GetViewportWidth() const
{
	return m_pRenderDriver ? m_pRenderDriver->GetWidth() : 800;
}

//----------------------------------------------------------------------------------------------
unsigned int CRenderSDK::GetViewportHeight() const
{ 
	return m_pRenderDriver ? m_pRenderDriver->GetHeight() : 600;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SwapBuffer()
{
	EnterCS();

	m_pRenderAdjacency->swapBuffer();

	for_each( RenderStack[ActiveStack].begin(),
			  RenderStack[ActiveStack].end(),
		      DeleteVectorFntor());

	for_each( RenderStackPost[ActiveStack].begin(),
		RenderStackPost[ActiveStack].end(),
		DeleteVectorFntor());

	RenderStack[ActiveStack].clear();		  // clear active stack
	RenderStackPost[ActiveStack].clear();	  // clear active post stack

	ActiveStack = (ActiveStack == 0) ? 1 : 0; // toggle active stack

	LeaveCS();
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::Render(SRenderContext *pContext, int cps /*= 0*/)
{
	EnterCS();

	RenderCPS = cps;

	SRenderContext *pActiveContext = (pContext != 0) ? pContext : m_pRenderDriver->GetDefaultContext();

	m_pRenderAdjacency->render(pContext);
/*
	// render to targets first
	TRenderAdjacencyQuevue::iterator IterAdj = RenderAdjacencyQuevue[ActiveStack].begin();
	TRenderAdjacencyQuevue::iterator IterEnd = RenderAdjacencyQuevue[ActiveStack].end();

 	while (IterAdj != IterEnd)
 	{
		if (IterAdj->pRenderContext != pActiveContext)
		{
			++IterAdj; // move to next adjacency
			continue;
		}

 		if (IterAdj->RTarget)
 		{
			m_pRenderDriver->SetRenderTarget(IterAdj->RTarget, IterAdj->bClearTarget, IterAdj->m_ClearColor);

			// viewer parameters
 			m_pRenderDriver->SetViewMatrix(IterAdj->RT_ViewMatrix.GetPtr());
 			m_pRenderDriver->SetProjMatrix(IterAdj->RT_ProjMatrix.GetPtr());
			m_pRenderDriver->SetNearPlane(IterAdj->fNearPlane);
			m_pRenderDriver->SetFarPlane(IterAdj->fFarPlane);
 			m_pRenderDriver->SetViewPos(IterAdj->ViewPos.GetPtr());

 			RenderAdjacency(*IterAdj);
			
			m_pRenderDriver->EndRenderTarget(IterAdj->RTarget);
 		}
 		++IterAdj; // move to next adjacency
 	}

	m_pRenderDriver->DriverBeginDraw();

	// basic render
	IterAdj = RenderAdjacencyQuevue[ActiveStack].begin();
	IterEnd = RenderAdjacencyQuevue[ActiveStack].end();

	while (IterAdj != IterEnd)
	{
		if (IterAdj->pRenderContext != pActiveContext)
		{
			++IterAdj; // move to next adjacency
			continue;
		}

		if (IterAdj->RTarget == 0)
		{
			// viewer parameters
			m_pRenderDriver->SetViewMatrix(IterAdj->ViewMatrix.GetPtr());
			m_pRenderDriver->SetProjMatrix(IterAdj->ProjMatrix.GetPtr());
			m_pRenderDriver->SetNearPlane(IterAdj->fNearPlane);
			m_pRenderDriver->SetFarPlane(IterAdj->fFarPlane);
			m_pRenderDriver->SetViewPos(IterAdj->ViewPos.GetPtr());

			// params
			m_pRenderDriver->m_bFog = IterAdj->bFog;
			m_pRenderDriver->m_fFogMin = IterAdj->fFogMin;
			m_pRenderDriver->m_fFogMax = IterAdj->fFogMax;
			m_pRenderDriver->m_fFogDensity = IterAdj->fFogDensity;
			m_pRenderDriver->m_FogColor = IterAdj->FogColor;

			RenderAdjacency(*IterAdj);
		}
		++IterAdj; // move to next adjacency
	}

	// default viewport
// 	while (IterAdj != IterEnd)
// 	{
// 		if ((*IterAdj)->RTarget)
// 		{
// 			m_pRenderDriver->SetRenderTarget((*IterAdj)->RTarget, (*IterAdj)->bClearTarget);
// 			RenderAdjacency(*IterAdj);
// 		}
// 		++IterAdj; // move to next adjacency
// 	}*/

	Present(pActiveContext);


// 	// separate render
// 	TRenderAdjacencyQuevue::iterator IterAdj = RenderAdjacencyQuevue[ActiveStack].begin();
// 	TRenderAdjacencyQuevue::iterator IterEnd = RenderAdjacencyQuevue[ActiveStack].end();
// 
// 	while (IterAdj != IterEnd)
// 	{
// 		// execute command
// 		TVRenderStack::iterator IterCommand	= RenderStack[ActiveStack].begin() + IterAdj->IdxCommandStart;
// 		TVRenderStack::iterator IterCommandEnd = IterCommand + IterAdj->NumCommands;
// 
// 		while (IterCommand != IterCommandEnd)
// 		{
// 			(*IterCommand)->Execute();
// 			++IterCommand;
// 		}
// 
// 		//render objects
// 		TVRenderQuevue::iterator IterObject = RenderQuevue[ActiveStack].begin() + IterAdj->IdxObjectStart;
// 		TVRenderQuevue::iterator IterObjectEnd = IterObject + IterAdj->NumObjects;
// 
// 		while (IterObject != IterObjectEnd)
// 		{
// 			(*IterObject)->Render();
// 			++IterObject;
// 		}
// 
// 		++IterAdj; // move to next adjacency
// 	}
// 

	// execute post command
	TVRenderStack::iterator IterCommand	= RenderStackPost[ActiveStack].begin();
	TVRenderStack::iterator IterCommandEnd = RenderStackPost[ActiveStack].end();

	while (IterCommand != IterCommandEnd)
	{
		(*IterCommand)->Execute();
		++IterCommand;
	}

	m_pRenderDriver->PopContext(); // reset context

	LeaveCS();
}
/*
//----------------------------------------------------------------------------------------------
void CRenderSDK::RenderAdjacency(RenderQuevueAdjacency &Adjacency)
{
	// execute command
	TVRenderStack::iterator IterCommand	= RenderStack[ActiveStack].begin() + Adjacency.IdxCommandStart;
	TVRenderStack::iterator IterCommandEnd = IterCommand + Adjacency.NumCommands;

	while (IterCommand != IterCommandEnd)
	{
		(*IterCommand)->Execute();
		++IterCommand;
	}

	//render objects
	TVRenderQuevue::iterator IterObject = m_RenderQuevue[ActiveStack].begin() + Adjacency.IdxObjectStart;
	TVRenderQuevue::iterator IterObjectEnd = IterObject + Adjacency.NumObjects;

	while (IterObject != IterObjectEnd)
	{
		m_pRenderDriver->SetObjectFlags(&(*IterObject).uFlags);
		(*IterObject).pRenderObject->Render();
		++IterObject;
	}
	
	// debug render
	if (Adjacency.DebugInfo.DotsList.size() > 0 ||
		Adjacency.DebugInfo.PointList.size() > 0 ||
		Adjacency.DebugInfo.PointListNoZ.size() > 0)
	{
		for (std::vector<SDbgPoint>::iterator Iter = Adjacency.DebugInfo.DotsList.begin(); Iter != Adjacency.DebugInfo.DotsList.end(); ++Iter)
		{
			m_pRenderDriver->AddDot((*Iter).Point.GetPtr(), (*Iter).Color);
		}

		for (std::vector<SDbgPoint>::iterator Iter = Adjacency.DebugInfo.PointList.begin(); Iter != Adjacency.DebugInfo.PointList.end(); ++Iter)
		{
			m_pRenderDriver->AddLine((*Iter).Point.GetPtr(), 0, (*Iter).Color);
		}

		for (std::vector<SDbgPoint>::iterator Iter = Adjacency.DebugInfo.PointListNoZ.begin(); Iter != Adjacency.DebugInfo.PointListNoZ.end(); ++Iter)
		{
			m_pRenderDriver->AddLine((*Iter).Point.GetPtr(), 0, (*Iter).Color, false);
		}
		m_pRenderDriver->DrawDebugLines();
	}

	if (Adjacency.DebugInfo.TriangleList.size() > 0)
	{
		for (std::vector<SDbgTriangle>::iterator Iter = Adjacency.DebugInfo.TriangleList.begin(); Iter != Adjacency.DebugInfo.TriangleList.end(); ++Iter)
		{
			m_pRenderDriver->AddTriangle((*Iter).P0.GetPtr(), (*Iter).P1.GetPtr(), (*Iter).P2.GetPtr(), (*Iter).Color);
		}
		m_pRenderDriver->DrawDebugTriangles();
	}

	if (Adjacency.DebugInfo.SphereList.size() > 0)
	{
		for (std::vector<SDbgSphere>::iterator Iter = Adjacency.DebugInfo.SphereList.begin(); Iter != Adjacency.DebugInfo.SphereList.end(); ++Iter)
		{
			m_pRenderDriver->RenderDebugSphere((*Iter).Pos.GetPtr(), (*Iter).rad, (*Iter).Color, (*Iter).Seg);
		}
	}
}*/

//----------------------------------------------------------------------------------------------
void CRenderSDK::Present(const SRenderContext *pContext)
{
	m_pRenderDriver->Present(pContext);
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::UpdateResources(bool bLock/*=false*/)
{
	for_each( RenderStack[0].begin(), RenderStack[0].end(),	DeleteVectorFntor());
	for_each( RenderStack[1].begin(), RenderStack[1].end(),	DeleteVectorFntor());

	for_each( RenderStackPost[0].begin(), RenderStackPost[0].end(),	DeleteVectorFntor());
	for_each( RenderStackPost[1].begin(), RenderStackPost[1].end(),	DeleteVectorFntor());

	RenderStack[0].clear();
	RenderStack[1].clear();

	RenderStackPost[0].clear();
	RenderStackPost[1].clear();

	m_pRenderDriver->UpdateDriverThread();
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetViewMatrix(const Matrix& M)
{
	View = M;

	if (!bFakeDraw)
	{
		RenderSDK::SRVariantRenderCommand* render_cmd = m_pRenderAdjacency->PushRenderCommand();
		render_cmd->vt = RenderSDK::ERC_ViewMatrix;

		memcpy(&render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m, M.GetPtr(), 16 * sizeof(float));
	}
}

//----------------------------------------------------------------------------------------------
Matrix CRenderSDK::GetViewMatrix()
{
	// TODO: Get last from stack
	return View;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::GetViewport(Vector4f& OutViewport)
{
	OutViewport.x = -View.t2.x * (1.f / View.__row0.Length());
	OutViewport.y = -View.t2.y * (1.f / View.__row1.Length());

	OutViewport.z = OutViewport.x + GetViewportWidth() * (1.f / View.__row0.Length());
	OutViewport.w = OutViewport.y + GetViewportHeight() * (1.f / View.__row1.Length());
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetProjMatrix(const Matrix& M)
{
	Proj = M;

	if (!bFakeDraw)
	{
		RenderSDK::SRVariantRenderCommand *render_cmd = m_pRenderAdjacency->PushRenderCommand();
		render_cmd->vt = RenderSDK::ERC_ProjMatrix;

		memcpy(&render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m, M.GetPtr(), 16 * sizeof(float));
	}
}

//----------------------------------------------------------------------------------------------
Matrix CRenderSDK::GetProjMatrix()
{
	return Proj;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetProjCropMatrix(const Matrix &M)
{
	ProjCrop = M;

	if (!bFakeDraw)
	{
		RenderSDK::SRVariantRenderCommand *render_cmd = m_pRenderAdjacency->PushRenderCommand();
		render_cmd->vt = RenderSDK::ERC_CropMatrix;

		memcpy(&render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m, M.GetPtr(), 16 * sizeof(float));
	}
}

//----------------------------------------------------------------------------------------------
Matrix CRenderSDK::GetProjCropMatrix()
{
	return ProjCrop;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetTransform(const Matrix &M)
{
	World = M;

	if (!bFakeDraw)
	{
		RenderSDK::SRVariantRenderCommand *render_cmd = m_pRenderAdjacency->PushRenderCommand();
		render_cmd->vt = RenderSDK::ERC_WorldMatrix;

		memcpy(&render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m, M.GetPtr(), 16 * sizeof(float));
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetBlendOpacity(float blend)
{
	if (!bFakeDraw)
	{
		PushToRenderStack(new RenderChunk_SetOpacity<D3DDriver, float>(m_pRenderDriver, blend));
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::PushBlendOpacity(float blend)
{
	if (AlphaBlendStack.empty()) // add base factor
	{
		AlphaBlendStack.push(1.f/*m_pRenderDriver->GetBlendOpacity()*/);
	}

	float BlendFactor = AlphaBlendStack.top() * blend;

	AlphaBlendStack.push(BlendFactor);
	
	SetBlendOpacity(BlendFactor);
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::PopBlendOpacity()
{
	if (!AlphaBlendStack.empty())
	{
		AlphaBlendStack.pop();
		SetBlendOpacity(AlphaBlendStack.top());

		if (AlphaBlendStack.size() == 1)
		{
			AlphaBlendStack.pop();
		}
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::ResetBlend()
{
	while (!AlphaBlendStack.empty())
	{
		AlphaBlendStack.pop();
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::PushRenderQuevueAdjaency()
{
	RenderSDK::SRTVariant_Adjacency& outAdjacency = m_pRenderAdjacency->PushRenderQuevueAdjaency();
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::PopRenderQuevueAdjaency()
{
	m_pRenderAdjacency->PopRenderQuevueAdjaency();
}

//----------------------------------------------------------------------------------------------
RenderSDK::SRTVariant_Adjacency& CRenderSDK::GetCurrQuevueAdjaency()
{
	return m_pRenderAdjacency->GetCurrentAdjacency();
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::PushToRenderStack(CommandBase *pChunk, bool Post /*= false*/)
{
	byte NonActive = (ActiveStack == 0) ? 1 : 0;

	if (!Post)
	{
		RenderStack[NonActive].push_back(pChunk);
	}else
	{
		RenderStackPost[NonActive].push_back(pChunk);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::PushObjectToQuevue(const CRenderObject *pObject, unsigned int flags /*= 0*/)
{
	RenderSDK::SRVariantRenderCommand *render_cmd = m_pRenderAdjacency->PushRenderCommand();
	render_cmd->vt = RenderSDK::ERC_Object;
	render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_5.pRenderObject = pObject;
	render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_5.uFlags = flags;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetSwapFlag()
{
	bFlagSwapStack = true;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetRenderTarget(const Comp_RenderTarget * RenderTarget, bool bClearTarget /*= false*/)
{

}

//----------------------------------------------------------------------------------------------
void CRenderSDK::EndRenderTarget()
{
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height, float min_z /*= 0.f*/, float max_z /*= 1.f*/)
{
	RenderSDK::SRVariantRenderCommand *render_cmd = m_pRenderAdjacency->PushRenderCommand();
	render_cmd->vt = RenderSDK::ERC_Viewport;
	render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.x = x;
	render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.y = y;
	render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.width = width;
	render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.height = height;
	render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.MinZ = min_z;
	render_cmd->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.MaxZ = max_z;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawObject(const CRenderObject *pObject)
{	
	SetTransform(pObject->GetRWTM());

	if (!bFakeDraw)
	{
		PushObjectToQuevue(pObject, pObject->GetFlags()); // new functionality instead memalloc
	}
	
	if (RenderInfoMask != RI_NONE)
	{
		CollectRenderInfoData(pObject);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawObject(const CRenderObject *pObject, const Matrix &WorldTransform)
{
	//SetTransform(pObject->GetRWTM());

	if (!bFakeDraw)
	{
		//PushToRenderStack(new RenderChunk<CRenderObject, CRenderObject>(&CRenderObject::Render, Object));
		//PushObjectToQuevue(Object); // new functionality instead memalloc
	}

	if (RenderInfoMask != RI_NONE)
	{
		CollectRenderInfoData(pObject);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawDot(const Vector &point, unsigned int Color, bool bZEnable/* = true*/)
{
	if (!bFakeDraw && bDrawHelpers)
	{
		m_pRenderAdjacency->auxDrawDot(point, Color, bZEnable);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawLine(const Vector& start, const Vector& end, unsigned int Color, bool bZEnable /*= true*/)
{
	if (!bFakeDraw && bDrawHelpers)
	{
		m_pRenderAdjacency->auxDrawLine(start, end, Color, bZEnable);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawLine(const Vector2f& start, const Vector2f& end, unsigned int Color, bool bZEnable /*= true*/)
{
	if (!bFakeDraw && bDrawHelpers)
	{
		m_pRenderAdjacency->auxDrawLine(start, end, Color, bZEnable);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawRectangle(Vector2f &start, Vector2f& end, unsigned int Color)
{
	if (!bFakeDraw && bDrawHelpers)
	{
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawCircle(const Matrix &WTM, float rad, unsigned int Color, unsigned int segments /*= 16*/)
{
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawSphere(Vector &Pos, float rad, unsigned int color, unsigned short segments /*= 16*/)
{
/*	RenderQuevueAdjacency& Queve = GetCurrQuevueAdjaency();

	Queve.DebugInfo.SphereList.push_back(SDbgSphere(Pos, rad, color, segments));*/
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawText(wchar_t* text, const Vector& position, unsigned int color/* = 0xffffffff*/, bool shadowed/* = false*/)
{
	Vector2f pos2d;
	Vector pos = position;
   
	m_pRenderDriver->ProjectWorldToScreen(pos2d.GetPtr(), pos.GetPtr());
	m_pRenderDriver->_DrawText2D(text, pos2d.x, pos2d.y, color, shadowed);
}

//----------------------------------------------------------------------------------------------
Vector2f CRenderSDK::ProjectViewport(Vector2f &v_out, const Vector& InPos)
{
	m_pRenderDriver->ProjectWorldToScreen(v_out.GetPtr(), InPos.GetPtr());
	return v_out;
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawText2D(const wchar_t* text,const Vector2f& position, unsigned int color/* = 0xffffffff*/, bool shadowed/* = false*/)
{
	m_pRenderDriver->_DrawText2D(text, position.x, position.y, color, shadowed);
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawText2D(const wchar_t* text, const Vector2f& position, const Vector2f& extension, unsigned int color/* = 0xffffffff*/, bool shadowed/* = false*/)
{
	if (!bFakeDraw)
	{
		m_pRenderDriver->_DrawText2D(text, position.x, position.y, extension.x, extension.y, color, shadowed);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawTriangle(const Vector &p0, const Vector &p1, const Vector &p2, unsigned int color /*= 0xffffffff*/)
{
	if (!bFakeDraw && bDrawHelpers)
	{
		m_pRenderAdjacency->auxDrawTriangle(p0, p1, p2, color);
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawTriangle2D(const Vector2f &P0, const Vector2f &P1, const Vector2f &P2, unsigned int color/* = 0xffffffff*/)
{
	if (!bFakeDraw)
	{
/*		RenderQuevueAdjacency& Queve = GetCurrQuevueAdjaency();

		Queve.DebugInfo.TriangleList.push_back(SDbgTriangle(Vector(P0.x, P0.y, 0.f), Vector(P1.x, P1.y, 0.f), Vector(P2.x, P2.y, 0.f), color));*/
	}
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::DrawBounds(const Matrix& WTM, const Bounds3f &Bound, unsigned int Color /* = 0xffffffff*/)
{
	Vector  Points[8];    // corners of the box
	 
	Points[0] = Vector(Bound.bound_min.x, Bound.bound_min.y, Bound.bound_min.z) + WTM.t; // xyz
	Points[1] = Vector(Bound.bound_max.x, Bound.bound_min.y, Bound.bound_min.z) + WTM.t; // Xyz
	Points[2] = Vector(Bound.bound_min.x, Bound.bound_max.y, Bound.bound_min.z) + WTM.t; // xYz
	Points[3] = Vector(Bound.bound_max.x, Bound.bound_max.y, Bound.bound_min.z) + WTM.t; // XYz
	Points[4] = Vector(Bound.bound_min.x, Bound.bound_min.y, Bound.bound_max.z) + WTM.t; // xyZ
	Points[5] = Vector(Bound.bound_max.x, Bound.bound_min.y, Bound.bound_max.z) + WTM.t; // XyZ
	Points[6] = Vector(Bound.bound_min.x, Bound.bound_max.y, Bound.bound_max.z) + WTM.t; // xYZ
	Points[7] = Vector(Bound.bound_max.x, Bound.bound_max.y, Bound.bound_max.z) + WTM.t; // XYZ

	DrawLine(Points[0], Points[2], Color);
	DrawLine(Points[2], Points[3], Color);
	DrawLine(Points[3], Points[1], Color);
	DrawLine(Points[1], Points[0], Color);

	DrawLine(Points[4], Points[6], Color);
	DrawLine(Points[6], Points[7], Color);
	DrawLine(Points[7], Points[5], Color);
	DrawLine(Points[5], Points[4], Color);

	DrawLine(Points[0], Points[4], Color);
	DrawLine(Points[1], Points[5], Color);
	DrawLine(Points[2], Points[6], Color);
	DrawLine(Points[3], Points[7], Color);
}
//------------------------------------------------------------------------
void CRenderSDK::DrawBounds2f(const Matrix&  WTM, const Bounds3f &Bound, unsigned int Color/* = 0xffffffff*/)
{
	DrawLine(WTM.t2 + Vector2f(Bound.bound_min.x, Bound.bound_min.y), 
			(WTM.t2 + Vector2f(Bound.bound_min.x, Bound.bound_min.y)) + Vector2f(Bound.bound_max.x - Bound.bound_min.x, 0), Color);

	DrawLine((WTM.t2 + Vector2f(Bound.bound_min.x, Bound.bound_min.y)) + Vector2f(Bound.bound_max.x - Bound.bound_min.x, 0), 
			(WTM.t2 + Vector2f(Bound.bound_min.x, Bound.bound_min.y)) + Vector2f(Bound.bound_max.x - Bound.bound_min.x, Bound.bound_max.y - Bound.bound_min.y), Color);

	DrawLine((WTM.t2 + Vector2f(Bound.bound_min.x, Bound.bound_min.y)) + Vector2f(Bound.bound_max.x - Bound.bound_min.x, Bound.bound_max.y - Bound.bound_min.y),
			(WTM.t2 + Vector2f(Bound.bound_min.x, Bound.bound_min.y)) + Vector2f(0, Bound.bound_max.y - Bound.bound_min.y), Color);

	DrawLine(WTM.t2 + Vector2f(Bound.bound_min.x, Bound.bound_min.y), 
			(WTM.t2 + Vector2f(Bound.bound_min.x, Bound.bound_min.y)) + Vector2f(0, Bound.bound_max.y - Bound.bound_min.y), Color);
}

//------------------------------------------------------------------------
void CRenderSDK::RenderPoint(Vector& point)
{
    Vector Y(0,10,0);
    Vector X(10,0,0);
    Vector Z(0,0,10);

    Vector _X; _X = point + X;
    Vector _Y; _Y = point + Y;
    Vector _Z; _Z = point + Z;
                                       
   // m_pRenderDriver->AddLine(point.GetPtr(),_X.GetPtr(),0xffff0000);
   // m_pRenderDriver->AddLine(point.GetPtr(),_Y.GetPtr(),0xffff0000);
   // m_pRenderDriver->AddLine(point.GetPtr(),_Z.GetPtr(),0xffff0000);

    _X = point - X;
    _Y = point - Y;
    _Z = point - Z;

   // m_pRenderDriver->AddLine(point.GetPtr(),_X.GetPtr(),0xffff0000);
   // m_pRenderDriver->AddLine(point.GetPtr(),_Y.GetPtr(),0xffff0000);
   // m_pRenderDriver->AddLine(point.GetPtr(),_Z.GetPtr(),0xffff0000);
}

//------------------------------------------------------------------------
BOOL CRenderSDK::DrawAABox(Vector& min,Vector& max, DWORD color, BOOL zEnable /*= TRUE*/,BOOL bWired /*= TRUE*/)
{
	Vector point00(min.x,min.y,min.z);
    Vector point01(max.x, min.y, min.z);
	Vector point02(min.x, min.y, max.z);
	Vector point03(max.x, min.y, max.z);

	Vector point10(min.x, max.y, min.z);
    Vector point11(max.x, max.y, min.z);
	Vector point12(min.x, max.y, max.z);
	Vector point13(max.x, max.y, max.z);
	
// 	m_pRenderDriver->AddLine(point00.GetPtr(),point01.GetPtr(),color);
// 	m_pRenderDriver->AddLine(point01.GetPtr(),point03.GetPtr(),color);
// 	m_pRenderDriver->AddLine(point03.GetPtr(),point02.GetPtr(),color);
//  m_pRenderDriver->AddLine(point02.GetPtr(),point00.GetPtr(),color);
// 
// 	m_pRenderDriver->AddLine(point10.GetPtr(),point11.GetPtr(),color);
// 	m_pRenderDriver->AddLine(point11.GetPtr(),point13.GetPtr(),color);
// 	m_pRenderDriver->AddLine(point13.GetPtr(),point12.GetPtr(),color);
// 	m_pRenderDriver->AddLine(point12.GetPtr(),point10.GetPtr(),color);
// 
// 	m_pRenderDriver->AddLine(point00.GetPtr(),point10.GetPtr(),color);
// 	m_pRenderDriver->AddLine(point01.GetPtr(),point11.GetPtr(),color);
// 	m_pRenderDriver->AddLine(point02.GetPtr(),point12.GetPtr(),color);
// 	m_pRenderDriver->AddLine(point03.GetPtr(),point13.GetPtr(),color);

    return TRUE;
}

//------------------------------------------------------------------------
BOOL CRenderSDK::DrawBox2D(Vector2f& min, Vector2f& max,unsigned int color)
{
	Vector2f point00(min.x, min.y);
	Vector2f point01(max.x, min.y);
	Vector2f point02(min.x, max.y);
	Vector2f point03(max.x, max.y);
// 
// 	m_pRenderDriver->AddLine2D(point00.GetPtr(), point01.GetPtr(), color);
//     m_pRenderDriver->AddLine2D(point01.GetPtr(), point03.GetPtr(), color);
//     m_pRenderDriver->AddLine2D(point03.GetPtr(), point02.GetPtr(), color);
// 	m_pRenderDriver->AddLine2D(point02.GetPtr(), point00.GetPtr(), color);

	return TRUE;
}

//--------------------------------------------------------------------------------------
void CRenderSDK::_DebugDrawRectangle2D(Vector2f& P0, Vector2f& P1, unsigned int color /*= 0xffffffff*/)
{
	Vector2f LP0 = P0;
	Vector2f LP1 = P0 + Vector2f(P1.x - P0.x, 0.f);
	Vector2f LP2 = P0 + Vector2f(P1.x - P0.x, P1.y - P0.y);
	Vector2f LP3 = P0 + Vector2f(0.f, P1.y - P0.y);

	// clockwise line rendering order
	DrawLine(LP0, LP1, color);
	DrawLine(LP1, LP2, color);
	DrawLine(LP2, LP3, color);
	DrawLine(LP3, LP0, color);
}

//--------------------------------------------------------------------------------------
void CRenderSDK::_DebugDrawRectangle2D(Vector2f& P0, Vector2f& P1, Vector2f& P2, Vector2f& P3, unsigned int color /*= 0xffffffff*/, bool bZEnable/* = true*/)
{
	DrawLine(P0, P1, color, bZEnable);
	DrawLine(P1, P2, color, bZEnable);
	DrawLine(P2, P3, color, bZEnable);
	DrawLine(P3, P0, color, bZEnable);
}

//--------------------------------------------------------------------------------------
Vector CRenderSDK::UnprojectScreenToWorld(const Vector2f& InValue, const Matrix& ViewM, const Matrix &ProjM)
{
	Vector OutValue;

	//m_pRenderDriver->UnprojectScreenToWorld(OutValue.GetPtr(), ViewM.GetPtr(), ProjM.GetPtr(), InValue.GetPtr());

	return OutValue;
}

//--------------------------------------------------------------------------------------
void CRenderSDK::SetPointLight(const Vector& position, const Matrix& LightView, const Matrix& LightProj, unsigned int color)
{
	if (!bFakeDraw){
		//PushToRenderStack(new RenderChunk_SetLightPosition<D3DDriver, Vector, Matrix>(m_pRenderDriver, position, LightView, LightProj, color));
	}
}

//--------------------------------------------------------------------------------------
void CRenderSDK::SetProjectedLight(const Vector &position,
								   const Vector &Direction,
								   const Matrix& LightView,
								   const Matrix& LightProj,
								   unsigned int color,
								   const Vector4f& SplitDistances,
								   const Matrix *MatrixArray,
								   unsigned int Num)
{
	if (!bFakeDraw)
	{
		PushToRenderStack(
			new RenderChunk_SetLightProjected<D3DDriver, Vector, Matrix, Vector4f>(m_pRenderDriver,
																					position,
																					Direction,
																					LightView,
																					LightProj,
																					color,
																					SplitDistances,
																					MatrixArray,
																					Num));
	}
}

//--------------------------------------------------------------------------------------
void CRenderSDK::ResetRenderInfoData()
{
	RI_NumObjects = 0;
	RI_RenderObjectsBound.SetUnvalid();
	RI_ShadowCastersBound.SetUnvalid();
	RI_ShadowRecieversBound.SetUnvalid();
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetBias(float DepthBias, float SlopeDepthBias)
{
	PushToRenderStack(new RenderChunk_SetBias<D3DDriver, float>(&D3DDriver::SetBias, m_pRenderDriver, DepthBias, SlopeDepthBias));
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetDebugInfoCPS(bool bParallel, int eventCPS, int updateCPS, int renderCPS, int timerCPS, int streamCPS)
{
	m_pRenderDriver->SetDebugInfoCPS(bParallel, eventCPS, updateCPS, renderCPS, timerCPS, streamCPS);
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::SetDeltaTime(float DeltaTime)
{
	m_pRenderDriver->SetDeltaTime(DeltaTime);
}

//----------------------------------------------------------------------------------------------
void CRenderSDK::CollectRenderInfoData(const CRenderObject * Object)
{
	/*assert(Object->GetWorldBound().IsValid());

	if (RenderInfoMask & RI_NUMBER_CALLS){
		RI_NumObjects++;
	}

	if (RenderInfoMask & RI_RENDER_BOUNDS)
	{
		RI_RenderObjectsBound += Object->GetRWorldBound();
	}

	if ((RenderInfoMask & RI_RENDER_BOUNDS_SHADOW_CASTER) && Object->GetCastShadows())
	{
		RI_ShadowCastersBound += Object->GetWorldBound();
	}

	if ((RenderInfoMask & RI_RENDER_BOUNDS_SHADOW_RECIEVERS) && Object->GetReciveShadows())
	{
		RI_ShadowRecieversBound += Object->GetWorldBound();
	}*/
}

