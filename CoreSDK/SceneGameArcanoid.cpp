#include "coresdkafx.h"

REGISTER_CLASS(CSceneGameArcanoid, CScene);

//----------------------------------------------------------------------------------------------
CSceneGameArcanoid::CSceneGameArcanoid(const CObjectAbstract *pParent)
: Super(pParent)
{
	NEW_OBJECT_TRANSIENT_CHILD(m_pSceneCamera, CCamera, "SceneCamera", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pPlayerController, CPawnCueBall, "BallCue", this);
}

//----------------------------------------------------------------------------------------------
CSceneGameArcanoid::~CSceneGameArcanoid()
{

}

//----------------------------------------------------------------------------------------------
void CSceneGameArcanoid::Initialize()
{
	m_pPlayerController->SetPosition_(Vector(5, 15, 0));

	Super::Initialize();

	if (!m_pCoreSDK->IsAEditor() && m_pCoreSDK->IsAGame())
	{
		const CameraManager *pMgr = GetAppMain()->GetCameraManager();

		if (pMgr)
		{
			const_cast<CameraManager*>(pMgr)->ActivateCamera(m_pSceneCamera);

			Vector CamPos = m_pSceneCamera->GetPosition_();

			CamPos.x = -7.0f;
			CamPos.y = 8.1f;
			//CamPos.z += 0.01f;
		
			m_pSceneCamera->SetYaw(0.f);
			m_pSceneCamera->SetPitch(0.f);
			m_pSceneCamera->SetRoll(0.f);

			m_pSceneCamera->SetPosition_(CamPos);

			m_pSceneCamera->UpdateCamera();
		}
	}
}

//----------------------------------------------------------------------------------------------
void CSceneGameArcanoid::DoDraw()
{
	if (!m_pCoreSDK->IsAEditor() && m_pCoreSDK->IsAGame()){
	}

	Super::DoDraw();
}