#pragma once

#include "UIScreenObject.h"

class UIComp_DrawImage;

class EXPORT UIImage : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(UIImage, UIScreenObject);

public:
	UIImage(const CObjectAbstract * Parent);
	virtual ~UIImage();

	virtual void		DoDraw();

	void LoadImage(const wchar_t * filename, IListener * pListener = 0);

	bool GetInfo(unsigned int &Width, unsigned int &Height) const;
	bool GetDIB(BYTE** pData, bool bResample = false, unsigned int NewWidth = 0, unsigned int NewHeight = 0) const;

protected:
	UIComp_DrawImage	*m_pImageComponent; // transient
};