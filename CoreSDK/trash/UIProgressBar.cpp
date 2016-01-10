#include "coresdkafx.h"

REGISTER_CLASS_A(UIProgressBar, UIImage)
	new PropertyFLOAT("ValueProgress", (BYTE*)&((UIProgressBar*)NULL)->ValueProgress - (BYTE*)NULL,	"UIProgressBar", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyFLOAT("ValueMin", (BYTE*)&((UIProgressBar*)NULL)->ValueMin - (BYTE*)NULL, "UIProgressBar", "Value",	READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyFLOAT("ValueMax", (BYTE*)&((UIProgressBar*)NULL)->ValueMax - (BYTE*)NULL, "UIProgressBar", "Value",	READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(UIProgressBar, UIImage);

//----------------------------------------------------------------------------------------------
UIProgressBar::UIProgressBar(const CObjectAbstract *Parent)
: Super(Parent)
, ValueProgress(0.5f)
, ValueMin(0.f)
, ValueMax(1.f)
{
	// add transient object:
	NEW_OBJECT_TRANSIENT_CHILD(FillImageComponent, UIComp_DrawImage, "UIImage_FillImageComponent", this);
}

//----------------------------------------------------------------------------------------------
UIProgressBar::UIProgressBar(const UIProgressBar &Source)
: Super(Source)
{
	if (this != &Source)
	{
		// clone transient object:
		FillImageComponent = CLONE_OBJECT(UIComp_DrawImage, Source.FillImageComponent, Source.FillImageComponent->GetName(), this);

		ValueProgress = Source.ValueProgress;
		ValueMin = Source.ValueMin;
		ValueMax = Source.ValueMax;
	}
}

//----------------------------------------------------------------------------------------------
UIProgressBar::~UIProgressBar()
{

}

//----------------------------------------------------------------------------------------------
void UIProgressBar::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void UIProgressBar::DoDraw()
{
	Super::DoDraw();
/*
  	Matrix TWTM = GetTransformedWTM();
  	Vector2f MatrixScale = GetTransformedSize();
 
 	FillImageComponent->GetStaticMesh()->SetTransformedWTM(scale(TWTM, MatrixScale.x * ValueProgress, MatrixScale.y, 1.f));
 	FillImageComponent->DrawImage();*/
}