
#include "stdafx.h"
#include "FontCrtlBarFrameWnd.h"

void CFontCtrlBarFrameWnd::Init() {
	m_pSizeBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("SIZELIST")));
	m_pColorBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("COLORLIST")));
}

void CFontCtrlBarFrameWnd::Notify(DuiLib::TNotifyUI& msg)
{
	UINT nType = 0;
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if( msg.sType == _T("click") ) {
		if( msg.pSender == m_pSizeBtn ) { 
			nType = 11; //显示字体大小窗口
		} else if( msg.pSender == m_pColorBtn ) {
			nType = 12; //显示字体颜色窗口
		}
		::SendMessage(m_pCatchScreenDlg->m_hWnd, WM_GETCOMMAND, 0, nType);
	} else if(msg.sType==_T("selectchanged")) {
		DuiLib::CDuiString name = msg.pSender->GetName();
		//TODO
	}
}

LRESULT CFontCtrlBarFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	DuiLib::CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	DuiLib::CControlUI* pRoot = builder.Create(_T("FontCrtl.xml"), (UINT)0,  &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();

	m_pColorBtn->SetBkColor(0XFFFF0000);
	return 0;
}

LRESULT CFontCtrlBarFrameWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	m_pm.MessageHandler(uMsg, wParam, lParam, lRes);

	CRect rcColorBtn = m_pColorBtn->GetPos();
	rcColorBtn.left = rcColorBtn.left + 4;
	rcColorBtn.top = rcColorBtn.top + 3;
	rcColorBtn.right = rcColorBtn.right - 13;
	rcColorBtn.bottom = rcColorBtn.bottom - 3;

	HDC dc = ::GetDC(m_hWnd);
	DWORD color = m_pColorBtn->GetBkColor();
	::FillRect(dc, &rcColorBtn, CBrush(RGB(GetBValue(color), GetGValue(color), GetRValue(color))));

	return S_OK;
}

LRESULT CFontCtrlBarFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case 100:
	case 101:
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:
	case 110:
	case 111:
		lRes = OnFontSizeChanged(uMsg, wParam, lParam, bHandled); break;
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

LRESULT CFontCtrlBarFrameWnd::OnFontSizeChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(uMsg)
	{
	case 100:
		m_pSizeBtn->SetText(_T("9"));
		break;
	case 101:
		m_pSizeBtn->SetText(_T("10"));
		break;
	case 102:
		m_pSizeBtn->SetText(_T("11"));
		break;
	case 103:
		m_pSizeBtn->SetText(_T("12"));
		break;
	case 104:
		m_pSizeBtn->SetText(_T("13"));
		break;
	case 105:
		m_pSizeBtn->SetText(_T("14"));
		break;
	case 106:
		m_pSizeBtn->SetText(_T("18"));
		break;
	case 107:
		m_pSizeBtn->SetText(_T("24"));
		break;
	case 108:
		m_pSizeBtn->SetText(_T("36"));
		break;
	case 109:
		m_pSizeBtn->SetText(_T("48"));
		break;
	case 110:
		m_pSizeBtn->SetText(_T("72"));
		break;
	case 111: 
		m_pSizeBtn->SetText(_T("96"));
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CFontCtrlBarFrameWnd::OnBtnBkColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	switch(uMsg) {
	case 200:
		m_pColorBtn->SetBkColor(0XFF808000);
		break;
	case 201:
		m_pColorBtn->SetBkColor(0XFFF0FBFF);
		break;
	case 202:
		m_pColorBtn->SetBkColor(0XFF00FF00);
		break;
	case 203:
		m_pColorBtn->SetBkColor(0XFF0000FF);
		break;
	case 204:
		m_pColorBtn->SetBkColor(0XFFA6CAF0);
		break;
	case 205:
		m_pColorBtn->SetBkColor(0XFFFFFF00);
		break;
	case 206:
		m_pColorBtn->SetBkColor(0XFF00FFFF);
		break;
	case 207:
		m_pColorBtn->SetBkColor(0XFFFF00FF);
		break;
	case 208:
		m_pColorBtn->SetBkColor(0XFFFF0000);
		break;
	case 209:
		m_pColorBtn->SetBkColor(0XFFFF8000);
		break;
	case 210:
		m_pColorBtn->SetBkColor(0XFF000000);
		break;
	case 211:
		m_pColorBtn->SetBkColor(0XFF008000);
		break;
	}
	return 0;
}