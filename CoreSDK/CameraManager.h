
#pragma once

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

	CameraManager(const CameraManager &other) = delete;
	CameraManager operator= (const CameraManager &other) = delete;

public:
	CameraManager(class CCoreSDK *pAppMain);
	~CameraManager();

	bool RegisterCamera(CCamera *pCamera, SRenderContext *pContext = nullptr);
	bool UnregisterCamera(CCamera *pCamera);

	CCamera* GetActiveCamera(SRenderContext *pContext = nullptr) const;
	bool ActivateCamera(CCamera *pCamera);

	class CCamera* GetCamera(size_t Index) const;

	void Update(float dt);

	/** reinit cameras projection */
	void ViewportResized(SRenderContext *context);

	void Clear();

protected:
private:
	std::vector<SCameraContext*> m_VecCtxtCameras;

	CCoreSDK *m_pAppMain;
};