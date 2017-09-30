#ifndef COMMONFRAMEWND_H
#define COMMONFRAMEWND_H

#include "ControlEx.h"
#include "CatchScreenDlg.h"

#define OFFSET_X 2  
#define OFFSET_Y 2  

void DrawRoundRectange(Graphics &g, Color pens, int x, int y, int width, int height);
void FillRoundRectangle(Graphics &g, Color color, int x, int y, int width, int height);

class CommonBarFrameWnd : public DuiLib::CWindowWnd, public DuiLib::INotifyUI
{
public:
	CommonBarFrameWnd(CCatchScreenDlg* dlg) { m_pCatchScreenDlg = dlg; m_relatedWnd = 0; };
	virtual ~CommonBarFrameWnd() {}

	void SetRelatedWnd(HWND hWnd) { m_relatedWnd = hWnd; }
	void OnPrepare();

protected:
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	DuiLib::CPaintManagerUI m_pm;
	CCatchScreenDlg *m_pCatchScreenDlg;
	HWND m_relatedWnd;
};

#endif //COMMONFRAMEWND_H