#include "RenderAdjacency.h"
#include <stdlib.h>

namespace RenderSDK
{
//----------------------------------------------------------------------------------------------
RenderAdjacency::RenderAdjacency()
	: m_nMaxAdjacency(64)
	, m_pVariantAdjacency(nullptr)
{
	assert(m_nMaxAdjacency > 0);
	m_pVariantAdjacency = (LPRTVARIANT) malloc(sizeof(SRTVariant_Adjacency));
}

//----------------------------------------------------------------------------------------------
RenderAdjacency::~RenderAdjacency()
{
	assert(m_pVariantAdjacency);
	free(m_pVariantAdjacency);
}

}