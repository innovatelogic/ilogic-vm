#pragma once

#include "Camera.h"

using namespace oes::common_base;

class CORESDK_API CameraEditor : public CCamera
{

public:
	CameraEditor(const CObjectAbstract * Parent);
	~CameraEditor();

	void RenderDebug();
	
	inline_ virtual void UpdateCullInfo();

	oes::common_base::Vector GetUpVector();
    oes::common_base::Vector GetRtVector();
    oes::common_base::Vector GetZtVector();

protected:
private:

};