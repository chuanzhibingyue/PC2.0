#pragma once

class CCatchScreenDlg;
// CInputEdit

class CInputEdit : public CEdit
{
	DECLARE_DYNAMIC(CInputEdit)

public:
	CInputEdit();
	virtual ~CInputEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void SetMoveRect(CRect rc);

	CRect m_rect;
	BOOL m_bDrag;

	int m_ndist_left;
	int m_ndist_top;
	int m_ndist_right;
	int m_ndist_bottom;
	CCatchScreenDlg* m_pDlg;
	CFont *m_pFont;
	afx_msg void OnEnChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);

	void SetScreenDlg(CCatchScreenDlg* dlg) { m_pDlg = dlg; }
};


