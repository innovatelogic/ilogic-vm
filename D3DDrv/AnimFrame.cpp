#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //------------------------------------------------------------------------
        AnimFrame::AnimFrame(unsigned int numBones)
            : Poses(NULL)
            , NumBones(numBones)
        {
            assert(NumBones != 0);

            Poses = new float*[NumBones];

            for (int Index = 0; Index < NumBones; ++Index)
            {
                *(Poses + Index) = new float[7];
            }
        }

        //------------------------------------------------------------------------
        AnimFrame::~AnimFrame()
        {
            for (int Index = 0; Index < NumBones; ++Index)
            {
                delete[] Poses[Index];
            }
            delete[] Poses;
        }

        //------------------------------------------------------------------------
        void AnimFrame::ReadFromStream(std::ifstream & stream)
        {
            for (int Index = 0; Index < NumBones; ++Index)
            {
                stream >> Poses[Index][0] >> Poses[Index][1] >> Poses[Index][2] >> Poses[Index][3];
                stream >> Poses[Index][4] >> Poses[Index][5] >> Poses[Index][6];
            }
        }

        //------------------------------------------------------------------------
        void AnimFrame::GetFramePosition(float * pQuat, float * pPos, unsigned int BoneIndex)
        {
            *(pQuat + 0) = Poses[BoneIndex][0];
            *(pQuat + 1) = Poses[BoneIndex][1];
            *(pQuat + 2) = Poses[BoneIndex][2];
            *(pQuat + 3) = Poses[BoneIndex][3];

            *(pPos + 0) = Poses[BoneIndex][4];
            *(pPos + 1) = Poses[BoneIndex][5];
            *(pPos + 2) = Poses[BoneIndex][6];
        }
    }
}