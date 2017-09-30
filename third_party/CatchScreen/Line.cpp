#include "stdafx.h"
#include "Line.h"
#include "math.h"
CLine::CLine(CPoint ptStart,CPoint ptEnd,COLORREF color,int size)
	:CShape(color, size)
{
	m_startPt = ptStart;
	m_endPt = ptEnd;
}

CLine::~CLine(void)
{
}
void CLine::Drawing(CDC *pDC)
{
	ASSERT(pDC);
	double slopy , cosy , siny;
	double Par = 10.0;  //length of Arrow (>)
	slopy = atan2( double( m_startPt.y - m_endPt.y ),double( m_startPt.x - m_endPt.x ) );
	cosy = cos( slopy );
	siny = sin( slopy ); //need math.h for these functions

	//draw a line between the 2 endpoint
	Pen pen(Color(GetRValue(m_col), GetGValue(m_col), GetBValue(m_col)), REAL(m_size));
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.DrawLine(&pen, m_startPt.x, m_startPt.y, m_endPt.x, m_endPt.y);

	CPoint pt1( m_endPt.x + int( Par * cosy - ( Par / 2.0 * siny ) ),m_endPt.y + int( Par * siny + ( Par / 2.0 * cosy ) ));
	CPoint pt2(m_endPt.x + int( Par * cosy + Par / 2.0 * siny ),m_endPt.y - int( Par / 2.0 * cosy - Par * siny ));
	CPoint ptArray[3] = {m_endPt,pt1,pt2};

	pDC->Polygon(ptArray,3);
 	HRGN hRgn = CreatePolygonRgn(ptArray,3,ALTERNATE);
 	CBrush brush(m_col/*RGB(255,0,0)*/);//
 	CBrush *pBrush = pDC->SelectObject(&brush);
 	pDC->FillRgn(CRgn::FromHandle(hRgn),&brush);
 	pDC->SelectObject(pBrush);
 	DeleteObject(hRgn);
}

