#include "splash_window.h"

namespace oes
{
    namespace nmLauncher
    {
        SplashWindow::SplashWindow()
            : m_hWndSplash(nullptr)
            , m_hSplashBitmap(nullptr)
            , m_hSplashLogo(nullptr)
            , m_SplWidth(0)
            , m_SplHeight(0)
            , m_LogoWidth(0)
            , m_LogoHeight(0)
        {
            wcscpy(m_szAppSplashName, TEXT("Splash"));
        }

        //----------------------------------------------------------------------------------------------
        SplashWindow::~SplashWindow()
        {

        }

        //----------------------------------------------------------------------------------------------
        int SplashWindow::InitclassexSplash()
        {
            m_wcAppSplash.cbSize = sizeof(m_wcAppSplash);
            m_wcAppSplash.style = CS_OWNDC | CS_BYTEALIGNWINDOW | CS_BYTEALIGNCLIENT;
            m_wcAppSplash.lpfnWndProc = SplashWindow::SplashWndProcMain;
            m_wcAppSplash.cbClsExtra = 0;
            m_wcAppSplash.cbWndExtra = 0;
            m_wcAppSplash.hInstance = GetModuleHandle(NULL);
            m_wcAppSplash.hIcon = NULL;
            m_wcAppSplash.hCursor = LoadCursor(0, IDC_ARROW);
            m_wcAppSplash.hbrBackground = NULL;
            m_wcAppSplash.lpszMenuName = NULL;
            m_wcAppSplash.lpszClassName = m_szAppSplashName;
            m_wcAppSplash.hIconSm = NULL;

            if (!RegisterClassEx(&m_wcAppSplash)) {
                return 0;
            }
            return 1;
        }

        //----------------------------------------------------------------------------------------------
        int SplashWindow::CreateSplashWindow()
        {
            int res = InitclassexSplash();

            if (res)
            {
                m_hSplashBitmap = (HBITMAP)::LoadImage(NULL, L"resources/SplashScreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

                if (m_hSplashBitmap)
                {
                    BITMAP bitmap;
                    ::GetObject(m_hSplashBitmap, sizeof(BITMAP), &bitmap);

                    m_SplWidth = bitmap.bmWidth;
                    m_SplHeight = bitmap.bmHeight;
                }

                m_hSplashLogo = (HBITMAP)::LoadImage(NULL, L"resources/Logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

                if (m_hSplashLogo)
                {
                    BITMAP bitmap;
                    ::GetObject(m_hSplashLogo, sizeof(BITMAP), &bitmap);

                    m_LogoWidth = bitmap.bmWidth;
                    m_LogoHeight = bitmap.bmHeight;
                }

                int width = GetSystemMetrics(SM_CXFULLSCREEN);
                int height = GetSystemMetrics(SM_CYFULLSCREEN);

                width = width / 2 - m_SplWidth / 2;
                height = height / 2 - m_SplHeight / 2;

                m_hWndSplash = CreateWindow(m_szAppSplashName,
                    APP_NAME,
                    WS_POPUPWINDOW,
                    width, height,
                    m_SplWidth, m_SplHeight,
                    NULL, NULL,
                    GetModuleHandle(NULL),
                    NULL);

                if (!m_hWndSplash) {
                    return 0;
                }

                ::SetWindowLong(m_hWndSplash, GWL_USERDATA, (LONG)this);

                ShowSplash();
            }
            return res;
        }

        //----------------------------------------------------------------------------------------------
        void SplashWindow::ShowSplash()
        {
            ::ShowWindow(m_hWndSplash, SW_SHOWNORMAL);
            ::UpdateWindow(m_hWndSplash);
            Sleep(500); // :)
        }

        //----------------------------------------------------------------------------------------------
        void SplashWindow::CloseSplash()
        {
            ::DestroyWindow(m_hWndSplash);
            m_hWndSplash = NULL;
        }

        //----------------------------------------------------------------------------------------------
        void SplashWindow::OutWindowMessageLog(char* str)
        {
            if (m_hWndSplash != NULL)
            {
                m_SplashScreenMessage = str;
                ::SendMessage(m_hWndSplash, WM_PAINT, 0, 0);
            }
        }

        //----------------------------------------------------------------------------------------------
        LRESULT SplashWindow::SplashWndProcMain(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
        {
            switch (messg)
            {
            case WM_PAINT:
            {
                HDC	  hdc;
                HFONT hCustomFont;
                HFONT hCustomFont2;

                hdc = ::GetDC(hWnd);

                //hdc = BeginPaint(hWnd, &PaintStruct);

                hCustomFont = CreateFont(
                    16, 7,
                    0, 0,
                    500, 0,
                    0, 0,
                    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH,
                    L"Verdana");

                hCustomFont2 = CreateFont(
                    12, 5,
                    0, 0,
                    500, 0,
                    0, 0,
                    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH,
                    L"Verdana");

                SplashWindow * wnd = reinterpret_cast<SplashWindow*>(::GetWindowLong(hWnd, GWL_USERDATA));

                if (hdc)
                {
                    HDC     hMemDC;
                    HBITMAP hOldBitmap;

                    hMemDC = CreateCompatibleDC(hdc);

                    hOldBitmap = (HBITMAP)::SelectObject(hMemDC, wnd->GetSplashBitmap());
                    BitBlt(hdc, 0, 0, wnd->GetSplashWidth(), wnd->GetSplashHeight(), hMemDC, 0, 0, SRCCOPY);

                    hOldBitmap = (HBITMAP)::SelectObject(hMemDC, wnd->GetSplashLogo());
                    BitBlt(hdc, wnd->GetSplashWidth() - wnd->GetLogoWidth() - 15, wnd->GetSplashHeight() - wnd->GetLogoHeight() * 2,
                        wnd->GetLogoWidth(), wnd->GetLogoHeight(), hMemDC, 0, 0, SRCCOPY);

                    SelectObject(hMemDC, hOldBitmap);
                    DeleteDC(hMemDC);
                }

                SelectObject(hdc, hCustomFont);
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkMode(hdc, TRANSPARENT);

                std::wstring str = ConvertStringToWideString(STR_COPYRIGHT);
                ::TextOut(hdc, (int)(wnd->GetSplashWidth() - 160), (int)(wnd->GetSplashHeight() - 20), str.c_str(), (int)wcslen(str.c_str()));

                str = ConvertStringToWideString(STR_SOURCE_COPYRIGHT);
                ::TextOut(hdc, (int)(wnd->GetSplashWidth() - 234), (int)(wnd->GetSplashHeight() - 45), str.c_str(), (int)wcslen(str.c_str()));

                SelectObject(hdc, hCustomFont2);
                std::wstring strlog = ConvertStringToWideString(wnd->m_SplashScreenMessage);
                ::TextOut(hdc, 10, (int)(wnd->GetSplashHeight() - 35), strlog.c_str(), (int)wcslen(strlog.c_str()));

                DeleteObject(hCustomFont);
                DeleteObject(hCustomFont2);

                ::ReleaseDC(hWnd, hdc);

                //EndPaint(hWnd, &PaintStruct);
            }break;
            }
            return ::DefWindowProc(hWnd, messg, wParam, lParam);
        }
    }
}