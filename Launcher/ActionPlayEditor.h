#ifndef __actionplayeditor_h__
#define __actionplayeditor_h__

#ifdef WIN32
#pragma once
#endif
/*
#include "..\..\Foundation\StdafxFoundation.h"

class CWin32ActionPlayEditor
{
public:
	struct PropertyAdapter
	{
		class CActor* Actor;
		int PropertyIndex;

		PropertyAdapter(CActor * actor, int Index)
			: Actor(actor)
			, PropertyIndex(Index)
		{	
		}
	};

	typedef std::map<int, PropertyAdapter>	MapObjectProperty;
	typedef MapObjectProperty::iterator		ObjectPropertyIter;

	typedef void (*ContextMenuFunction) (HWND hwnd, POINT pt, class CActor * Actor);
	typedef bool (*ContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef int  (*GetResourceIconIndex)(const std::string & TypeName);

	static void Initialize(HWND hWndParent,
							ContextMenuFunction pfMenu,
							ContextMenuProcessor pfnMenuProcessor,
							GetResourceIconIndex pfnGetResourceIconIndex,
							HIMAGELIST hImageList,
							const class CWindowManager *WMgr);

	static void ShowDialogWindow(bool bShow = true);
	static bool IsVisible();

	static void Update(const class CActor * Sender, ESystemEventID EventId);

	static void HandlePopupContextMenu(HWND hwnd, POINT pt);
	static bool	ContextMenuProcessorAction(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

	static void HandleLinkContextMenu(HWND hwnd, POINT pt);
	static bool	ContextMenuProcessorLink(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);


	static bool AddActorProperties(class CActor * Actor);
	static bool FillPropertyTabs();
	
	static void Release();
protected:
private:

};
*/
#endif//__actionplayeditor_h__