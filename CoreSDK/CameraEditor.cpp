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
Vector CameraEditor::GetUpVector()
{
	return GetLTM_()._row1;
}

//------------------------------------------------------------------------
Vector CameraEditor::GetRtVector()
{
	return GetLTM_()._row0;
}

//------------------------------------------------------------------------
Vector CameraEditor::GetZtVector()
{
	return GetLTM_()._row2;
}