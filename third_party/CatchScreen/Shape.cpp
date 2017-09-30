// Shape.cpp : 实现文件
//

#include "stdafx.h"
#include "CatchScreen.h"
#include "Shape.h"


// CShape

CShape::CShape(COLORREF color, int size)
{
	m_col = color;
	m_size = size;
}

CShape::~CShape()
{
}


// CShape 成员函数
