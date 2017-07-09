#include "UIImagePivotControl.h"
#include "UIViewPivotControl.h"
#include "UIComp_DrawImage.h"

REGISTER_CLASS(UIImagePivotControl, UIImage)

//----------------------------------------------------------------------------------------------
UIImagePivotControl::UIImagePivotControl(const CObjectAbstract *Parent)
: UIImage(Parent)
{
	NEW_OBJECT_TRANSIENT_CHILD(m_pPivot, UIViewPivotControl, "Pivot", this);
	
	m_pImageComponent->SetRenderTarget(m_pPivot->GetRenderTarget());
}

//----------------------------------------------------------------------------------------------
UIImagePivotControl::~UIImagePivotControl()
{
	m_pImageComponent->SetRenderTarget(0);
}

//----------------------------------------------------------------------------------------------
bool UIImagePivotControl::DoEventPressed(const MouseInputData &InputData)
{
	MouseInputData RelInputData(InputData);
	RelInputData.MousePos = InputData.MousePos - GetTransformedWTM_().t2;

	if (m_pPivot->ProcessClick(RelInputData, GetTransformedSize_())){
		return true;
	}
	return Super::DoEventPressed(InputData);
}

//----------------------------------------------------------------------------------------------
bool UIImagePivotControl::DoEventReleased(const MouseInputData &InputData)
{
	return Super::DoEventReleased(InputData);
}