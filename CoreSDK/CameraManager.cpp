#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
CameraManager::CameraManager(CCoreSDK *pAppMain)
: m_pAppMain(pAppMain)
{
	Clear();
}

//----------------------------------------------------------------------------------------------
CameraManager::~CameraManager()
{

}

//----------------------------------------------------------------------------------------------
bool CameraManager::RegisterCamera(CCamera *pCamera, SRenderContext *pContext /*= 0*/)
{
	bool bResult = false;

	if (pContext == 0){
		pContext = m_pAppMain->GetRenderSDK()->GetRenderDriver()->GetDefaultContext();
	}

	SCameraContext *pActiveContext = 0;

	// find context
	for (std::vector<SCameraContext*>::iterator Iter = m_VecCtxtCameras.begin(); Iter != m_VecCtxtCameras.end(); ++Iter)
	{
		if ((*Iter)->pRenderContext == pContext)
		{
			pActiveContext = (*Iter);
			break;
		}
	}

	if (pActiveContext == 0)
	{
		pActiveContext = new SCameraContext;
		pActiveContext->pRenderContext = pContext;

		m_VecCtxtCameras.push_back(pActiveContext);
	}

	assert(pActiveContext);

	TVecCameraConstIter Iter = std::find(pActiveContext->vecCameras.begin(), pActiveContext->vecCameras.end(), pCamera);

	if (Iter == pActiveContext->vecCameras.end())
	{
		if (pActiveContext->vecCameras.empty()){
			pActiveContext->pActiveCamera = pCamera;
		}
		pActiveContext->vecCameras.push_back(pCamera);

		bResult = true;
	}

	// TODO: validate camera already register in other context

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CameraManager::UnregisterCamera(CCamera *pCamera)
{
	bool bResult = false;

	// find context
	for (std::vector<SCameraContext*>::iterator Iter = m_VecCtxtCameras.begin(); Iter != m_VecCtxtCameras.end(); ++Iter)
	{
		TVecCameraConstIter IterFind = std::find((*Iter)->vecCameras.begin(), (*Iter)->vecCameras.end(), pCamera);

		if (IterFind != (*Iter)->vecCameras.end())
		{
			(*Iter)->vecCameras.erase(IterFind);

			// activate new camera active if array not empty
			if ((*Iter)->pActiveCamera == pCamera)
			{
				(*Iter)->pActiveCamera = (!(*Iter)->vecCameras.empty()) ? *((*Iter)->vecCameras.begin()) : 0;

				if ((*Iter)->pActiveCamera == 0)  // no cameras in context destroy self context
				{
					delete *Iter; // delete self context
					m_VecCtxtCameras.erase(Iter);
				}
			}
		}
	}

/*	TVecCameraConstIter Iter = std::find(m_VecCameras.begin(), m_VecCameras.end(), Camera);

	if (Iter != m_VecCameras.end())
	{
		CCamera *pNewActive = (!m_VecCameras.empty()) ? const_cast<CCamera*>(*(m_VecCameras.begin())) : NULL;
		
		// adjust default camera settings
		if ((m_pActiveCamera == *Iter) && pNewActive)
		{
			pNewActive->SetYaw(m_pActiveCamera->GetYaw());
			pNewActive->SetPitch(m_pActiveCamera->GetPitch());
			pNewActive->SetRoll(m_pActiveCamera->GetRoll());
			pNewActive->SetLTM_(m_pActiveCamera->GetLTM_());

			pNewActive->UpdateRotationQuat();
			pNewActive->UpdateCamera();
			pNewActive->UpdateCullInfo();
		}

		m_VecCameras.erase(Iter);
		m_pActiveCamera = pNewActive;

		bResult = true;
	}*/
	return bResult;
}

//----------------------------------------------------------------------------------------------
CCamera* CameraManager::GetActiveCamera(SRenderContext *pContext /*= 0*/) const 
{ 
	if (pContext == 0){
		pContext = m_pAppMain->GetRenderSDK()->GetRenderDriver()->GetDefaultContext();
	}

	// find context
	for (std::vector<SCameraContext*>::const_iterator Iter = m_VecCtxtCameras.begin(); Iter != m_VecCtxtCameras.end(); ++Iter)
	{
		if ((*Iter)->pRenderContext == pContext)
		{
			return (*Iter)->pActiveCamera;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
bool CameraManager::ActivateCamera(CCamera *pCamera)
{
	bool bResult = false;

	// find context
	for (std::vector<SCameraContext*>::const_iterator Iter = m_VecCtxtCameras.begin(); Iter != m_VecCtxtCameras.end(); ++Iter)
	{
		TVecCameraConstIter IterFind = std::find((*Iter)->vecCameras.begin(), (*Iter)->vecCameras.end(), pCamera);

		if (IterFind != (*Iter)->vecCameras.end())
		{
			(*Iter)->pActiveCamera = pCamera;
			bResult = true;
			break;
		}
	}

	/*TVecCameraConstIter Iter = std::find(m_VecCameras.begin(), m_VecCameras.end(), pCamera);

	if (Iter != m_VecCameras.end())
	{
		m_pActiveCamera = *Iter;
		bResult = true;
	}*/
	return bResult;
}

//----------------------------------------------------------------------------------------------
CCamera* CameraManager::GetCamera(size_t Index) const
{
	/*if (Index < m_VecCameras.size()){
		return m_VecCameras[Index];
	}*/
	return 0;
}

//----------------------------------------------------------------------------------------------
void CameraManager::Update(float dt)
{
// 	TVecCameraIter Iter = m_VecCameras.begin();
// 	TVecCameraIter IterEnd = m_VecCameras.end();
// 
// 	while (Iter != IterEnd)
// 	{
// 		const_cast<CCamera*>((*Iter))->Update_(dt);
// 		++Iter;
// 	}
}

//----------------------------------------------------------------------------------------------
void CameraManager::ViewportResized()
{
/*	TVecCameraIter Iter = m_VecCameras.begin();
	TVecCameraIter IterEnd = m_VecCameras.end();
	 
	while (Iter != IterEnd)
	{
		const_cast<CCamera*>((*Iter))->CalcProjMatrix();
		++Iter;
	}*/
}

//----------------------------------------------------------------------------------------------
void CameraManager::Clear()
{

}