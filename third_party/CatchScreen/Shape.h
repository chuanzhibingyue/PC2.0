#pragma once

// CShape ÃüÁîÄ¿±ê

class CShape : public CObject
{
public:
	CShape(COLORREF color, int size);
	virtual ~CShape();
	virtual void Drawing(CDC *pDC) = 0;

	CPoint m_startPt;
	CPoint m_endPt;
	COLORREF m_col;
	int m_size;
};


