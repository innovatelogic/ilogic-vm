#include "RenderAdjacency.h"
#include <stdlib.h>
#include <climits>

#define MAX_ADJACENCY 64
#define HALF_ADJACENCY 32

#define MAX_RCOMMANDS 65535 + 1
#define HALF_RCOMMANDS 32768

namespace RenderSDK
{
//----------------------------------------------------------------------------------------------
RenderAdjacency::RenderAdjacency()
	: m_ActiveStack(0)
{
	m_pVariantAdjacency = (LPRTVARIANT) malloc(sizeof(SRTVariant_Adjacency) * MAX_ADJACENCY);
	m_pVariantCommands = (LPRTVARIANTCMD) malloc(sizeof(SRVariantRenderCommand) * MAX_RCOMMANDS);
}

//----------------------------------------------------------------------------------------------
RenderAdjacency::~RenderAdjacency()
{
	assert(m_pVariantAdjacency);
	free(m_pVariantAdjacency);

	assert(m_pVariantCommands);
	free(m_pVariantCommands);
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
	byte NonActive = (m_ActiveStack == 0) ? 1 : 0;

	assert(m_aContext[NonActive].nIndexAdjaency < HALF_ADJACENCY - 1);

	const size_t shift = m_aContext[NonActive].nIndexAdjaency;

	return *(m_pVariantAdjacency + (HALF_ADJACENCY * NonActive) + shift);
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
}