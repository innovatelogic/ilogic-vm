#pragma once

#include "Camera.h"

class CORESDK_API CameraEditor : public CCamera
{

public:
	CameraEditor(const CObjectAbstract * Parent);
	~CameraEditor();

	void RenderDebug();
	
	inline_ virtual void UpdateCullInfo();

	Vector GetUpVector();
    Vector GetRtVector();
    Vector GetZtVector();
    
protected:
private:

};