#pragma once

#include "platform_specific.h"
#include "mathlib.h"
#include "RenderSDKInterface.h"

namespace RenderSDK
{
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
typedef unsigned short RTVARTYPE;

typedef struct SRTVariant_Adjacency
{	
	union
	{
		struct __SRTVariant_Adjacency
		{
			RTVARTYPE vt;
			WORD reserved0;
			WORD reserved1;
			union 
			{
				size_t	idxCommandStart;	// start command index
				size_t	numCommands;		// end command index

				size_t	idxObjectStart;		// start objects index
				size_t	numObjects;			// objects number 

				float	projMatrix[16];
				float	viewMatrix[16];

				float	fNearPlane;
				float	fFarPlane;

				float	viewPos[4];

				const class D3DRenderTarget *rt_target;
				float	rt_ProjMatrix[16];
				float	rt_ViewMatrix[16];
				bool	bClearTarget;
				DWORD	clearColor;

				unsigned int RenderInfoMask;

				// viewport params
				DWORD x;
				DWORD y;
				DWORD Width;	// Zero value mean default viewport
				DWORD Height;	// Zero value mean default viewport
				float fMinZ;
				float fMaxZ;

				// fog params
				bool			bFog;
				EFogMode		eFogMode;
				float			fFogMin;
				float			fFogMax;
				float			fFogDensity;
				unsigned int	nFogColor;

				AdjDebugInfo *debugInfo; // debug render info

				class SRenderContext *pRenderContext;
			} __RT_VARIANT_NAME_3;
		} __RT_VARIANT_NAME_2;

	} __RT_VARIANT_NAME_1;
} RTVARIANT, *LPRTVARIANT;

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class RenderAdjacency
{
	//RenderAdjacency(const RenderAdjacency &that) = delete;
	//RenderAdjacency& operator=(const RenderAdjacency &that) = delete;

public:
	RenderAdjacency();
	~RenderAdjacency();

protected:
private:
	size_t		m_nMaxAdjacency;
	LPRTVARIANT m_pVariantAdjacency;
};
}