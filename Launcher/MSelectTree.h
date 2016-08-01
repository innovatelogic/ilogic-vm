#if !defined(AFX_MSELECTTREE_H__20051118_B832_CDD2_D48F_0080AD509054__INCLUDED_)
#define AFX_MSELECTTREE_H__20051118_B832_CDD2_D48F_0080AD509054__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// MultiSelectTree - A tree control with multi-select capabilities
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2005 Bjarke Viksoe.
//
// Add the following macro to the parent's message map:
//   REFLECT_NOTIFICATIONS()
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __cplusplus
  #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLCTRLS_H__
  #error MSelectTree.h requires atlctrls.h to be included first
#endif


// Extended MultiSelectTree styles
#define MTVS_EX_NOMARQUEE   0x00000001

// New control notifications
#define TVN_ITEMSELECTING 0x0001
#define TVN_ITEMSELECTED  0x0002
#define TVN_ITEMSELECTFINISHED  0x0003

template< class T, class TBase = CTreeViewCtrl, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CMultiSelectTreeImpl : 
   public CWindowImpl< T, TBase, TWinTraits >,
   public CCustomDraw< T >
{
public:
   DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

   DWORD m_dwExStyle;              // Additional styles
   HTREEITEM m_hExtSelStart;       // Item where SHIFT was last pressed
   bool m_bMarquee;                // Are we drawing rubberband?
   POINT m_ptDragStart;            // Point where rubberband started
   POINT m_ptDragOld;              // Last mousepos of rubberband

   typedef struct tagTVDATA 
   {
      HTREEITEM hItem;
      bool bSelected;
      COLORREF clrText;
      COLORREF clrBack;
      HFONT hFont;
   } TVDATA;

   CSimpleMap<HTREEITEM, TVDATA> m_aData;

   // Message map and handlers
   BEGIN_MSG_MAP(CMultiSelectTreeImpl)
       MESSAGE_HANDLER(WM_CREATE, OnCreate)
       MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
       MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
       MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
       MESSAGE_HANDLER(WM_CHAR, OnChar)
       MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
       MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
       MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
       MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
       MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChange)
       MESSAGE_HANDLER(TVM_INSERTITEM, OnInsertItem)
       REFLECTED_NOTIFY_CODE_HANDLER(TVN_DELETEITEM, OnDeleteItem)
       CHAIN_MSG_MAP_ALT(CCustomDraw< T >, 1)
   END_MSG_MAP()

   CMultiSelectTreeImpl() : m_dwExStyle(0), m_bMarquee(false)
   {
   }

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
      if( bRet ) _Init();
      return bRet;
   }
   DWORD SetExtendedTreeStyle(DWORD dwStyle)
   {
      ATLASSERT(!m_ctrlTree.IsWindow());   // Before control is created, please!
      DWORD dwOldStyle = m_dwTreeStyle;
      m_dwTreeStyle = dwStyle;
      return dwOldStyle;
   }

   void SelectItem(HTREEITEM hItem, BOOL bSelect)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      _SelectItem(hItem, bSelect == TRUE);
      if( bSelect ) TBase::SelectItem(hItem);
   }
   HTREEITEM GetSelectedItem() const
   {
      ATLASSERT(false);  // Not usable!
      return GetFirstSelectedItem();
   }
   HTREEITEM GetFocusItem() const
   {
      return TBase::GetSelectedItem();
   }
   UINT GetItemState(HTREEITEM hItem, UINT nStateMask) const
   {
      UINT nRes = TBase::GetItemState(hItem, nStateMask);
      if( (nStateMask & TVIS_SELECTED) != 0 ) {
         int iIndex = m_aData.FindKey(hItem);
         if( iIndex >= 0 ) {
            nRes &= ~TVIS_SELECTED;
            if( m_aData.GetValueAt(iIndex).bSelected ) nRes |= TVIS_SELECTED;
         }
      }
      return nRes;
   }

   HTREEITEM GetFirstSelectedItem() const
   {
      if( m_aData.GetSize() == 0 ) return NULL;
      for( int i = 0; i < m_aData.GetSize(); i++ ) {
         if( m_aData.GetValueAt(i).bSelected ) return m_aData.GetValueAt(i).hItem;
      }
      return NULL;
   }
   HTREEITEM GetNextSelectedItem(HTREEITEM hItem) const
   {
      int iIndex = m_aData.FindKey(hItem);
      if( iIndex < 0 ) return NULL;
      for( int i = iIndex + 1; i < m_aData.GetSize(); i++ ) {
         if( m_aData.GetValueAt(i).bSelected ) return m_aData.GetValueAt(i).hItem;
      }
      return NULL;
   }
   int GetSelectedCount() const
   {
      int nCount = 0;
      for( int i = 0; i < m_aData.GetSize(); i++ ) {
         if( m_aData.GetValueAt(i).bSelected ) nCount++;
      }
      return nCount;
   }

   BOOL SetItemColors(HTREEITEM hItem, COLORREF clrText, COLORREF clrBack = CLR_NONE)
   {
      int iIndex = m_aData.FindKey(hItem);
      if( iIndex < 0 ) return FALSE;
      TVDATA& data = m_aData.GetValueAt(iIndex);
      data.clrText = clrText;
      data.clrBack = clrBack;
      return TRUE;
   }
   BOOL SetItemFont(HTREEITEM hItem, HFONT hFont)
   {
      int iIndex = m_aData.FindKey(hItem);
      if( iIndex < 0 ) return FALSE;
      TVDATA& data = m_aData.GetValueAt(iIndex);
      data.hFont = hFont;
      return TRUE;
   }

   // Implementation

   void _Init()
   {
      ATLASSERT(::IsWindow(m_hWnd));

      ModifyStyle(TVS_SHOWSELALWAYS, 0);
   }

   void _SelectItem(int iIndex, bool bSelect, int action = TVC_UNKNOWN)
   {
      if( iIndex < 0 ) return;
      TVDATA& data = m_aData.GetValueAt(iIndex);
      // Don't change if state is already upadted (avoids flicker)
      if( data.bSelected == bSelect ) return;
      // Send notifications
      NMTREEVIEW nmtv = { 0 };
      nmtv.hdr.code = TVN_ITEMSELECTING;
      nmtv.hdr.hwndFrom = m_hWnd;
      nmtv.hdr.idFrom = GetDlgCtrlID();
      nmtv.action = action;
      nmtv.itemNew.hItem = data.hItem;
      nmtv.itemNew.lParam = GetItemData(data.hItem);
      nmtv.itemNew.state = bSelect ? TVIS_SELECTED : 0;
      nmtv.itemNew.stateMask = TVIS_SELECTED;
      if( ::SendMessage(GetParent(), WM_NOTIFY, nmtv.hdr.idFrom, (LPARAM) &nmtv) != 0 ) return;
      // Change state
      data.bSelected = bSelect;
      // Repaint item
      RECT rcItem = { 0 };
      if( GetItemRect(data.hItem, &rcItem, FALSE) ) InvalidateRect(&rcItem, TRUE);
      // More notifications
      nmtv.hdr.code = TVN_ITEMSELECTED;
      ::SendMessage(GetParent(), WM_NOTIFY, nmtv.hdr.idFrom, (LPARAM) &nmtv);
   }
   void _SelectItem(HTREEITEM hItem, bool bSelect, int action = TVC_UNKNOWN)
   {
      _SelectItem(m_aData.FindKey(hItem), bSelect, action);
   }
   void _SelectTree(HTREEITEM hItem, HTREEITEM hGoal, int action)
   {
      if( !_SelectTreeSub(hItem, hGoal, action) ) return;
      hItem = GetParentItem(hItem);
      while( (hItem = GetNextSiblingItem(hItem)) != NULL ) {
         if( !_SelectTreeSub(hItem, hGoal, action) ) return;
      }
   }
   bool _SelectTreeSub(HTREEITEM hItem, HTREEITEM hGoal, int action)
   {
      while( hItem != NULL ) {
         _SelectItem(hItem, true, action);
         if( hItem == hGoal ) return false;
         if( (TBase::GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) != 0 ) {
            if( !_SelectTreeSub(GetChildItem(hItem), hGoal, action) ) return false;
         }
         hItem = GetNextSiblingItem(hItem);
      }
      return true;
   }
   void _SelectBox(RECT rc)
   {
      HTREEITEM hItem = GetFirstVisibleItem();
      while( hItem != NULL ) {
         RECT rcItem = { 0 };
         GetItemRect(hItem, &rcItem, TRUE);
         RECT rcTemp = { 0 };
         _SelectItem(hItem, ::IntersectRect(&rcTemp, &rcItem, &rc) == TRUE, TVC_BYMOUSE);
         hItem = GetNextVisibleItem(hItem);
      }
   }
   void _NormalizeRect(RECT& rc) const
   {
      if( rc.left > rc.right ) {
         int t = rc.right; rc.right = rc.left; rc.left = t;
      }
      if( rc.top > rc.bottom ) {
         int t = rc.bottom; rc.bottom = rc.top; rc.top = t;
      }
   }
   void _DrawDragRect(POINT pt)
   {
      CClientDC dc = m_hWnd;
      SIZE szFrame = { 1, 1 };
      RECT rect = { m_ptDragStart.x, m_ptDragStart.y, pt.x, pt.y };
      _NormalizeRect(rect);
      CBrush brush = CDCHandle::GetHalftoneBrush();
      if( !brush.IsNull() ) {
         HBRUSH hOldBrush = dc.SelectBrush(brush);
         dc.PatBlt(rect.left, rect.top, rect.right - rect.left, szFrame.cy, PATINVERT);
         dc.PatBlt(rect.left, rect.bottom - szFrame.cy, rect.right - rect.left, szFrame.cy, PATINVERT);
         dc.PatBlt(rect.left, rect.top + szFrame.cy, szFrame.cx, rect.bottom - rect.top - (szFrame.cy * 2), PATINVERT);
         dc.PatBlt(rect.right - szFrame.cx, rect.top + szFrame.cy, szFrame.cx, rect.bottom - rect.top - (szFrame.cy * 2), PATINVERT);
         dc.SelectBrush(hOldBrush);
      }
   }



   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc();
      _Init();
      return lRes;
   }
   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      m_aData.RemoveAll();
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if( wParam == VK_SHIFT ) {
         m_hExtSelStart = GetFocusItem();
      }
      if( ::GetAsyncKeyState(VK_SHIFT) < 0 && m_hExtSelStart == GetFocusItem() ) 
      {
         switch( wParam ) {
         case VK_UP:
         case VK_DOWN:
         case VK_HOME:
         case VK_END:
         case VK_NEXT:
         case VK_PRIOR:
            for( int i = 0; i < m_aData.GetSize(); i++ ) _SelectItem(i, false, TVC_BYKEYBOARD);
         }
      }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if( ::GetAsyncKeyState(VK_SHIFT) < 0 ) {
         switch( wParam ) {
         case VK_UP:
         case VK_DOWN:
         case VK_HOME:
         case VK_END:
         case VK_NEXT:
         case VK_PRIOR:
            HTREEITEM hItem = GetFocusItem();
            // Is current or first-shift-item the upper item?
            RECT rcItem1 = { 0 };
            RECT rcItem2 = { 0 };
            GetItemRect(m_hExtSelStart, &rcItem1, TRUE);
            GetItemRect(hItem, &rcItem2, TRUE);
            // Select from current item to item where SHIFT was pressed
            if( rcItem1.top > rcItem2.top ) _SelectTree(hItem, m_hExtSelStart, TVC_BYKEYBOARD);
            else _SelectTree(m_hExtSelStart, hItem, TVC_BYKEYBOARD);
            _SelectItem(hItem, true, TVC_BYKEYBOARD);
            SelectionFinished();
         }
      }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if( wParam == VK_SPACE ) {
         HTREEITEM hItem = GetFocusItem();
         _SelectItem(hItem, (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED) == 0, TVC_BYKEYBOARD);
         return 0;
      }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc();
      // FIX: We really need the focus-rectangle in this control since it
      //      improves the navigation a lot. So let's ask Windows to display it.
#ifndef WM_UPDATEUISTATE
      const UINT WM_UPDATEUISTATE = 0x0128;
      const UINT UIS_CLEAR        = 0x0002;
      const UINT UISF_HIDEFOCUS   = 0x0001;
#endif
      SendMessage(WM_UPDATEUISTATE, MAKEWPARAM(UIS_CLEAR, UISF_HIDEFOCUS));
      return lRes;
   }
   LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
   {
      bHandled = FALSE;
      // Hit-test and figure out where we're clicking...
      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      TVHITTESTINFO hti = { 0 };
      hti.pt = pt;
      HTREEITEM hItem = HitTest(&hti);
      if( (hItem == NULL || (hti.flags & TVHT_ONITEMRIGHT) != 0) ) {
         if( (m_dwExStyle & MTVS_EX_NOMARQUEE) == 0 && ::DragDetect(m_hWnd, pt) ) {
            // Great we're dragging a rubber-band
            // Clear selection first
            for( int i = 0; i < m_aData.GetSize(); i++ ) _SelectItem(i, false, TVC_BYMOUSE);
            UpdateWindow();
            // Now start drawing the rubber-band...
            SetCapture();
            m_ptDragStart = m_ptDragOld = pt;
            _DrawDragRect(pt);
            m_bMarquee = true;
            bHandled = TRUE;
            return 0;
         }
      }
      if( hItem == NULL ) return 0;
      if( (hti.flags & TVHT_ONITEMBUTTON) != 0 ) return 0;      
      // Great, let's do an advanced selection
      if( (hti.flags & TVHT_ONITEMRIGHT) != 0 ) {
         for( int i = 0; i < m_aData.GetSize(); i++ ) _SelectItem(i, false, TVC_BYMOUSE);
         return 0;
      }
      int iIndex = m_aData.FindKey(hItem);
      if( iIndex < 0 ) return 0;
      // Simulate drag'n'drop?
      if( m_aData.GetValueAt(iIndex).bSelected && (GetStyle() & TVS_DISABLEDRAGDROP) == 0 && ::DragDetect(m_hWnd, pt) ) {
         NMTREEVIEW nmtv = { 0 };
         nmtv.hdr.code = TVN_BEGINDRAG;
         nmtv.hdr.hwndFrom = m_hWnd;
         nmtv.hdr.idFrom = GetDlgCtrlID();
         nmtv.itemNew.hItem = hItem;
         nmtv.itemNew.lParam = GetItemData(hItem);
         if( ::SendMessage(GetParent(), WM_NOTIFY, nmtv.hdr.idFrom, (LPARAM) &nmtv) != 0 ) return 0;
         return 0;
      }
      TVDATA& data = m_aData.GetValueAt(iIndex);
      if( ::GetAsyncKeyState(VK_SHIFT) < 0 ) {
         // Is current or first-shift-item the upper item?
         RECT rcItem1 = { 0 };
         RECT rcItem2 = { 0 };
         GetItemRect(m_hExtSelStart, &rcItem1, TRUE);
         GetItemRect(hItem, &rcItem2, TRUE);
         // Select from current item to item where SHIFT was pressed
         if (rcItem1.top > rcItem2.top)
         {
             _SelectTree(hItem, m_hExtSelStart, TVC_BYMOUSE);
         }
         else
         {
             _SelectTree(m_hExtSelStart, hItem, TVC_BYMOUSE);
         }
         SelectionFinished();
      }
      else if( ::GetAsyncKeyState(VK_CONTROL) < 0 ) {
         // Just toggle item
         _SelectItem(iIndex, !data.bSelected, TVC_BYMOUSE);
         SelectionFinished();
      }
      else {
         // Remove current selection and replace it with clicked item
          for (int i = 0; i < m_aData.GetSize(); i++) {
              _SelectItem(i, i == iIndex, TVC_BYMOUSE);
          }
         SelectionFinished();
      }
      return 0;
   }
   LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      //SelectionFinished();
       if (m_bMarquee) { ReleaseCapture(); }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
   {
      if( m_bMarquee ) {
         POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
         RECT rc = { m_ptDragStart.x, m_ptDragStart.y, pt.x, pt.y };
         _DrawDragRect(m_ptDragOld);
         _NormalizeRect(rc);
         _SelectBox(rc);
         UpdateWindow();
         _DrawDragRect(pt);
         m_ptDragOld = pt;
      }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnCaptureChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if( m_bMarquee ) {
         _DrawDragRect(m_ptDragOld);
         m_bMarquee = false;
         SelectionFinished();
      }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnInsertItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      HTREEITEM hItem = (HTREEITEM) DefWindowProc(uMsg, wParam, lParam);
      if( hItem == NULL ) return (LRESULT) hItem;
      // We manage a bit of extra information for each item. We'll store
      // this in an ATL::CSimpleMap. Not a particular speedy structure for lookups.
      // Don't keep too many items around in the tree!
      TVDATA data;
      data.hItem = hItem;
      data.bSelected = false;
      data.hFont = NULL;
      data.clrText = CLR_NONE;
      data.clrBack = CLR_NONE;
      m_aData.Add(hItem, data);
      return (LRESULT) hItem;
   }
   LRESULT OnDeleteItem(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
   {
      LPNMTREEVIEW lpNMTV = (LPNMTREEVIEW) pnmh;
      m_aData.Remove(lpNMTV->itemNew.hItem);
      bHandled = TRUE;
      return 0;
   }

   // Custom Draw

   DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
   {
      return CDRF_NOTIFYITEMDRAW;   // We need per-item notifications
   }
   DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
   {
      LPNMTVCUSTOMDRAW lpTVCD = (LPNMTVCUSTOMDRAW) lpNMCustomDraw;
      HTREEITEM hItem = (HTREEITEM) lpTVCD->nmcd.dwItemSpec;
      DWORD dwRet = CDRF_DODEFAULT;
      int iIndex = m_aData.FindKey(hItem);
      if( iIndex >= 0 ) {
         TVDATA& data = m_aData.GetValueAt(iIndex);
         // Trick TreeView into displaying correct selection colors
         if( data.bSelected ) {
            lpTVCD->clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
            lpTVCD->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);            
         }
         else
         {
            // Might as well change colors and font while we're at it...
            if( data.clrText != CLR_NONE ) lpTVCD->clrText = data.clrText;
            if( data.clrBack != CLR_NONE ) lpTVCD->clrTextBk = data.clrBack;
            // Special case of tree-item actually have selection, but our
            // state says it is currently not selected (CTRL+click on same item twice).
            if( (lpTVCD->nmcd.uItemState & CDIS_SELECTED) != 0 )
            {
               CDCHandle dc = lpTVCD->nmcd.hdc;
               COLORREF clrText = data.clrText;
               if( clrText == CLR_NONE ) clrText = GetTextColor();
               if( clrText == CLR_NONE ) clrText = ::GetSysColor(COLOR_WINDOWTEXT);
               COLORREF clrBack = data.clrBack;
               if( clrBack == CLR_NONE ) clrBack = GetBkColor();
               if( clrBack == CLR_NONE ) clrBack = ::GetSysColor(COLOR_WINDOW);
               dc.SetTextColor(clrText);
               dc.SetBkColor(clrBack);
            }
         }
         if( data.hFont != NULL ) ::SelectObject(lpTVCD->nmcd.hdc, data.hFont);
         dwRet = CDRF_NEWFONT;
      }
      return dwRet;
   }

   void SelectionFinished()
   {
       NMTREEVIEW nmtv = { 0 };
       nmtv.hdr.code = TVN_ITEMSELECTFINISHED;
       nmtv.hdr.hwndFrom = m_hWnd;
       nmtv.hdr.idFrom = GetDlgCtrlID();
       ::SendMessage(GetParent(), WM_NOTIFY, nmtv.hdr.idFrom, (LPARAM)&nmtv);
   }
};

class CMultiSelectTreeCtrl : public CMultiSelectTreeImpl<CMultiSelectTreeCtrl, CTreeViewCtrl, CWinTraitsOR<TVS_SHOWSELALWAYS> >
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_MultiSelectTree"), GetWndClassName())  
};


#endif // !defined(AFX_MSELECTTREE_H__20051118_B832_CDD2_D48F_0080AD509054__INCLUDED_)

