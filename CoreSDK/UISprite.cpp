#include "coresdkafx.h"

REGISTER_CLASS(UISprite, UIImage);

//----------------------------------------------------------------------------------
UISprite::UISprite(const CObjectAbstract * Parent /*= NULL*/)
: Super(Parent)
{

}

//----------------------------------------------------------------------------------
UISprite::UISprite(const UISprite &Source)
: Super(Source)
{
	if (&Source != this)
	{

	}
}

//----------------------------------------------------------------------------------
UISprite::~UISprite()
{

}

//----------------------------------------------------------------------------------
bool UISprite::HitTest(const Vector2f &Position)
{
	Vector2f LocalPosition = Position - GetTransformedWTM_().t2;

	// calc projection to local axis
	Vector2f AxisX = GetTransformedWTM_().__row0;
	Vector2f AxisY = GetTransformedWTM_().__row1;

	AxisX.Normalize();
	AxisY.Normalize();

	Matrix IObjectMatrix;	
	invert(IObjectMatrix, GetTransformedWTM_());
	Vector2f TPosition = Vector2f(LocalPosition.Dot(AxisX) * IObjectMatrix.__row0.Length(),
									LocalPosition.Dot(AxisY) * IObjectMatrix.__row1.Length());

	Vector2f Size = GetTransformedSize_();
	bool inX = (TPosition.x >= -Size.x/2 && TPosition.x <= Size.x/2);
	bool inY = (TPosition.y >= -Size.y/2 && TPosition.y <= Size.y/2);
	return inX && inY; 
}

//----------------------------------------------------------------------------------
void UISprite::DoDraw()
{
//  	Vector2f MatrixScale = GetTransformedSize();
//  	Vector2f TSize		 = GetTransformedSize();
//  
//  	Matrix TWTM = WorldMatrixTransform * GetMatrixTranslation(Vector(-GetSize().x * 0.5f, -TSize.y * 0.5f, 0.f)) * 
//  										 GetMatrixScale(Vector(MatrixScale.x, MatrixScale.y, 1.f));
//  	
// 	ImageComponent->DrawImage();
}
//----------------------------------------------------------------------------------
void UISprite::DrawBounds() const
{
	/*if (IsFocused())
	{
		Matrix IWTM;
		invert(IWTM, WorldMatrixTransform);

		Vector2f hsize = GetSize() * -0.5f;
		Vector2f WAxisX(fabs(hsize.x * 2.f), 0.f);
		Vector2f WAxisY(0.f, fabs(hsize.y * 2.f));

		GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord(hsize, IWTM), 
										WorldMatrixTransform.t2 + transform_coord(hsize + WAxisX, IWTM), COLOR_YELLOW);
		GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord(hsize + WAxisX, IWTM),
										WorldMatrixTransform.t2 + transform_coord(hsize + (WAxisX + WAxisY), IWTM), COLOR_YELLOW);
		GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord(hsize + (WAxisX + WAxisY), IWTM),
										WorldMatrixTransform.t2 + transform_coord(hsize + WAxisY, IWTM), COLOR_YELLOW);
		GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord(hsize + WAxisY, IWTM), 
										WorldMatrixTransform.t2 + transform_coord(hsize, IWTM), COLOR_YELLOW);
	}

	Matrix I;
	if (GetAppMain()->GetObjectBoundsVisible()){
		GetRenderComponent()->DrawBounds2f(WorldMatrixTransform, Bounds, COLOR_BLUE);
	}
	if (GetAppMain()->GetSparitalSubdivisionVisible()){
		GetRenderComponent()->DrawBounds2f(I, CompositeBounds, COLOR_GREEN);
	}*/
}
//----------------------------------------------------------------------------------
/*void UISprite::DoRebuildBounds()
{
	Vector AxisX(1.f, 0.f, 0.f);
	Vector AxisY(0.f, 1.f, 0.f);

	Vector2f MatrixScale = GetTransformedSize_();

	float px0 = WorldMatrixTransform._row0.Dot(AxisX);
	float px1 = WorldMatrixTransform._row1.Dot(AxisX);

	float py0 = WorldMatrixTransform._row0.Dot(AxisY);
	float py1 = WorldMatrixTransform._row1.Dot(AxisY);

	float LeftX = 0.f;
	float LeftY = 0.f;

	Vector2f D = Vector2f(fabs(px0 * MatrixScale.x) + fabs(px1 * MatrixScale.y), 
							fabs(py0 * MatrixScale.x) + fabs(py1 * MatrixScale.y));

	Vector HalfD = Vector(D.x * 0.5f, D.y * 0.5f, 0.f);

	Bounds.SetBounds(-HalfD, HalfD);
}*/