#ifndef __viewportinterface_h__
#define __viewportinterface_h__

#ifdef _WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"
#include "ObjectAbstract.h"

class CORESDK_API ViewportInterface
{
public:
	ViewportInterface(const class CObjectAbstract *pParent);
	virtual ~ViewportInterface();

	void RegisterViewportInterface(const CActor *Src);
	void UnregisterViewportInterface();

	const Matrix&	GetViewportViewMatrix() const { return m_ViewMatrix; }
	void			SetViewportViewMatrix(const Matrix &matrix) { m_ViewMatrix = matrix; }

	const Matrix&	GetViewportProjMatrix() const { return m_ProjMatrix; }
	void			SetViewportProjMatrix(const Matrix &matrix) { m_ProjMatrix = matrix; }

	const Vector&	GetViewPoint() const { return m_ViewPoint; }
	void			SetViewPoint(const Vector &Value) { m_ViewPoint = Value; }

	float			GetNearPlane() const { return m_fNearPlane; }
	void			SetNearPlane(float fValue) { m_fNearPlane = fValue; }

	float			GetFarPlane() const { return m_fFarPlane; }
	void			SetFarPlane(float fValue) { m_fFarPlane = fValue; }

	virtual void	DrawViewport() {}

	Matrix m_CameraWTM;

protected:
private:
	Matrix	m_ViewMatrix;
	Matrix	m_ProjMatrix;
	Vector	m_ViewPoint;

	float	m_fNearPlane;
	float	m_fFarPlane;

	TNodeMap<class CActor, class ViewportInterface> *m_pNode;

	mutable class CCoreSDK *m_pCoreSDK;
};

#endif