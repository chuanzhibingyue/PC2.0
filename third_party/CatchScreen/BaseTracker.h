#pragma once
#ifndef _BASETRACKER_
#define _BASETRACKER_

#define CX_BORDER   1
#define CY_BORDER   1
#define WM_UPDATE_TOOLBAR (WM_USER+100)
#define WM_DRAG_END (WM_USER+101)
#define CRIT_RECTTRACKER    5
#define POINT_SIZE_OFFSET   3

void AFXAPI AfxLockGlobals(int nLockType);
void AFXAPI AfxUnlockGlobals(int nLockType);
void AFXAPI AfxDeleteObject(HGDIOBJ* pObject);

class BaseTracker
{
public:
	// Constructors
	BaseTracker();
	BaseTracker(LPCRECT lpSrcRect, UINT nStyle);
	virtual ~BaseTracker();

	// Style Flags
	enum StyleFlags
	{
		solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		resizeInside = 8, resizeOutside = 16, hatchInside = 32,
		resizeMiddle =80 //…Ë÷√÷–º‰
	};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	};

public:
	void Draw(CDC* pDC) const;
	void GetTrueRect(LPRECT lpTrueRect) const;
	void SetResizeCursor(UINT nID_N_S, UINT nID_W_E, UINT nID_NW_SE, UINT nID_NE_SW, UINT nIDMiddle);
	void CreatePen();
	void SetRectColor(COLORREF rectColor);
	void GetHandleRect(int nHandle, CRect* pHandleRect) const;
	void GetModifyPointers(int nHandle, int**ppx, int**ppy, int* px, int*py);
	void Construct();
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert =TRUE, CWnd* pWndClipTo = NULL);
	BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert = TRUE);
	BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point, CWnd* pWndClipTo);
	BOOL SetHandler(HWND hwnd);
	int  HitTestHandles(CPoint point) const;
	int  HitTest(CPoint point) const;
	int  NormalizeHit(int nHandle) const;

	void SetAllowDrag(BOOL bAllow) { m_bAllowDrag = bAllow; }
	BOOL IsAllowDrag() const { return m_bAllowDrag; }

	virtual void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);
	virtual void AdjustRect(int nHandle, LPRECT lpRect);
	virtual void OnChangedRect(const CRect& rectOld);
	virtual UINT GetHandleMask() const;
	virtual int  GetHandleSize(LPCRECT lpRect = NULL) const;

public:
	COLORREF m_rectColor;
	BOOL     m_bAllowInvert; 
	BOOL     m_bErase;
	BOOL     m_bFinalErase;
	CRect    m_rectLast;
	CSize    m_sizeLast;
	UINT     m_nStyle; 
	CRect    m_rect; 
	CSize    m_sizeMin;
	int      m_nHandleSize;
	HWND     m_hWnd;

private:
	BOOL     m_bAllowDrag;

};
#endif
