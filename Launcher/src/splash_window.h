#pragma once

#include "../LauncherStdAfx.h"

namespace oes
{
    namespace nmLauncher
    {
        class SplashWindow
        {

        public:
            SplashWindow();
            ~SplashWindow();

            int InitclassexSplash();

            int CreateSplashWindow();

            void ShowSplash();

            void CloseSplash();

            void OutWindowMessageLog(char* str);

            static LRESULT CALLBACK SplashWndProcMain(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

            HBITMAP		 GetSplashBitmap() const { return m_hSplashBitmap; }
            unsigned int GetSplashWidth()  const { return m_SplWidth; }
            unsigned int GetSplashHeight() const { return m_SplHeight; }

            HBITMAP		 GetSplashLogo() const { return m_hSplashLogo; }
            unsigned int GetLogoWidth()  const { return m_LogoWidth; }
            unsigned int GetLogoHeight() const { return m_LogoHeight; }

        private:
            WNDCLASSEX  m_wcAppSplash;
            WCHAR       m_szAppSplashName[255];
            HWND		m_hWndSplash;

            HBITMAP		m_hSplashBitmap;
            HBITMAP		m_hSplashLogo;

            unsigned int m_SplWidth;
            unsigned int m_SplHeight;

            unsigned int m_LogoWidth;
            unsigned int m_LogoHeight;

            std::string	 m_SplashScreenMessage;
        };
    }
}