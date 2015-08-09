#ifndef __Label_h__
#define __Label_h__

#ifdef _WIN32
#pragma once
#endif

#include "UIScreenObject.h"

class UIComp_DrawFont;

class EXPORT Label : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(Label, UIScreenObject);

public:
	Label(const CObjectAbstract * Parent);
	Label(const Label & Source);
	virtual ~Label();

	virtual void DoDraw();

	virtual void SetText(const std::wstring& text) { Text = text; }
	virtual std::wstring GetText() { return Text; }

protected:
private: 
	UIComp_DrawFont * FontComponent;
	std::wstring Text;
};

#endif//__Label_h__