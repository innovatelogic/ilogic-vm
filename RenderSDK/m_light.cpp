#include "m_light.h"

m_node::node_type m_light::type = LIGHT;

//------------------------------------------------------------------------
m_light::m_light()
{
	color     = Vector4f(1.f,1.f,1.f,1.f);
	specular  = Vector4f(1.f,1.f,1.f,1.f);
	ambient   = Vector4f(0.f,0.f,0.f,0.f);
	light     = ANONYMOUS;          
	direction = Vector(0.f,0.f,0.f);
	range = 0.f;
	Kc    = 0.f;
	Kl    = 0.f;
	Kq    = 0.f;
	falloff = 0.f;
	theta   = 0.f;
	phi     = 0.f;
	specular_exp = 0.f;
}

//------------------------------------------------------------------------
m_light::~m_light()
{
	;//
}

//------------------------------------------------------------------------
m_node::node_type m_light::get_type() const
{
	return type;
}

//------------------------------------------------------------------------
m_output_stream & operator << (m_output_stream& rOutputStream, const m_light& rLight)
{
	// First write the node part of light
	rOutputStream << dynamic_cast<const m_node &>(rLight);

	// Light colors
	rOutputStream << rLight.color << rLight.specular << rLight.ambient;

	// Light type
	rOutputStream << static_cast<const unsigned int>(rLight.light);

	// Specular exponent
	rOutputStream << rLight.specular_exp;

	// Direction and range
	rOutputStream << rLight.direction;
	rOutputStream << rLight.range;

	// Attenuation parameters
	rOutputStream << rLight.Kc << rLight.Kl << rLight.Kq;

	// Spotlight parameters
	rOutputStream << rLight.falloff << rLight.theta << rLight.phi;

	return rOutputStream;
}

//------------------------------------------------------------------------
m_input_stream & operator >> (m_input_stream& rInputStream, m_light& rLight)
{                             
	// First write the node part of light
	rInputStream >> dynamic_cast<m_node &>(rLight);

	// Light colors
	rInputStream >> rLight.color >> rLight.specular >> rLight.ambient;

	// Light type
	rInputStream >> reinterpret_cast<unsigned int&>(rLight.light);

	// Specular exponent
	rInputStream >> rLight.specular_exp;

	// Direction and range
	rInputStream >> rLight.direction;
	rInputStream >> rLight.range;

	// Attenuation parameters
	rInputStream >> rLight.Kc >> rLight.Kl >> rLight.Kq;

	// Spotlight parameters
	rInputStream >> rLight.falloff >> rLight.theta >> rLight.phi;
	return rInputStream;
}
