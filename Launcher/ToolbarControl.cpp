#include "ToolbarControl.h"
#include <assert.h>

//----------------------------------------------------------------------------------------------
CToolbarControl::CToolbarControl(HWND hwnd,
								 unsigned int ImageWidth,
								 unsigned int ImageHeight,
								 unsigned int NumImages,
								 unsigned int BitDepth /*= ILC_COLOR24*/)
: m_hWnd(hwnd)
, m_ImageWidth(ImageWidth)
, m_ImageHeight(ImageHeight)
, m_NumImages(NumImages)
, m_BitDepth(BitDepth)
{

}

//----------------------------------------------------------------------------------------------
CToolbarControl::~CToolbarControl()
{

}

//----------------------------------------------------------------------------------------------
void CToolbarControl::AttachToolbarImages(UINT inNormalImageID,	UINT inDisabledImageID,	UINT inHotImageID)
{
	// make high-color image lists for each of the bitmaps
	MakeToolbarImageList(inNormalImageID, m_ToolbarImages);
	MakeToolbarImageList(inDisabledImageID, m_ToolbarImagesDisabled);
	MakeToolbarImageList(inHotImageID, m_ToolbarImagesHot);

	// get the toolbar control associated with the CToolbar object
	CToolBarCtrl	barCtrl = m_hWnd;

	// attach the image lists to the toolbar control
	barCtrl.SetImageList (m_ToolbarImages.m_hImageList);
	barCtrl.SetDisabledImageList (m_ToolbarImagesDisabled.m_hImageList);
	barCtrl.SetHotImageList (m_ToolbarImagesHot.m_hImageList);
}

//----------------------------------------------------------------------------------------------
void CToolbarControl::MakeToolbarImageList(UINT inBitmapID, CImageList &outImageList)
{
	CBitmap	bm;

	// if we use CBitmap::LoadBitmap() to load the bitmap, the colors
	// will be reduced to the bit depth of the main screen and we won't
	// be able to access the pixels directly. To avoid those problems,
	// we'll load the bitmap as a DIBSection instead and attach the
	// DIBSection to the CBitmap.
	HRSRC rcBtm = FindResource((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(inBitmapID), RT_BITMAP);
	bm.Attach((HBITMAP)LoadImage((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(inBitmapID), IMAGE_BITMAP, 0, 0, (LR_DEFAULTSIZE | LR_CREATEDIBSECTION)));

	// replace the specified color in the bitmap with the user's
	// button color
	ReplaceBackgroundColor(bm);

	// create a 24 bit image list with the same dimensions and number
	// of buttons as the toolbar
	outImageList.Create(m_ImageWidth, m_ImageHeight, m_BitDepth, m_NumImages, 0);

	// attach the bitmap to the image list
	outImageList.Add(bm.m_hBitmap, RGB (0, 0, 0));
}

//----------------------------------------------------------------------------------------------
void CToolbarControl::ReplaceBackgroundColor(CBitmap &ioBM)
{
	// figure out how many pixels there are in the bitmap
	BITMAP bmInfo;

	ioBM.GetBitmap(&bmInfo);

	if (bmInfo.bmBitsPixel < 24)
		return;
	// add support for additional bit depths here if you choose
	assert(bmInfo.bmBitsPixel == 24);
	assert(bmInfo.bmWidthBytes >= (bmInfo.bmWidth * 3));

	const UINT	numPixels (bmInfo.bmHeight * bmInfo.bmWidth);

	// get a pointer to the pixels
	DIBSECTION  ds;

	GetObject(ioBM.m_hBitmap, sizeof(DIBSECTION), &ds);

	RGBTRIPLE*	pixels = reinterpret_cast<RGBTRIPLE*>(ds.dsBm.bmBits);
	assert(pixels != NULL);

	// get the user's preferred button color from the system
	const COLORREF		buttonColor (::GetSysColor (COLOR_BTNFACE));
	const RGBTRIPLE		userBackgroundColor = {	GetBValue (buttonColor), GetGValue (buttonColor), GetRValue (buttonColor)};

	// search through the pixels, substituting the user's button
	// color for any pixel that has the magic background color

	const COLORREF kBackgroundColor(GetSysColor(COLOR_3DFACE));
	for (UINT i = 0; i < numPixels; ++i)
	{
		if (pixels[i].rgbtBlue == GetBValue(kBackgroundColor) &&
			pixels[i].rgbtGreen ==  GetGValue(kBackgroundColor) &&
			pixels[i].rgbtRed ==  GetRValue(kBackgroundColor))
		{
			pixels[i] = userBackgroundColor;
		}
	}
}