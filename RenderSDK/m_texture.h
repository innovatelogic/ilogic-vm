#pragma once

#include "m_stdafx.h"

using namespace oes::common_base;

struct EXPORT m_texture
{
    m_texture();
	m_texture(const m_texture & tex);
	virtual ~m_texture();
	
	const m_texture & operator= (const m_texture & tex);
	void copy_from(const m_texture &tex);

	// defines the different types of textures
	typedef enum _tex_type
	{
		CUSTOM              = 0x00000000,
		AMBIENT             = 0x00000001,
		DIFFUSE             = 0x00000002,
		SPECULAR            = 0x00000003, // specular color map
		SPECULAR_POWER      = 0x00000004, // specular highlight map
		GLOSS               = 0x00000005,
		SELF_ILLUMATION     = 0x00000006,
		BUMP                = 0x00000007,
		NORMAL              = 0x00000008,
		OPACITY             = 0x00000009,
		REFLECTION          = 0x0000000A,
		REFRACTION          = 0x0000000B,
		CUBE_MAP            = 0x0000000C
	} tex_type;

	tex_type        type;
	std::string		name;
	Matrix          tex_mat;        // texture matrix
};

/// Write a scene to an nv_output_stream.
EXPORT class m_output_stream & operator << (m_output_stream & rOutputStream, const m_texture & rTexture);

/// Read a scene from an nv_input_stream.
EXPORT class m_input_stream & operator >> (m_input_stream & rInputStream, m_texture & rTexture);
