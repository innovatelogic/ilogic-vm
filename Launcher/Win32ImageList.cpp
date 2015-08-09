#include "LauncherStdAfx.h"

#define WM_USER_UPDATE_SCROLL				WM_APP+0

//----------------------------------------------------------------------------------------------
CImgThumbnail::CImgThumbnail(UIImage* SrcImg, const int Size)
: m_pSrcImage(SrcImg)
, m_SizeThumbnail(Size)
{
}

//----------------------------------------------------------------------------------------------
CImgThumbnail::~CImgThumbnail()
{
	Free();
}

//----------------------------------------------------------------------------------------------
bool CImgThumbnail::Initialize(HDC hdc)
{
	bool bResult = false;

	if (m_pSrcImage)
	{
		size_t ThumbnailWidth = m_SizeThumbnail;
		size_t ThumbnailHeight = m_SizeThumbnail;

		const byte BitsCount = 32;

		unsigned int SrcWidth = 0;
		unsigned int SrcHeight = 0;

		BYTE * pData = 0;
		if (m_pSrcImage->GetInfo(SrcWidth, SrcHeight) &&
			m_pSrcImage->GetDIB(&pData, true, ThumbnailWidth, ThumbnailHeight))
		{
			LONG lBmpSize = ThumbnailWidth * ThumbnailHeight * (BitsCount / 8);
			BITMAPINFO bmpInfo = {0};
			bmpInfo.bmiHeader.biBitCount = BitsCount;
			bmpInfo.bmiHeader.biHeight = ThumbnailHeight;
			bmpInfo.bmiHeader.biWidth = ThumbnailWidth;
			bmpInfo.bmiHeader.biPlanes = 1;
			bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

			// Pointer to access the pixels of bitmap
			UINT * pPixels = 0;
			m_hBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&
				bmpInfo, DIB_RGB_COLORS, (void**)&
				pPixels, NULL, 0);

			for (size_t y = 0; y < ThumbnailHeight; ++y)
			{
				for (size_t x = 0; x < ThumbnailWidth; ++x)
				{
					*((DWORD*)pPixels + (x + (((ThumbnailHeight - 1) - y) * ThumbnailWidth))) = *((DWORD*)pData + (x + (y * ThumbnailWidth)));
				}
			}
			delete [] pData;
			bResult = true;
		}
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
void CImgThumbnail::Free()
{
	DeleteObject(m_hBitmap);
}

//----------------------------------------------------------------------------------------------
void CImgThumbnail::Draw(HDC hdc, HDC MemDC, int PosX, int PosY)
{
	// Select the new bitmap
	SelectObject(MemDC, m_hBitmap);

	// Copy the bits from the memory DC into the current dc
	BitBlt(hdc, PosX, PosY, m_SizeThumbnail, m_SizeThumbnail ,MemDC, 0, 0, SRCCOPY);
}

//----------------------------------------------------------------------------------------------
Win32ImageList::Win32ImageList(HWND hWndParent, const class CWindowManager *pWndMgr)
: m_hwndParent(hWndParent)
, m_hwndCanvas(0)
, m_hwndButtonLeft(0)
, m_hwndButtonRight(0)
, m_hwndScroll(0)
, m_ViewOffsetX(0)
, m_ViewOffsetY(0)
, m_pWindowManager(pWndMgr)
, m_SizeThumbnail(64)
, m_BorderWidth(4)
, m_nSelectedElement(INDEX_NONE)
{
	RECT rWindowParent, rWindowCanvas;
	GetWindowRect(m_hwndParent, &rWindowParent);

	CLIENTCREATESTRUCT MDIClientCreateStruct; // Structure to be used for MDI client area

	MDIClientCreateStruct.hWindowMenu	= NULL;
	MDIClientCreateStruct.idFirstChild	= IDM_FIRSTCHILD;

	m_hwndCanvas = CreateWindowEx(
		WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES,
		TEXT("MDIClient"),
		NULL,  
		WS_CHILD | WS_VISIBLE | WS_TABSTOP, 
		0,
		0,
		rWindowParent.right - rWindowParent.left,
		rWindowParent.bottom - rWindowParent.top, 
		m_hwndParent,
		NULL,
		GetModuleHandle(NULL),
		(void*) &MDIClientCreateStruct);

	LONG lExStyle = GetWindowLong(m_hwndCanvas, GWL_EXSTYLE);
	lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	SetWindowLong(m_hwndCanvas, GWL_EXSTYLE, lExStyle);

	SetWindowPos(m_hwndCanvas, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

	GetWindowRect(m_hwndCanvas, &rWindowCanvas);

	int ScrollHeight = GetSystemMetrics(SM_CYHSCROLL);
	int iHThumb = GetSystemMetrics(SM_CXHTHUMB);

	m_hwndScroll = CreateWindow(
		TEXT("SCROLLBAR"), 
		(LPCWSTR)NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
		0,
		0,
		rWindowCanvas.right - rWindowCanvas.left,
		rWindowCanvas.bottom - rWindowCanvas.top,
		m_hwndCanvas,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	DragAcceptFiles(m_hwndCanvas, TRUE);

	SetWindowLong(m_hwndCanvas, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hwndCanvas, GWL_WNDPROC, (LONG)Win32ImageList::WndProcStatic);

	UpdateScrollbar();
}

//----------------------------------------------------------------------------------------------
Win32ImageList::~Win32ImageList()
{
	for_each(m_VecImgThumbnails.begin(), m_VecImgThumbnails.end(), DeleteVectorFntor());
	m_VecImgThumbnails.clear();
}

//----------------------------------------------------------------------------------------------
LRESULT Win32ImageList::WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	switch(messg)
	{
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);

			RECT rWindowParent;
			GetWindowRect(m_hwndParent, &rWindowParent);

			// fill background
			HBRUSH whiteBrush = CreateSolidBrush(RGB(0, 0, 0));
			RECT back_rct;
			back_rct.left = 0;
			back_rct.right = rWindowParent.right - rWindowParent.left;
			back_rct.top = 0;
			back_rct.bottom = rWindowParent.bottom - rWindowParent.top;
			::FillRect(hdc, &back_rct, whiteBrush);

			{
				RECT rect;
				GetClientRect(hWnd, &rect);

				HDC MemDC = CreateCompatibleDC(hdc);

				int xOffset = (int)((-m_ViewOffsetX / 100.f) * GetTotalWidth());

				int Index = 0;
				for (TVecImgThumbnailIter Iter = m_VecImgThumbnails.begin(); Iter != m_VecImgThumbnails.end(); ++Iter, ++Index)
				{
					if (xOffset + m_SizeThumbnail + m_BorderWidth < 0 || xOffset > rect.right){
						continue;
					}

					if (m_nSelectedElement == Index)
					{
						HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));

						RECT select_rct;
						select_rct.left = xOffset;
						select_rct.right = xOffset + m_SizeThumbnail + m_BorderWidth * 2;
						select_rct.top = 0;
						select_rct.bottom = m_SizeThumbnail + m_BorderWidth * 2;
						::FillRect(hdc, &select_rct, yellowBrush);
					}

					(*Iter)->Draw(hdc, MemDC, xOffset + m_BorderWidth, m_BorderWidth);

					xOffset += m_SizeThumbnail + (m_BorderWidth * 2);
				}

				// Restore the old bitmap
				DeleteDC(MemDC);
			}
			EndPaint(hWnd, &ps);
			return 0;
		}break;

	case WM_DROPFILES:
		{
		/*	TCHAR lpszFile[MAX_PATH] = {0};
			UINT uFile = 0;
			HDROP hDrop = (HDROP)wParam;

			uFile = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL);
			if (uFile != 1)
			{
				MessageBox (hWnd, _T("Dropping multiple files is not supported."), NULL, MB_ICONERROR);
				DragFinish(hDrop);
				break;
			}

			lpszFile[0] = '\0';
			if (!DragQueryFile(hDrop, 0, lpszFile, MAX_PATH))
			{
				MessageBox(NULL, _T("Dropping file failed."), NULL, MB_ICONINFORMATION);
				DragFinish(hDrop);
				break;
			}

			// generate projection
			UIImage * pImage = static_cast<UIImage*>(GENERATE_OBJECT_SDK("UIImage", "UIImage", m_pWindowManager->GetCoreSDK()->GetExplorerInstance()->GetExplorer2D()));

			m_VecImgThumbnails.push_back(new CImgThumbnail(pImage, m_SizeThumbnail));
			
			pImage->LoadImage(lpszFile, this);

			m_nSelectedElement = m_VecImgThumbnails.size() - 1; // select last

//			CImageProjection * NewObject = static_cast<CImageProjection*>(
//				GENERATE_OBJECT_SDK("CImageProjection", "ImageProjection", m_pWindowManager->GetCoreSDK()->GetExplorerInstance()->GetExplorer2D()));

			DragFinish(hDrop);*/
		}break;

	case WM_HSCROLL:
		{
			int xCurrentScroll = m_ViewOffsetX;
			int xDelta = 0;     // xDelta = new_pos - current_pos  
			int xNewPos = 0;    // new position 

			RECT rectCanvas;
			GetClientRect(m_hwndCanvas, &rectCanvas);

			int Width = rectCanvas.right - rectCanvas.left;
			int TWidth = GetTotalWidth();
			int ScrollPct = 100 - (int)floor(((float)Width * 100.f) / (float)TWidth);

			switch (LOWORD(wParam)) 
			{ 
				// User clicked the scroll bar shaft left of the scroll box. 
			case SB_PAGEUP: 
				xNewPos = xCurrentScroll - 10; 
				break; 

				// User clicked the scroll bar shaft right of the scroll box. 
			case SB_PAGEDOWN: 
				xNewPos = xCurrentScroll + 10; 
				break; 

				// User clicked the left arrow. 
			case SB_LINEUP: 
				xNewPos = xCurrentScroll - 5; 
				break; 

				// User clicked the right arrow. 
			case SB_LINEDOWN: 
				xNewPos = xCurrentScroll + 5; 
				break; 

				// User dragged the scroll box. 
			case SB_THUMBPOSITION:
			case  SB_THUMBTRACK:
				xNewPos = HIWORD(wParam); 
				break; 

			default: 
				xNewPos = xCurrentScroll;
				break;
			} 

			// New position must be between 0 and the screen width. 
			xNewPos = (xNewPos < 0) ? 0 : (xNewPos > ScrollPct) ? ScrollPct : xNewPos;

			// If the current position does not change, do not scroll.
			if (xNewPos == xCurrentScroll) 
				break;

			// Determine the amount scrolled (in pixels). 
			xDelta = xNewPos - xCurrentScroll;

			// Reset the current scroll position. 
			m_ViewOffsetX = xNewPos;

			SCROLLINFO si;
			// Reset the scroll bar. 
			si.cbSize = sizeof(si); 
			si.fMask  = SIF_POS; 
			si.nPos   = xNewPos; 
			SetScrollInfo(m_hwndScroll, SB_CTL, &si, TRUE);

			RedrawWindow();
		}break;

	case WM_LBUTTONUP:
		{
			int xPos = (int)LOWORD(lParam);
			int yPos = (int)HIWORD(lParam);

			RECT rect;
			GetClientRect(hWnd, &rect);
			int xOffset = (int)((-m_ViewOffsetX / 100.f) * GetTotalWidth());

			int Index = 0;
			for (TVecImgThumbnailIter Iter = m_VecImgThumbnails.begin(); Iter != m_VecImgThumbnails.end(); ++Iter, ++Index)
			{
				if (xOffset + m_SizeThumbnail + m_BorderWidth < 0 || xOffset > rect.right){
					continue;
				}

				if ((xPos >= xOffset && xPos <= (int)(xOffset + m_SizeThumbnail + m_BorderWidth * 2)) &&
					(yPos >= 0 && yPos <= (int)(m_SizeThumbnail + m_BorderWidth * 2)))
				{
					if (m_nSelectedElement != Index)
					{
						m_nSelectedElement = Index;
						RedrawWindow();
					}
					break;
				}
				xOffset += m_SizeThumbnail + (m_BorderWidth * 2);
			}
		}break;

	case WM_RBUTTONUP: // context menu
		{
			//@TODO
		}break;

	case WM_USER_UPDATE_SCROLL:
		UpdateScrollbar();
		break;
	}
	return DefWindowProc(hWnd, messg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
void Win32ImageList::OnOperationComplete(CObjectAbstract* pObject)
{
/*	for (TVecImgThumbnailIter Iter = m_VecImgThumbnails.begin(); Iter != m_VecImgThumbnails.end(); ++Iter)
	{
		if ((*Iter)->GetSrcImage() == pObject)
		{
			m_pWindowManager->GetCoreSDK()->GetRenderSDK()->EnterCS();

			HDC hdc = GetDC(m_pWindowManager->GetWindowManagerHWND());
			(*Iter)->Initialize(hdc);
			
			m_pWindowManager->GetCoreSDK()->GetRenderSDK()->LeaveCS();

			PostMessage(m_hwndCanvas, WM_USER_UPDATE_SCROLL, 0, 0);

			RedrawWindow();

			break;
		}
	}*/
}

//----------------------------------------------------------------------------------------------
void Win32ImageList::OnOperationFailed(CObjectAbstract* pObject)
{
	for (TVecImgThumbnailIter Iter = m_VecImgThumbnails.begin(); Iter != m_VecImgThumbnails.end(); ++Iter)
	{
		if ((*Iter)->GetSrcImage() == pObject)
		{
			m_VecImgThumbnails.erase(Iter);

			PostMessage(m_hwndCanvas, WM_USER_UPDATE_SCROLL, 0, 0);

			RedrawWindow();

			break;
		}
	}
}

//----------------------------------------------------------------------------------------------
void Win32ImageList::OnResize()
{
	RECT rect;
	GetClientRect(m_hwndParent, &rect);

	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;

	/* Adjust the children's size and position */
	MoveWindow(m_hwndCanvas, rect.left, rect.top, Width, Height, TRUE);

	int ScrollHeight = GetSystemMetrics(SM_CYHSCROLL);

	RECT rectCanvas;
	GetClientRect(m_hwndCanvas, &rectCanvas);
	MoveWindow(m_hwndScroll, 0, Height - ScrollHeight, Width, ScrollHeight, TRUE);

	UpdateScrollbar();
}

//----------------------------------------------------------------------------------------------
void Win32ImageList::RedrawWindow()
{
	InvalidateRect(m_hwndCanvas, NULL, TRUE);
}

//----------------------------------------------------------------------------------------------
LRESULT CALLBACK Win32ImageList::WndProcStatic(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	Win32ImageList * pImageList = reinterpret_cast<Win32ImageList*>(GetWindowLong(hWnd, GWL_USERDATA));
	if (pImageList){
		return pImageList->WndProc(hWnd, messg, wParam, lParam);
	}
	return DefWindowProc(hWnd, messg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
unsigned int Win32ImageList::GetTotalWidth() const
{
	return (m_SizeThumbnail + (m_BorderWidth * 2)) * m_VecImgThumbnails.size();
}

//----------------------------------------------------------------------------------------------
void Win32ImageList::UpdateScrollbar()
{
	RECT rWindowCanvas;
	GetWindowRect(m_hwndCanvas, &rWindowCanvas);

	unsigned int Width = rWindowCanvas.right - rWindowCanvas.left;
	unsigned int TotalWidth = GetTotalWidth();

	bool bShow = TotalWidth > Width;
	ShowScrollBar(m_hwndScroll, SB_CTL, bShow);

	if (bShow)
	{
 		SCROLLINFO srlInfo;
 		srlInfo.cbSize = sizeof(SCROLLINFO);
 		srlInfo.fMask = SIF_ALL;
 		srlInfo.nMin = 0;
 		srlInfo.nMax = 100;
 		srlInfo.nPos = 0;
 		srlInfo.nPage = (int)ceil((((float)Width * srlInfo.nMax) / TotalWidth));
 
 		SetScrollInfo(m_hwndScroll, SB_CTL, &srlInfo, true);
	}
}