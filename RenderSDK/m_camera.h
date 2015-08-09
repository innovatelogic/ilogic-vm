#ifndef _m_camera_h_ 
#define _m_camera_h_

#pragma once

#include "m_stdafx.h"
#include "m_node.h"

class m_output_stream;
class m_input_stream;

//////////////////////////////////////////////////////////////////////////
struct EXPORT m_camera: public m_node
{
	m_camera();
	virtual ~m_camera();

	typedef enum _camera_type
	{
		ANONYMOUS,
		POINT,
		DIRECTIONAL,
		SPOT
	} camera_type;

	static  node_type type;              // to be set to CAMERA
	virtual node_type get_type() const;  // return the node type

	float       fov;            // field of view
	float       focal_length;   // focal length
};

EXPORT m_output_stream & operator << (m_output_stream & rOutputStream, const m_camera & rCamera);
EXPORT m_input_stream & operator >> (m_input_stream & rInputStream, m_camera & rCamera);


#endif // _m_camera_h_