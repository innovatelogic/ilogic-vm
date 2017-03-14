
#pragma once

#include "../Foundation/StdafxFoundation.h"
#include "include/PxCamera.h"
#include "src/Camera.h"

class EXPORT CameraManager
{
public:
    typedef CCamera T;

	typedef std::vector<T*>  TVecCameras;
	typedef TVecCameras::iterator		 TVecCameraIter;
	typedef TVecCameras::const_iterator	 TVecCameraConstIter;
	typedef class SRenderContext		 TSRenderContext;

	struct SCameraContext
	{
		TSRenderContext *pRenderContext;
		TVecCameras		vecCameras;
		T			    *pActiveCamera;
	};

	CameraManager(const CameraManager &other) = delete;
	CameraManager operator= (const CameraManager &other) = delete;

public:
	CameraManager(class CCoreSDK *pAppMain);
	~CameraManager();

	bool RegisterCamera(T *pCamera, SRenderContext *pContext = nullptr);
	bool UnregisterCamera(T *pCamera);

	T* GetActiveCamera(SRenderContext *pContext = nullptr) const;
	bool ActivateCamera(T *pCamera);

	class T* GetCamera(size_t Index) const;

	void Update(float dt);

	/** reinit cameras projection */
	void ViewportResized(SRenderContext *context);

	void Clear();

protected:
private:
	std::vector<SCameraContext*> m_VecCtxtCameras;

	CCoreSDK *m_pAppMain;
};