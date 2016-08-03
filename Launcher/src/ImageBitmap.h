#ifndef __imagebitmap_h__
#define __imagebitmap_h__

#ifdef _WIN32
#pragma once
#endif

class CImageBitmap
{
public:
	CImageBitmap();
	~CImageBitmap();

	bool LoadImageFrom(LPCWSTR fileName);
	void Clear();

	LPBITMAPINFO GetBMInfoPtr() const {return m_pPicInfo;}	
	RGBQUAD* GetBMColorTabPtr() const;

	LONG GetBMWidth() const;
	LONG GetBMHeight() const;

	BYTE* GetBMDataPtr() const { return m_pPictData; }

protected:
private:
	LPBITMAPINFO	m_pPicInfo;
	PBYTE			m_pPictData;

	HBITMAP			m_Bitmap;
	tagBITMAP		m_BitmapInfo;
	RGBQUAD 		*m_lpRGB;

	int				m_PictWidth;
	int				m_PictHeight;
};

#endif//__imagebitmap_h__