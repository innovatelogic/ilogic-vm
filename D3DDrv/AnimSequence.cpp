#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //------------------------------------------------------------------------
        AnimSequence::AnimSequence(unsigned int Start, unsigned int End)
            : StartFrame(Start)
            , EndFrame(End)
            , AnimTime(0.f)
        {
            NumFrames = EndFrame - StartFrame;
            AnimFrames.reserve(NumFrames);
        }
        //------------------------------------------------------------------------
        AnimSequence::~AnimSequence()
        {
            for (unsigned int Index = 0; Index < AnimFrames.size(); ++Index) {
                delete AnimFrames[Index];
            }
            AnimFrames.clear();
        }

        //------------------------------------------------------------------------
        void AnimSequence::ReadFromStream(std::ifstream& stream, int numBones)
        {
            for (unsigned int Index = 0; Index < NumFrames; ++Index)
            {
                AnimFrame * Frame = new AnimFrame(numBones);
                Frame->ReadFromStream(stream);
                AnimFrames.push_back(Frame);
            }
        }
    }
}