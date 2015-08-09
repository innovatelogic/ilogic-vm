#include "m_camera.h"
#include "m_interface.h"

//////////////////////////////////////////////////////////////////////////
m_node::node_type m_camera::type = CAMERA;

//////////////////////////////////////////////////////////////////////////
m_camera::m_camera()
{
	fov = 0.f;
	focal_length = 0.f;
}

//////////////////////////////////////////////////////////////////////////
m_camera::~m_camera()
{
   ;//
}

//////////////////////////////////////////////////////////////////////////
m_node::node_type m_camera::get_type() const // return the node type
{
	return type;
}

//////////////////////////////////////////////////////////////////////////
m_output_stream & operator << (m_output_stream & rOutputStream, const m_camera & rCamera)
{
	// First write the node part of camera
	rOutputStream << dynamic_cast<const m_node &>(rCamera);

	// Camera parameters
	rOutputStream << rCamera.fov << rCamera.focal_length;

	return rOutputStream;
} 

//////////////////////////////////////////////////////////////////////////
m_input_stream & operator >> (m_input_stream & rInputStream, m_camera & rCamera)
{
	// First write the node part of camera
	rInputStream >> dynamic_cast<m_node &>(rCamera);

	// Camera parameters
	rInputStream >> rCamera.fov >> rCamera.focal_length;

    return rInputStream;
}

