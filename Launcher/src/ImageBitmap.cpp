#include "../LauncherStdAfx.h"

//----------------------------------------------------------------------------------------------
CImageBitmap::CImageBitmap()
: m_pPicInfo(0)
, m_pPictData(0)
, m_Bitmap(0)
, m_lpRGB(0)
, m_PictWidth(0)
, m_PictHeight(0)
{

}

//----------------------------------------------------------------------------------------------
CImageBitmap::~CImageBitmap()
{
	Clear();
}

//----------------------------------------------------------------------------------------------
bool CImageBitmap::LoadImageFrom(LPCWSTR fileName)
{
	bool bResult = false;

	m_Bitmap = (HBITMAP)::LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	if (m_Bitmap)
	{
		BITMAP bitmap;
		::GetObject(m_Bitmap, sizeof(BITMAP), &bitmap);

		m_PictWidth = bitmap.bmWidth;
		m_PictHeight = bitmap.bmHeight;

		GetObject(m_Bitmap, sizeof(tagBITMAP), &m_BitmapInfo);
		GetObject(m_Bitmap, sizeof(tagRGBQUAD), m_lpRGB);

		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
void CImageBitmap::Clear()
{
	if(m_pPictData!=NULL)
		delete[] m_pPictData;

	m_pPictData=NULL;

	if(m_pPicInfo!=NULL) 
		delete[] m_pPicInfo;

	m_pPicInfo = NULL;
}

//----------------------------------------------------------------------------------------------
RGBQUAD* CImageBitmap::GetBMColorTabPtr() const
{
	return m_pPicInfo ? (LPRGBQUAD) (((BYTE*)(m_pPicInfo))+sizeof(BITMAPINFOHEADER)) : 0;
}

//----------------------------------------------------------------------------------------------
LONG CImageBitmap::GetBMWidth() const
{
	return m_pPicInfo ? m_pPicInfo->bmiHeader.biWidth : 0;
}

//----------------------------------------------------------------------------------------------
LONG CImageBitmap::GetBMHeight() const
{
	return m_pPicInfo ? m_pPicInfo->bmiHeader.biHeight : 0;
}