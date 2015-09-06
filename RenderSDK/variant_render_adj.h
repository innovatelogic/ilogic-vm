#pragma once

#include "platform_specific.h"

namespace RenderSDK
{
	enum ERenderAdjType
	{
		ERA_None = 0,
		ERA_Adjacency,
		ERA_MAX,
	};

	typedef ERenderAdjType RTVARTYPE;

	typedef struct SRTVariant_Adjacency
	{	
		union
		{
			RTVARTYPE vt;
			WORD reserved0;
			WORD reserved1;

			struct __SRTVariant_Adjacency
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
			
			} __RT_VARIANT_NAME_2;

		} __RT_VARIANT_NAME_1;
	} RTVARIANT, *LPRTVARIANT;

}//RenderSDK