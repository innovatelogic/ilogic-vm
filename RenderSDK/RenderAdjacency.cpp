#include "RenderAdjacency.h"
#include "RenderSDKInterface.h"
#include "RenderContext.h"
#include "RenderTypes.h"
#include "RenderObject.h"
#include "../D3DDrv/d3ddriverstdafx.h"

#include <stdlib.h>
#include <climits>

#define MAX_ADJACENCY 64
#define HALF_ADJACENCY 32

#define MAX_RCOMMANDS 65535 + 1
#define HALF_RCOMMANDS 32768

namespace RenderSDK
{
//----------------------------------------------------------------------------------------------
RenderAdjacency::SDebugInfo::SDebugInfo()
{
	pDots = (SDbgPoint*) malloc(sizeof(SDbgPoint) * MAX_RCOMMANDS);
	pLines = (SDbgPoint*) malloc(sizeof(SDbgPoint) * MAX_RCOMMANDS);
	pLinesNoZ = (SDbgPoint*) malloc(sizeof(SDbgPoint) * MAX_RCOMMANDS);
	pTriangles = (SDbgTriangle*) malloc(sizeof(SDbgTriangle) * MAX_RCOMMANDS);
	pSpheres = (SDbgSphere*) malloc(sizeof(SDbgSphere) * MAX_RCOMMANDS);
}

//----------------------------------------------------------------------------------------------
RenderAdjacency::SDebugInfo::~SDebugInfo()
{
	free(pDots);
	free(pLines);
	free(pLinesNoZ);
	free(pTriangles);
	free(pSpheres);
}

//----------------------------------------------------------------------------------------------
RenderAdjacency::RenderAdjacency(D3DDriver *driver)
	: m_ActiveStack(0)
	, m_pRenderDriver(driver)
{
	m_pVariantAdjacency = (LPRTVARIANT) malloc(sizeof(SRTVariant_Adjacency) * MAX_ADJACENCY);
	m_pVariantCommands = (LPRTVARIANTCMD) malloc(sizeof(SRVariantRenderCommand) * MAX_RCOMMANDS);

	m_pDbgInfo = new SDebugInfo;
}

//----------------------------------------------------------------------------------------------
RenderAdjacency::~RenderAdjacency()
{
	assert(m_pVariantAdjacency);
	free(m_pVariantAdjacency);

	assert(m_pVariantCommands);
	free(m_pVariantCommands);

	delete m_pDbgInfo;
}

//----------------------------------------------------------------------------------------------
SRTVariant_Adjacency& RenderAdjacency::PushRenderQuevueAdjaency()
{
	byte nonActive = (m_ActiveStack == 0) ? 1 : 0;

	assert(m_aContext[nonActive].nIndexAdjaency < HALF_ADJACENCY - 1);

	const size_t &shift = m_aContext[nonActive].nIndexAdjaency;
	const size_t &curr_command = m_aContext[nonActive].nIndexCommand;

	SRTVariant_Adjacency &refAdjacency = *(m_pVariantAdjacency + (HALF_ADJACENCY * nonActive) + shift);

	// init base indexes
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.idxCommandStart = (HALF_RCOMMANDS * nonActive) + curr_command;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands = 0;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext = nullptr;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target = nullptr;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_drawn = false;
	// aux debug info
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotStartIdx = (HALF_RCOMMANDS * nonActive) + m_aContext[nonActive].nIndexDot;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLineStartIdx = (HALF_RCOMMANDS * nonActive) + m_aContext[nonActive].nIndexLine;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLineZStartIdx = (HALF_RCOMMANDS * nonActive) + m_aContext[nonActive].nIndexLineZ;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTriangleStartIdx = (HALF_RCOMMANDS * nonActive) + m_aContext[nonActive].nIndexTriangle;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nSphereStartIdx = (HALF_RCOMMANDS * nonActive) + m_aContext[nonActive].nIndexSphere;

	//zero numbers
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotsNum = 0;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesNum = 0;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesZNum = 0;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTrianglesNum = 0;
	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nSpheresNum = 0;

	return refAdjacency;
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::PopRenderQuevueAdjaency()
{
	byte nonActive = (m_ActiveStack == 0) ? 1 : 0;

	SRTVariant_Adjacency &adjacency = GetCurrentAdjacency();

	m_aContext[nonActive].nIndexCommand += adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands;
	m_aContext[nonActive].nIndexDot += adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotsNum;
	m_aContext[nonActive].nIndexLine += adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesNum;
	m_aContext[nonActive].nIndexLineZ += adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesZNum;
	m_aContext[nonActive].nIndexTriangle += adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTrianglesNum;
	m_aContext[nonActive].nIndexSphere += adjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nSpheresNum;

	m_aContext[nonActive].nIndexAdjaency++;
}

//----------------------------------------------------------------------------------------------
SRTVariant_Adjacency& RenderAdjacency::GetCurrentAdjacency()
{
	byte nonActive = (m_ActiveStack == 0) ? 1 : 0;

	assert(m_aContext[nonActive].nIndexAdjaency < HALF_ADJACENCY - 1);

	const size_t shift = m_aContext[nonActive].nIndexAdjaency;

	return *(m_pVariantAdjacency + (HALF_ADJACENCY * nonActive) + shift);
}

//----------------------------------------------------------------------------------------------
SRVariantRenderCommand* RenderAdjacency::PushRenderCommand()
{
	SRTVariant_Adjacency &refAdjacency = GetCurrentAdjacency();

	const size_t startCmdIndex = refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.idxCommandStart;

	SRVariantRenderCommand *nextCommand = m_pVariantCommands + (startCmdIndex + refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands);

	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands++;

	assert(refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands < HALF_RCOMMANDS - 1);

	return nextCommand;
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::swapBuffer()
{
	// reset already drawn context
	m_aContext[m_ActiveStack].nIndexAdjaency = 0;
	m_aContext[m_ActiveStack].nIndexCommand  = 0;	
	m_aContext[m_ActiveStack].nIndexDot = 0;
	m_aContext[m_ActiveStack].nIndexLine = 0;
	m_aContext[m_ActiveStack].nIndexLineZ = 0;
	m_aContext[m_ActiveStack].nIndexTriangle = 0;
	m_aContext[m_ActiveStack].nIndexSphere = 0;

	m_ActiveStack = (m_ActiveStack == 0) ? 1 : 0; // toggle active stack
}

//----------------------------------------------------------------------------------------------
LPRTVARIANT RenderAdjacency::getAdjBuffer(size_t index) const
{ 
	assert(index >= 0 && index < 2);

	return m_pVariantAdjacency + (HALF_ADJACENCY * index);
}

//----------------------------------------------------------------------------------------------
LPRTVARIANTCMD RenderAdjacency::getActiveCmd(size_t index) const
{
	assert(index >= 0 && index < MAX_RCOMMANDS - 1);
	
	return m_pVariantCommands + index;
}

//----------------------------------------------------------------------------------------------
LPRTVARIANT RenderAdjacency::begin_adj(size_t index) const
{
	assert(index >= 0 && index < 2);

	return m_pVariantAdjacency + (HALF_ADJACENCY * index);
}

//----------------------------------------------------------------------------------------------
LPRTVARIANT RenderAdjacency::end_adj(size_t index) const
{
	assert(index >= 0 && index < 2);

	return m_pVariantAdjacency + (HALF_ADJACENCY * index) + m_aContext[index].nIndexAdjaency;
}

//----------------------------------------------------------------------------------------------
LPRTVARIANTCMD RenderAdjacency::begin_cmd(size_t index) const
{
	assert(index >= 0 && index < 2);

	return m_pVariantCommands + (HALF_RCOMMANDS * index);
}

//----------------------------------------------------------------------------------------------
LPRTVARIANTCMD RenderAdjacency::end_cmd(size_t index) const
{
	assert(index >= 0 && index < 2);

	return m_pVariantCommands + (HALF_RCOMMANDS * index) + m_aContext[index].nIndexCommand;
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::render(SRenderContext *pContext)
{
	SRenderContext *pActiveContext = (pContext != 0) ? pContext : m_pRenderDriver->GetDefaultContext();

	// render targets first
	renderTargets(pActiveContext);

	// forward rendering
	m_pRenderDriver->PushContext(pActiveContext);

	m_pRenderDriver->DriverBeginDraw();

	int activeStack = getActiveStackIndex();

	RenderSDK::RenderAdjacency::IteratorAdjacency iter(begin_adj(activeStack));
	RenderSDK::RenderAdjacency::IteratorAdjacency iter_end(end_adj(activeStack));

	while (iter != iter_end)
	{
		if (iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext != pActiveContext)
		{
			++iter; // move to next adjacency
			continue;
		}

		if (iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target == nullptr)
		{
			m_pRenderDriver->SetViewMatrix(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewMatrix);
			m_pRenderDriver->SetProjMatrix(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.projMatrix);
			m_pRenderDriver->SetNearPlane(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fNearPlane);
			m_pRenderDriver->SetFarPlane(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFarPlane);
			m_pRenderDriver->SetViewPos(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewPos);

			// params
			m_pRenderDriver->m_bFog = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bFog;
			m_pRenderDriver->m_fFogMin = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMin;
			m_pRenderDriver->m_fFogMax = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMax;
			m_pRenderDriver->m_fFogDensity = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogDensity;
			m_pRenderDriver->m_FogColor = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nFogColor;

			renderAdjacency(*iter);
		}
		++iter;
	}
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::auxDrawDot(const Vector &point, unsigned int color, bool bZEnable /*= true*/)
{
	SRTVariant_Adjacency &refAdjacency = GetCurrentAdjacency();

	const size_t startDotsIdx = refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotStartIdx;

	SDbgPoint *nextDot = m_pDbgInfo->pDots + (startDotsIdx + refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotsNum);

	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotsNum++;

	assert(refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotsNum < HALF_RCOMMANDS - 1);

	nextDot->Point = point;
	nextDot->Color = color;
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::auxDrawLine(const Vector &start, const Vector &end, unsigned int color, bool bZEnable /*= true*/)
{
	SRTVariant_Adjacency &refAdjacency = GetCurrentAdjacency();

	if (bZEnable)
	{
		const size_t startLinesIdx = refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLineStartIdx;

		SDbgPoint *nextLine = m_pDbgInfo->pLines + (startLinesIdx + refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesNum);

		nextLine->Point = start;
		nextLine->Color = color;

		(nextLine + 1)->Point = end;
		(nextLine + 1)->Color = color;	
		
		refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesNum += 2; // count dots

		assert(refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesNum < HALF_RCOMMANDS - 1);
	}
	else
	{
		const size_t startLinesIdx = refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLineZStartIdx;

		SDbgPoint *nextLine = m_pDbgInfo->pLinesNoZ + (startLinesIdx + refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesZNum);

		nextLine->Point = start;
		nextLine->Color = color;

		(nextLine + 1)->Point = end;
		(nextLine + 1)->Color = color;
		
		refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesZNum += 2; // count dots

		assert(refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesZNum < HALF_RCOMMANDS - 1);
	}
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::auxDrawLine(const Vector2f &start, const Vector2f &end, unsigned int color, bool bZEnable /*= true*/)
{

}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::auxDrawSphere(Vector &pos, float rad, unsigned int color, unsigned short segments /*= 16*/)
{

}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::auxDrawTriangle(const Vector &p0, const Vector &p1, const Vector &p2, unsigned int color /*= 0xffffffff*/)
{
	SRTVariant_Adjacency &refAdjacency = GetCurrentAdjacency();

	const size_t startTriIdx = refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTriangleStartIdx;

	SDbgTriangle *nextTriangle = m_pDbgInfo->pTriangles + (startTriIdx + refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTrianglesNum);

	nextTriangle->P0 = p0;
	nextTriangle->P1 = p1;
	nextTriangle->P2 = p2;
	nextTriangle->Color = color;

	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTrianglesNum++;

	assert(refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTrianglesNum < HALF_RCOMMANDS - 1);
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::auxDrawTriangle2D(const Vector2f &p0, const Vector2f &p1, const Vector2f &p2, unsigned int color /*= 0xffffffff*/)
{

}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::renderAdjacency(const LPRTVARIANT adjacency)
{
	size_t startCmd = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.idxCommandStart;
	size_t numCmd = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands;

	int activeStack = getActiveStackIndex();

	RenderSDK::LPRTVARIANTCMD pCommandBegin = getActiveCmd(startCmd);

	while (numCmd)
	{
		switch (pCommandBegin->vt)
		{
		case RenderSDK::ERC_WorldMatrix:
			m_pRenderDriver->SetWorldMatrix(pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m);
			break;
		case RenderSDK::ERC_ViewMatrix:
			m_pRenderDriver->SetViewMatrix(pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m);
			break;
		case RenderSDK::ERC_ProjMatrix:
			m_pRenderDriver->SetProjMatrix(pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m);
			break;
		case RenderSDK::ERC_CropMatrix:
			m_pRenderDriver->SetProjCropMatrix(pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m);
			break;
		case RenderSDK::ERC_Viewport:
			m_pRenderDriver->SetViewport(pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.x, 
			pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.y, 
			pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.width,
			pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.height,
			pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.MinZ,
			pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_4.MaxZ);
			break;
		case RenderSDK::ERC_Object:
			m_pRenderDriver->SetObjectFlags(&pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_5.uFlags);
			pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_5.pRenderObject->Render();
			break;
		default:
			assert(false);
			break;
		}

		pCommandBegin++;
		--numCmd;
	}

	renderAux(adjacency);
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::renderAux(const LPRTVARIANT adjacency)
{	
	int activeStack = getActiveStackIndex();
	
	// dots
	{
		size_t startDot = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotStartIdx;
		size_t numDots = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nDotsNum;

		SDbgPoint *pPoint = m_pDbgInfo->pDots + startDot;

		while (numDots)
		{
			m_pRenderDriver->AddDot(pPoint->Point.GetPtr(), pPoint->Color);
			pPoint++;
			--numDots;
		}
	}
	
	// lines
	{
		size_t startLine = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLineStartIdx;
		size_t numLinePoints = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesNum;

		SDbgPoint *pPoint = m_pDbgInfo->pLines + startLine;

		while (numLinePoints)
		{
			m_pRenderDriver->AddLine(pPoint->Point.GetPtr(), 0, pPoint->Color);
			pPoint++;
			numLinePoints--;
		}
	}

	// lines Z
	{
		size_t startLine = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLineZStartIdx;
		size_t numLinePoints = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nLinesZNum;

		SDbgPoint *pPoint = m_pDbgInfo->pLinesNoZ + startLine;

		while (numLinePoints)
		{
			m_pRenderDriver->AddLine(pPoint->Point.GetPtr(), 0, pPoint->Color, false);
			pPoint++;
			numLinePoints--;
		}
	}

	m_pRenderDriver->DrawDebugLines();

	// triangles
	{
		size_t startTriangle = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTriangleStartIdx;
		size_t numTriangles = adjacency->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nTrianglesNum;

		SDbgTriangle *pTriangle = m_pDbgInfo->pTriangles + startTriangle;

		while (numTriangles)
		{
			m_pRenderDriver->AddTriangle(pTriangle->P0.GetPtr(), pTriangle->P1.GetPtr(), pTriangle->P2.GetPtr(), pTriangle->Color);
			pTriangle++;
			numTriangles--;
		}

		m_pRenderDriver->DrawDebugTriangles();
	}
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::renderRenderTargets(SRenderContext *pContext)
{
	int activeStack = getActiveStackIndex();

	RenderSDK::RenderAdjacency::IteratorAdjacency iter(begin_adj(activeStack));
	RenderSDK::RenderAdjacency::IteratorAdjacency iter_end(end_adj(activeStack));

	while (iter != iter_end)
	{
		if (iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext != pContext)
		{
			++iter; // move to next adjacency
			continue;
		}

		if (iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target)
		{
			m_pRenderDriver->SetRenderTarget(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target,
											 iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bClearTarget,
											 iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.clearColor);

			// viewer parameters
			m_pRenderDriver->SetViewMatrix(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewMatrix);
			m_pRenderDriver->SetProjMatrix(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.projMatrix);
			m_pRenderDriver->SetNearPlane(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fNearPlane);
			m_pRenderDriver->SetFarPlane(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFarPlane);
			m_pRenderDriver->SetViewPos(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewPos);

			// params
			m_pRenderDriver->m_bFog = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bFog;
			m_pRenderDriver->m_fFogMin = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMin;
			m_pRenderDriver->m_fFogMax = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMax;
			m_pRenderDriver->m_fFogDensity = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogDensity;
			m_pRenderDriver->m_FogColor = iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nFogColor;

			renderAdjacency(*iter);

			m_pRenderDriver->EndRenderTarget(iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target);
		}
		++iter;
	}
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::renderTargets(SRenderContext *pContext)
{
	int activeStack = getActiveStackIndex();

	RenderSDK::RenderAdjacency::IteratorAdjacency iter(begin_adj(activeStack));
	RenderSDK::RenderAdjacency::IteratorAdjacency iter_end(end_adj(activeStack));

	std::stack<RenderSDK::RenderAdjacency::IteratorAdjacency> stackRT;

	while (iter != iter_end)
	{
		if (iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext != pContext)
		{
			++iter; // move to next adjacency
			continue;
		}

		if (iter->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target)
		{
			stackRT.push(iter);
		}
		++iter;
	}

	while (!stackRT.empty())
	{
		RenderSDK::RenderAdjacency::IteratorAdjacency top = stackRT.top();

		m_pRenderDriver->SetRenderTarget(top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target,
										 top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bClearTarget,
										 top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.clearColor);

		m_pRenderDriver->SetViewMatrix(top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewMatrix);
		m_pRenderDriver->SetProjMatrix(top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.projMatrix);
		m_pRenderDriver->SetNearPlane(top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fNearPlane);
		m_pRenderDriver->SetFarPlane(top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFarPlane);
		m_pRenderDriver->SetViewPos(top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.viewPos);

		// params
		m_pRenderDriver->m_bFog = top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.bFog;
		m_pRenderDriver->m_fFogMin = top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMin;
		m_pRenderDriver->m_fFogMax = top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogMax;
		m_pRenderDriver->m_fFogDensity = top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.fFogDensity;
		m_pRenderDriver->m_FogColor = top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.nFogColor;

		renderAdjacency(*top);

		m_pRenderDriver->EndRenderTarget(top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target);

		top->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_drawn = true;

		stackRT.pop();
	}
}
}