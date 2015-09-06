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
SRVariantRenderCommand& RenderAdjacency::PushRenderCommand()
{
	byte NonActive = (m_ActiveStack == 0) ? 1 : 0;

	assert(m_aContext[NonActive].nIndexAdjaency < HALF_ADJACENCY - 1);

	const size_t shift = m_aContext[NonActive].nIndexAdjaency;

	SRTVariant_Adjacency &refAdjacency = *(m_pVariantAdjacency + (HALF_ADJACENCY * NonActive) + shift);

	const size_t startCmdIndex = refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.idxCommandStart;

	refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands++;

	assert(refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands < HALF_RCOMMANDS - 1);

	return m_pVariantCommands[startCmdIndex + refAdjacency.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.numCommands];
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::swapBuffer()
{
	m_ActiveStack = (m_ActiveStack == 0) ? 1 : 0; // toggle active stack

	m_aContext[m_ActiveStack].nIndexAdjaency = 0;
	m_aContext[m_ActiveStack].nIndexCommand  = 0;
}

//----------------------------------------------------------------------------------------------
LPRTVARIANT RenderAdjacency::begin(size_t index) const
{
	assert(index >= 0 && index < 2);

	return m_pVariantAdjacency + (HALF_ADJACENCY * index);
}

//----------------------------------------------------------------------------------------------
LPRTVARIANT RenderAdjacency::end(size_t index) const
{
	assert(index >= 0 && index < 2);

	return m_pVariantAdjacency + (HALF_ADJACENCY * index) + m_aContext[index].nIndexAdjaency;
}
}