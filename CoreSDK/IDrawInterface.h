#ifndef __idrawinterface_h__
#define __idrawinterface_h__

#ifdef WIN32
#pragma once
#endif

#include "IBaseInterface.h"
#include "../Foundation/StdafxFoundation.h"

class CObjectAbstract;

class CORESDK_API IDrawInterface : public IBaseInterface
{
	DECLARE_INTERFACE_CLASS(IDrawInterface);

public:
	IDrawInterface(const CObjectAbstract *pParent);
	IDrawInterface(const IDrawInterface &Source);
	virtual ~IDrawInterface();

	virtual void RegisterDrawInterface(const CActor *pSrc, int SrcParent = 1);
	virtual void UnregisterDrawInterface();

	inline_ class CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }
	void					SetCoreSDK(class CCoreSDK* pCoreSDK) const { m_pCoreSDK = pCoreSDK; }

	virtual bool	DoVisibilityTest_() const;
	
	virtual bool PrePropertyChangeIntf(const char *PropertyName);
	virtual void OnPropertyChangedIntf(const char *PropertyName);
	

	/** 
	 * Calls in main thread to fill render instructions for render thread.
	 * Used to call DoDraw method.
	 */
	void Draw();

	/** 
	 *	AS_NONE,
	 *	AS_ENABLED
	 *	AS_VISIBLE
	 *	AS_FOCUSED
	 *	AS_LOCKED
	 *	AS_PRESSED
	 *	AS_MOUSE_OVER
	 *
	 * @return VisualStates value.
	 */
	unsigned int	GetVisualStates() const { return m_VisualStates; }

	/** 
	 * Set object's states value.
	 *
	 * @param [in] State - new state value.
	 */
	void	SetVisualStates(unsigned int State) { m_VisualStates = State; }

	virtual void DrawController() const;
	virtual void DrawBounds() const;

	virtual bool ProcessController(const MouseInputData &InputData);
	virtual bool ProcessController(const MouseMoveInputData &InputData);
	virtual bool ProcessControllerRelease(const MouseInputData &InputData);

	virtual bool ProcessPress(const MouseInputData &InputData);
	virtual bool ProcessRelease(const MouseInputData &InputData);

	virtual bool ProcessMouseMove(const MouseMoveInputData &InputData);
	virtual bool ProcessMouseWheel(float ds);

	virtual void SetFocus(bool bFlag = true);
	virtual bool IsFocused() const { return (m_VisualStates & AS_FOCUSED) != 0; }

	virtual void SetMouseOver(bool bFlag = true);
	virtual bool IsMouseOver() const { return (m_VisualStates & AS_MOUSE_OVER) != 0; }
	
	static bool	  GetMBPressed();
	static Vector GetUserStartMousePos();
	static Vector GetUserStartMouseDisplace();

	bool	GetVisible() const { return m_bVisible; }
	void	SetVisible(bool visible) { m_bVisible = visible; }

	bool	GetRenderDebug() const { return m_bRenderDebug; }
	void	SetRenderDebug(bool flag) { m_bRenderDebug = flag; }


	/** Pivot management */
	inline_ void			SetLTM_(const Matrix &matrix) const { m_LTM_ = matrix; }
	inline_ Matrix			GetLTM_() const { return m_LTM_; }

	inline_ virtual	void	SetPivot_(const Matrix& matrix) const { m_LTM_ = matrix; }
	inline_ virtual	Matrix	GetPivot_() const { return m_LTM_; }

	inline_ virtual	void	SetSTM_(const Matrix3f &matrix) const	{ m_STM_ = matrix; }
	inline_ virtual	Matrix3f GetSTM_() const { return m_STM_; }

	inline_ virtual	void	SetPosition_(const Vector &pos) { m_LTM_.t = pos; }
	inline_ virtual	Vector	GetPosition_() const { return m_LTM_.t; }

	inline_ void			SetBounds_(const Bounds3f &matrix)	{ m_Bounds = matrix; }
	inline_ Bounds3f		GetBounds_() const { return m_Bounds; }

	inline_ void			SetCompositeBounds_(const Bounds3f &bound) { m_CompositeBounds = bound; }
	inline_ Bounds3f		GetCompositeBounds_() const { return m_CompositeBounds; }

	inline_ Matrix			GetTransformedWTM_() const { return m_WorldMatrixTransform; }
	inline_ void			SetTransformedWTM_(const Matrix &matrix) { m_WorldMatrixTransform = matrix; }

	Vector&					GetRotator() const { return m_YawPitchRoll; }
	void					SetRotator(Vector &V) { m_YawPitchRoll = V; }

	/* transform rebuild build */
	virtual void			DoBuildWorldTransform_(const Matrix &WTM);
	virtual void			DoBuildSizeTransform() {}

    /*!
    * rebuilds compounds bbox tree
    * call after DoBuildWorldTransform_
    * complexity: N(n)??
    */
    virtual void            DoBuildCompounds();

	inline_ virtual	Vector2f	GetTransformedSize_() const { return Vector2f(1.f, 1.f); }

	/** Transforms local space position in to global space */
	virtual Vector&		LocalToGlobalTransform(Vector& OutGlobalPoint, const Vector& InLocalPoint) const;
	virtual Vector&		GlobalToLocalTransform(Vector& OutLocalPoint, const Vector& InGlobalPoint) const;
	virtual Matrix		CalcWorldTransform() const;

	virtual bool		IsValidBounds() { return m_Bounds.IsValid(); }

	/** mouse input availability indicator */
	virtual bool		IsHandleMouseInput() const { return true; }
	
	static bool	GetObjectAInLocalSpaceB(Matrix& outLTM, const IDrawInterface *pObjectA, const IDrawInterface *pObjectB);
	static bool	TransformObjectAToObjectBLocalPos(CActor *pObjectA, const CActor *pObjectB, Vector &LocalBPos = Vector(0.f, 0.f, 0.f), bool bKeepAOrientation = true);

	/**
	*	Calculates object parent world matrix.
	*	ATTENTION: function is slow avoid to use it in real time.
	*/
	static	Matrix		GetParentWorldTransform(const CActor *pObject);

protected:
	/** 
	* Calls in main thread to fill render instructions for render thread.
	*/
	virtual void	DoDraw() {}

	virtual void	ProcessControllerTranslate(const MouseMoveInputData &InputData);
	virtual void	ProcessControllerRotateLocal(const MouseMoveInputData &InputData);
	virtual void	ProcessControllerScaleLocal(const MouseMoveInputData &InputData);

	virtual bool	OnMouseMove(const MouseMoveInputData &InputData);
	virtual bool	OnMouseWheel(float ds);

public:
	static Vector	 m_SUserStartMousePosition;
	static Vector	 m_SUserStartMouseDisplace;
	static bool		 m_bSMiddleButtonPressed;

protected:
	TNodeMap<class CActor, class IDrawInterface> *m_pNode;

	mutable class CCoreSDK *m_pCoreSDK;

protected:
	unsigned int		m_VisualStates;

	/** visibility state flag */
	mutable bool		m_bVisible;

	/**  debug render mode  */
	bool				m_bRenderDebug;

	mutable Matrix		m_LTM_;

	/**  STM Represents Scale Transformation Matrix. */
	mutable Matrix3f	m_STM_;
	
	/** precomputed world matrix */
	mutable Matrix		m_WorldMatrixTransform;

	/* For editor's purpose only*/
	mutable Vector		m_YawPitchRoll;

	Bounds3f			m_Bounds;
	Bounds3f			m_CompositeBounds;
};

#endif//__idrawinterface_h__