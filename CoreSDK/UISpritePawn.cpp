
#include "coresdkafx.h"

REGISTER_CLASS(UISpritePawn, UISprite);

//----------------------------------------------------------------------------------
UISpritePawn::UISpritePawn(const CObjectAbstract * Parent /*= NULL*/)
: Super(Parent)
{
// 	// add transient object:
 	NEW_OBJECT_TRANSIENT_CHILD(AnimController, AnimationController2D, "AnimController", this);
 	NEW_OBJECT_TRANSIENT_CHILD(CharacterController, BoxCharacterController, "CharacterController", this);
}

//----------------------------------------------------------------------------------
UISpritePawn::UISpritePawn(const UISpritePawn &Source)
: Super(Source)
{
	if (&Source != this)
	{
		// clone transient object:
		AnimController = CLONE_OBJECT(AnimationController2D, Source.AnimController, Source.AnimController->GetName(), this);
		CharacterController = CLONE_OBJECT(BoxCharacterController, Source.CharacterController, Source.CharacterController->GetName(), this);
	}
}

//----------------------------------------------------------------------------------
UISpritePawn::~UISpritePawn()
{

}