#ifndef _m_animation_h_
#define _m_animation_h_

#include "m_stdafx.h"

#ifdef _WIN32
#pragma once
#endif

struct EXPORT m_animation
{
	m_animation();
	virtual ~m_animation();

	unsigned int    num_keys;       // number of animation keyframes
	float           freq;           // sampling frequency

	// animation tracks - null means not available
	Quaternion    * rot;             // rotation
	Vector        * pos;             // position
	Vector        * scale;           // scale
};

EXPORT class m_output_stream & operator << (m_output_stream & rOutputStream, const m_animation & rAnimation);
EXPORT class m_input_stream & operator >> (m_input_stream & rInputStream, m_animation & rAnimation);

#endif //_m_animation_h_