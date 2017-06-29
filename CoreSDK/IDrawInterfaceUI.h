#ifndef __idrawinterfaceui_h__
#define __idrawinterfaceui_h__

#ifdef WIN32
#pragma once
#endif

#include "coresdkafx.h"
#include "IDrawInterface.h"


class CORESDK_API IDrawInterfaceUI : public IDrawInterface
{
	DECLARE_INTERFACE_CLASS(IDrawInterfaceUI);

public:
	IDrawInterfaceUI(const CObjectAbstract *pParent);
	IDrawInterfaceUI(const IDrawInterfaceUI &Source);
	virtual ~IDrawInterfaceUI();

	virtual void RegisterDrawInterface(const CActor *Src, int SrcParent = 1);

	/** 
	 * Calls from Property class to signalize that it have change.
	 * 
	 * @param PropertyName [in] current property name have change.
	 */
	virtual bool PrePropertyChangeIntf(const char* PropertyName);

	/** 
	 * Calls from Property class to signalize that it have changed.
	 * 
	 * @param PropertyName [in] current property name have changed.
	 */
	virtual void OnPropertyChangedIntf(const char *PropertyName);

	virtual void DrawController() const;
	virtual void DrawBounds() const;

	virtual bool ProcessController(const MouseInputData &InputData);
	virtual bool ProcessController(const MouseMoveInputData &InputData);

	virtual bool ProcessPress(const MouseInputData &InputData);
	virtual bool ProcessRelease(const MouseInputData &InputData);

	virtual void DoBuildWorldTransform_(const Matrix &WTM);

	/*
		calls for transform percentage size presentation in to world size.
		should be call when rebuild scene, change screen resolution etc.
	*/
	virtual void	DoBuildSizeTransform();

	virtual float	GetParentWidth_() const;
	virtual float	GetParentHeight_() const;

	inline_ virtual Vector2f	GetTransformedSize_() const { return m_SizeTransformed; }

	inline_ virtual	Matrix	GetPivot_() const;
	inline_ virtual	void	SetPivot_(const Matrix& matrix) const;

	/*
	*  convert transform in to new format
	   used for predict correct transformation.		
	*/
	virtual void	ConvertPixelTransform_H(EPixelTransform ValueFrom, EPixelTransform ValueTo);
	virtual void	ConvertPixelTransform_V(EPixelTransform ValueFrom, EPixelTransform ValueTo);
	virtual void	ConvertSizeTransform_H(EPixelTransform ValueFrom, EPixelTransform ValueTo);
	virtual void	ConvertSizeTransform_V(EPixelTransform ValueFrom, EPixelTransform ValueTo);

	virtual Vector&	GlobalToLocalTransform(Vector& OutLocalPoint, const Vector& InGlobalPoint) const;

	/** get set position transformation */
	virtual EPixelTransform GetPixelTransform_H() const { return m_PixelTransform[0]; }
	virtual EPixelTransform GetPixelTransform_V() const { return m_PixelTransform[1]; }
	virtual void            SetPixelTransform_H(EPixelTransform  pixelTransform) { m_PixelTransform[0] = pixelTransform; }
	virtual void            SetPixelTransform_V(EPixelTransform  pixelTransform) { m_PixelTransform[1] = pixelTransform; }

	/** size transformation */
	virtual EPixelTransform GetSizePixelTransform_H() const { return m_SizeTransform[0]; }
	virtual void            SetSizePixelTransform_H(EPixelTransform pixelTransform) { m_SizeTransform[0] = pixelTransform; }
	virtual EPixelTransform GetSizePixelTransform_V() { return m_SizeTransform[1]; }
	virtual void            SetSizePixelTransform_V(EPixelTransform  pixelTransform) { m_SizeTransform[1] = pixelTransform; }

protected:
	virtual void	ProcessControllerTranslate(const MouseMoveInputData &InputData);
	virtual void	ProcessControllerRotateLocal(const MouseMoveInputData &InputData);
	virtual void	ProcessControllerScaleLocal(const MouseMoveInputData &InputData);

	virtual bool	OnMouseMove(const MouseMoveInputData &InputData);

private:

	union  // data
	{
		/** unified size scalar value */
		struct{
			Vector2f		m_Size;
		};
		struct{
			float			m_Width, m_Height;
		};
	};

	/** transformation flags */
	union  // data
	{
		struct{
			EPixelTransform		m_PixelTransform[2];
		};
		struct{
			int m_HPixelTransform, m_VPixelTransform;
		};
	};

	union  // data
	{
		struct{
			EPixelTransform		m_SizeTransform[2];
		};
		struct{
			int m_HSizeTransform, m_VSizeTransform;
		};
	};

	/** world transformed size */ 
	Vector2f		m_SizeTransformed;

protected:
	/* clipping flags */
	Vector4f		m_vClipViewport;
	bool			m_bPushClipping;
};

#endif//__idrawinterfaceui_h__