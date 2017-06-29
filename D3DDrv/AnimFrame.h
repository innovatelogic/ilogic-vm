
#pragma once

#include "dxstdafx.h"

namespace oes
{
    namespace d3d
    {
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
    }
}