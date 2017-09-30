#include "stdafx.h"
#include "WinSpy.h"

HWND CSCWinSpy<CSCWinFilter>::m_hWndTarget = NULL;
std::vector<CSCWinInfo*> CSCWinSpy<CSCWinFilter>::m_arSnapshot;

CRect CSCWinSpy<CSCWinFilter>::m_deflateRect = 0;