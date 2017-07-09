
#pragma once

#include "UIScreenObject.h"

class UIComp_DrawFont;

class EXPORT Label : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(Label, UIScreenObject);

public:
	Label(const CObjectAbstract * Parent);
	virtual ~Label();

	virtual void DoDraw();

	virtual void SetText(const std::wstring& text) { Text = text; }
	virtual std::wstring GetText() { return Text; }

protected:
private: 
	UIComp_DrawFont * FontComponent;
	std::wstring Text;
};