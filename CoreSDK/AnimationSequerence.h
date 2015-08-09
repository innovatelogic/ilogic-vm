#ifndef __animationsequerence_h__
#define __animationsequerence_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class AnimationSequerence : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(AnimationSequerence, ActorAllocator);

	struct AnimFrame
	{
		Vector4f UV;

		AnimFrame()	
		{
			//REGISTER_PROPERTY_REF("UV", "Value", this, AnimFrame, Vector4f, AnimFrame::GetUV, AnimFrame::SetUV, CTRL_EDIT, READ_WRITE, TRUE, false);
		}

		AnimFrame(const AnimFrame &Source) 
		{
			if (this != &Source)
			{
				UV = Source.UV;
				//REGISTER_PROPERTY_REF("UV", "Value", this, AnimFrame, Vector4f, AnimFrame::GetUV, AnimFrame::SetUV, CTRL_EDIT, READ_WRITE, TRUE, false);
			}
		}

		virtual ~AnimFrame()
		{
		}

		inline_ virtual bool IsInitialized() const { return true; }

		Vector4f GetUV() { return UV; }
		void	 SetUV(Vector4f& Value) { UV = Value; }
	};

public:
	AnimationSequerence(const CObjectAbstract* Parent);
	AnimationSequerence(const AnimationSequerence& Source);
	~AnimationSequerence();

protected:
private:

	TArray<AnimFrame> AnimFrames;
};

#endif