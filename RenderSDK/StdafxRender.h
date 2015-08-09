#ifndef _std_afx_render_h_
#define _std_afx_render_h_

#ifdef _WIN32
  #pragma once
#else 
#endif

#include "../D3DDrv/d3ddriverstdafx.h"

#include "RenderTypes.h"
#include "RenderObject.h"
#include "Comp_MeshNode.h"
#include "Comp_SkeletonObject.h"
#include "Comp_AnimTrack.h"
#include "Comp_AnimMesh.h"
#include "Comp_StaticMesh.h"
#include "Comp_MaterialBaseEffect.h"
#include "Comp_MaterialEffect.h"
#include "Comp_MaterialEffectUI.h"
#include "Comp_MaterialEffectFont.h"
#include "Comp_MaterialButton.h"
#include "Comp_MaterialEffectLava.h"
#include "Comp_EnvSceneInfo.h"
#include "Comp_RenderTarget.h"
#include "UIComp_DrawImage.h"
#include "UIComp_DrawImageState.h"
#include "UIComp_DrawFont.h"
#include "UIComp_DrawFontState.h"
#include "UIComp_DrawMesh.h"
#include "BoneObject.h"

#include "m_interface.h"
#include "m_file.h"
#include "m_scene.h"
#include "m_node.h"
#include "m_model.h"
#include "m_mesh.h"
#include "m_material.h"
#include "m_texture.h"
#include "m_animation.h"

#include "D3DMeshAllocator.h"
#include "FXMatManager.h"

#include "RenderSDK.h"

#endif // _std_afx_render_h_