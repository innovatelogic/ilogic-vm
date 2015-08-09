#include "coresdkafx.h"

REGISTER_CLASS(DummyObject, ActorAllocator);

//----------------------------------------------------------------------------------
DummyObject::DummyObject(const CObjectAbstract * Parent)
: Super(Parent)
{

}
//----------------------------------------------------------------------------------
DummyObject::DummyObject(const DummyObject &Source)
: Super(Source)
{

}
//----------------------------------------------------------------------------------
DummyObject::~DummyObject()
{

}

//----------------------------------------------------------------------------------
void DummyObject::DoDraw()
{
/*	Vector       vecCross[6];

	vecCross[0] = Vector(0.f, -2.f, 0.f);
	vecCross[1] = Vector(0.f, 2.f, 0.f);
	vecCross[2] = Vector(-2.f, 0.f, 0.f);
	vecCross[3] = Vector(2.f, 0.f,  0.f);
	vecCross[4] = Vector(0.f, 0.f, -2.f);
	vecCross[5] = Vector(0.f, 0.f, 2.f);

	for (int index = 0; index < 6; ++index)
	{
		vecCross[index] = transform_coord(vecCross[index], WorldMatrixTransform);
	}
	*/
	//GetRenderComponent()->SetTransform(WorldMatrixTransform);
// 	GetRenderComponent()->DrawLine(vecCross[0], vecCross[1], COLOR_BLUE);
// 	GetRenderComponent()->DrawLine(vecCross[2], vecCross[3], COLOR_BLUE);
// 	GetRenderComponent()->DrawLine(vecCross[4], vecCross[5], COLOR_BLUE);
}

