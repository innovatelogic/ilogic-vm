#pragma once

#include "game_types.h"
#include "IBaseInterface.h"
#include "../Foundation/StdafxFoundation.h"

class CObjectAbstract;
class CCoreSDK;
class CActor;

class CORESDK_API IDrawInterface : public IBaseInterface
{
	DECLARE_INTERFACE_CLASS(IDrawInterface);

public:
	IDrawInterface(const CObjectAbstract *parent);
	IDrawInterface(const IDrawInterface &source);
	virtual ~IDrawInterface();

	virtual void RegisterDrawInterface(const CActor *pSrc, int SrcParent = 1);
	virtual void UnregisterDrawInterface();

	inline_ CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }
	void	SetCoreSDK(CCoreSDK* pCoreSDK) const { m_pCoreSDK = pCoreSDK; }

    TNodeMap<CActor, IDrawInterface>* GetNode() const { return m_pNode; }

	virtual bool	DoVisibilityTest_() const;
	
	virtual bool PrePropertyChangeIntf(const char *propertyName);
	virtual void OnPropertyChangedIntf(const char *propertyName);
	
	/** 
	 * Calls in main thread to fill render instructions for render thread.
	 * Used to call DoDraw method.
	 */
	void Draw();

	/** 
	 * @return VisualStates value.
	 */
	unsigned int	GetVisualStates() const { return m_VisualStates; }

	/** 
	 * Set object's states value.
	 *
	 * @param [in] State - new state value.
	 */
	void	SetVisualStates(unsigned int state) { m_VisualStates = state; }

	virtual void DrawBounds() const;

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
    virtual void    DoBuildCompounds();

	inline_ virtual	Vector2f    GetTransformedSize_() const { return Vector2f(1.f, 1.f); }

	/** Transforms local space position in to global space */
	virtual Vector& LocalToGlobalTransform(Vector& OutGlobalPoint, const Vector& InLocalPoint) const;
	virtual Vector& GlobalToLocalTransform(Vector& OutLocalPoint, const Vector& InGlobalPoint) const;
	virtual Matrix  CalcWorldTransform() const;

	virtual bool    IsValidBounds() { return m_Bounds.IsValid(); }
	
    void AddYawPitchRoll(const Vector &ypr) { m_YawPitchRoll += ypr; }

    /*!
     * Hierarchy helper function
     */
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

protected:
	TNodeMap<CActor, IDrawInterface> *m_pNode;

	mutable CCoreSDK *m_pCoreSDK;

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