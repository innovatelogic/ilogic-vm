#include "CameraEditor.h"
#include "CoreSDK.h"
#include "RenderSDK.h"

REGISTER_CLASS(CameraEditor, CCamera);

//------------------------------------------------------------------------
CameraEditor::CameraEditor(const CObjectAbstract * Parent/* = NULL*/)
: CCamera(Parent)
{
	;//
}

//------------------------------------------------------------------------
CameraEditor::~CameraEditor()
{
	;//
} 

//------------------------------------------------------------------------
void CameraEditor::UpdateCullInfo()
{	

}

//------------------------------------------------------------------------
void CameraEditor::RenderDebug()
{

}

//------------------------------------------------------------------------
oes::common_base::Vector CameraEditor::GetUpVector()
{
	return GetLTM_()._row1;
}

//------------------------------------------------------------------------
oes::common_base::Vector CameraEditor::GetRtVector()
{
	return GetLTM_()._row0;
}

//------------------------------------------------------------------------
oes::common_base::Vector CameraEditor::GetZtVector()
{
	return GetLTM_()._row2;
}