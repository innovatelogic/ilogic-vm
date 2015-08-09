#include "coresdkafx.h"

REGISTER_CLASS(MeshCollisionComponent, CollisionController);

//----------------------------------------------------------------------------------
MeshCollisionComponent::MeshCollisionComponent(const CObjectAbstract * Parent)
: Super(Parent)
{

}

//----------------------------------------------------------------------------------
MeshCollisionComponent::MeshCollisionComponent(const MeshCollisionComponent & Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------
MeshCollisionComponent::~MeshCollisionComponent()
{

}

//----------------------------------------------------------------------------------
void MeshCollisionComponent::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------
void MeshCollisionComponent::Initialize()
{
	Super::Initialize();
}