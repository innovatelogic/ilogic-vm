#include "RenderAdjacency.h"
#include <stdlib.h>

#define MAX_ADJACENCY 64

namespace RenderSDK
{
//----------------------------------------------------------------------------------------------
RenderAdjacency::RenderAdjacency()
{
	m_pVariantAdjacency[0] = (LPRTVARIANT) malloc(sizeof(SRTVariant_Adjacency) * MAX_ADJACENCY);
	m_pVariantAdjacency[1] = (LPRTVARIANT) malloc(sizeof(SRTVariant_Adjacency) * MAX_ADJACENCY);
}

//----------------------------------------------------------------------------------------------
RenderAdjacency::~RenderAdjacency()
{
	assert(m_pVariantAdjacency[0]);
	free(m_pVariantAdjacency[0]);

	assert(m_pVariantAdjacency[1]);
	free(m_pVariantAdjacency[1]);
}

//----------------------------------------------------------------------------------------------
SRTVariant_Adjacency& RenderAdjacency::PushRenderQuevueAdjaency()
{
	return *m_pVariantAdjacency[0];
}

//----------------------------------------------------------------------------------------------
void RenderAdjacency::PopRenderQuevueAdjaency()
{

}

}