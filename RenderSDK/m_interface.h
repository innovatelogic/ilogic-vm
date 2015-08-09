#ifndef _m_interface_
#define _m_interface_

#pragma once

#include "platform_specific.h"

#ifndef _m_refcount_h_
#include "m_refcount.h"
#endif // _m_refcount_

#ifndef _m_animation_h_
#include "m_animation.h"
#endif // _m_animation_h_

#ifndef _m_visitor_h_
#include "m_visitor.h"
#endif // _m_visitor_h_

#ifndef _m_node_h_
#include "m_node.h"
#endif // _m_node_h_

#ifndef _m_texture_h_
#include "m_texture.h"
#endif // _m_texture_h_

#ifndef _m_texcoord_set_h_
#include "m_texcoord_set.h"
#endif // _m_texcoord_set_h_

#ifndef _m_material_h_
#include "m_material.h"
#endif // _m_material_h_

#ifndef _m_camera_h_
#include "m_camera.h"
#endif // _m_camera_h_

#ifndef _m_light_h_
#include "m_light.h"
#endif // _m_light_h_

#ifndef _m_mesh_h_
#include "m_mesh.h"
#endif // _m_mesh_h_

#ifndef _m_model_h_
#include "m_model.h"
#endif // _m_model_h_

#ifndef _m_scene_h_
#include "m_scene.h"
#endif // _m_scene_h_

#ifndef _m_drawmodel_visitor_h_
#include "m_drawmodel_visitor.h"
#endif // _m_drawmodel_visitor_h_

#ifndef _m_factory_h_
#include "m_factory.h"
#endif // _m_factory_h_

#include "m_streams.h"
#include "m_file.h"

#include "streams.h"
#include "mathlib.h"

#define     M_LHS     0x00000001
#define     M_RHS     0x00000002

#endif //_m_interface_