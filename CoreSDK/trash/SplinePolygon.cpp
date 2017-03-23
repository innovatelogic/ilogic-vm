#include "coresdkafx.h"

REGISTER_CLASS(SplinePolygon, ActorAllocator);

//----------------------------------------------------------------------------------------------
SplinePolygon::SplinePolygon(const CObjectAbstract * Parent)
: Super(Parent)
, IDrawInterface(Parent)
{

}

//----------------------------------------------------------------------------------------------
SplinePolygon::~SplinePolygon()
{

}

//----------------------------------------------------------------------------------------------
void SplinePolygon::Initialize()
{
	Super::Initialize();

	Rebuild();
}

//----------------------------------------------------------------------------------------------
void SplinePolygon::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
bool SplinePolygon::Rebuild()
{
	return true;
}

//----------------------------------------------------------------------------------------------
void SplinePolygon::DoRebuildMesh()
{

}