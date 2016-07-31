#pragma once

#include "ToolBarHelper.h"
#include "WTLPropertyGrid.h"

#define ID_BUTTON_CATEGORIZED_A				983
#define ID_BUTTON_SORTED					984

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T_CLASS>
class CPanePropertyContainer : public CPaneContainer
{
	template<typename U>
	class CToolbarContainer : public CWindowImpl<CToolbarContainer<U>>
	{
	public:
		BEGIN_MSG_MAP(CToolbarContainer)
			MESSAGE_HANDLER(WM_CREATE, OnCreate)
			MESSAGE_HANDLER(WM_COMMAND, OnLBClick)
			REFLECT_NOTIFICATIONS()
		END_MSG_MAP()

        CToolbarContainer(CPanePropertyContainer<T_CLASS> *pOwner);
        LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
        LRESULT OnLBClick(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	private:
		CPanePropertyContainer<T_CLASS>  *m_pOwner;
	};

public:
	BEGIN_MSG_MAP(CPanePropertyContainer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(WM_USER_DELETE_ALL, OnDeleteAll)
		MESSAGE_HANDLER(WM_USER_PROPS_FILLED, OnFilledProps)
		MESSAGE_HANDLER(WM_USER_UPDATE_PROPS, OnUpdateProps)
		CHAIN_MSG_MAP(CPaneContainer)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CPanePropertyContainer();
	~CPanePropertyContainer();

    void SetAppMain(CCoreSDK *app);

    void SetEditor(editors::TIEditor editor);

    void SetRenderContext(SRenderContext *pContext);

    void Update(const T_CLASS *pSender, ESystemEventID EventId);

    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnNotify(UINT, WPARAM, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHendled);
	LRESULT OnDeleteAll(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&);
	LRESULT OnFilledProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&);
	LRESULT OnUpdateProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&);

	bool AddTabPage(const char *name, int Index = 0);
	bool ClearTabPages();

	void SetGridViewStyle(EGridView Value);
    
    void OnObjectSelected();

protected:
public:
	CToolbarContainer<T_CLASS> *m_pMenuCtrl;
	CTabCtrl m_pTabCtrl;

	CToolBarCtrl m_ToolBar;
	class CWTLPropertyGrid<T_CLASS> *m_pPropGrid;

    std::shared_ptr<editors::IEditor> m_editor;
};

#include "pane_property_container.ipp"