#ifndef __windowmanager_h__
#define __windowmanager_h__

#ifdef _MSC_VER
#pragma warning( disable : 4996 ) // _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef _WIN32
#pragma once
#endif

#include "..\..\Foundation\StdafxFoundation.h"

class EXPORT CWindowManager
{
public:
	CWindowManager(class CCoreSDK *pCoreSDK);
	~CWindowManager();
	
	class CCoreSDK* GetCoreSDK() const { return m_pAppMain; }
	
	bool Initialize();

	/** initialization */
	void	Init(); 
	void	InitSplash();

	/** windows class register */
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

	LRESULT WndProcMain(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

	HBITMAP		 GetSplashBitmap() const { return m_hSplashBitmap; }
	unsigned int GetSplashWidth()  const { return m_SplWidth; }
	unsigned int GetSplashHeight() const { return m_SplHeight; }

	HBITMAP		 GetSplashLogo() const { return m_hSplashLogo; }
	unsigned int GetLogoWidth()  const { return m_LogoWidth; }
	unsigned int GetLogoHeight() const { return m_LogoHeight; }

	void UpdateFlagsState() const;
	void OutWindowMessageLog(std::string &message);

	void QuitApplication();

	void OnDrawSplitter(HWND hWnd, int xPos, int yPos);

protected:
	void GetHitChilds(class CActor* InActor, Matrix& InMatrix, Vector2f& Point, std::vector<class CActor*> &OutVector);


public:
	class CCoreSDK *m_pAppMain;

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

	bool ShiftPressed;
	bool CtrlPressed;

	HBITMAP m_hSplashBitmap;
	HBITMAP m_hSplashLogo;
};

#endif//__windowmanager_h__
