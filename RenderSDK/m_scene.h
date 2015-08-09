#ifndef _m_scene_h_
#define _m_scene_h_

#ifdef _WIN32
#pragma once
#endif

#include "m_stdafx.h"
#include "m_refcount.h"

//------------------------------------------------------------------------
struct EXPORT m_scene : public m_refcount
{
	typedef std::vector<struct m_node*>		TVecNodes;
	typedef TVecNodes::iterator				TVecNodesIterator;

	typedef std::vector<struct m_texture*>	TVecTextures;
	typedef TVecTextures::iterator			TVecTexturesIterator;

	typedef std::vector<struct m_material*>	TVecMaterials;
	typedef TVecMaterials::iterator			TVecMaterialsIterator;

	m_scene();
	virtual ~m_scene();

	virtual void		Release();
	unsigned int		find_node_idx(const struct m_node * node);

	// visitor entry point
	virtual bool		accept(class m_visitor & visitor) const;

	std::string			name;

	// scene nodes
	TVecNodes			nodes;

	// textures
	TVecTextures		textures;

	// materials
	TVecMaterials		materials;

	// properties
	Vector4f			ambient;        // ambient color

	// scene bounding box information: models, lights, skeletons,...
	Vector				aabb_min;
	Vector				aabb_max;

	// models bounding box information: models only.
	Vector				models_aabb_min;
	Vector				models_aabb_max;

	// animation...
	unsigned int		num_keys;
};

EXPORT class m_output_stream & operator << (class m_output_stream & rOutputStream, const m_scene & rScene);
EXPORT class m_input_stream & operator >> (class m_input_stream & rInputStream, m_scene & rScene);

EXPORT class m_output_stream & SaveObjects(class m_output_stream & rOutputStream, const m_scene & Scene);

#endif //_m_scene_h_