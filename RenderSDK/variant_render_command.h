#pragma once

#include "platform_specific.h"

class CRenderObject;

namespace RenderSDK
{
	enum ERenderCmdType
	{
		ERC_None = 0,
		ERC_WorldMatrix,
		ERC_ViewMatrix,
		ERC_ProjMatrix,
		ERC_CropMatrix,
		ERC_Viewport,
		ERC_Object,
		ERC_MAX,
	};

	typedef ERenderCmdType RTVARTYPECMD;

	typedef struct SRVariantRenderCommand
	{
			RTVARTYPECMD vt;
			WORD reserved0;
			WORD reserved1;

		union
		{
			struct __SRTVariant_matrix
			{
				float m[16];
			} __RT_VARIANT_NAME_2;

			struct __SRTVariant_fval
			{
				float value;
			} __RT_VARIANT_NAME_3;

			struct __SRTVariant_viewport
			{
				int x;
				int y;
				int width;
				int height;
				float MinZ;
				float MaxZ;
			} __RT_VARIANT_NAME_4;

			struct __SRTVariant_obj
			{
				const CRenderObject *pRenderObject;
				unsigned int			   uFlags;
			} __RT_VARIANT_NAME_5;

		}__RT_VARIANT_NAME_1;

	} RTVARIANTCMD, *LPRTVARIANTCMD;
}