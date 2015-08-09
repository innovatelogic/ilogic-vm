#include "d3ddriverstdafx.h"

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