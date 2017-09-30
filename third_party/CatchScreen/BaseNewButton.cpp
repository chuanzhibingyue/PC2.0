// BaseNewButton.cpp : 实现文件
//

#include "stdafx.h"
#include "BaseNewButton.h"


// BaseNewButton

IMPLEMENT_DYNAMIC(BaseNewButton, CButton)

BaseNewButton::BaseNewButton()
{
	m_state        = NORMAL;
	m_fg           = RGB(0, 0, 0);
	m_bg           = RGB(255, 255, 255);
	m_textPos      = CPoint(0,0);
	m_bMouseOver   = FALSE;
	m_bEnabled     = TRUE;
	m_bStretchBlt  = TRUE;
	m_bgDraw       = RGB(255, 0, 255);
	m_nImageWidth  = 68;
	m_nImageHeight = 21;
	m_nFlashIndex  = 0;
	m_nFlashCount  = 0;
}

BaseNewButton::~BaseNewButton()
{
}


BEGIN_MESSAGE_MAP(BaseNewButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_ENABLE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void BaseNewButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{

	//// TODO:  添加您的代码以绘制指定项
	//CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	UINT state = lpDIS->itemState; 
	//CRect rect;
	//rect.CopyRect(&lpDIS->rcItem); 

	m_state = NORMAL;

	if (state & ODS_FOCUS)
	{
		if (state & ODS_SELECTED)
		{ 
			m_state = DOWN;
		}
		else
		{
			if(m_bMouseOver)
				m_state = HOVER;
		}
	}
	else
	{
		m_state = NORMAL;
	}
	if (state & ODS_DISABLED)
	{
		m_state = DISABLE;
		m_bEnabled = FALSE;
	}

	DrawButton();
}


void BaseNewButton::DrawButton()
{
	CClientDC dc(this);
	CDC DCMemory;
	CRect cliRect;
	GetClientRect(&cliRect);
	DCMemory.CreateCompatibleDC(&dc);
	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, cliRect.Width(), cliRect.Height());
	CBitmap *pOldBitmapScreen = DCMemory.SelectObject(&btScreen);
	
	//画背景 
	if (!m_imgNormal.IsNull())
	{
		m_imgNormal.StretchBlt(DCMemory, cliRect.left, cliRect.top, cliRect.Width(), cliRect.Height(), 4*m_nImageWidth, 0, m_nImageWidth, m_nImageHeight, SRCCOPY);
	}

	CString strText;
	GetWindowText(strText);
	DCMemory.SetBkMode(TRANSPARENT);
	CFont fontBtn;
	fontBtn.CreatePointFont(90, L"Arial");
	CFont *pOldFont = DCMemory.SelectObject(&fontBtn);
	switch(m_state)
	{
	case NORMAL:
		{
			if(!m_imgNormal.IsNull())
			{
				m_imgNormal.TransparentBlt(DCMemory.m_hDC, cliRect.left, cliRect.top, m_nImageWidth, m_nImageHeight, 0, 0, m_nImageWidth, m_nImageHeight, m_bgDraw);
			}
			if (strText.GetLength() > 0)
			{
				DCMemory.SetTextColor(m_fg);
				if (m_textPos.x == 0)
				{
					DCMemory.DrawText(strText, strText.GetLength(), cliRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				}
				else
				{
					DCMemory.TextOut(m_textPos.x, m_textPos.y, strText);
				}
			}
		}
		break;
	case HOVER:
		{
			if(!m_imgNormal.IsNull())
			{
				m_imgNormal.TransparentBlt(DCMemory.m_hDC, cliRect.left, cliRect.top, m_nImageWidth, m_nImageHeight, m_nImageWidth, 0, m_nImageWidth, m_nImageHeight, m_bgDraw);
			}
			if (strText.GetLength() > 0)
			{
				DCMemory.SetTextColor(m_fg);
				if (m_textPos.x == 0)
				{
					DCMemory.DrawText(strText, strText.GetLength(), cliRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				}
				else
				{
					DCMemory.TextOut(m_textPos.x, m_textPos.y, strText);
				}
			}
		}
		break;
	case DOWN:
		{
			if(!m_imgNormal.IsNull())
			{
				if(m_bStretchBlt)
				{
					m_imgNormal.TransparentBlt(DCMemory.m_hDC, cliRect.left+1, cliRect.top+1, m_nImageWidth, m_nImageHeight, 2*m_nImageWidth, 0, m_nImageWidth, m_nImageHeight, m_bgDraw);
				}
				else
				{
					m_imgNormal.TransparentBlt(DCMemory.m_hDC, cliRect.left, cliRect.top, m_nImageWidth, m_nImageHeight, 2*m_nImageWidth, 0, m_nImageWidth, m_nImageHeight, m_bgDraw);
				}
			}
			if (strText.GetLength() > 0)
			{
				DCMemory.SetTextColor(m_fg);
				if (m_textPos.x == 0)
				{
					DCMemory.DrawText(strText, strText.GetLength(), cliRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				}
				else
				{
					DCMemory.TextOut(m_textPos.x, m_textPos.y, strText);
				}
			}
		}
		break;
	case DISABLE:
		if(!m_imgNormal.IsNull())
		{
			m_imgNormal.TransparentBlt(DCMemory.m_hDC, cliRect.left, cliRect.top, m_nImageWidth, m_nImageHeight, 3*m_nImageWidth, 0, m_nImageWidth, m_nImageHeight, m_bgDraw);
		}
		if (strText.GetLength() > 0)
		{
			DCMemory.SetTextColor(RGB(180, 180, 180));
			if (m_textPos.x == 0)
			{
				DCMemory.DrawText(strText, strText.GetLength(), cliRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
			else
			{
				DCMemory.TextOut(m_textPos.x, m_textPos.y, strText);
			}
		}
		break;
	case FLASH:
		{
			if(!m_imgNormal.IsNull())
			{
				m_imgNormal.TransparentBlt(DCMemory.m_hDC, cliRect.left, cliRect.top, m_nImageWidth, m_nImageHeight, (5+m_nFlashIndex)*m_nImageWidth, 0, m_nImageWidth, m_nImageHeight, m_bgDraw);
			}
		}
		break;
	default:
		break;
	}

	dc.BitBlt(cliRect.left, cliRect.top, cliRect.Width(), cliRect.Height(), &DCMemory, 0, 0, SRCCOPY);
	DCMemory.SelectObject(pOldFont);
	DCMemory.SelectObject(pOldBitmapScreen);
	fontBtn.DeleteObject();
	btScreen.DeleteObject();
	DCMemory.DeleteDC();
}

void BaseNewButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_bMouseOver && m_bEnabled)
	{
		m_bMouseOver = TRUE;
		m_state      = HOVER;

		CPoint	ptMouse;
		CRect	winRect;
		GetCursorPos(&ptMouse);
		GetWindowRect(&winRect);	
		if (!winRect.PtInRect(ptMouse) && m_bMouseOver && m_bEnabled)
		{
			SetTimer(1,10,NULL);
			return;
		}
		DrawButton();
		SetTimer(1,10,NULL);
	}
	CButton::OnMouseMove(nFlags, point);
}

void BaseNewButton::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1000)
	{
		m_state = FLASH;
		m_nFlashIndex++;
		m_nFlashCount++;
		if (m_nFlashIndex >= 4)
		{
			m_nFlashIndex = 0;
		}
		if (m_nFlashCount > 10)
		{
			m_bMouseOver  = FALSE;
			m_state       = NORMAL;
			m_nFlashIndex = 0;
			m_nFlashCount = 0;
			KillTimer(1000);
			//GetParent()->SendMessage(WM_COMMAND, (WPARAM)GetDlgCtrlID(), (LPARAM)NULL);
		}
		DrawButton();
	}
	else if (nIDEvent == 1)
	{
		CPoint	ptMouse;
		CRect	winRect;
		GetCursorPos(&ptMouse);
		GetWindowRect(&winRect);	
		if (!winRect.PtInRect(ptMouse) && m_bMouseOver && m_bEnabled)
		{
			KillTimer(1);
			m_bMouseOver = FALSE;
			m_state      = NORMAL;
			DrawButton();
		}
	}
	CButton::OnTimer(nIDEvent);
}

void BaseNewButton::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(NULL, BS_OWNERDRAW); 
	CButton::PreSubclassWindow();
}

void BaseNewButton::SetWindowText(LPCTSTR lpszString)
{
	CButton::SetWindowText(lpszString);
	DrawButton();
}

void BaseNewButton::SetImage(CString strNormal, int nImageWidth, int nImageHeight)
{
	m_nImageWidth  = nImageWidth;
	m_nImageHeight = nImageHeight;
	if (!m_imgNormal.IsNull())
	{
		DeleteObject(m_imgNormal.Detach());
	}
	DrawButton();
}

void BaseNewButton::SetTextColor(COLORREF fgcolor,COLORREF bgcolor)
{
	m_fg = fgcolor;
	m_bg = bgcolor;
	DrawButton();
}

void BaseNewButton::SetTextPos(CPoint point)
{
	m_textPos = point;
	DrawButton();
}

void BaseNewButton::SetStretchBlt(BOOL bStretchBlt)
{
	m_bStretchBlt = bStretchBlt;
}

//闪动控钮
void BaseNewButton::SetFlashButton()
{
	SetTimer(1000, 200, NULL);
}

void BaseNewButton::SetImage(UINT nNormalID, int nImageWidth, int nImageHeight)
{
	m_nImageWidth  = nImageWidth;
	m_nImageHeight = nImageHeight;
	if (!m_imgNormal.IsNull())
	{
		DeleteObject(m_imgNormal.Detach());
	}
	m_imgNormal.LoadFromResource(AfxGetApp()->m_hInstance, nNormalID); 
	DrawButton();
}


BOOL BaseNewButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;// CButton::OnEraseBkgnd(pDC);
}

BOOL BaseNewButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HAND));
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	return TRUE;
	//return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void BaseNewButton::OnEnable(BOOL bEnable)
{
	CButton::OnEnable(bEnable);
	// TODO: 在此处添加消息处理程序代码
	if(bEnable)
	{
		m_bEnabled = TRUE;
	}
	else
	{
		m_bEnabled = FALSE;
	}
}

void BaseNewButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//GetParent()->SendMessage(WM_BUTTON_RIGHTCHICK, (WPARAM)GetDlgCtrlID(), (LPARAM)&m_hWnd);
	CButton::OnRButtonDown(nFlags, point);
}

/*
typedef struct tagDRAWITEMSTRUCT {
UINT   CtlType;                        //控件类型
UINT   CtlID;                          //控件ID
UINT   itemID;                         //菜单项、列表框或组合框中某一项的索引值
UINT   itemAction;                     //控件行为
UINT   itemState;                      //控件状态
HWND   hwndItem;                       //父窗口句柄或菜单句柄
HDC    hDC;                            //控件对应的绘图设备句柄
RECT   rcItem;                         //控件所占据的矩形区域
DWORD  itemData;                       //列表框或组合框中某一项的值
} DRAWITEMSTRUCT, *PDRAWITEMSTRUCT, *LPDRAWITEMSTRUCT;
*/