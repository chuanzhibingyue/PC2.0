// CatchScreenDlg.h : header file
//

#if !defined(AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_)
#define AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Resource.h"
#include "Shape.h"
#include "BaseTracker.h"
#include "Rectangle.h"
#include "Line.h"
#include "Ellipse.h"
#include "Curve.h"
#include "EditText.h"
#include "InputEdit.h"
#include "CatchScreenDll.h"
#include "WinSpy.h"

class CCatchScreenBarFrameWnd; 
class CShapeCtrlBarFrameWnd;
class ColorListBarFrameWnd;
class CFontCtrlBarFrameWnd;
class CFontSizeListBarFrameWnd;

#define WM_GETCOMMAND WM_USER + 1988

#define SHAPE_COLOR_LIST 0
#define FONT_COLOR_LIST 1
#define FONT_SIZE_LIST 2
/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg dialog

class CCatchScreenDlg : public CDialog
{
public:
	CCatchScreenDlg(void* userData, const wstring& path, CaptureScreenCallBack callBack, CWnd* pParent = NULL);	// standard constructor
	~CCatchScreenDlg();

	enum { IDD = IDD_CATCHSCREEN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	void DrawImage(CDC *pDC,CPoint point);         //在截图区域绘图
	void Reset();                                  //重新设置截图相关信息
	void DrawArrow(CDC *pDC,CPoint ptStart,CPoint ptEnd);  //画实心箭头
	void DrawCrossTip(HDC hDC);
	void DrawSubToolArrow(CDC *pDC, HWND wnd, bool bUp);

	void NormalizeRect(RECT* rect);

	void SetShapeSize(int val) { m_shapeSize = val; }
	void SetShapeColor(COLORREF val) { m_shapeColor = val; }

	void SetTextSize(int val) { m_textSize = val; }
	void SetTextColor(COLORREF val) { m_textColor = val; }

	int GetTextSize() const { return m_textSize; }
	COLORREF GetTextColor() const { return m_textColor; }

	void SetDeflateParam(const CRect& deflateRect) { m_winSpy.SetDeflateParam(deflateRect); }

	void HideAllBarWindow(bool onlySubWindow = false);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuCommand(UINT);
	afx_msg void OnPaint(void);
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnUpdateToolBar(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnGetCommond(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnDragEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	void ShowScreenShotBar(bool bShow);
	void ShowShapeCtrlBar(int xoffset);
	void ShowColorListBar();
	void ShowFontCtrlBar();
	void ShowFontSizeListBar();
	void ShowFontColorListBar();

	void ResetHoverOverState();
	void ExitCatchScreen();

	void HighDPIHandle();
	void HighDPIEnlarge(HWND hWnd);

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	HBITMAP CopyScreenToBitmap(LPRECT lpRect, BOOL copyToClipboard = FALSE);

	int getListWindow_Top(int listType) const;
	int getSubTooBarWindow_Top() const;
	int getToolBarWindow_Top() const;

public:
	CCatchScreenBarFrameWnd* m_pScreenShotBar;	//截图工具栏

	CShapeCtrlBarFrameWnd* m_pShapeCtrlBar;		  //图形控制栏
	ColorListBarFrameWnd* m_pShapeColorListDlg;   //图形颜色列表

	CFontCtrlBarFrameWnd* m_pFontCtrlbar;		  //字体控制栏
	ColorListBarFrameWnd* m_pFontColorListDlg;	  //字体颜色列表
	CFontSizeListBarFrameWnd* m_pFontSizeListDlg; //字体大小列表

	BaseTracker m_rectTracker;     //像皮筋类
	BOOL m_bFirstCapture;              //是否为首次截取

private:
	int m_shapeSize;
	COLORREF m_shapeColor;

	int m_textSize;
	COLORREF m_textColor;

	HBITMAP m_hDesktopBitmap;

	int m_xScreen;
	int m_yScreen;

	CInputEdit m_Edit;

	BOOL m_bShowMsg;                //显示截取矩形大小信息
	BOOL m_bCapturing;                   //是否为截取状态
	
	BOOL m_bQuit;                   //是否为退出
	BOOL m_bDrawImage;				//绘图类型，包括矩形，直线，箭头，曲线，文字等
	DWORD m_nDrawType;
	CPoint m_startPt;				//截取矩形左上角
	CPoint m_endPt;             //截取矩形右下角
	CPoint m_startPt2;			//绘图的左上角
	CPoint m_prePt;             //绘图上一个点
	
	//CMyTracker m_rectTracker2;
	CBrush m_brush;					//
	HCURSOR m_hCursor;              //光标
	CBitmap * m_pBitmap;            //背景位图
	CList<CShape*, CShape*>m_list;   //保存绘图
	CList<CPoint, CPoint>m_pointList;

	BOOL m_bBtnState[5];           //保存绘图中5个按钮的状态 false为正常态，true为下压态
	BOOL m_bFisrtDrawArrow;        //防止一开始就画2个实心箭头
	BOOL m_bMenuVisiable;
	CFont m_Font;
	CRgn m_rgn;						//背景擦除区域

	CString m_csFilePath;

	CaptureScreenCallBack m_fnCallBack;
	wstring m_filePath;
	void * m_userData;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	CSCWinSpy<CSCWinFilter> m_winSpy;
	CRect m_rcHover;
	CPoint m_lButtonDownPt;
};

float GetDeviceScaleFactor();
int LogicalToDevice(int value);

#endif // !defined(AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_)
#include "afxwin.h"
