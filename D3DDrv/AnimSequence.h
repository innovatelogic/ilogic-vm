#ifndef __animsequence_h__
#define __animsequence_h__

#ifdef WIN32
#pragma once
#pragma warning(disable: 4251)
#endif

#include "dxstdafx.h"

class D3DDRIVER_API AnimSequence
{
	typedef std::vector<class AnimFrame*> TAnimFrames;

public:
	AnimSequence(unsigned int Start, unsigned int End);
	~AnimSequence();

	virtual void ReadFromStream(std::ifstream& stream, int numBones);

//	bool operator < (const AnimSequence & Other) { return Other.StartFrame < StartFrame; }

	unsigned int NumFrames;
	unsigned int StartFrame;
	unsigned int EndFrame;
	float AnimTime;
	TAnimFrames AnimFrames;
};

#endif//__animsequence_h__