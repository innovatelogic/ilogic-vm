#ifndef _m_light_h_ 
#define _m_light_h_

#ifdef _WIN32
#pragma once
#endif

#include "m_node.h"

struct EXPORT m_light: public m_node
{
     m_light();
	 virtual ~m_light();

	 typedef enum _light_type
	 {
        ANONYMOUS,
		POINT,
		DIRECTIONAL,
		SPOT,
	 } light_type;

      static node_type type;  // to be set to LIGHT

	  virtual node_type get_type() const;

	  Vector4f       color;          // light color
	  Vector4f       specular;       // light specular
	  Vector4f       ambient;        // light ambient
	  light_type     light;          
	  float          specular_exp;   // specular exponent
       
	  Vector         direction;

	  float          range;

	  // light attenuation:
	  //           1
	  // ----------------------
	  // Kc + Kl * d + Kq * d^2
	  float       Kc;             // constant term
	  float       Kl;             // linear term
	  float       Kq;             // quadratic term

	  // spot light properties...
	  float       falloff;        // spot falloff (d3d) or cutoff (ogl)
	  float       theta;          // inner cone angle - fully illuminated 
	  // theta elt [0, phi]
	  float       phi;            // outer cone angle - fading out
};

EXPORT m_output_stream & operator << (m_output_stream& rOutputStream, const m_light& rLight);
EXPORT m_input_stream & operator >> (m_input_stream& rInputStream, m_light& rLight);

#endif//_m_light_h_