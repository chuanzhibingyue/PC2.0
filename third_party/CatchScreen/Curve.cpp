#include "stdafx.h"
#include "Curve.h"

CCurve::CCurve(CList<CPoint,CPoint>*pointList,COLORREF color,int size)
	:CShape(color, size)
{
	ASSERT(pointList);
	POSITION pos = pointList->GetHeadPosition();
	while (NULL != pos)
	{
		m_pointList.AddTail(pointList->GetNext(pos) );

	}
}

CCurve::~CCurve(void)
{
}
void CCurve::Drawing(CDC *pDC)
{
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Pen pen(Color(GetRValue(m_col), GetGValue(m_col), GetBValue(m_col)), REAL(m_size));
	
	POSITION pos = m_pointList.GetHeadPosition();
	Point* pointArray = new Point[m_pointList.GetCount()];
	for (int i = 0; i < m_pointList.GetCount() - 1; i++)
	{
		CPoint point = m_pointList.GetNext(pos);
		pointArray[i] = Point(point.x, point.y);
	}
	graphics.DrawLines(&pen, pointArray, m_pointList.GetCount() - 1);
	delete[] pointArray;
}
