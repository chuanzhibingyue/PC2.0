#pragma once
#include "shape.h"

class CEllipse :
	public CShape
{
public:
	CEllipse(CPoint,CPoint,COLORREF color,int size);
	void Drawing(CDC *pDC);
	virtual ~CEllipse(void);
};
