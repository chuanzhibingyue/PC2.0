
#include "stdafx.h"
#include "ShapeCtrlBarFrameWnd.h"

void CShapeCtrlBarFrameWnd::Init() {
	m_pColorListBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("COLORLIST")));
	m_pSmallOpt = static_cast<DuiLib::COptionUI*>(m_pm.FindControl(_T("SMALL")));
	m_pMediumOpt = static_cast<DuiLib::COptionUI*>(m_pm.FindControl(_T("MEDIUM")));
	m_pLargeOpt = static_cast<DuiLib::COptionUI*>(m_pm.FindControl(_T("LARGE")));
}

void CShapeCtrlBarFrameWnd::Notify(DuiLib::TNotifyUI& msg)
{
	UINT nType = 0;
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else {
		DuiLib::CDuiString name = msg.pSender->GetName();
		if( msg.sType == _T("click") ) {
			if( msg.pSender == m_pColorListBtn ) { 
				UINT nType = 10;
				::SendMessage(m_pCatchScreenDlg->GetSafeHwnd(), WM_GETCOMMAND, 0, nType);
			} 
		} else if(msg.sType==_T("selectchanged")) {
			if (name == _T("SMALL")) {
				m_pCatchScreenDlg->SetShapeSize(2);
			} else if (name == _T("MEDIUM")) {
				m_pCatchScreenDlg->SetShapeSize(4);
			} else if (name == _T("LARGE")) {
				m_pCatchScreenDlg->SetShapeSize(8);
			}
		}

		::SetWindowPos(m_relatedWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	}
}

LRESULT CShapeCtrlBarFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	DuiLib::CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	DuiLib::CControlUI* pRoot = builder.Create(_T("ShapeCrtl.xml"), (UINT)0,  &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();

	m_pColorListBtn->SetBkColor(0XFFFF0000);
	return 0;
}

LRESULT CShapeCtrlBarFrameWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	m_pm.MessageHandler(uMsg, wParam, lParam, lRes);

	CRect rcColorListBtn = m_pColorListBtn->GetPos();
	rcColorListBtn.left = rcColorListBtn.left + 4;
	rcColorListBtn.top = rcColorListBtn.top + 3;
	rcColorListBtn.right = rcColorListBtn.right - 11;
	rcColorListBtn.bottom = rcColorListBtn.bottom - 3;

	HDC dc = ::GetDC(m_hWnd);
	DWORD color = m_pColorListBtn->GetBkColor();
	::FillRect(dc, &rcColorListBtn, CBrush(RGB(GetBValue(color), GetGValue(color), GetRValue(color))));

	return S_OK;
}

LRESULT CShapeCtrlBarFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
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
	case WM_PAINT:		   lRes = OnPaint(uMsg, wParam, lParam, bHandled); break;
	case 200:			   
	case 201:
	case 202:
	case 203:
	case 204:
	case 205:
	case 206:
	case 207:
	case 208:
	case 209:
	case 210:
	case 211:
		lRes = OnBtnBkColorChange(uMsg, wParam, lParam, bHandled); break;
	default:
	bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CShapeCtrlBarFrameWnd::OnBtnBkColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	switch(uMsg) {
	case 200:
		m_pColorListBtn->SetBkColor(0XFF808000);
		break;
	case 201:
		m_pColorListBtn->SetBkColor(0XFFF0FBFF);
		break;
	case 202:
		m_pColorListBtn->SetBkColor(0XFF00FF00);
		break;
	case 203:
		m_pColorListBtn->SetBkColor(0XFF0000FF);
		break;
	case 204:
		m_pColorListBtn->SetBkColor(0XFFA6CAF0);
		break;
	case 205:
		m_pColorListBtn->SetBkColor(0XFFFFFF00);
		break;
	case 206:
		m_pColorListBtn->SetBkColor(0XFF00FFFF);
		break;
	case 207:
		m_pColorListBtn->SetBkColor(0XFFFF00FF);
		break;
	case 208:
		m_pColorListBtn->SetBkColor(0XFFFF0000);
		break;
	case 209:
		m_pColorListBtn->SetBkColor(0XFFFF8000);
		break;
	case 210:
		m_pColorListBtn->SetBkColor(0XFF000000);
		break;
	case 211:
		m_pColorListBtn->SetBkColor(0XFF008000);
		break;
	}
	
	return 0;
}