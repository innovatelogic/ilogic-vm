#ifndef __uiscene_h__
#define __uiscene_h__

#include "UIScreenObject.h"

class CORESDK_API UIScene : public UIScreenObject, public IUIInputController
{
	DECLARE_CLASS_SIMPLE(UIScene, UIScreenObject);

public:
	UIScene(const CObjectAbstract *parent);
	virtual ~UIScene();

	virtual void	OnPropertyChanged(const char* PropertyName);

	virtual void	Initialize();

	/**
	 * Pre release object's call.
	 * Calls prior memory deallocation.
	 *
	 * Suggestion: signal to unregister object will be delete soon
	 */
	virtual void	PreRelease();

	virtual void	DoDraw();

	virtual void	Update(float DeltaTime);

	ESceneInputmode	GetInputMode() const { return m_eInputMode; }
	void			SetInputMode(ESceneInputmode Value) { m_eInputMode = Value; }

	EHandleInputKey		GetHandleInputKeyMode() const { return m_eHandleInputKey; }
	EHandleInputMouse	GetHandleInputMouseMode() const { return m_eHandleInputMouse; }

	virtual bool	HitTest(const Vector2f &Position);

	virtual bool	IsHandleMouseInput() const;
	virtual bool	IsHandleKeyInput() const;

	virtual bool	ProcessButtonUI(const EventInput &InputData);

protected:
private:
	ESceneInputmode		m_eInputMode;
	EHandleInputKey		m_eHandleInputKey;
	EHandleInputMouse	m_eHandleInputMouse;
};

#endif//__uiscene_h__