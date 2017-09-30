
#include "stdafx.h"
#include "ColorListBarFrameWnd.h"
#include "ShapeCtrlBarFrameWnd.h"
#include "FontCrtlBarFrameWnd.h"

void ColorListBarFrameWnd::Init() {
	m_pBrowNBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("BROWN")));
	m_pWhiteBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("WHITE")));
	m_pLawngreenBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("LAWNGREEN")));
	m_pNavyBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("NAVY")));
	m_pAzureBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("AZURE")));
	m_pYelloWBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("YELLOW")));
	m_pCyanBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("CYAN")));
	m_pPinkBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("PINK")));
	m_pRedBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("RED")));
	m_pBisqueBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("BISQUE")));
	m_pBlackBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("BLACK")));
	m_pDarkgreenBtn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("DARKGREEN")));
}

void ColorListBarFrameWnd::Notify(DuiLib::TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if( msg.sType == _T("click") ) {
		if( msg.pSender == m_pBrowNBtn ) { 
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0X80, 0X80, 0X00));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(200, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0X80, 0X80, 0X00));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(200, 0, 0);
			}
		} else if( msg.pSender == m_pWhiteBtn ) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0Xf0, 0Xfb, 0Xff));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(201, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0Xf0, 0Xfb, 0Xff));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(201, 0, 0);
			}
		} else if( msg.pSender == m_pLawngreenBtn ) { 
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0X00, 0Xff, 0X00));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(202, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0X00, 0Xff, 0X00));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(202, 0, 0);
			}
		}else if( msg.pSender == m_pNavyBtn ) { 
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0X00, 0X00, 0Xff));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(203, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0X00, 0X00, 0Xff));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(203, 0, 0);
			}
		} else if ( msg.pSender == m_pAzureBtn ) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0Xa6, 0Xca, 0Xf0));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(204, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0Xa6, 0Xca, 0Xf0));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(204, 0, 0);
			}
		} else if ( msg.pSender == m_pYelloWBtn ) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0Xff, 0Xff, 0X00));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(205, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0Xff, 0Xff, 0X00));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(205, 0, 0);
			}
		} else if ( msg.pSender == m_pCyanBtn ) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0X00, 0Xff, 0Xff));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(206, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0X00, 0Xff, 0Xff));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(206, 0, 0);
			}
		} else if ( msg.pSender == m_pPinkBtn ) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0xff, 0X00, 0xff));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(207, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0xff, 0X00, 0xff));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(207, 0, 0);
			}
		} else if ( msg.pSender == m_pRedBtn ) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0xff, 0X00, 0X00));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(208, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0xff, 0X00, 0X00));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(208, 0, 0);
			}
		} else if ( msg.pSender == m_pBisqueBtn ) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0Xff, 0X80, 0X00));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(209, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0Xff, 0X80, 0X00));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(209, 0, 0);
			}
		} else if (msg.pSender == m_pBlackBtn) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0X00, 0X00, 0X00));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(210, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0X00, 0X00, 0X00));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(210, 0, 0);
			}
		} else if ( msg.pSender == m_pDarkgreenBtn ) {
			if (m_colorType == 0) {
				m_pCatchScreenDlg->SetShapeColor(RGB(0X00, 0X80, 0X00));
				m_pCatchScreenDlg->m_pShapeColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pShapeCtrlBar->SendMessage(211, 0, 0);
			} else {
				m_pCatchScreenDlg->SetTextColor(RGB(0X00, 0X80, 0X00));
				m_pCatchScreenDlg->m_pFontColorListDlg->ShowWindow(SW_HIDE);
				m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(211, 0, 0);
			}
		}
	} else if(msg.sType==_T("selectchanged")) {
		DuiLib::CDuiString name = msg.pSender->GetName();
		//TODO
	}
}

LRESULT ColorListBarFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	DuiLib::CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	DuiLib::CControlUI* pRoot = NULL;
	if (m_colorType == 0) {
		pRoot = builder.Create(_T("ColorList.xml"), (UINT)0,  &cb, &m_pm);
	} else {
		pRoot = builder.Create(_T("FontColorList.xml"), (UINT)0,  &cb, &m_pm);
	}
	
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

void ColorListBarFrameWnd::DrawMouseHoverEffect(DuiLib::CButtonUI* button, const CPoint& cursorPt)
{
	CRect rcButton = button->GetPos();
	if (!rcButton.PtInRect(cursorPt))
		return;

	Gdiplus::Graphics graphics(::GetDC(m_hWnd));
	DrawRoundRectange(graphics, Color(231, 231, 231), rcButton.left, rcButton.top, rcButton.Width() - 1, rcButton.Height() - 1);
	FillRoundRectangle(graphics, Color(231, 231, 231), rcButton.left, rcButton.top, rcButton.Width() - 1, rcButton.Height() - 1);
	
	DWORD color = button->GetBkColor();
	rcButton.DeflateRect(2, 2);
	::FillRect(graphics.GetHDC(), &rcButton, CBrush(RGB(GetBValue(color), GetGValue(color), GetRValue(color))));
}

LRESULT ColorListBarFrameWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	m_pm.MessageHandler(uMsg, wParam, lParam, lRes);

	CPoint point;
	GetCursorPos(&point);
	::ScreenToClient(m_hWnd, &point);

	DrawMouseHoverEffect(m_pRedBtn, point);
	DrawMouseHoverEffect(m_pPinkBtn, point);
	DrawMouseHoverEffect(m_pYelloWBtn, point);
	DrawMouseHoverEffect(m_pAzureBtn, point);
	DrawMouseHoverEffect(m_pNavyBtn, point);
	DrawMouseHoverEffect(m_pLawngreenBtn, point);
	DrawMouseHoverEffect(m_pBisqueBtn, point);
	DrawMouseHoverEffect(m_pBrowNBtn, point);
	DrawMouseHoverEffect(m_pBlackBtn, point);
	DrawMouseHoverEffect(m_pWhiteBtn, point);

	return S_OK;
}

LRESULT ColorListBarFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	default:
	bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
