
#include "stdafx.h"
#include "FontSizeListBarFrameWnd.h"
#include "FontCrtlBarFrameWnd.h"


void CFontSizeListBarFrameWnd::Init() {
	m_pSize9Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("9")));
	m_pSize10Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("10")));
	m_pSize11Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("11")));
	m_pSize12Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("12")));
	m_pSize13Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("13")));
	m_pSize14Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("14")));
	m_pSize18Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("18")));
	m_pSize24Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("24")));
	m_pSize36Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("36")));
	m_pSize48Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("48")));
	m_pSize72Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("72")));
	m_pSize96Btn = static_cast<DuiLib::CButtonUI*>(m_pm.FindControl(_T("96")));
}

void CFontSizeListBarFrameWnd::Notify(DuiLib::TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if( msg.sType == _T("click") ) {
		if( msg.pSender == m_pSize9Btn ) { 
			m_pCatchScreenDlg->SetTextSize(9);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(100, 0, 0);
		} else if( msg.pSender == m_pSize10Btn ) {
			m_pCatchScreenDlg->SetTextSize(10);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(101, 0, 0);
		} else if( msg.pSender == m_pSize11Btn ) { 
			m_pCatchScreenDlg->SetTextSize(11);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(102, 0, 0);
		}else if( msg.pSender == m_pSize12Btn ) { 
			m_pCatchScreenDlg->SetTextSize(12);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(103, 0, 0);
		} else if ( msg.pSender == m_pSize13Btn ) {
			m_pCatchScreenDlg->SetTextSize(13);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(104, 0, 0);
		} else if ( msg.pSender == m_pSize14Btn ) {
			m_pCatchScreenDlg->SetTextSize(14);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(105, 0, 0);
		} else if ( msg.pSender == m_pSize18Btn ) {
			m_pCatchScreenDlg->SetTextSize(18);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(106, 0, 0);
		} else if ( msg.pSender == m_pSize24Btn ) {
			m_pCatchScreenDlg->SetTextSize(24);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(107, 0, 0);
		} else if ( msg.pSender == m_pSize36Btn ) {
			m_pCatchScreenDlg->SetTextSize(36);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(108, 0, 0);
		} else if ( msg.pSender == m_pSize48Btn ) {
			m_pCatchScreenDlg->SetTextSize(48);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(109, 0, 0);
		} else if ( msg.pSender == m_pSize72Btn ) {
			m_pCatchScreenDlg->SetTextSize(72);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(110, 0, 0);
		} else if ( msg.pSender == m_pSize96Btn ) {
			m_pCatchScreenDlg->SetTextSize(96);
			m_pCatchScreenDlg->m_pFontSizeListDlg->ShowWindow(SW_HIDE);
			m_pCatchScreenDlg->m_pFontCtrlbar->SendMessage(111, 0, 0);
		}
	} else if(msg.sType==_T("selectchanged")) {
		DuiLib::CDuiString name = msg.pSender->GetName();
		//TODO
	}
}

LRESULT CFontSizeListBarFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	DuiLib::CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	DuiLib::CControlUI* pRoot = builder.Create(_T("FontSizeList.xml"), (UINT)0,  &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

void CFontSizeListBarFrameWnd::DrawSelectedButton(Gdiplus::Graphics& graphics) {
	DuiLib::CButtonUI* sizeButton = NULL;
	switch (m_pCatchScreenDlg->GetTextSize())
	{
	case 9:
		sizeButton = m_pSize9Btn;
		break;
	case 10:
		sizeButton = m_pSize10Btn;
		break;
	case 11:
		sizeButton = m_pSize11Btn;
		break;
	case 12:
		sizeButton = m_pSize12Btn;
		break;
	case 13:
		sizeButton = m_pSize13Btn;
		break;
	case 14:
		sizeButton = m_pSize14Btn;
		break;
	case 18:
		sizeButton = m_pSize18Btn;
		break;
	case 24:
		sizeButton = m_pSize24Btn;
		break;
	case 36:
		sizeButton = m_pSize36Btn;
		break;
	case 48:
		sizeButton = m_pSize48Btn;
		break;
	case 72:
		sizeButton = m_pSize72Btn;
		break;
	case 96:
		sizeButton = m_pSize96Btn;
		break;
	default:
		break;
	}

	if (sizeButton != NULL) {
		CPoint point;
		GetCursorPos(&point);
		::ScreenToClient(m_hWnd, &point);

		CRect rcButton = sizeButton->GetPos();
		if (!rcButton.PtInRect(point))
			DrawEffect(graphics, sizeButton, Color(255, 255, 255), Color(255, 255, 255), Color(45, 61, 77), DuiLib::CPaintManagerUI::GetInstancePath() + _T("skin\\font_size_selected.png"));
		else
			DrawEffect(graphics, sizeButton, Color(255, 255, 255), Color(45, 61, 77), Color(255, 255, 255, 255), DuiLib::CPaintManagerUI::GetInstancePath() + _T("skin\\font_size_to_select.png"));
	}
}

void CFontSizeListBarFrameWnd::DrawMouseHoverEffect(Gdiplus::Graphics& graphics, DuiLib::CButtonUI* button, const CPoint& cursorPt)
{
	CRect rcButton = button->GetPos();
	if (!rcButton.PtInRect(cursorPt))
		return;

	DrawEffect(graphics, button, Color(231, 231, 231), Color(45, 61, 77), Color(255, 255, 255, 255), NULL);
}

void CFontSizeListBarFrameWnd::DrawEffect(Gdiplus::Graphics &graphics, DuiLib::CButtonUI* button, const Color& round, const Color& fill, const Color& textcolor, const WCHAR* filename) {
	CRect rcButton = button->GetPos();

	Brush *fill_brush = new SolidBrush(fill);
	graphics.FillRectangle(fill_brush, rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height());
	delete fill_brush;

	FontFamily fontFamily(L"Arial");
	Gdiplus::Font myFont(&fontFamily, 9, FontStyleRegular, UnitPoint);

	rcButton.DeflateRect(0, 0);
	StringFormat format;
	format.SetAlignment(StringAlignmentFar);

	//size
	RectF rect(rcButton.left, rcButton.top, rcButton.right, rcButton.bottom);
	rect.Offset(-1, -1);
	graphics.DrawString(button->GetName(), wcslen(button->GetName()), &myFont, rect, &format, &SolidBrush(textcolor));

	//image
	if (filename != NULL)
	{
		Image image(filename);
		graphics.DrawImage(&image, RectF(rcButton.left + 4, rcButton.top + 3, image.GetWidth(), image.GetHeight()));
	}
}

LRESULT CFontSizeListBarFrameWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	m_pm.MessageHandler(uMsg, wParam, lParam, lRes);

	CPoint point;
	GetCursorPos(&point);
	::ScreenToClient(m_hWnd, &point);

	CRect rcWindow;
	GetWindowRect(m_hWnd, &rcWindow);

	Gdiplus::Graphics graphics(::GetDC(m_hWnd));

	DrawMouseHoverEffect(graphics, m_pSize9Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize10Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize11Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize12Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize13Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize14Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize18Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize24Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize36Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize48Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize72Btn, point);
	DrawMouseHoverEffect(graphics, m_pSize96Btn, point);

	DrawSelectedButton(graphics);

	return S_OK;
}

LRESULT CFontSizeListBarFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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