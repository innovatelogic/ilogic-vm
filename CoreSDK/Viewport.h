#ifndef __viewport_h__
#define __viewport_h__

#pragma once

class CORESDK_API Viewport
{
public:
	float rightViewPortX;
	float rightViewPortY;
	float rightViewPortWidth;
	float rightViewPortHeight;
	float rightViewPortMinZ;
	float rightViewPortMaxZ;

	Viewport()
	{
		rightViewPortX = 0.f;
		rightViewPortY = 0.f;
		rightViewPortWidth = 1.f;
		rightViewPortHeight =1.f;
		rightViewPortMinZ = 0.f;
		rightViewPortMaxZ = 1.f;
	}
};

#endif//__viewport_h__