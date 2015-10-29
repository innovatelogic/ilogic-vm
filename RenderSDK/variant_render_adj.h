#pragma once

#include "platform_specific.h"

class D3DRenderTarget;

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

				// debug info
				size_t nDotStartIdx;
				size_t nDotsNum;
				size_t nLineStartIdx;
				size_t nLinesNum;
				size_t nLineZStartIdx;
				size_t nLinesZNum;
				size_t nTriangleStartIdx;
				size_t nTrianglesNum;
				size_t nSphereStartIdx;
				size_t nSpheresNum;

				float	projMatrix[16];
				float	viewMatrix[16];

				float	fNearPlane;
				float	fFarPlane;

				float	viewPos[4];

				const D3DRenderTarget *rt_target;
				float	rt_ProjMatrix[16];
				float	rt_ViewMatrix[16];
				bool	bClearTarget;
				DWORD	clearColor;
				bool	rt_drawn;

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

				class SRenderContext *pRenderContext;
			
			} __RT_VARIANT_NAME_2;

		} __RT_VARIANT_NAME_1;
	} RTVARIANT, *LPRTVARIANT;

}//RenderSDK