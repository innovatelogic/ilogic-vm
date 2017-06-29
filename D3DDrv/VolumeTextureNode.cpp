#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        VolumeTextureNode::VolumeTextureNode()
            //: VolumeTexture(NULL)
        {

        }

        //----------------------------------------------------------------------------------------------
        VolumeTextureNode::~VolumeTextureNode()
        {

        }

        //----------------------------------------------------------------------------------------------
        void VolumeTextureNode::DoRelease()
        {
            /*if (VolumeTexture)
            {
                SAFE_RELEASE(VolumeTexture);
                VolumeTexture = NULL;
            }*/
            Refcount::DoRelease();
        }
    }
}