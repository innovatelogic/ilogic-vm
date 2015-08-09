#ifndef __windowmanager_h__
#define __windowmanager_h__

#ifdef _MSC_VER
#pragma warning( disable : 4996 ) // _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef _WIN32
#pragma once
#endif

#include "..\..\Foundation\StdafxFoundation.h"

enum ESizeDirection
{
	SZ_Left = 0,
	SZ_Right,
	SZ_Top,
	SZ_TopLeft,
	SZ_TopRight,
	SZ_Bottom,
	SZ_BottomLeft,
	SZ_BottomRight,
};

enum ECursorDir
{
	CD_EW = 0,
	CD_NS,
	CD_NWSE,
	CD_NESW,
	CD_MAX
};
/*
class EXPORT CWindowManager
{
	typedef void (*ContextMenuFunction)(HWND hwnd, POINT pt, class CActor *pActor);
	typedef bool (*ContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef HICON (*GetOperationBitmap)(const std::string &StrType);

public:
	CWindowManager(class CCoreSDK * appMain);
	~CWindowManager();
	
	class CCoreSDK* GetCoreSDK() const { return m_pAppMain; }
	
	bool Initialize(ContextMenuFunction pfMenu, ContextMenuProcessor pfnMenuProcessor, GetOperationBitmap pfnOperationBitmap);

	void	Init(); 
	void	InitSplash();
	int		Initclassex(); 
	int		InitclassexSplash();

	int		Create();  
	int		CreateSplashWindow(); 
	bool	PeekMessageWindow();

	void	Show();
	void	ShowSplash();
	void	CloseSplash();

	void	Destroy();

	HWND	GetWindowManagerHWND() const { return m_hWnd; }

	void	SetWindowRes(int Width, int Height) { m_displayModeWidth = Width; m_displayModeHeight = Height;  }
	int		GetWindowWidth() const  { return m_displayModeWidth; }
	int		GetWindowHeight() const { return m_displayModeHeight; }

	HBITMAP		 GetSplashBitmap() const { return m_hSplashBitmap; }
	unsigned int GetSplashWidth()  const { return m_SplWidth; }
	unsigned int GetSplashHeight() const { return m_SplHeight; }

	HBITMAP		 GetSplashLogo() const { return m_hSplashLogo; }
	unsigned int GetLogoWidth()  const { return m_LogoWidth; }
	unsigned int GetLogoHeight() const { return m_LogoHeight; }

	void UpdateFlagsState() const;
	void OutWindowMessageLog(std::string& message);

	void QuitApplication();

	void Resize(LPRECT NewRect, ESizeDirection Dir);

	void OnNCLBClick(HWND hWnd, int xPos, int yPos);
	void OnWndLBRelease(HWND hWnd, int xPos, int yPos);

	void OnWndMaximized(int width, int height);
	void OnDrawSplitter(HWND hWnd, int xPos, int yPos);

	void SetCursorDir(ECursorDir Dir);
	void ResizeFinish(HWND hWnd, int xPos, int yPos);

	bool			GetSizing() const { return m_bSizing; }
	ESizeDirection	GetSizingDir() const { return m_ESizeDirection; }
	HWND			GetHWNDCaptured() const { return m_hWndCaptured; }

	class Win32ImageList* GetImageList() const { return m_pImageList; }

	class CWindowPackageExplorer* GetPackageExplorerWindow() const { return m_pWndPackageExplorer; }

protected:
	void GetHitChilds(class CActor* InActor, Matrix& InMatrix, Vector2f& Point, std::vector<class CActor*> &OutVector);
	
	void ResizeFinishCentral(int xPos, int yPos,
							 LONG WindowCenterWidth,
							 LONG WindowCenterHeight,
							 LONG WindowBottomHeight,
							 LONG WindowRightWidth,
							 LONG WindowLeftWidth,
							 LONG WindowBottomWidth,
							 LONG WindowBottomTopWidth,
							 LONG WindowBottomTopHeight,
							 LONG WindowBottomBottomWidth,
							 LONG WindowBottomBottomHeight,
							 LONG WindowRightTopWidth,
							 LONG WindowRightTopHeight,
							 LONG WindowRightBottomWidth,
							 LONG WindowRightBottomHeight,
							 RECT &rWindowMain,
							 RECT &rWindowMainCl,
							 RECT &rWindowTop,
							 RECT &rWindowLeft,
							 RECT &rWindowRight,
							 RECT &rWindowCenter,
							 RECT &rWindowBottom,
							 RECT &rWindowBottomTop,
							 RECT &rWindowBottomBottom,
							 RECT &rWindowRightTop,
							 RECT &rWindowRightBottom);

	void ResizeFinishLeft(int xPos, int yPos,
							LONG WindowCenterWidth,
							LONG WindowCenterHeight,
							LONG WindowBottomHeight,
							LONG WindowRightWidth,
							LONG WindowLeftWidth,
							LONG WindowBottomWidth,
							LONG WindowBottomTopWidth,
							LONG WindowBottomTopHeight,
							LONG WindowBottomBottomWidth,
							LONG WindowBottomBottomHeight,
							LONG WindowRightTopWidth,
							LONG WindowRightTopHeight,
							LONG WindowRightBottomWidth,
							LONG WindowRightBottomHeight,
							RECT &rWindowMain,
							RECT &rWindowMainCl,
							RECT &rWindowTop,
							RECT &rWindowLeft,
							RECT &rWindowRight,
							RECT &rWindowCenter,
							RECT &rWindowBottom,
							RECT &rWindowBottomTop,
							RECT &rWindowBottomBottom,
							RECT &rWindowRightTop,
							RECT &rWindowRightBottom);

	void ResizeFinishBottom(int xPos, int yPos,
							LONG WindowCenterWidth,
							LONG WindowCenterHeight,
							LONG WindowBottomHeight,
							LONG WindowRightWidth,
							LONG WindowLeftWidth,
							LONG WindowBottomWidth,
							LONG WindowBottomTopWidth,
							LONG WindowBottomTopHeight,
							LONG WindowBottomBottomWidth,
							LONG WindowBottomBottomHeight,
							LONG WindowRightTopWidth,
							LONG WindowRightTopHeight,
							LONG WindowRightBottomWidth,
							LONG WindowRightBottomHeight,
							RECT &rWindowMain,
							RECT &rWindowMainCl,
							RECT &rWindowTop,
							RECT &rWindowLeft,
							RECT &rWindowRight,
							RECT &rWindowCenter,
							RECT &rWindowBottom,
							RECT &rWindowBottomTop,
							RECT &rWindowBottomBottom,
							RECT &rWindowRightTop,
							RECT &rWindowRightBottom);

	void ResizeFinishBottomTop(int xPos, int yPos,
								LONG WindowCenterWidth,
								LONG WindowCenterHeight,
								LONG WindowBottomHeight,
								LONG WindowRightWidth,
								LONG WindowLeftWidth,
								LONG WindowBottomWidth,
								LONG WindowBottomTopWidth,
								LONG WindowBottomTopHeight,
								LONG WindowBottomBottomWidth,
								LONG WindowBottomBottomHeight,
								LONG WindowRightTopWidth,
								LONG WindowRightTopHeight,
								LONG WindowRightBottomWidth,
								LONG WindowRightBottomHeight,
								RECT &rWindowMain,
								RECT &rWindowMainCl,
								RECT &rWindowTop,
								RECT &rWindowLeft,
								RECT &rWindowRight,
								RECT &rWindowCenter,
								RECT &rWindowBottom,
								RECT &rWindowBottomTop,
								RECT &rWindowBottomBottom,
								RECT &rWindowRightTop,
								RECT &rWindowRightBottom);

	void ResizeFinishBottomBottom(int xPos, int yPos,
								  LONG WindowCenterWidth,
								  LONG WindowCenterHeight,
								  LONG WindowBottomHeight,
								  LONG WindowRightWidth,
								  LONG WindowLeftWidth,
								  LONG WindowBottomWidth,
								  LONG WindowBottomTopWidth,
								  LONG WindowBottomTopHeight,
								  LONG WindowBottomBottomWidth,
								  LONG WindowBottomBottomHeight,
								  LONG WindowRightTopWidth,
								  LONG WindowRightTopHeight,
								  LONG WindowRightBottomWidth,
								  LONG WindowRightBottomHeight,
								  RECT &rWindowMain,
								  RECT &rWindowMainCl,
								  RECT &rWindowTop,
								  RECT &rWindowLeft,
								  RECT &rWindowRight,
								  RECT &rWindowCenter,
								  RECT &rWindowBottom,
								  RECT &rWindowBottomTop,
								  RECT &rWindowBottomBottom,
								  RECT &rWindowRightTop,
								  RECT &rWindowRightBottom);

	void ResizeFinishRightTop(int xPos, int yPos,
							  LONG WindowCenterWidth,
							  LONG WindowCenterHeight,
							  LONG WindowBottomHeight,
							  LONG WindowRightWidth,
							  LONG WindowLeftWidth,
							  LONG WindowBottomWidth,
							  LONG WindowBottomTopWidth,
							  LONG WindowBottomTopHeight,
							  LONG WindowBottomBottomWidth,
							  LONG WindowBottomBottomHeight,
							  LONG WindowRightTopWidth,
							  LONG WindowRightTopHeight,
							  LONG WindowRightBottomWidth,
							  LONG WindowRightBottomHeight,
							  RECT &rWindowMain,
							  RECT &rWindowMainCl,
							  RECT &rWindowTop,
							  RECT &rWindowLeft,
							  RECT &rWindowRight,
							  RECT &rWindowCenter,
							  RECT &rWindowBottom,
							  RECT &rWindowBottomTop,
							  RECT &rWindowBottomBottom,
							  RECT &rWindowRightTop,
							  RECT &rWindowRightBottom);

	void ResizeFinishRightBottom(int xPos, int yPos,
								 LONG WindowCenterWidth,
								 LONG WindowCenterHeight,
								 LONG WindowBottomHeight,
								 LONG WindowRightWidth,
								 LONG WindowLeftWidth,
								 LONG WindowBottomWidth,
								 LONG WindowBottomTopWidth,
								 LONG WindowBottomTopHeight,
								 LONG WindowBottomBottomWidth,
								 LONG WindowBottomBottomHeight,
								 LONG WindowRightTopWidth,
								 LONG WindowRightTopHeight,
								 LONG WindowRightBottomWidth,
								 LONG WindowRightBottomHeight,
								 RECT &rWindowMain,
								 RECT &rWindowMainCl,
								 RECT &rWindowTop,
								 RECT &rWindowLeft,
								 RECT &rWindowRight,
								 RECT &rWindowCenter,
								 RECT &rWindowBottom,
								 RECT &rWindowBottomTop,
								 RECT &rWindowBottomBottom,
								 RECT &rWindowRightTop,
								 RECT &rWindowRightBottom);

private:
	void AdjustRightFrameHeight(LONG Delta, 
								LONG WindowRightTopHeight,
								LONG WindowRightBottomHeight,
								RECT &rWindowRightTop,
								RECT &rWindowRightBottom);

public:
	class CCoreSDK * m_pAppMain;

	WCHAR      m_szAppName[255];
	WCHAR      m_szAppSplashName[255];

	HWND		m_hWnd;

	HWND		m_hWndSplash;
	MSG			m_Msg;

	WNDCLASSEX m_wcApp;
	WNDCLASSEX m_wcAppBottom;
	WNDCLASSEX m_wcAppSplash;

	int m_displayModeHeight;
	int m_displayModeWidth;

	unsigned int m_SplWidth;
	unsigned int m_SplHeight;

	unsigned int m_LogoWidth;
	unsigned int m_LogoHeight;

	unsigned int MousePosPrevX;
	unsigned int MousePosPrevY;

	std::string OpenFileName;

	HBITMAP m_hSplashBitmap;
	HBITMAP m_hSplashLogo;

	HCURSOR	m_hcSizeEW;
	HCURSOR	m_hcSizeNS;
	HCURSOR	m_hcSizeNWSE;
	HCURSOR	m_hcSizeNESW;

	// sizing
//public:
	bool	m_bSizing;
	ESizeDirection m_ESizeDirection;
	HWND	m_hWndCaptured;
	int		m_nBorderOffsetX;
	int		m_nBorderOffsetY;

public:
	ContextMenuFunction		pfnContextMenu;
	ContextMenuProcessor	pfnContextMenuProcessor;
	GetOperationBitmap		pfnGetOperationBitmap;

	class Win32ImageList *m_pImageList;

	class IWindowLayout *m_pCtrlWindow;
	class IWindowLayout *m_pCtrlWindowTop;
	class IWindowLayout *m_pCtrlWindowBootom;
	class IWindowLayout *m_pCtrlWindowBootomTop;
	class IWindowLayout *m_pCtrlWindowBootomBottom;
	class IWindowLayout *m_pCtrlWindowLeft;
	class IWindowLayout *m_pCtrlWindowRight;
	class IWindowLayout *m_pCtrlWindowRightTop;
	class CWindowChildLayoutRightBottom *m_pCtrlWindowRightBottom;

	class CWindowPackageExplorer *m_pWndPackageExplorer;

	bool m_bShiftPressed;
	bool m_bCtrlPressed;
};
*/
#endif//__windowmanager_h__
