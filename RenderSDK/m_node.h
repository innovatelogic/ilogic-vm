#ifndef _m_node_h_
#define _m_node_h_

#ifdef _WIN32
#pragma once
#endif

#include "m_stdafx.h"

#include "m_animation.h"
#include "m_streams.h"
#include "m_attribute.h"
#include "m_refcount.h"
#include "m_visitor.h"
#include "m_scene.h"
#include "m_collision.h"

//-----------------------------------------------------------------------
struct EXPORT m_node: public m_refcount
{
    m_node();
	virtual ~m_node();

	struct m_camera * asCamera();
	struct m_model *  asModel();
	struct m_light *  asLight();
	
	typedef enum _node_type
	{
		ANONYMOUS       = 0x00000000,
		GEOMETRY        = 0x00000001,
		LIGHT           = 0x00000002,
		CAMERA          = 0x00000003
	} node_type;

	static  node_type   type;				// to be set to ANONYMOUS
	virtual node_type   get_type() const;	// return the node type

	virtual bool        accept(const m_scene & scene, m_visitor & visitor) const;

	virtual m_node*		find_node(const std::string& _name);

	std::string			name;
	Matrix				xform;              // model matrix transform, i.e.:
											// transforms a vertex into world space
	// hierarchy
	std::vector<unsigned int>	childrens;	
	std::vector<m_node*>		Childrens;
	m_node*						Parent;
											// into the scene node array
	unsigned int		parent;				// index to the parent - 
											// if parent == NV_BAD_IDX this is a root node
	unsigned int		target;				// index to the target - 
											// if target == NV_BAD_IDX there isn't any target
											// A target defines an "abstract relation" before
											// two nodes. For example, if "this" node is a camera
											// target defines the object the camera is looking at.
											// To look at more than one node, create a hierarchy
											// with the interested node as children, and target a
											// dummy parent node.
    
	m_animation			anim;				// animation 
	m_attribute			attr;				// attribute
	m_collision			ctype;
};

EXPORT m_output_stream & operator << (m_output_stream & rOutputStream, const m_node & rNode);
EXPORT m_input_stream & operator >> (m_input_stream & rInputStream, m_node & rNode);

#endif//_m_node_h_