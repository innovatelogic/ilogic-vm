#pragma once

#include "ActorAllocator.h"
#include "IDrawInterfaceUI.h"
#include "game_types.h"

/** Class: UIScreenObject 
 *  Desc:  basic UI screen presentation class. 
 */
class CORESDK_API UIScreenObject : public ActorAllocator, public IDrawInterfaceUI
{
	DECLARE_CLASS_SIMPLE(UIScreenObject, ActorAllocator);

public:
	// constructor & destructor
    UIScreenObject(const CObjectAbstract *Parent);
	virtual ~UIScreenObject();
	
	/* properties */
	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual bool			AddChildNode(const CActor * actor, int Index = INDEX_NONE);

	virtual bool			HitTest(const Vector2f &Position);
	virtual void			SetEnabled(bool enable);

	virtual void			SetFocus(bool bFlag = true);

	virtual void			SetMouseOver(bool bFlag = true);
		
	virtual bool			PushClipBound(Matrix& WTM);
	virtual void			PopClipBound();

	/** 
	 * Editor helper function. 
	 * [1]. This function Place object located in parent space local transform matrix in to transformed in Position [Param1].
	 * [2]. Rotate object around local origin.
	 */
	virtual bool			UIEditor_ProcessDelete();
	
	virtual void			SetClipBounds(bool clip) { ClipBounds = clip;}
	virtual bool			GetClipBounds() { return ClipBounds; }

	virtual bool			DoEventPressed(const MouseInputData& InputData);
    virtual bool			DoEventReleased(const MouseInputData& InputData);

	virtual bool			DoVisibilityTest_();

protected:
	virtual void			ResolveActiveState() {}

private:
	EActiveState			ActiveState;

	float					Opacity;

	bool					ClipBounds; 
};