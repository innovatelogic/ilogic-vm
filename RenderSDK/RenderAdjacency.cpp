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
	pSpheres = (SDbgSphere*) malloc(sizeof(SDbgSphere) * MAX_RCOMMANDS);;
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
	const size_t &cmd_shift = m_aContext[nonActive].nIndexCommand;

	SRTVariant_Adjacency &refAdjacency = *(m_pVariantAdjacency + (HALF_ADJACENCY * nonActive) + shift);

	// init base indexes
	{
		refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.idxCommandStart = (HALF_RCOMMANDS * nonActive) + cmd_shift;
		refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands = 0;
		refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext = nullptr;
		refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.rt_target = nullptr;
	}

	return refAdjacency;
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::PopRenderQuevueAdjaency()
{
	byte nonActive = (m_ActiveStack == 0) ? 1 : 0;

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
		}
		++iter;
	}
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
			m_pRenderDriver->SetWorldMatrix(pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m);
			break;
		case RenderSDK::ERC_ProjMatrix:
			m_pRenderDriver->SetWorldMatrix(pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m);
			break;
		case RenderSDK::ERC_CropMatrix:
			m_pRenderDriver->SetWorldMatrix(pCommandBegin->__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.m);
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
}
}