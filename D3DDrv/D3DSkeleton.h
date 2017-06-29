
#pragma once

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;
        class SkeletonNode;

        class D3DDRIVER_API D3DSkeleton : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(D3DSkeleton, D3DRenderInterface);

        public:
            D3DSkeleton(const D3DDriver * Interface);
            D3DSkeleton(const D3DSkeleton & Source);
            ~D3DSkeleton();

            virtual bool Load(const std::string& filename);
            virtual void Release();

            virtual float* GetBoneBindMatrix(float * pMatrix, int BoneIndex) const;
            virtual bool   GetFramePosition(float * pQuat, float * pPos, const std::string & Name, unsigned int BoneIndex, float Time) const;
            virtual unsigned int GetNumFrames(const std::string & Name) const;
        protected:
        private:
            SkeletonNode * pSkeletonNode;
        };
    }
}