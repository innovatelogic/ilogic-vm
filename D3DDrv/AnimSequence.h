
#pragma once

#ifdef WIN32
#pragma warning(disable: 4251)
#endif

#include "dxstdafx.h"

namespace oes
{
    namespace d3d
    {
        class AnimFrame;

        class D3DDRIVER_API AnimSequence
        {
            typedef std::vector<AnimFrame*> TAnimFrames;

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
    }
}
