#pragma once

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlgdi.h>
#include <atlmisc.h>
#include <atlctrls.h>

#include "reflx.h"
#include "ListVeiwEdit.h"
#include "ListViewCombobox.h"
#include "ListViewEditResource.h"
#include "property_reactor.h"

#define WM_ADJUST_COLUMN_WIDTH 1001

enum EGridView
{
	EGV_Categorized = 0,
	EGV_SortByName,
};

struct SPropertyGroup
{
	std::string GroupName;
	oes::rflex::TVecPropertyClass VecPropertyClasses;

	SPropertyGroup(const std::string& Name)
		: GroupName(Name)
	{
	}
};

typedef std::vector<struct SPropertyGroup*>	TVecPropertyGroup;
typedef TVecPropertyGroup::const_iterator	TVecPropertyGroupConstIter;
typedef TVecPropertyGroup::iterator			TVecPropertyGroupIter;

// for sorted arrays
struct SPropertyWrapper
{
    oes::rflex::Property_Base	*pProp;
	oes::rflex::SPropertyClass	*pClass;
	bool bDisclosed;

	SPropertyWrapper(oes::rflex::Property_Base* prop, oes::rflex::SPropertyClass* pclass)
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

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class IGridImpl
{
public:
	virtual LRESULT OnLVEndLabelEdit(WPARAM wParam) = 0;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------

typedef CWinTraitsOR<WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_OWNERDATA, WS_EX_CLIENTEDGE>	CListTraits;

template<class T>
class CWTLPropertyGrid : public CWindowImpl<CWTLPropertyGrid<T>, CListViewCtrl, CListTraits>, public IGridImpl
{
	typedef CListViewEdit<IGridImpl> TEdit;
	typedef CListViewComboBox<IGridImpl> TComboBox;
	typedef CListViewEditResource<IGridImpl> TResourceEdit;

    // proxy plain stuff to getdispdata routine
    struct SFetchData 
    {
        SFetchData(int _id, const oes::rflex::SClassNode * _class, oes::rflex::Property_Base *_prop)
            : id(_id)
            , pclass(_class)
            , property(_prop){}

        int id;
        const oes::rflex::SClassNode *pclass;
        oes::rflex::Property_Base *property;
    };
    

public:
	DECLARE_WND_SUPERCLASS(NULL, WC_LISTVIEW)

	BEGIN_MSG_MAP(CWTLPropertyGrid)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
		MESSAGE_HANDLER(WM_COMMAND, OnLBClick)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnScrollWindow)
		MESSAGE_HANDLER(WM_VSCROLL, OnScrollWindow)
		MESSAGE_HANDLER(WM_USER_DELETE_ALL, OnDeleteAll)
		MESSAGE_HANDLER(WM_USER_FILL_PROPS, OnFillProps)
		MESSAGE_HANDLER(WM_USER_UPDATE_PROPS, OnUpdateProps)
		MESSAGE_HANDLER(WM_ADJUST_COLUMN_WIDTH, OnAdjustColumnWidth)
		REFLECTED_NOTIFY_CODE_HANDLER(HDN_BEGINTRACKW, OnBeginTrack)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	CWTLPropertyGrid(editors::TIEditor &editor);
    ~CWTLPropertyGrid();

	//----------------------------------------------------------------------------------------------
	BOOL PreTranslateMessage(MSG* pMsg) { pMsg; return FALSE; }

	T*	 GetSelected() const { return m_pSelectedActor; }
	void SetSelected(T *pActor) { m_pSelectedActor = pActor; }

	void SetRenderContext(SRenderContext *pContext) { m_pRenderContext = pContext; }

    // Used with Views: Overridden create function sets required listview styles
    HWND Create(HWND hWndParent, 
        ATL::_U_RECT rect = NULL,
        LPCTSTR szWindowName = NULL,
        DWORD dwStyle = 0, 
        DWORD dwExStyle = 0,
        ATL::_U_MENUorID MenuOrID = 0U,
        LPVOID lpCreateParam = NULL);

    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled);
    LRESULT OnNotify(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLBClick(UINT, WPARAM, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnScrollWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
    LRESULT OnDeleteAll(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&);
    LRESULT OnFillProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&);
    LRESULT OnUpdateProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&);
    LRESULT OnAdjustColumnWidth(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnBeginTrack(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    virtual LRESULT OnLVEndLabelEdit(WPARAM wParam);
    LRESULT OnLVCustomDraw(int, LPNMHDR pNMHDR, BOOL&);

    void ScrollWindow();

    void AdjustColumnsSize();

    long HandleCustomDraw(LPNMLVCUSTOMDRAW pNMLVCD);

    void CustomDrawProperty(LPNMLVCUSTOMDRAW pNMLVCD);

    void FillModel();

    void FillPropertyGrid(std::vector<T*> &actors);
    void FillPropertyTabs();
    void FillPropertyData(T *pActor);

    SPropertyGroup* GetGroupByName(const std::string &name) const;

    bool IsClassAllowed(const std::string& name) const;
    bool IsDisclosed(const std::string &name) const;

    BOOL FillListProperties();
    void FillPropertyDataTransient(T *pActor);

	void UpdatePreview();

    BOOL GETDISPINFO_FillList(LVITEMA *pItem);

    bool GetPropertyByIndex(int InPlainIndex, int IndexGroup, 
        oes::rflex::SPropertyClass **OutClass, oes::rflex::Property_Base **OutProperty, int &OutMemoryOffset) const;

    bool IsPropertyBOOL(const oes::rflex::Property_Base * Prop);

    void _Update(const T *pSender, ESystemEventID EventId);

    void ClearListProperties();
	
    BOOL ClickListProperties(LPNMLISTVIEW plvdi);

    
    BOOL ProcessClickListProperties(LPNMLISTVIEW plvdi, oes::rflex::SPropertyClass *Class,
                                    Property_Base* Prop, int MemOffset /*= 0*/);

    BOOL ClickListComboProperties(LPNMLISTVIEW plvdi);

    void HideChildControls();

	const TVecPropertyGroup& GetPropertyGroups() const { return m_PropertyGroups; }
    
    void GetPropertyGroups(std::vector<std::string> &groups) const { return m_propReactor->FetchGroups(groups); }

	void SetSelectedGroup(int group) { m_nSelectedGroup = group; }

    void SetGridViewStyle(EGridView Value);

    void LoadHelperImageList();

	void PushContext();
    void PopContext();

    
protected:

private:
       void ClickProperty(int index);

       /*!
        * returns true if selected elements have a similar value
        otherwise if selected elements are empty or different value return false
        */
        bool GetPropertySelectedBatch(oes::rflex::Property_Base *prop, std::string &out);

        void ShowEditWindowControl(int index, SFetchData &data, const std::wstring &value);
        void ShowEditResourceControl(int index, SFetchData &data, const std::wstring &value);
        void ShowEditColorControl(int index, const std::string &value);
        void ShowEditComboControl(int index, const std::wstring &value);
private:
	TEdit						*m_pEdit;
	TComboBox					*m_pComboBox;
	TResourceEdit				*m_pResourceEdit;

	EGridView					m_GridViewStyle;
	T							*m_pSelectedActor;
	CriticalSection				m_PropertyCS;

	TVecPropertyGroup			m_PropertyGroups;
	TVecPropertyWrapper			m_VSortedProperties;
	std::vector<std::string>	m_VCollapsedClassNames;

	// helpful bitmaps
	HIMAGELIST m_ImageListHelper;

	int				m_nSelectedGroup;
	unsigned int	m_EditingPropertyIndex;
	int				m_MemoryOffset;

	CFont			m_Font;
	LPCTSTR			m_TypeFace;
	int				m_PointSize;
	BOOL			m_bInit;

	CCoreSDK		*m_pAppMain;
	SRenderContext	*m_pRenderContext;

    std::shared_ptr<oes::rflex::PropertyReactor<T>> m_propReactor;

    // cache data all fields in plain array
    std::vector<SFetchData> m_cacheDataAll;
    std::vector<SFetchData> m_cacheFill;

    wchar_t wbuf[256];

    editors::TIEditor m_editor;
};

#include "wtlpropertygrid.ipp"