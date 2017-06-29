#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        SkeletonNode::SkeletonNode()
            : NumBones(0)
            , BonesBindPose(NULL)
        {

        }

        //----------------------------------------------------------------------------------------------
        SkeletonNode::~SkeletonNode()
        {

        }

        //----------------------------------------------------------------------------------------------
        void SkeletonNode::DoRelease()
        {
            for (int Index = 0; Index < NumBones; ++Index)
            {
                delete[] BonesBindPose[Index];
            }
            delete[] BonesBindPose;

            for (TMapAnimSequence::iterator Iter = MapAnimSequence.begin(); Iter != MapAnimSequence.end(); ++Iter)
            {
                delete Iter->second;
            }
            MapAnimSequence.clear();

            Refcount::DoRelease();
        }

        //----------------------------------------------------------------------------------------------
        bool SkeletonNode::ReadMarkers(std::string& filename)
        {
            bool bResult = false;

            std::ifstream stream;
            stream.open(filename.c_str(), std::ios::in);

            if (stream.is_open())
            {
                int NumMarkers;
                stream >> NumMarkers;

                for (int Index = 0; Index < NumMarkers; ++Index)
                {
                    std::string Name;
                    int StartKey, LastKey;

                    stream >> StartKey >> LastKey >> Name;
                    MapAnimSequence.insert(std::make_pair(Name, new AnimSequence(StartKey, LastKey)));
                }

                bResult = true;
                stream.close();
            }

            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        bool SkeletonNode::ReadFromStream(std::ifstream& stream)
        {
            bool bResult = false;

            /*	stream >> NumBones;

                BonesBindPose = new float*[NumBones];

                for (int Index = 0; Index < NumBones; ++Index)
                {
                    *(BonesBindPose + Index) = new float[16];
                }

                for (int Index = 0; Index < NumBones; ++Index)
                {
                    int boneId;
                    stream >> boneId;

                    stream >> BonesBindPose[boneId][0] >> BonesBindPose[boneId][1] >> BonesBindPose[boneId][2] >> BonesBindPose[boneId][3] >>
                        BonesBindPose[boneId][4] >> BonesBindPose[boneId][5] >> BonesBindPose[boneId][6] >> BonesBindPose[boneId][7] >>
                        BonesBindPose[boneId][8] >> BonesBindPose[boneId][9] >> BonesBindPose[boneId][10] >> BonesBindPose[boneId][11] >>
                        BonesBindPose[boneId][12] >> BonesBindPose[boneId][13] >> BonesBindPose[boneId][14] >> BonesBindPose[boneId][15];
                }

                int NumFrames = 0;
                stream >> NumFrames;

                std::vector<AnimSequence*> VTmpSortedSeq;
                for (TMapAnimSequence::iterator Iter = MapAnimSequence.begin(); Iter != MapAnimSequence.end(); ++Iter)
                {
                    VTmpSortedSeq.push_back(Iter->second);
                }

                std::sort(VTmpSortedSeq.begin(), VTmpSortedSeq.end(),
                    boost::bind(&AnimSequence::StartFrame, _1) < boost::bind(&AnimSequence::StartFrame, _2)
                    );

                unsigned int CurrentFrame = 0;

                for (size_t Index = 0; Index < VTmpSortedSeq.size(); ++Index)
                {
                    while (CurrentFrame < VTmpSortedSeq[Index]->StartFrame)
                    {
                        for (int SkipIndex = 0; SkipIndex < NumBones; ++SkipIndex)
                        {
                            float p;
                            stream >> p >> p >> p >> p;
                            stream >> p >> p >> p;
                        }
                        CurrentFrame++;
                    }

                    VTmpSortedSeq[Index]->ReadFromStream(stream, NumBones);
                    CurrentFrame = VTmpSortedSeq[Index]->EndFrame;
                }
                */
            bResult = true;

            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        float* SkeletonNode::GetBoneBindMatrix(float *pMatrix, int BoneIndex) const
        {
            *(pMatrix + 0) = BonesBindPose[BoneIndex][0];
            *(pMatrix + 1) = BonesBindPose[BoneIndex][1];
            *(pMatrix + 2) = BonesBindPose[BoneIndex][2];
            *(pMatrix + 3) = BonesBindPose[BoneIndex][3];
            *(pMatrix + 4) = BonesBindPose[BoneIndex][4];
            *(pMatrix + 5) = BonesBindPose[BoneIndex][5];
            *(pMatrix + 6) = BonesBindPose[BoneIndex][6];
            *(pMatrix + 7) = BonesBindPose[BoneIndex][7];
            *(pMatrix + 8) = BonesBindPose[BoneIndex][8];
            *(pMatrix + 9) = BonesBindPose[BoneIndex][9];
            *(pMatrix + 10) = BonesBindPose[BoneIndex][10];
            *(pMatrix + 11) = BonesBindPose[BoneIndex][11];
            *(pMatrix + 12) = BonesBindPose[BoneIndex][12];
            *(pMatrix + 13) = BonesBindPose[BoneIndex][13];
            *(pMatrix + 14) = BonesBindPose[BoneIndex][14];
            *(pMatrix + 15) = BonesBindPose[BoneIndex][15];

            return pMatrix;
        }

        //----------------------------------------------------------------------------------------------
        bool SkeletonNode::GetFramePosition(float * pQuat, float * pPos, const std::string & Name, unsigned int BoneIndex, float Time) const
        {
            bool bResult = false;

            TMapAnimSequence::const_iterator Iter = MapAnimSequence.find(Name);

            if (Iter != MapAnimSequence.end())
            {
                size_t FrameIndex = size_t(Time);

                if (FrameIndex >= 0 && FrameIndex < Iter->second->NumFrames)
                {
                    Iter->second->AnimFrames[FrameIndex]->GetFramePosition(pQuat, pPos, BoneIndex);
                    bResult = true;
                }
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        unsigned int SkeletonNode::GetNumFrames(const std::string & Name) const
        {
            unsigned int OutValue = 0;

            TMapAnimSequence::const_iterator Iter = MapAnimSequence.find(Name);

            if (Iter != MapAnimSequence.end())
            {
                return Iter->second->NumFrames;
            }
            return OutValue;
        }
    }
}