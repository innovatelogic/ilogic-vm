#include "CameraActor.h"
#include "CoreSDK.h"
#include "RenderSDK.h"

REGISTER_CLASS(CameraActor, CCamera);

//------------------------------------------------------------------------
CameraActor::CameraActor(const CObjectAbstract * Parent/*= NULL*/)
: CCamera(Parent)
{
    ;//
}

//------------------------------------------------------------------------
CameraActor::~CameraActor()
{
    ;//
} 

//------------------------------------------------------------------------
void CameraActor::UpdateCullInfo()
{	
}

//------------------------------------------------------------------------
BOOL CameraActor::Render(const Matrix &WTM)
{
   RenderDebug();

   return TRUE;
}

//------------------------------------------------------------------------
void CameraActor::RenderDebug()
{
}

//------------------------------------------------------------------------
Vector CameraActor::GetUpVector()
{
    return GetLTM_()._row1;
}

//------------------------------------------------------------------------
Vector CameraActor::GetRtVector()
{
    return GetLTM_()._row0;
}

//------------------------------------------------------------------------
Vector CameraActor::GetZtVector()
{
    return GetLTM_()._row2;
}