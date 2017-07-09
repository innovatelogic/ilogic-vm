#pragma once

#include "UIScreenObject.h"

class EXPORT LabelButton : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(LabelButton, UIScreenObject);

public:
	LabelButton(const CObjectAbstract * Parent);
	virtual ~LabelButton();

	virtual void	OnPropertyChanged(const char* PropertyName);

	virtual void	Initialize();

	virtual void	DoDraw();

	virtual void		 SetText(const std::wstring& text) { Text = text; }
	virtual std::wstring GetText() const { return Text;	}

	virtual void	SetTextPositionX(float x);
	virtual float	GetTextPositionX() const;

	virtual void	SetTextPositionY(float y);
	virtual float	GetTextPositionY() const;

	virtual bool	DoEventPressed(const MouseInputData& InputData);
	virtual bool	DoEventReleased(const MouseInputData& InputData);
	 
protected:
	virtual void	ResolveActiveState();

	virtual void	SuperDeserializerInternal(tinyxml2::XMLElement *pTree);
	virtual bool	SuperSerializerInternal(std::ofstream &stream);
	virtual bool	SuperSerializerInternal(std::stringstream &stream);


protected:
private:
	class UIComp_DrawImageState *ImageComponents;
	class UIComp_DrawFontState *FontComponentState;
	
	// actions
	//class ActionHandler * ActionPress;
	//class ActionHandler * ActionRelease;

	std::wstring Text;
	float TextPositionX;
	float TextPositionY;
};