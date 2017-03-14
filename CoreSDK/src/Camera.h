
#pragma once

#include "reflx.h"
#include "NpActorTemplate.h"
#include "../include/PxCamera.h"
#include "../IDrawInterface.h"
#include "../IUpdateInterface.h"
#include "../IInputInterface.h"


#define NUM_SPLITS 3

//template<typename APIClass = core_sdk_api::PxCamera>
class CORESDK_API CCamera : public core_sdk_api::NpActorTemplate<core_sdk_api::PxCamera>,
							public IDrawInterface,
							public IUpdateInterface,
							public IInputInterface
{
public:
    typedef core_sdk_api::NpActorTemplate<core_sdk_api::PxCamera> NpCameraT;

private:
	DECLARE_CLASS_SIMPLE(CCamera, NpCameraT);

public:
	struct Subfrustra
	{
		Matrix			SubProjMatrix;
		UniCullinfo		SubUniCullinfo;
		Vector			vecSubFrustum[8];
		Bounds3f		Bounds;
		float			DistNear;
		float			DistFar;
	};

    //CCamera(const CCamera &other) = delete;
    CCamera& operator=(const CCamera &other) = delete;

public:
	CCamera(const CObjectAbstract *pParent);
	virtual ~CCamera();

	virtual void			Initialize();

	virtual void			OnPropertyChanged(const char *pPropertyName);
	virtual void			DoDraw();
	
	virtual bool			ProxyProcessInputKey(const class IInputInterface *pSender, const EventInput &InputData);
	virtual void			ProcessScroll(int pos);

	virtual void			SetFocus(bool bFlag = true);

	virtual const			Matrix& GetProjMatrix() const { return m_projmatrix; }
	virtual const			Matrix& GetViewMatrix() const { return m_viewmatrix; }

	void			        SetOrtho(bool value) override { m_bOrtho = value; }
	bool			        GetOrtho() const override  { return m_bOrtho; }

	void			        SetFov(float fov) override  { m_Fov = fov; }
	float			        GetFov() const override { return m_Fov; }

    void			        SetAspect(float aspect) override { Aspect = aspect; }
    float			        GetAspect() const override { return Aspect; }

    void			        SetNearDist(float _near) override { NearDist = _near; }
    float			        GetNearDist() const override { return NearDist; }

    void			        SetFarDist(float _far) override { FarDist = _far; }
    float			        GetFarDist() const override { return FarDist; }

    void			        SetYaw(float yaw) override { m_fYaw = yaw; }
    float			        GetYaw() const override { return m_fYaw; }

    void			        SetPitch(float pitch) override { m_fPitch = pitch; }
    float			        GetPitch() const override { return m_fPitch; }

    void			        SetRoll(float roll) override { m_fRoll = roll; }
    float			        GetRoll() const override { return m_fRoll; }

	virtual Vector			GetDirection() const;
	virtual Vector			GetStrafe() const;

	virtual Quaternion		GetRot() const { return m_qRot; }

	virtual bool			OnMouseMove(const MouseMoveInputData &InputData);
	virtual bool			OnMouseWheel(float ds);

	virtual void			ActivateCamera();
	virtual bool			CheckBoxVisibility(const Bounds3f & BBox) const;

	virtual void			Update(float dt);
	virtual void			SetInterp(Quaternion& RotTo, float time = 0.5f);

	virtual void			UpdateCamera();
	
	virtual void			UpdateCullInfo();
	virtual void			BuildCullPlanes(UniCullinfo	&OutCullinfo, const Vector FrustrumPoints[]);

	virtual void			BuildFrustrumPoints();
	
	virtual void			CalcProjMatrix();
	virtual void			UpdateSplitDist();
	virtual void			UpdateRotationQuat();

	class SRenderContext* GetRenderContext() const { return m_pRenderContext; }
	void SetRenderContext(SRenderContext *pContext) { m_pRenderContext = pContext; }

	bool IsArcball() const { return m_bArcball; }

protected:
protected:
	bool m_bArcball;

	/** true if ortho projection matrix */
	bool m_bOrtho;

	/** camera's fov */
	float m_Fov;

	/** aspect ratio */
	float Aspect;

	/** clipping distance */
	float NearDist;
	float FarDist;

	float m_fYaw, m_fPitch, m_fRoll;
	Quaternion m_qRot;

	Matrix m_viewmatrix;
	Matrix m_projmatrix;

public:
	UniCullinfo		 m_UniCullinfo;
	Vector			 vecFrustum[8];

	// subfrustrum data
	Subfrustra		Subfrustras[NUM_SPLITS];

private:
	// interpolation data
	bool m_bInterp;
	Quaternion m_qRotStart;
	Quaternion m_qRotTo;

	float m_IntTime;

	class SRenderContext *m_pRenderContext;
};