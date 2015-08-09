#ifndef __animframe_h__
#define __animframe_h__

#ifdef WIN32
#pragma once
#endif

#include "dxstdafx.h"

class D3DDRIVER_API AnimFrame
{
public:
	AnimFrame(unsigned int numBones);
	~AnimFrame();

 	void ReadFromStream(std::ifstream & stream);
	void GetFramePosition(float * pQuat, float * pPos, unsigned int BoneIndex);

protected:
private:
	int	NumBones;
	float **Poses;
};

#endif//__animframe_h__