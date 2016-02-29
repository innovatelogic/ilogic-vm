#pragma once

#include "Camera.h"

class CORESDK_API CameraActor : public CCamera
{
public:
	CameraActor(const CObjectAbstract * Parent);
    virtual ~CameraActor();

	void RenderDebug();

	inline_ virtual BOOL Render(const Matrix &WTM);

	inline_ virtual void UpdateCullInfo();

    virtual Vector GetUpVector();
    virtual Vector GetRtVector();
	virtual Vector GetZtVector();

protected:
private:

};
