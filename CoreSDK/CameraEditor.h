#ifndef __camera_editor_actor_h__
#define __camera_editor_actor_h__

#pragma once

#include "Camera.h"

class CORESDK_API CameraEditor : public CCamera
{

public:
	CameraEditor(const CObjectAbstract * Parent);
	~CameraEditor();

	void RenderDebug();
	
	inline_ virtual void UpdateCullInfo();

	virtual Vector GetUpVector();
	virtual Vector GetRtVector();
	virtual Vector GetZtVector();

protected:
private:

};


#endif/__camera_editor_actor_h__