#pragma once
#include "shape.h"

class CCurve :
	public CShape
{
public:
	CCurve(CList<CPoint,CPoint>*, COLORREF color,int size);
	~CCurve(void);
	void Drawing(CDC *pDC);
	CList<CPoint,CPoint>m_pointList;
};
