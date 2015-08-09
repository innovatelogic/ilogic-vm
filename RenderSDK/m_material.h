#ifndef _m_material_h_
#define _m_material_h_

#ifdef _WIN32
#pragma once
#endif

#include "m_stdafx.h"
#include "m_attribute.h"

struct EXPORT m_material 
{
	m_material();
	virtual ~m_material();
	
	std::string			name;			// name of the material
	unsigned int		id;             // material id

	std::string			fx_file;
	std::string			tehnique;

	// material colors...
	Vector4f            diffuse;        // diffuse color
	Vector4f            specular;       // specular color
	float               shininess;      // specular exponent
	Vector4f            ambient;        // ambient color
	Vector4f            emission;       // emitted light intensity of the material
	float               transparent;    // level of transparency
	                                    // fog properties...
	bool                fog;            // fog
	Vector4f            fog_color;      // fog color

	m_attribute			attr;

	std::vector<unsigned int>	textures;
	std::vector<unsigned int>	tex_channel;
};

/// Write a material to an nv_output_stream.
EXPORT m_output_stream & operator << (m_output_stream & rOutputStream, const m_material & rMaterial);

/// Read a material from an nv_input_stream.
EXPORT m_input_stream & operator >> (m_input_stream & rInputStream, m_material & rMaterial);

EXPORT void WriteToStreamMaterial(std::stringstream &ss, const m_material * material);

EXPORT void LoadMaterial(const char *Path, m_material *pOutMaterial);

#endif // _m_material_h_