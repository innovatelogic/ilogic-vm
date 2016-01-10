#ifndef __win32propertygrid_h__
#define __win32propertygrid_h__

#ifdef WIN32
#pragma once
#endif 

#include "..\..\Foundation\StdafxFoundation.h"
/*
class Win32PropertyGrid
{
public:
	enum EGridView
	{
		EGV_Categorized = 0,
		EGV_SortByName,
	};

private:
	struct PropertyGroup
	{
		std::string GroupName;
		TVecPropertyClass VecPropertyClasses;
		
		PropertyGroup(const std::string& Name)
			: GroupName(Name)
		{
		}
	};

	typedef std::vector<struct PropertyGroup*>	TVecPropertyGroup;
	typedef TVecPropertyGroup::const_iterator	TVecPropertyGroupConstIter;
	typedef TVecPropertyGroup::iterator			TVecPropertyGroupIter;	

public:
	// for sorted arrays
	struct SPropertyWrapper
	{
		Property_Base	*pProp;
		SPropertyClass	*pClass;
		bool bDisclosed;

		SPropertyWrapper(Property_Base* prop, SPropertyClass* pclass)
			: pProp(prop)
			, pClass(pclass)
			, bDisclosed(false)
		{
		}

		static bool	CompByName(const SPropertyWrapper *A1, const SPropertyWrapper *A2)
		{
			std::string A1_low(A1->pProp->GetName()); 
			std::string A2_low(A2->pProp->GetName()); 

			std::transform(A1_low.begin(), A1_low.end(), A1_low.begin(), ::tolower);
			std::transform(A2_low.begin(), A2_low.end(), A2_low.begin(), ::tolower);

			if (A1_low.compare(A2_low) < 0)
				return true;
			return false;
		}
	};

	typedef std::vector<struct SPropertyWrapper*> TVecPropertyWrapper;
	typedef TVecPropertyWrapper::const_iterator	 TVecPropertyWrapperConstIter;
	typedef TVecPropertyWrapper::iterator		 TVecPropertyWrapperIter;	

public:
	Win32PropertyGrid(HWND hWndParent, const class CWindowManager *pWMgr);
	~Win32PropertyGrid();

	class CActor* GetSelected() const { return m_pSelectedActor; }
	void		  SetSelected(class CActor* pActor) { m_pSelectedActor = pActor; }

	void LoadHelperImageList();

	bool AddTabPage(std::string &Value, int Index);
	bool ClearTabPages();

	void FillPropertyData(class CActor *pActor);
	void FillPropertyDataTransient(class CActor *pActor);
	void UpdatePreview();
	
	HWND GetHWNDParent() const { return m_hwndParent; }	
	HWND GetHWNDChildTop() { return m_hWndTop; }
	HWND GetHWNDChildBottom() { return m_hWndBottom; }
	HWND GetHWNDTab() const { return m_hwndTab; }
	HWND GetHWNDListProperty() const { return m_hwndListProperty; }
	HWND GetHWNDEditbox() const { return m_hwndEditEditbox; }
	HWND GetHWNDCombobox() const { return m_hwndEditComboList; }
	HWND GetHWNDComboBtn() const { return m_hwndEditComboBtn; }

	bool ProcessTabCtrl(HWND hwnd, UINT messg, WPARAM wParam, LPARAM lParam);

	bool FillPropertyTabs();
	BOOL FillListProperties();
	int	 FillTransientPropertyList(const CActor* InActor, int &InPropertyListIndex);
	void ClearListProperties();

	BOOL FillComboProperties(LPNMLISTVIEW plvdi);

	BOOL GETDISPINFO_FillList(LPARAM lParam);
	BOOL FillListParam(LPARAM lParam, const SPropertyClass *PropClass, const Property_Base *Prop, int MemOffset = 0);
	BOOL GETDISPINFO_FillComboList(LPARAM lParam);

	long HandleNotify(HWND hWndDlg, int nIDCtrl, LPNMHDR pNMHDR);
	long HandleCustomDraw(HWND hWndTreeView, LPNMLVCUSTOMDRAW pNMLVCD);
	void CustomDrawProperty(LPNMLVCUSTOMDRAW pNMLVCD, const SPropertyClass *PropClass, const Property_Base *Prop, int MemOffset = 0);

	long HandleNotifyCombobox(HWND hWndDlg, int nIDCtrl, LPNMHDR pNMHDR);
	long HandleCustomDrawCombobox(HWND hWndTreeView, LPNMLVCUSTOMDRAW pNMLVCD);

	BOOL ClickListProperties(LPNMLISTVIEW plvdi);
	BOOL ProcessClickListProperties(LPNMLISTVIEW plvdi, SPropertyClass *Class, Property_Base* Prop, int MemOffset = 0);
	BOOL ClickComboList(LPNMLISTVIEW plvdi);

	void HideChildControls();

	void Size(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	
	void SetGridViewStyle(EGridView Value);
	EGridView GetGridViewStyle() const { return GridViewStyle; }

	void Redraw() const;

protected:
	static BOOL CALLBACK SubClassProcList(HWND hwnd, UINT messg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK SubClassProcComboBtn(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK SubClassProcComboList(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK SubClassProcEdit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK SubClassProcEditBtn(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK SubClassProcEditBtnClear(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK SubClassProcEditBtnArrayAdd(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK SubClassProcEditBtnArrayDel(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK ProcPropWndTop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ProcPropWndBottom(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	PropertyGroup * GetGroupByName(const std::string &Name) const;
	bool			IsClassAllowed(const std::string &Name) const;
	bool			IsDisclosed(const std::string &Name) const;

	bool GetPropertyByIndex(int InPlainIndex, int IndexGroup, struct SPropertyClass **OutClass, class Property_Base** OutProperty, int &OutMemoryOffset) const;
		
	HWND m_hWndTop;
	HWND m_hWndBottom;

	HWND m_hwndParent;
	HWND m_hwndTab;
	HWND m_hwndListProperty;
	HWND m_hwndEditEditbox;
	HWND m_hwndEditComboList;
	HWND m_hwndEditComboBtn;
	HWND m_hwndEditEditboxBtnBrowse;
	HWND m_hwndEditEditboxBtnClear;

	// helpful bitmaps
	HIMAGELIST ImageListHelper;
	
	TVecPropertyGroup PropertyGroups;
	TVecPropertyWrapper VSortedProperties;
	std::vector<std::string> VCollapsedClassNames;

public:
	CriticalSection PropertyCS;
	const class CWindowManager *m_pWindowManager;
	
	WNDPROC lpfnTabCtrlWndProc;
	WNDPROC m_lpfnListProc;
	WNDPROC m_lpfnEditboxWndProc;
	WNDPROC m_lpfnComboboxListWndProc;
	WNDPROC m_lpfnComboboxBtnWndProc;
	WNDPROC m_lpfnEditEditboxBtnBrowse;
	WNDPROC m_lpfnEditEditboxBtnClear;

	unsigned int m_EditingPropertyIndex;
	int			 m_MemoryOffset;

private:
	EGridView GridViewStyle;

	CActor *m_pSelectedActor;
};
*/
#endif//__win32propertygrid_h__