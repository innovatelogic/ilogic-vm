#include "coresdkafx.h"

REGISTER_CLASS(DirectionalLight, ActorAllocator);

//----------------------------------------------------------------------------------
DirectionalLight::DirectionalLight(const class CObjectAbstract * Parent)
: Super(Parent)
{

}

//----------------------------------------------------------------------------------
DirectionalLight::DirectionalLight(const DirectionalLight & Source)
: Super(Source)
{

}

//----------------------------------------------------------------------------------
DirectionalLight::~DirectionalLight()
{

}