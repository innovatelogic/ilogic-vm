#ifndef __cameramaneger_h__
#define __cameramaneger_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"

class EXPORT CameraManager
{
	typedef std::vector<class CCamera*>  TVecCameras;
	typedef TVecCameras::iterator		 TVecCameraIter;
	typedef TVecCameras::const_iterator	 TVecCameraConstIter;

	typedef class SRenderContext		 TSRenderContext;

	struct SCameraContext
	{
		TSRenderContext *pRenderContext;
		TVecCameras		vecCameras;
		CCamera			*pActiveCamera;
	};

public:
	CameraManager(class CCoreSDK *pAppMain);
	~CameraManager();

	bool RegisterCamera(CCamera *pCamera, class SRenderContext *pContext = 0);
	bool UnregisterCamera(CCamera *pCamera);

	CCamera* GetActiveCamera(class SRenderContext *pContext = 0) const;
	bool ActivateCamera(CCamera *pCamera);

	class CCamera* GetCamera(size_t Index) const;

	void Update(float dt);

	/** reinit cameras projection */
	void ViewportResized();

	void Clear();

protected:
private:
	std::vector<SCameraContext*> m_VecCtxtCameras;

	CCoreSDK *m_pAppMain;
};

#endif//__cameramaneger_h__