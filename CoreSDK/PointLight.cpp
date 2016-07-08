#include "coresdkafx.h"

REGISTER_CLASS_A(PointLight, ActorAllocator)
	new PropertyUINT("Color", (BYTE*)&((PointLight*)NULL)->Color - (BYTE*)NULL,	"PointLight", "Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(PointLight, ActorAllocator);

//----------------------------------------------------------------------------------
PointLight::PointLight(const CObjectAbstract * Parent)
: Super(Parent)
, Color(0xffffffff)
{
	perspective(LightProjMatrix, 90.f, 1.f, 1.f, 5000.f);
	//ortho(LightProjMatrix, 0.f, 1000.f, 1000.f, 0.f, 1.f, MAX_FLOAT);
}

//----------------------------------------------------------------------------------
PointLight::~PointLight()
{

}

//----------------------------------------------------------------------------------
void PointLight::DoDraw()
{
//	GetRenderComponent()->SetTransform(WorldMatrixTransform);

#ifndef FINAL_RELEASE
	if (GetAppMain()->IsAEditor() && !GetAppMain()->IsAGame())
	{
		Vector  vecFrustum[8];    // corners of the view frustum

		vecFrustum[0] = Vector(-5 / 2.f, -5 / 2.f,  -5 / 2.f); // xyz
		vecFrustum[1] = Vector( 5 / 2.f, -5 / 2.f,  -5 / 2.f); // Xyz
		vecFrustum[2] = Vector(-5 / 2.f,  5 / 2.f,  -5 / 2.f); // xYz
		vecFrustum[3] = Vector( 5 / 2.f,  5 / 2.f,  -5 / 2.f); // XYz
		vecFrustum[4] = Vector(-5 / 2.f, -5 / 2.f,  5 / 2.f); // xyZ
		vecFrustum[5] = Vector( 5 / 2.f, -5 / 2.f,  5 / 2.f); // XyZ
		vecFrustum[6] = Vector(-5 / 2.f,  5 / 2.f,  5 / 2.f); // xYZ
		vecFrustum[7] = Vector( 5 / 2.f,  5 / 2.f,  5 / 2.f); // XYZ

		for (int index = 0; index < 8; ++index)
		{
			//vecFrustum[index] = transform_coord(vecFrustum[index], WorldMatrixTransform);
		}

		GetRenderComponent()->DrawLine(vecFrustum[0], vecFrustum[1], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[1], vecFrustum[3], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[3], vecFrustum[2], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[2], vecFrustum[0], COLOR_WHITE);

		GetRenderComponent()->DrawLine(vecFrustum[4], vecFrustum[5], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[5], vecFrustum[7], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[7], vecFrustum[6], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[6], vecFrustum[4], COLOR_WHITE);

		GetRenderComponent()->DrawLine(vecFrustum[0], vecFrustum[4], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[1], vecFrustum[5], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[3], vecFrustum[7], COLOR_WHITE);
		GetRenderComponent()->DrawLine(vecFrustum[2], vecFrustum[6], COLOR_WHITE);
	}
#endif//FINAL_RELEASE

	//look_at(LightViewMatrix, GetTransformedWTM().t, Vector(0.f, 0.f, 0.f), Vector(0.f, 1.f, 0.f));
}