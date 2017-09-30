#pragma once
#include "shape.h"

class CLine :
	public CShape
{
public:
	CLine(CPoint,CPoint,COLORREF color,int size);
	void Drawing(CDC *pDC);
	virtual ~CLine(void);
};
