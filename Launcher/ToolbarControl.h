#ifndef __toolbarcontrol_h__
#define __toolbarcontrol_h__

#pragma once

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlgdi.h>
#include <commctrl.h>

class CToolbarControl
{
public:
	CToolbarControl(HWND hwnd, 
					unsigned int ImageWidth,
					unsigned int ImageHeight,
					unsigned int NumImages,
					unsigned int BitDepth = ILC_COLOR24);

	~CToolbarControl();

	void AttachToolbarImages(UINT inNormalImageID,	UINT inDisabledImageID,	UINT inHotImageID);

protected:
	void MakeToolbarImageList (UINT inBitmapID, CImageList&	outImageList);

	void ReplaceBackgroundColor(CBitmap &ioBM);

private:
	HWND	m_hWnd;

	CImageList	m_ToolbarImagesDisabled;
	CImageList	m_ToolbarImagesHot;
	CImageList	m_ToolbarImages;

	unsigned int	m_ImageWidth;
	unsigned int	m_ImageHeight;
	unsigned int	m_NumImages;
	unsigned int	m_BitDepth;
};


#endif//__toolbarcontrol_h__