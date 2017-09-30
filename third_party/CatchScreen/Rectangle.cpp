#include "stdafx.h"
#include "Rectangle.h"

CRectangle::CRectangle(CPoint ptStart,CPoint ptEnd, COLORREF color,int size)
	:CShape(color, size)
{
	m_startPt = ptStart;
	m_endPt = ptEnd;
}

CRectangle::~CRectangle(void)
{
}
void CRectangle::Drawing(CDC *pDC)
{
	CBrush *pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(CRect(m_startPt,m_endPt));
	pDC->SelectObject(pOldBrush);
    return ;
}
