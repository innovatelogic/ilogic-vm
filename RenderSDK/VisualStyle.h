#ifndef __visualstyle_h__
#define __visualstyle_h__

#ifdef _WIN32
#pragma once
#endif

#include <../Foundation/xmlparser.h>
#include "mathlib.h"
#include "RenderSDKInterface.h"

#define NUM_STATES 5

class CRenderSDK;

class RENDERSDK_API VisualStyle
{
public:  
   std::string Name;
   Vector2f    Size; 

   std::string STATE_NAME[NUM_STATES];
   std::string STATE_NAME_UV[NUM_STATES];

   //Texture2D ActiveStates[NUM_STATES]; 

   VisualStyle()
	   :Size(0.f,0.f)
   {
	   STATE_NAME[0] = std::string("StateNormal");
	   STATE_NAME[1] = std::string("StateFocused");
	   STATE_NAME[2] = std::string("StateActive");
	   STATE_NAME[3] = std::string("StatePressed");
	   STATE_NAME[4] = std::string("StateDisable");

	   STATE_NAME_UV[0] = std::string("StateNormalUV");
	   STATE_NAME_UV[1] = std::string("StateFocusedUV");
	   STATE_NAME_UV[2] = std::string("StateActiveUV");
	   STATE_NAME_UV[3] = std::string("StatePressedUV");
	   STATE_NAME_UV[4] = std::string("StateDisableUV");
   }

   ~VisualStyle()
   {
   }

   void LoadStyle(CRenderSDK * RC);

   void SetName(const std::string &name) { Name = name; }
   void SetSize(const Vector2f& size) { Size = size; } 

   void SetStateStyle(int IndexState, const std::string& Reference, const Vector4f& UV)
   { 
	   if (IndexState >= 0 && IndexState < NUM_STATES)
	   {
		   Vector4f CopyUV = UV;

		   if (Size.x != 0.f && Size.y != 0.f) {
			   CopyUV = Vector4f(UV.x/Size.x, UV.y/Size.y, UV.z/Size.x, UV.w/Size.y ); 
		   }

// 		   ActiveStates[IndexState].SetReference(Reference);
// 		   ActiveStates[IndexState].SetTexcoords(CopyUV.x,CopyUV.y,CopyUV.z,CopyUV.w);
	   }
   }
};

#endif//__visualstyle_h__