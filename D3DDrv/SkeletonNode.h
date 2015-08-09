#ifndef __skeletonnode_h__
#define __skeletonnode_h__

#ifdef WIN32
#pragma once
#endif

#include "Refcount.h"

class D3DDRIVER_API SkeletonNode : public Refcount
{
	typedef std::map<std::string, class AnimSequence*> TMapAnimSequence;

public:
	SkeletonNode();
	virtual ~SkeletonNode();

	bool ReadMarkers(std::string& filename);
	bool ReadFromStream(std::ifstream& stream);

	float* GetBoneBindMatrix(float * pMatrix, int BoneIndex) const;
	bool   GetFramePosition(float * pQuat, float * pPos, const std::string & Name, unsigned int BoneIndex, float Time) const;
	unsigned int GetNumFrames(const std::string & Name) const;

protected:
	virtual void DoRelease();

	int NumBones;
	float** BonesBindPose;
	TMapAnimSequence MapAnimSequence;
};

#endif//__skeletonnode_h__