#include "coresdkafx.h"

static const bool V_TRUE = true;
static const bool V_FALSE = false;
static const float fZeroVal = 0.f;
static const float fFowDef = 60.f;
static const float fAspectDef = 1.3333f;
static const float fNearDistDef = 1.0f;
static const float fFarDistDef = 5000.f;

REGISTER_CLASS_A(CCamera, ActorAllocator)
	new PropertyBOOL("bArcball", (BYTE*)&((CCamera*)NULL)->m_bArcball - (BYTE*)NULL, "CCamera", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE), 
	new PropertyBOOL("bOrtho", (BYTE*)&((CCamera*)NULL)->bOrtho - (BYTE*)NULL, "CCamera", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyFLOAT("Fov", (BYTE*)&((CCamera*)NULL)->Fov - (BYTE*)NULL, "CCamera", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fFowDef),
	new PropertyFLOAT("Aspect", (BYTE*)&((CCamera*)NULL)->Aspect - (BYTE*)NULL, "CCamera", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fAspectDef),
	new PropertyFLOAT("NearDist", (BYTE*)&((CCamera*)NULL)->NearDist - (BYTE*)NULL, "CCamera", "Value", READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fNearDistDef),
	new PropertyFLOAT("FarDist", (BYTE*)&((CCamera*)NULL)->FarDist - (BYTE*)NULL, "CCamera", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fFarDistDef),
	new PropertyFLOAT("m_fYaw", (BYTE*)&((CCamera*)NULL)->m_fYaw - (BYTE*)NULL, "CCamera", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fZeroVal),
	new PropertyFLOAT("m_fPitch", (BYTE*)&((CCamera*)NULL)->m_fPitch - (BYTE*)NULL, "CCamera", "Value", READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fZeroVal),
	new PropertyFLOAT("m_fRoll", (BYTE*)&((CCamera*)NULL)->m_fRoll - (BYTE*)NULL, "CCamera", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fZeroVal)
END_REGISTER_CLASS(CCamera, ActorAllocator);

//----------------------------------------------------------------------------------------------
CCamera::CCamera(const CObjectAbstract *pParent/*= NULL*/)
: Super(pParent)
, IDrawInterface(pParent)
, IUpdateInterface(pParent)
, IInputInterface(pParent)
, m_bArcball(V_FALSE)
, bOrtho(V_FALSE)
, Fov(fFowDef)
, Aspect(fAspectDef)
, NearDist(fNearDistDef)
, FarDist(fFarDistDef)
, m_fYaw(fZeroVal)
, m_fPitch(fZeroVal)
, m_fRoll(fZeroVal)
, m_bInterp(false)
, m_IntTime(0.f)
, m_qRot(0.f, 0.f, 0.f, 1.f)
, m_pRenderContext(0)
{
	RegisterDrawInterface(this);
	RegisterUpdateInterface(this, pParent);
	RegisterInputInterface(this);

	//((CCoreSDK*)pParent->GetUserData())->GetCameraManager()->RegisterCamera(this);

	//if (((CCoreSDK*)pParent->GetUserData())->IsAEditor() && !((CCoreSDK*)pParent->GetUserData())->IsAGame())
	{
		// setup for editor
		m_bArcball = true;
		m_fYaw = PI_CONST * 0.25f;
		m_fPitch = PI_CONST * 0.15f;

		UpdateRotationQuat();
	}
}

//----------------------------------------------------------------------------------------------
CCamera::CCamera(const CCamera &Source)
: Super(Source)
, IDrawInterface(Source)
, IUpdateInterface(Source)
, IInputInterface(Source)
{
	if (&Source != this)
	{
		RegisterDrawInterface(this);
		RegisterUpdateInterface(this, Source.GetParent());
		RegisterInputInterface(this);

		m_bArcball = Source.m_bArcball;
		bOrtho = Source.bOrtho;
		Fov = Source.Fov;
		Aspect = Source.Aspect;
		NearDist = Source.NearDist;
		FarDist = Source.FarDist;
		m_fYaw = Source.m_fYaw;
		m_fPitch = Source.m_fPitch;
		m_fRoll = Source.m_fRoll;
		m_bInterp = Source.m_bInterp;
	}
}

//----------------------------------------------------------------------------------------------
CCamera::~CCamera()
{
	GetAppMain()->GetCameraManager()->UnregisterCamera(this);
}

//----------------------------------------------------------------------------------------------
void CCamera::Initialize()
{
	Super::Initialize();

	REGISTER_EVENT_PROXY_INPUT(this, this, boost::bind<bool>(&CCamera::ProxyProcessInputKey, this, _1, _2));

	Matrix M;
	m_qRot.Normalize();
	m_qRot.ToMatrix(&M);

	Vector z = Vector(M._13, M._23, M._33);
	z.normalize();
	Vector eyeNew = -z * 40.f;
	SetPosition_(eyeNew);

	UpdateRotationQuat();
	UpdateSplitDist();
	CalcProjMatrix();
	UpdateCamera();
	UpdateCullInfo();

	//GetAppMain()->GetViewportManager()->RebuildTransform(this);

	// default registration
	GetAppMain()->GetCameraManager()->RegisterCamera(this);
}

//----------------------------------------------------------------------------------------------
void CCamera::OnPropertyChanged(const char* PropertyName) 
{
	bool bUpdate = false;

	if (!strcmp(PropertyName, "bOrtho") ||
		!strcmp(PropertyName, "Fov") ||
		!strcmp(PropertyName, "Aspect") || 
		!strcmp(PropertyName, "NearDist") ||
		!strcmp(PropertyName, "FarDist") || 
		!strcmp(PropertyName, "m_fYaw") ||
		!strcmp(PropertyName, "m_fPitch") ||
		!strcmp(PropertyName, "m_fRoll"))
	{
		bUpdate = true;
	}
	else if (!strcmp(PropertyName, "bArcball"))
	{
		Matrix M;
		m_qRot.Normalize();
		m_qRot.ToMatrix(&M);

		Vector z = Vector(M._13, M._23, M._33);
		z.normalize();
		Vector eyeNew = -z * GetPosition_().Length();
		SetPosition_(eyeNew);

		bUpdate = true;
	}

	if (bUpdate)
	{
		UpdateRotationQuat();
		UpdateSplitDist();
		CalcProjMatrix();
		UpdateCamera();
		UpdateCullInfo();
	}

	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void CCamera::DoDraw()
{
#ifndef FINAL_RELEASE
	if (GetAppMain()->IsAEditor() && GetRenderDebug())
	{
		GetRenderComponent()->DrawLine(vecFrustum[0], vecFrustum[2], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[2], vecFrustum[3], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[3], vecFrustum[1], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[1], vecFrustum[0], COLOR_YELLOW);

		GetRenderComponent()->DrawLine(vecFrustum[4], vecFrustum[6], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[6], vecFrustum[7], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[7], vecFrustum[5], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[5], vecFrustum[4], COLOR_YELLOW);

		GetRenderComponent()->DrawLine(vecFrustum[0], vecFrustum[4], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[1], vecFrustum[5], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[2], vecFrustum[6], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(vecFrustum[3], vecFrustum[7], COLOR_YELLOW);

		// sub frustrum 0
		//GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[0], Subfrustras[0].vecSubFrustum[2], COLOR_RED);
		//GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[2], Subfrustras[0].vecSubFrustum[3], COLOR_RED);
		//GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[3], Subfrustras[0].vecSubFrustum[1], COLOR_RED);
		//GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[1], Subfrustras[0].vecSubFrustum[0], COLOR_RED);

		GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[4], Subfrustras[0].vecSubFrustum[6], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[6], Subfrustras[0].vecSubFrustum[7], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[7], Subfrustras[0].vecSubFrustum[5], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[5], Subfrustras[0].vecSubFrustum[4], COLOR_YELLOW);

		//Matrix I;
		//GetRenderComponent()->DrawBounds(I, Subfrustras[0].Bounds, 0xffb2b5ee);

		//GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[0], Subfrustras[0].vecSubFrustum[4], COLOR_RED);
		//GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[1], Subfrustras[0].vecSubFrustum[5], COLOR_RED);
		//GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[2], Subfrustras[0].vecSubFrustum[6], COLOR_RED);
		//GetRenderComponent()->DrawLine(Subfrustras[0].vecSubFrustum[3], Subfrustras[0].vecSubFrustum[7], COLOR_RED);

		// sub frustrum 1
		//GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[0], Subfrustras[1].vecSubFrustum[2], COLOR_GREEN);
		//GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[2], Subfrustras[1].vecSubFrustum[3], COLOR_GREEN);
		//GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[3], Subfrustras[1].vecSubFrustum[1], COLOR_GREEN);
		//GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[1], Subfrustras[1].vecSubFrustum[0], COLOR_GREEN);

		GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[4], Subfrustras[1].vecSubFrustum[6], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[6], Subfrustras[1].vecSubFrustum[7], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[7], Subfrustras[1].vecSubFrustum[5], COLOR_YELLOW);
		GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[5], Subfrustras[1].vecSubFrustum[4], COLOR_YELLOW);

		//GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[0], Subfrustras[1].vecSubFrustum[4], COLOR_GREEN);
		//GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[1], Subfrustras[1].vecSubFrustum[5], COLOR_GREEN);
		//GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[2], Subfrustras[1].vecSubFrustum[6], COLOR_GREEN);
		//GetRenderComponent()->DrawLine(Subfrustras[1].vecSubFrustum[3], Subfrustras[1].vecSubFrustum[7], COLOR_GREEN);

		// sub frustrum 2
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[0], Subfrustras[2].vecSubFrustum[2], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[2], Subfrustras[2].vecSubFrustum[3], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[3], Subfrustras[2].vecSubFrustum[1], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[1], Subfrustras[2].vecSubFrustum[0], COLOR_BLUE);

		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[4], Subfrustras[2].vecSubFrustum[6], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[6], Subfrustras[2].vecSubFrustum[7], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[7], Subfrustras[2].vecSubFrustum[5], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[5], Subfrustras[2].vecSubFrustum[4], COLOR_BLUE);

		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[0], Subfrustras[2].vecSubFrustum[4], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[1], Subfrustras[2].vecSubFrustum[5], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[2], Subfrustras[2].vecSubFrustum[6], COLOR_BLUE);
		//GetRenderComponent()->DrawLine(Subfrustras[2].vecSubFrustum[3], Subfrustras[2].vecSubFrustum[7], COLOR_BLUE);
	}
#endif
}

//----------------------------------------------------------------------------------------------
bool CCamera::ProxyProcessInputKey(const IInputInterface *pSender, const EventInput &InputData)
{
	bool bUpdate = false;

	float fMult = InputData.ShiftPressed ? 10.f : 1.f;

	switch (InputData.Key)
	{
	case KEY_UP:
			SetPosition_(GetPosition_() + GetDirection() * fMult);
			bUpdate = true;
		break;

	case KEY_DOWN:
			SetPosition_(GetPosition_() - GetDirection() * fMult);
			bUpdate = true;
		break;

	case KEY_RIGHT:
		if (m_bArcball)
		{
			SetPosition_(GetPosition_() + GetStrafe() * fMult);
			bUpdate = true;
		}break;

	case KEY_LEFT:
		if (m_bArcball)
		{
			SetPosition_(GetPosition_() - GetStrafe() * fMult);
			bUpdate = true;
		}break;

 	case KEY_F1:
 	case KEY_F2:
 		{
 			m_fYaw += 0.03f * (InputData.Key == KEY_F1 ? -1.f : 1.f);
 			bUpdate = true;
 		}break;
 
 	case KEY_F3:
 	case KEY_F4:
 		{
 			m_fPitch += 0.03f * (InputData.Key == KEY_F4 ? -1.f : 1.f);
 			bUpdate = true;
		}break;
	};

	if (bUpdate)
	{
		UpdateRotationQuat();
		UpdateCamera();
		UpdateCullInfo();

		//GetAppMain()->GetViewportManager()->RebuildTransform(this);
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------
void CCamera::ProcessScroll(int pos)
{
	const float mult = pos > 0 ? 5.f : -5.f;
	const float min_rad = 5.f;

	const auto dir = GetDirection();
	auto currPos = GetPosition_();
	auto newPos = currPos + dir * mult;
		
	if (m_bArcball)
	{ 
		Vector vecMinRad = -dir * min_rad;
		if (GetHalfSpace(dir, vecMinRad, currPos) !=
			GetHalfSpace(dir, vecMinRad, newPos))
		{
			newPos = vecMinRad - (dir * m_eps); // clamp to sphere with min_rad radius
		}
	}

	SetPosition_(newPos);

	UpdateRotationQuat();
	UpdateCamera();
	UpdateCullInfo();

	//GetAppMain()->GetViewportManager()->RebuildTransform(this);
}

//----------------------------------------------------------------------------------------------
void CCamera::SetFocus(bool bFlag /*= true*/)
{
	//IDrawInterface::SetFocus(bFlag);

	if (bFlag)
	{
		ActivateCamera();
	}
}

//----------------------------------------------------------------------------------------------
Vector CCamera::GetDirection() const
{
	return Vector(m_viewmatrix._13, m_viewmatrix._23, m_viewmatrix._33);
}

//----------------------------------------------------------------------------------------------
Vector CCamera::GetStrafe() const
{
	return Vector(m_viewmatrix._11, m_viewmatrix._21, m_viewmatrix._31);
}

//----------------------------------------------------------------------------------------------
bool CCamera::OnMouseMove(const MouseMoveInputData &InputData)
{
	if (InputData.bMiddleButtonPressed)
	{
		if (InputData.DeltaPos.x != 0.f){
			m_fYaw += 0.03f * (InputData.DeltaPos.x > 0.f ? -1.f : 1.f);
		}

		if (InputData.DeltaPos.y != 0.f){
			m_fPitch += 0.03f * (InputData.DeltaPos.y > 0.f ? -1.f : 1.f);
		}

		UpdateRotationQuat();

		if (m_bArcball)
		{
			Matrix M;
			m_qRot.Normalize();
			m_qRot.ToMatrix(&M);

			Vector z = Vector(M._13, M._23, M._33);
			z.normalize();
			Vector eyeNew = -z * GetPosition_().Length();
			SetPosition_(eyeNew);
		}

		UpdateCamera();
		UpdateCullInfo();

		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool CCamera::OnMouseWheel(float ds)
{
	ProcessScroll((int)ds);
	return true;
}

//----------------------------------------------------------------------------------------------
void CCamera::ActivateCamera()
{
	CameraManager *pMgr = GetAppMain()->GetCameraManager();

	assert(pMgr);

	pMgr->ActivateCamera(this);
}

//----------------------------------------------------------------------------------------------
bool CCamera::CheckBoxVisibility(const Bounds3f & BBox) const
{
	return FrustrumIntersectBBox(m_UniCullinfo, BBox);
}

//----------------------------------------------------------------------------------------------
void CCamera::Update(float dt)
{
	if (m_bInterp)
	{
		float fInt = m_IntTime / 0.3f;

		if (fInt > 1.f)
		{
			m_bInterp = false;
			return;
		}

		Slerp(m_qRot, fInt, m_qRotStart, m_qRotTo);

		Matrix M;
		m_qRot.Normalize();
		m_qRot.ToMatrix(&M);
		 
		Vector z = Vector(M._13, M._23, M._33);
		z.normalize();
		Vector eyeNew = -z * GetPosition_().Length();
		SetPosition_(eyeNew);

		UpdateCamera();
		UpdateCullInfo();

		m_IntTime += dt;
	}
}

//----------------------------------------------------------------------------------------------
void CCamera::UpdateCullInfo()
{	
	BuildFrustrumPoints();

	BuildCullPlanes(m_UniCullinfo, vecFrustum);

	BuildCullPlanes(Subfrustras[0].SubUniCullinfo, Subfrustras[0].vecSubFrustum);
	BuildCullPlanes(Subfrustras[1].SubUniCullinfo, Subfrustras[1].vecSubFrustum);
	BuildCullPlanes(Subfrustras[2].SubUniCullinfo, Subfrustras[2].vecSubFrustum);
}

//----------------------------------------------------------------------------------------------
void CCamera::BuildCullPlanes(UniCullinfo &OutCullinfo, const Vector FrustrumPoints[])
{
	OutCullinfo.Planes.clear();

	Vector N;

	// side 0
	cross(N, FrustrumPoints[4] - FrustrumPoints[0], FrustrumPoints[2] - FrustrumPoints[0]);
	N.normalize();
	OutCullinfo.Planes.push_back(Plane(N, FrustrumPoints[0]));

	// side 1
	cross(N, FrustrumPoints[2] - FrustrumPoints[0], FrustrumPoints[1] - FrustrumPoints[0]);
	N.normalize();
	OutCullinfo.Planes.push_back(Plane(N, FrustrumPoints[0]));

	// side 2
	cross(N, FrustrumPoints[1] - FrustrumPoints[0], FrustrumPoints[4] - FrustrumPoints[0]);
	N.normalize();
	OutCullinfo.Planes.push_back(Plane(N, FrustrumPoints[0]));

	// side 3
	cross(N, FrustrumPoints[5] - FrustrumPoints[7], FrustrumPoints[3] - FrustrumPoints[7]);
	N.normalize();
	OutCullinfo.Planes.push_back(Plane(N, FrustrumPoints[7]));

	// side 4
	cross(N, FrustrumPoints[6] - FrustrumPoints[7], FrustrumPoints[5] - FrustrumPoints[7]);
	N.normalize();
	OutCullinfo.Planes.push_back(Plane(N, FrustrumPoints[7]));

	// side 5
	cross(N, FrustrumPoints[3] - FrustrumPoints[7], FrustrumPoints[6] - FrustrumPoints[7]);
	N.normalize();
	OutCullinfo.Planes.push_back(Plane(N, FrustrumPoints[7]));
}

//----------------------------------------------------------------------------------------------
void CCamera::BuildFrustrumPoints()
{
	Matrix viewproj, iviewproj;

	vecFrustum[0] = Vector( -1.f, -1.f, 0.f); // xyz
	vecFrustum[1] = Vector( 1.f, -1.f, 0.f); // Xyz
	vecFrustum[2] = Vector( -1.f, 1.f, 0.f); // xYz
	vecFrustum[3] = Vector( 1.f, 1.f, 0.f); // XYz
	vecFrustum[4] = Vector( -1.f, -1.f, 1.f); // xyZ
	vecFrustum[5] = Vector( 1.f, -1.f, 1.f); // XyZ
	vecFrustum[6] = Vector( -1.f, 1.f, 1.f); // xYZ
	vecFrustum[7] = Vector( 1.f, 1.f, 1.f); // XYZ

	viewproj = m_viewmatrix * m_projmatrix;
	invert(iviewproj, viewproj);

	for (int index = 0; index < 8; ++index)
	{
		vecFrustum[index] = transform_coord(vecFrustum[index], iviewproj);
	}

	// build subfrustrums
	Matrix s_viewproj[3], s_iviewproj[3];

	Subfrustras[0].vecSubFrustum[0] = Subfrustras[1].vecSubFrustum[0] = Subfrustras[2].vecSubFrustum[0] = Vector( -1.f, -1.f, 0.f); // xyz
	Subfrustras[0].vecSubFrustum[1] = Subfrustras[1].vecSubFrustum[1] = Subfrustras[2].vecSubFrustum[1] = Vector( 1.f, -1.f, 0.f); // Xyz
	Subfrustras[0].vecSubFrustum[2] = Subfrustras[1].vecSubFrustum[2] = Subfrustras[2].vecSubFrustum[2] = Vector( -1.f, 1.f, 0.f); // xYz
	Subfrustras[0].vecSubFrustum[3] = Subfrustras[1].vecSubFrustum[3] = Subfrustras[2].vecSubFrustum[3] = Vector( 1.f, 1.f, 0.f); // XYz
	Subfrustras[0].vecSubFrustum[4] = Subfrustras[1].vecSubFrustum[4] = Subfrustras[2].vecSubFrustum[4] = Vector( -1.f, -1.f, 1.f); // xyZ
	Subfrustras[0].vecSubFrustum[5] = Subfrustras[1].vecSubFrustum[5] = Subfrustras[2].vecSubFrustum[5] = Vector( 1.f, -1.f, 1.f); // XyZ
	Subfrustras[0].vecSubFrustum[6] = Subfrustras[1].vecSubFrustum[6] = Subfrustras[2].vecSubFrustum[6] = Vector( -1.f, 1.f, 1.f); // xYZ
	Subfrustras[0].vecSubFrustum[7] = Subfrustras[1].vecSubFrustum[7] = Subfrustras[2].vecSubFrustum[7] = Vector( 1.f, 1.f, 1.f); // XYZ

	s_viewproj[0] = m_viewmatrix * Subfrustras[0].SubProjMatrix;
	s_viewproj[1] = m_viewmatrix * Subfrustras[1].SubProjMatrix;
	s_viewproj[2] = m_viewmatrix * Subfrustras[2].SubProjMatrix;

	invert(s_iviewproj[0], s_viewproj[0]);
	invert(s_iviewproj[1], s_viewproj[1]);
	invert(s_iviewproj[2], s_viewproj[2]);

	for (int index = 0; index < 8; ++index)
	{
		Subfrustras[0].vecSubFrustum[index] = transform_coord(Subfrustras[0].vecSubFrustum[index], s_iviewproj[0]);
		Subfrustras[1].vecSubFrustum[index] = transform_coord(Subfrustras[1].vecSubFrustum[index], s_iviewproj[1]);
		Subfrustras[2].vecSubFrustum[index] = transform_coord(Subfrustras[2].vecSubFrustum[index], s_iviewproj[2]);
	}

	for (int Index = 0; Index < NUM_SPLITS; ++Index)
	{
		Subfrustras[Index].Bounds.SetBounds(Vector(Subfrustras[Index].vecSubFrustum[0]), Vector(Subfrustras[Index].vecSubFrustum[0]));

		for (int IndexPoint = 1; IndexPoint < 8; ++IndexPoint)
		{
			Subfrustras[Index].Bounds += Subfrustras[Index].vecSubFrustum[IndexPoint];
		}
	}
}

//----------------------------------------------------------------------------------------------
void CCamera::UpdateCamera()
{
	m_qRot.Normalize();
	m_qRot.ToMatrix(&m_viewmatrix);

	Vector axisX(m_viewmatrix._11, m_viewmatrix._21, m_viewmatrix._31);
	Vector axisY(m_viewmatrix._12, m_viewmatrix._22, m_viewmatrix._32);
	Vector axisZ(m_viewmatrix._13, m_viewmatrix._23, m_viewmatrix._33);

	Vector eye = GetPosition_();

	m_viewmatrix.a03 = -axisX.Dot(eye);
	m_viewmatrix.a13 = -axisY.Dot(eye);
	m_viewmatrix.a23 = -axisZ.Dot(eye);

	//SetLTM_(m_LTM);

	GetAppMain()->GetViewportManager()->RebuildTransform(this);
}

//----------------------------------------------------------------------------------------------
void CCamera::CalcProjMatrix()
{
	float w = (float)GetRenderComponent()->GetViewportWidth();
	float h = (float)GetRenderComponent()->GetViewportHeight();

	if (!bOrtho)
	{
		float fAspect = w / h;
		perspective(m_projmatrix, Fov, fAspect, NearDist, FarDist);

		perspective(Subfrustras[0].SubProjMatrix, Fov, fAspect, Subfrustras[0].DistNear, Subfrustras[0].DistFar);
		perspective(Subfrustras[1].SubProjMatrix, Fov, fAspect, Subfrustras[1].DistNear, Subfrustras[1].DistFar);
		perspective(Subfrustras[2].SubProjMatrix, Fov, fAspect, Subfrustras[2].DistNear, Subfrustras[2].DistFar);	
	}
	else
	{
		w *= 0.5f;
		h *= 0.5f;

		ortho(m_projmatrix, -w, w, -h, h, NearDist, FarDist);

		ortho(Subfrustras[0].SubProjMatrix, -w, w, -h, h, Subfrustras[0].DistNear, Subfrustras[0].DistFar);
		ortho(Subfrustras[1].SubProjMatrix, -w, w, -h, h, Subfrustras[1].DistNear, Subfrustras[1].DistFar);
		ortho(Subfrustras[2].SubProjMatrix, -w, w, -h, h, Subfrustras[2].DistNear, Subfrustras[2].DistFar);
	}
}

//----------------------------------------------------------------------------------------------
void CCamera::UpdateSplitDist()
{
 	float lambda = 0.85f;
 	float ratio = FarDist / NearDist;
 
	Subfrustras[0].DistNear = NearDist;

	for (int Index = 1; Index < NUM_SPLITS; ++Index)
	{
		float si = Index / (float) NUM_SPLITS;

		Subfrustras[Index].DistNear = lambda * (NearDist * powf(ratio, si)) + (1 - lambda) * (NearDist + (FarDist - NearDist) * si);
		Subfrustras[Index - 1].DistFar = Subfrustras[Index].DistNear * 1.005f;
	}

	Subfrustras[NUM_SPLITS - 1].DistFar = FarDist;
}

//----------------------------------------------------------------------------------------------
void CCamera::UpdateRotationQuat()
{
	m_qRot.set_rot(m_fYaw, m_fPitch, m_fRoll);
}

//----------------------------------------------------------------------------------------------
void CCamera::SetInterp(Quaternion& RotTo, float time/* = 0.5f*/)
{
	m_bInterp = true;

 	m_qRotStart = m_qRot;

	Quaternion rotY(0.f, 0.f, 0.f, 1.f),
				rotP(0.f, 0.f, 0.f, 1.f),
				rotR(0.f, 0.f, 0.f, 1.f);

	rotY.set_rot(RotTo.x, 0.f, 0.f);
	rotP.set_rot(0.f, RotTo.y, 0.f);
	rotR.set_rot(0.f, 0.f, RotTo.z);

	rotP *= rotY;
	rotP *= rotR;

	Matrix M;
	rotP.Normalize();
	rotP.ToMatrix(&M);

	m_qRotTo = rotP;

	m_IntTime = 0.f;
}