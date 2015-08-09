#include "m_animation.h"
#include "m_interface.h"


//----------------------------------------------------------------------------------
m_animation::m_animation()
{
	num_keys = 0;      // number of animation keyframes
	freq     = 0.f;    // sampling frequency

	// animation tracks - null means not available
	rot = 0;            // rotation
	pos = 0;            // position
	scale = 0;          // scale
}

//----------------------------------------------------------------------------------
m_animation::~m_animation()
{
	if (pos){
		delete [] pos;
	}
	pos = 0;

	if (rot){
		delete [] rot;
	}
	rot = 0;

	if (scale){
		delete [] scale;
	}
	scale = 0;
}

//----------------------------------------------------------------------------------
m_output_stream & operator << (m_output_stream & rOutputStream, const m_animation & rAnimation)
{
	rOutputStream << rAnimation.num_keys;
	rOutputStream << rAnimation.freq;

	bool bRotation = (rAnimation.rot ? true : false);
	bool bPosition = (rAnimation.pos ? true : false);
	bool bScale    = (rAnimation.scale ? true : false);

    rOutputStream << bRotation << bPosition << bScale;

	for (unsigned int iKey = 0; iKey < rAnimation.num_keys; ++iKey)
	{
		if (bRotation)
			rOutputStream << rAnimation.rot[iKey];
		if (bPosition)
			rOutputStream << rAnimation.pos[iKey];
		if (bScale)
			rOutputStream << rAnimation.scale[iKey];
	}

	return rOutputStream;
}

//----------------------------------------------------------------------------------
m_input_stream & operator >> (m_input_stream & rInputStream, m_animation & rAnimation)
{
	rInputStream >> rAnimation.num_keys;
	rInputStream >> rAnimation.freq;

    bool bRotation;
	bool bPosition;
	bool bScale;

    rInputStream >> bRotation >> bPosition >> bScale;

	if (bRotation){
	  rAnimation.rot = new Quaternion[rAnimation.num_keys];
	}else{
      rAnimation.rot = 0;
	}

	if (bPosition)
	{
      rAnimation.pos = new Vector[rAnimation.num_keys]; 
	}else{
	  rAnimation.pos = 0;	
	}

	if(bScale)
	{
	  rAnimation.scale = new Vector[rAnimation.num_keys];
	}else{
      rAnimation.scale = 0; 
	} 

	for(unsigned int iKey = 0; iKey < rAnimation.num_keys; ++iKey)
	{
      if (bRotation)
	     rInputStream >> rAnimation.rot[iKey];

	  if (bPosition)
         rInputStream >> rAnimation.pos[iKey];

	  if (bScale)
         rInputStream >> rAnimation.scale[iKey];
	}

	return rInputStream;
}