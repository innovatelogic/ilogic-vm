#include "SplineCollision2DModificator.h"
#include "SplineAgregator.h"

REGISTER_CLASS(SplineCollision2DModificator, BaseModificator);

//------------------------------------------------------------------------
SplineCollision2DModificator::SplineCollision2DModificator(const CObjectAbstract * Parent /*= NULL*/)
: BaseModificator(Parent)
{
	;//
}

//------------------------------------------------------------------------
SplineCollision2DModificator::SplineCollision2DModificator(const SplineCollision2DModificator &Source)
: Super(Source)
{
	if (this != &Source)
	{
	}
}

//------------------------------------------------------------------------
SplineCollision2DModificator::~SplineCollision2DModificator()
{
	UNREGISTER_EVENT(GetParent(), this, Event_Updated);
	UNREGISTER_EVENT(GetParent(), this, Event_OnChangePivot);

	//GetPhysicsEngine()->ReleaseObject(this);
}

//------------------------------------------------------------------------
void SplineCollision2DModificator::Initialize()
{
	//REGISTER_EVENT_(GetParent(), this, boost::bind<bool>(&SplineCollision2DModificator::Apply, this, _1, _2), Event_Updated);
	//REGISTER_EVENT_(GetParent(), this, boost::bind<bool>(&SplineCollision2DModificator::Apply, this, _1, _2), Event_OnChangePivot);
}

//------------------------------------------------------------------------
void SplineCollision2DModificator::ApplyModificator(const CActor * Sender)
{
	Apply(Sender, Event_MAX);
}

//------------------------------------------------------------------------
bool SplineCollision2DModificator::Apply(const CActor * Sender, ESystemEventID EventId)
{
/*	const SplineAgregator * _Parent = dynamic_cast<const SplineAgregator*>(Sender);

	if (_Parent)
	{
		std::vector<NxVec3> Verts;
		std::vector<NxU32> Indices;

		float _Z = 500.f;
		// fill vertices
		for (size_t Index = 0; Index < _Parent->GetNumNodes(); ++Index)
		{
			Vector2f SplinePoint, TSplinePoint;

			if (_Parent->GetPointByIndex(Index, SplinePoint))
			{
				Vector OutPos = _Parent->LocalToGlobalTransform(OutPos, Vector(SplinePoint.x, SplinePoint.y, 0.f));
				
				Verts.push_back(NxVec3(OutPos.x, OutPos.y, -_Z));
				Verts.push_back(NxVec3(OutPos.x, OutPos.y, _Z ));
			}
		}

		// fill indexes
		for (size_t Index = 0; Index < _Parent->GetNumNodes(); ++Index)
		{
			size_t Index0 = (Index * 2);
			size_t Index1 = (Index0 + 1);
			size_t Index2 = (Index + 1 == _Parent->GetNumNodes()) ? 0 : Index0 + 2;
			size_t Index3 = (Index + 1 == _Parent->GetNumNodes()) ? 1 : Index0 + 3;

			Indices.push_back(NxU32(Index0));
			Indices.push_back(NxU32(Index1));
			Indices.push_back(NxU32(Index3));

			Indices.push_back(NxU32(Index3));
			Indices.push_back(NxU32(Index2));
			Indices.push_back(NxU32(Index0));
		}
		// build triangle mesh
		//GetPhysicsEngine()->CreateTriangleMesh(this, Verts, Indices);
	}*/
	return true;
}

