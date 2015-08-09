#ifndef __win32imagelist_h__
#define __win32imagelist_h__

#ifdef WIN32
#pragma once
#endif

//----------------------------------------------------------------------------------------------
class CImgThumbnail
{
public:
	CImgThumbnail(UIImage* SrcImg, const int Size);
	~CImgThumbnail();

	bool Initialize(HDC hdc);
	void Free();
	void Draw(HDC hdc, HDC MemDC, int PosX, int PosY);

	const UIImage* GetSrcImage() const { return m_pSrcImage; }

private:
	const unsigned int m_SizeThumbnail;
	const UIImage* m_pSrcImage;
	HBITMAP m_hBitmap;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class Win32ImageList : public IListener
{
	static const unsigned short MAX_CAPACITY = 256;

	typedef std::vector<CImgThumbnail*>	TVecImgThumbnail;
	typedef TVecImgThumbnail::iterator	TVecImgThumbnailIter;
	typedef TVecImgThumbnail::const_iterator TVecImgThumbnailConstIter;

public:
	Win32ImageList(HWND hWndParent, const class CWindowManager *pWndMgr);
	~Win32ImageList();

	LRESULT WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

	virtual void OnOperationComplete(class CObjectAbstract* pObject);
	virtual void OnOperationFailed(class CObjectAbstract* pObject);

	void OnResize();
	void RedrawWindow();

protected:
	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

	int	 GetOffsetX() const { return m_ViewOffsetX; }
	int	 GetOffsetY() const { return m_ViewOffsetY; }

	unsigned int GetTotalWidth() const;

	void UpdateScrollbar();

private:
	HWND m_hwndParent;
	HWND m_hwndCanvas;
	HWND m_hwndButtonLeft;
	HWND m_hwndButtonRight;
	HWND m_hwndScroll;

	int m_ViewOffsetX;
	int m_ViewOffsetY;
	
	TVecImgThumbnail m_VecImgThumbnails;
	const class CWindowManager *m_pWindowManager;

	const unsigned int m_SizeThumbnail;
	const unsigned int m_BorderWidth;

	int m_nSelectedElement;
};

#endif//__win32imagelist_h__