#ifndef __animtrack_h__
#define __animtrack_h__

#ifdef _WIN32
#pragma once
#endif

#include "platform_specific.h"
#include "mathlib.h"
/*
struct TransformPose
{
	Quaternion rot;
	Vector	   pos;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class AnimFrame
{
	typedef std::vector<TransformPose> TVecBonesTransform;

public:
	AnimFrame(int numBones)
	{
		BonesTransform.resize(numBones);
	}

	void AddBoneTransform(const Quaternion& rot, const Vector & pos, int boneID)
	{
		TransformPose Pose;
		Pose.rot = rot;
		Pose.pos = pos;

		BonesTransform[boneID] = Pose;
	}

	const TransformPose& GetBoneTransform(int Index) { return BonesTransform.at(Index); }

	size_t GetNumTransform() const { return BonesTransform.size(); }

	TVecBonesTransform	BonesTransform;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class AnimTrack
{
	typedef std::vector<AnimFrame> TVecAnimFrames;

public:
	int		Build(class IGameObject * IObject, class SkeletonObjectMax * Skeleton);
	size_t	GetNumFrames() const { return AnimFrames.size(); }
	const AnimFrame& GetAnimFrame(int Index) { return AnimFrames.at(Index); }

	void WriteToStream(std::stringstream &ss)
	{
		ss << GetNumFrames() << std::endl;

		for (size_t Index = 0; Index < GetNumFrames(); ++Index)
		{
			const AnimFrame & frame = GetAnimFrame(Index);

			for (size_t IndexFrame = 0; IndexFrame < frame.GetNumTransform(); ++IndexFrame)
			{
				ss << frame.BonesTransform[IndexFrame].rot.x << " " << frame.BonesTransform[IndexFrame].rot.y << " " << frame.BonesTransform[IndexFrame].rot.z << " " << frame.BonesTransform[IndexFrame].rot.w << std::endl;
				ss << frame.BonesTransform[IndexFrame].pos.x << " " << frame.BonesTransform[IndexFrame].pos.y << " " << frame.BonesTransform[IndexFrame].pos.z << std::endl;
			}
		}
	}

private:
	TVecAnimFrames AnimFrames;
};
*/
#endif//__animtrack_h__