#ifndef __d3ddriverstdafx_h__
#define __d3ddriverstdafx_h__

#ifdef _WIN32
#pragma once
#endif

#define MAX_LINE_POINTS 10240
#define MAX_DBG_TRIANGLES 1024
#define SHADOW_MAP_SIZE 1024

/** 
* Platform dependent primitives.
*/
#include "RenderContext.h"
#include "TextureNode.h"
#include "VolumeTextureNode.h"
#include "FontNode.h"
#include "MaterialEffectNode.h"
#include "SkeletonNode.h"
#include "RenderTargetNode.h"

#include "AnimFrame.h"
#include "AnimSequence.h"
#include "BaseMaterial.h"
#include "D3DAnimMesh.h"

#include "D3DMesh.h"
#include "D3DFontMesh.h"
#include "D3DShader.h"
#include "D3DSkeleton.h"
#include "D3DSkeletonData.h"
#include "D3DRenderTarget.h"
#include "Font2D.h"
#include "MaterialEffect.h"
#include "MaterialEffectUI.h"
#include "MaterialEffectFont.h"
#include "MaterialEffect_ButtonStates.h"
#include "MaterialEffectLava.h"

//#include "NvTriStrip.h"
#include "Refcount.h"
#include "SubMeshNode.h"
#include "Texture2D.h"
#include "Texture3D.h"

#include "D3DRenderInterface.h"
#include "D3DDriver.h"

#endif//__d3ddriverstdafx_h__