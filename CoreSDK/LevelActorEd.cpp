#include "coresdkafx.h"

REGISTER_CLASS(LevelActorEd, ActorAllocator);

//----------------------------------------------------------------------------------------------
LevelActorEd::LevelActorEd(const CObjectAbstract * Parent)
: Super(Parent)
{
	NEW_OBJECT_TRANSIENT_CHILD(MeshComponent, Comp_AnimMesh, "MeshComponent", this);
}

//----------------------------------------------------------------------------------------------
LevelActorEd::LevelActorEd(const LevelActorEd & Source)
: Super(Source)
{
	if (this != &Source)
	{
		XRef = Source.XRef;
		MeshComponent = CLONE_OBJECT(Comp_AnimMesh, Source.MeshComponent, "MeshComponent", this);
	}
}

//----------------------------------------------------------------------------------------------
LevelActorEd::~LevelActorEd()
{

}

//----------------------------------------------------------------------------------------------
void LevelActorEd::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void LevelActorEd::Initialize()
{
	Super::Initialize();

	MeshComponent->LoadMesh(GetXRef());
	//SetBounds(MeshComponent->GetBounds());

	GetAppMain()->GetUpdateManager()->RegisterActor(this);
}

//----------------------------------------------------------------------------------------------
void LevelActorEd::PreRelease()
{
	GetAppMain()->GetUpdateManager()->UnRegisterActor(this);
	Super::PreRelease();
}