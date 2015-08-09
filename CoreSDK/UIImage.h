#ifndef __uiimage_h__
#define __uiimage_h__

#ifdef _WIN32
#pragma once
#endif

#include "UIScreenObject.h"

class EXPORT UIImage : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(UIImage, UIScreenObject);

public:
	UIImage(const CObjectAbstract * Parent);
	UIImage(const UIImage& Source);
	virtual ~UIImage();

	virtual void		DoDraw();

	void LoadImage(const wchar_t * filename, IListener * pListener = 0);

	bool GetInfo(unsigned int &Width, unsigned int &Height) const;
	bool GetDIB(BYTE** pData, bool bResample = false, unsigned int NewWidth = 0, unsigned int NewHeight = 0) const;

protected:
	class UIComp_DrawImage	*m_pImageComponent; // transient
};

#endif//__uiimage_h__