#pragma once
#include "shape.h"

class CRectangle :
	public CShape
{
public:
	CRectangle(CPoint,CPoint, COLORREF color,int size);
	void Drawing(CDC *pDC);
	virtual ~CRectangle(void);
};
