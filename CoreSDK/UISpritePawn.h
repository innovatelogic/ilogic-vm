#ifndef __uispritepawn_h__
#define __uispritepawn_h__

#ifdef _WIN32
#pragma once
#endif

#include "UISprite.h"

class UISpritePawn : public UISprite
{
	DECLARE_CLASS_SIMPLE(UISpritePawn, UISprite);

public:
	UISpritePawn(const CObjectAbstract * Parent);
	UISpritePawn(const UISpritePawn &Source);
	virtual ~UISpritePawn();

protected:
private:
	class AnimationController2D	*AnimController;
	class BoxCharacterController *CharacterController;
};

#endif//__uispritepawn_h__