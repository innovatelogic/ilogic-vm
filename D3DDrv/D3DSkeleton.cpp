#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //-----------------------------------------------------------------------
        D3DSkeleton::D3DSkeleton(const D3DDriver * Interface)
            : Super(Interface)
            , pSkeletonNode(NULL)
        {

        }

        //-----------------------------------------------------------------------
        D3DSkeleton::D3DSkeleton(const D3DSkeleton & Source)
            : Super(Source)
            , pSkeletonNode(NULL)
        {
            if (this != &Source)
            {

            }
        }

        //-----------------------------------------------------------------------
        D3DSkeleton::~D3DSkeleton()
        {
            Release();
        }

        //-----------------------------------------------------------------------
        bool D3DSkeleton::Load(const std::string& filename)
        {
            bool bResult = false;

            Release();

            pSkeletonNode = m_pD3DInterface->GetSkeletonNode(filename.c_str());

            bResult = true;

            return bResult;
        }

        //-----------------------------------------------------------------------
        void D3DSkeleton::Release()
        {
            if (pSkeletonNode)
            {
                pSkeletonNode->Release();
                pSkeletonNode = NULL;
            }
        }

        //-----------------------------------------------------------------------
        float* D3DSkeleton::GetBoneBindMatrix(float * pMatrix, int BoneIndex) const
        {
            if (pSkeletonNode && pMatrix)
            {
                return pSkeletonNode->GetBoneBindMatrix(pMatrix, BoneIndex);
            }
            return NULL;
        }

        //-----------------------------------------------------------------------
        bool D3DSkeleton::GetFramePosition(float * pQuat, float * pPos, const std::string & Name, unsigned int BoneIndex, float Time) const
        {
            bool bResult = false;

            if (pSkeletonNode && pQuat && pPos)
            {
                pSkeletonNode->GetFramePosition(pQuat, pPos, Name, BoneIndex, Time);
                bResult = true;
            }
            return bResult;
        }

        //-----------------------------------------------------------------------
        unsigned int D3DSkeleton::GetNumFrames(const std::string & Name) const
        {
            unsigned int OutValue = 0;

            if (pSkeletonNode)
            {
                OutValue = pSkeletonNode->GetNumFrames(Name);
            }

            return OutValue;
        }
    }
}