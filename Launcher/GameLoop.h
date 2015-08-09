#ifndef __gameloop_h__
#define __gameloop_h__

#pragma once

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
class CGameLoop : public CMessageLoop
{
	typedef void (*pfnUpdate)(void);

public:
	CGameLoop(pfnUpdate pfn)
		: m_pfnCallback(pfn)
	{
	}

	virtual int Run()
	{
		bool	isActive = true;

		while (TRUE)
		{
			if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
			{
				//C: Check for the WM_QUIT message to exit the loop.
				if (WM_QUIT == m_msg.message)
				{
					ATLTRACE2(atlTraceUI, 0, _T("CGameLoop::Run - exiting\n"));
					break;		// WM_QUIT, exit message loop
				}
				//C: Flag the loop as active only if an active message
				//   is processed.
				if (IsIdleMessage(&m_msg))
				{
					isActive = true;
				}
				//C: Attmpt to translate and dispatch the messages.
				if (!PreTranslateMessage(&m_msg))
				{
					::TranslateMessage(&m_msg);
					::DispatchMessage(&m_msg);
				}
			}
			else if (isActive)
			{
				//C: Perform idle message processing.
				OnIdle(0);
				//C: Flag the loop as inactive.  This will prevent other idle messages
				//   from being processed while the message queue is empty.
				isActive = false;
			}
			else
			{
				//C: Is the game paused.
				// 				if (m_gameHandler->IsPaused())
				// 				{
				// 				//C: To keep the program from spinning needlessly, wait until the next
				// 				//   message enters the queue before processing any more data.
				// 					WaitMessage();
				// 				}
				//				else
				{
					//UpdateFrame();
					m_pfnCallback();
				}
			}
		}
		//C: Returns the exit code for the loop.
		return (int) m_msg.wParam;
	}

	pfnUpdate m_pfnCallback;
};


#endif//__gameloop_h__