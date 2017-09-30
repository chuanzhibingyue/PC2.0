
#include "stdafx.h"
#include "CatchScreenBarFrameWnd.h"

void CCatchScreenBarFrameWnd::Init() {
	m_pOkBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("OK")));
	m_pCancelBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("CANCEL")));
	m_pSaveBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("SAVE")));
	m_pFontBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("FONT")));
	m_pPenBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("PEN")));
	m_pArrowBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("ARROW")));
	m_pCircleBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("CIRCLE")));
	m_pSquareBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("SQUARE")));
	m_pRollback = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("ROLLBACK")));
}

void CCatchScreenBarFrameWnd::Notify(DuiLib::TNotifyUI& msg)
{
	UINT nType = 0;
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if( msg.sType == _T("click") ) {
		if( msg.pSender == m_pOkBtn ) { 
			nType = 9;
		} else if( msg.pSender == m_pCancelBtn ) {
			nType = 8;
		} else if( msg.pSender == m_pSaveBtn ) { 
			nType = 7;
		}else if( msg.pSender == m_pRollback ) { 
			nType = 6;
		} else if ( msg.pSender == m_pFontBtn ) {
			nType = 5;
		} else if ( msg.pSender == m_pPenBtn ) {
			nType = 4;
		} else if ( msg.pSender == m_pCircleBtn ) {
			nType = 3;
		} else if ( msg.pSender == m_pArrowBtn ) {
			nType = 2;
		} else if ( msg.pSender == m_pSquareBtn ) {
			nType = 1;
		}
		::SendMessage(m_pCatchScreenDlg->GetSafeHwnd(), WM_GETCOMMAND, 0, nType);

	} else if(msg.sType==_T("selectchanged")) { 
		DuiLib::CDuiString name = msg.pSender->GetName();
		DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
		if(name==_T("examine"))
			pControl->SelectItem(0);
		else if(name==_T("trojan"))
			pControl->SelectItem(1);
		else if(name==_T("plugins"))
			pControl->SelectItem(2);
		else if(name==_T("vulnerability"))
			pControl->SelectItem(3);
		else if(name==_T("rubbish"))
			pControl->SelectItem(4);
		else if(name==_T("cleanup"))
			pControl->SelectItem(5);
		else if(name==_T("fix"))
			pControl->SelectItem(6);
		else if(name==_T("tool"))
			pControl->SelectItem(7);
	}

	::SetWindowPos(m_relatedWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
}

LRESULT CCatchScreenBarFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	DuiLib::CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	DuiLib::CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CCatchScreenBarFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg )
	{
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_NCLBUTTONDOWN:
		m_pCatchScreenDlg->HideAllBarWindow(true);
		m_pCatchScreenDlg->Invalidate();
		bHandled = FALSE;
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		::SetWindowPos(m_relatedWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		bHandled = FALSE;
		break;
	default:
		bHandled = FALSE;
	}

	if( bHandled ) 
		return lRes;
	
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}