#ifndef  __ucamera_h__
#define  __ucamera_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"
#include "IDrawInterface.h"
#include "IUpdateInterface.h"
#include "IInputInterface.h"

#define NUM_SPLITS 3

class CORESDK_API CCamera : public ActorAllocator,
							public IDrawInterface,
							public IUpdateInterface,
							public IInputInterface
{
	DECLARE_CLASS_SIMPLE(CCamera, ActorAllocator);

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

public:
	CCamera(const CObjectAbstract *pParent);
	CCamera(const CCamera &object);
	virtual ~CCamera();

	virtual void			Initialize();

	virtual void			OnPropertyChanged(const char *pPropertyName);
	virtual void			DoDraw();
	
	virtual bool			ProxyProcessInputKey(const class IInputInterface *pSender, const EventInput &InputData);
	virtual void			ProcessScroll(int pos);

	virtual void			SetFocus(bool bFlag = true);

	virtual const			Matrix& GetProjMatrix() const { return m_projmatrix; }
	virtual const			Matrix& GetViewMatrix() const { return m_viewmatrix; }

	virtual void			SetOrtho(bool Value) { bOrtho = Value; }
	virtual bool			GetOrtho() const { return bOrtho; }

	virtual void			SetFov(float fov) { Fov = fov; }
	virtual float			GetFov() const { return Fov; }  

	virtual void			SetAspect(float aspect) { Aspect = aspect; } 
	virtual float			GetAspect() const {return Aspect;}

	virtual void			SetNearDist(float _near) { NearDist = _near; }
	virtual float			GetNearDist() const { return NearDist; } 

	virtual void			SetFarDist(float _far) { FarDist = _far; }
	virtual float			GetFarDist() const { return FarDist; } 

	virtual void			SetYaw(float yaw) { m_fYaw = yaw; }
	virtual float			GetYaw() const { return m_fYaw; }

	virtual void			SetPitch(float pitch) { m_fPitch = pitch; }
	virtual float			GetPitch() const { return m_fPitch; }

	virtual void			SetRoll(float roll) { m_fRoll = roll; }
	virtual float			GetRoll() const { return m_fRoll; }

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
	bool bOrtho;

	/** camera's fov */
	float Fov;

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

#endif //__ucamera_h__