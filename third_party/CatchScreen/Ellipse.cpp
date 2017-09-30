#include "stdafx.h"
#include "Ellipse.h"
#include <gdiplus.h>

CEllipse::CEllipse(CPoint ptStart,CPoint ptEnd,COLORREF color,int size)
	:CShape(color, size)
{
	m_startPt = ptStart;
	m_endPt = ptEnd;
}

CEllipse::~CEllipse(void)
{
}
void CEllipse::Drawing(CDC *pDC)
{
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Pen pen(Color(GetRValue(m_col), GetGValue(m_col), GetBValue(m_col)), REAL(m_size));
	graphics.DrawEllipse(&pen, m_startPt.x, m_startPt.y, m_endPt.x - m_startPt.x, m_endPt.y - m_startPt.y);

	return ;
}
