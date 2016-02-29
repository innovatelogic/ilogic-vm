#pragma once

#include "m_stdafx.h"

using namespace oes::common_base;

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
