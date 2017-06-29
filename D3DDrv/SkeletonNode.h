
#pragma once

#include "Refcount.h"

namespace oes
{
    namespace d3d
    {
        class AnimSequence;
        class D3DDRIVER_API SkeletonNode : public Refcount
        {
            typedef std::map<std::string, AnimSequence*> TMapAnimSequence;

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
    }
}