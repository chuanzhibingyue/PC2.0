// InputEdit.cpp : 实现文件
//

#include "stdafx.h"
/*#include "CatchScreen.h"*/
#include "InputEdit.h"
#include "CatchScreenDlg.h"


// CInputEdit

IMPLEMENT_DYNAMIC(CInputEdit, CEdit)

CInputEdit::CInputEdit()
{
	m_bDrag = FALSE;
	m_rect = 0;
	m_pFont = new CFont();
	m_pFont->CreateFont(
		-12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		GB2312_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体") ) ;                 // lpszFacename
}

CInputEdit::~CInputEdit()
{
	m_pFont->DeleteObject();
	delete m_pFont;
}


BEGIN_MESSAGE_MAP(CInputEdit, CEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(EN_CHANGE, &CInputEdit::OnEnChange)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CInputEdit 消息处理程序



void CInputEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	m_bDrag = TRUE;

	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	CPoint point1 = point;
	ClientToScreen(&point1);
	GetParent()->ScreenToClient(&point1);

	int nWidth = rc.Width();
	int nHeight = rc.Height();

	m_ndist_left=point1.x-rc.left;
	m_ndist_top=point1.y-rc.top;
	m_ndist_right=rc.right-point1.x;
	m_ndist_bottom=rc.bottom-point1.y;
	CEdit::OnLButtonDown(nFlags, point);
}

void CInputEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bDrag = FALSE;
	CEdit::OnLButtonUp(nFlags, point);
}

void CInputEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_bDrag)
	{
		CRect rc;
		GetWindowRect(&rc);
		GetParent()->ScreenToClient(&rc);

		CPoint point1 = point;
		ClientToScreen(&point1);
		GetParent()->ScreenToClient(&point1);

		int nWidth = rc.Width();
		int nHeight = rc.Height();
		
		//限制移动范围
		point1.x = point1.x>=m_rect.right?m_rect.right:point1.x;
		point1.x = point1.x<=m_rect.left?m_rect.left:point1.x;
		point1.y = point1.y>=m_rect.bottom?m_rect.bottom:point1.y;
		point1.y = point1.y<=m_rect.top?m_rect.top:point1.y;


		rc.left = point1.x-m_ndist_left;
		rc.top = point1.y-m_ndist_top;
		rc.right = point1.x+m_ndist_right;
		rc.bottom = point1.y+m_ndist_bottom;

		if (rc.left < m_rect.left)
		{
			rc.left = m_rect.left + 1;
			rc.right = nWidth + rc.left;
		}
		if (rc.right > m_rect.right)
		{
			rc.right = m_rect.right -1;
			rc.left = rc.right - nWidth;
		}
		if (rc.top < m_rect.top)
		{
			rc.top = m_rect.top + 1;
			rc.bottom = rc.top + nHeight;
		}
		if (rc.bottom > m_rect.bottom)
		{
			rc.bottom = m_rect.bottom - 1;
			rc.top = rc.bottom - nHeight;
		}


		MoveWindow(&rc);
		GetParent()->InvalidateRect(&m_rect);

		SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL) ) );

	}
	else
	{
		SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW) ) );
	}
	CEdit::OnMouseMove(nFlags, point);
}

void CInputEdit::SetMoveRect(CRect rc)
{
	m_rect = rc;
}
void CInputEdit::OnEnChange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CEdit::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	int nIndex = GetLineCount();
	TCHAR szContent[100] = {'\0'};

	int   n   =  GetLine(nIndex -1,   szContent,100);   
	if   (n> 0)   
	{   
		szContent[n]   =   0;   
	} 

	CDC *pDC = GetDC();
	CFont *pFont = GetFont();
	CFont *pOldFont = pDC->SelectObject(pFont);

	m_pFont->Detach();
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); 
	lf.lfHeight = m_pDlg->GetTextSize();
	_tcscpy(lf.lfFaceName, _T("宋体"));
	m_pFont->CreateFontIndirect(&lf);
	SetFont(m_pFont);

	//以CSize的形式返回字符串的高度和宽度
	CSize size = pDC->GetTextExtent(szContent);
	CSize szExtra(m_pDlg->GetTextSize(), m_pDlg->GetTextSize());

	// 释放
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	CRect rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);

	if (rect.bottom + szExtra.cy<m_rect.bottom)
	{
		rect.bottom=rect.top+(nIndex+1)*szExtra.cy;
	}

	if (size.cx + 2*szExtra.cx >rect.Width() )
	{
		rect.right = rect.left + rect.Width() + 2*szExtra.cx;
	}

	//限制EDIT矩形不超出截图矩形的范围
	if (rect.right >m_rect.right)
	{
		rect.right = m_rect.right - 3;
	}
	if (rect.bottom >m_rect.bottom)
	{
		rect.bottom = m_rect.bottom;
	}

	MoveWindow(&rect);
	// TODO:  在此添加控件通知处理程序代码
}




BOOL CInputEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect cliRect,rc;
	GetClientRect(&rc);
	cliRect = rc;
	cliRect.top -=1;
	cliRect.left -=1 ;
	cliRect.right +=1;
	cliRect.bottom +=1;
	//pDC->FillSolidRect(cliRect, /*m_clrBKLine*/RGB(255,0,0) );
	//cliRect.top +=1;
	//cliRect.left +=1;
	//cliRect.right -=1;
	//cliRect.bottom -=1;
	//pDC->FillSolidRect(cliRect, m_clrBKIn);

	CPen pen(PS_DASH,1,RGB(0,0,255) );
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(&cliRect);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	return true;

	return CEdit::OnEraseBkgnd(pDC);
}

BOOL CInputEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//当EDIT中的文字已经显示到截图区域的底部时，再按回车禁止换行
	if(WM_KEYDOWN   ==   pMsg-> message) 
	{ 
		if( (13 == pMsg->wParam) ) //enter  
		{
			CDC *pDC = GetDC();
			CFont *pFont = GetFont();
			CFont *pOldFont = pDC->SelectObject(pFont);

			//以CSize的形式返回字符串的高度和宽度
			CString str = _T("啊");
			CSize szExtra = pDC->GetTextExtent(str);

			// 释放
			pDC->SelectObject(pOldFont);

			CRect rect;
			GetWindowRect(&rect);
			GetParent()->ScreenToClient(&rect);

			if (rect.bottom + szExtra.cy>m_rect.bottom)
			{
				return   TRUE;  //一定要返回true
			}
		}
	} 
	return CEdit::PreTranslateMessage(pMsg);
}

HBRUSH CInputEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor != CTLCOLOR_EDIT)
	{
		return NULL;
	}
	pDC->SetTextColor( m_pDlg->GetTextColor()/*RGB(255,0,0) */);
	//CFont *pOldFont = pDC->SelectObject(m_pFont);

	return ::GetSysColorBrush(COLOR_WINDOW);
}
