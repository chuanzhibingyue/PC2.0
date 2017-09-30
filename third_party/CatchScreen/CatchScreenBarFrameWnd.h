#ifndef CATCHSCREENBARFRAMEWND_H
#define CATCHSCREENBARFRAMEWND_H

#include "ControlEx.h"
#include "CatchScreenDlg.h"
#include "CommonFrameWnd.h"

class CCatchScreenBarFrameWnd : public CommonBarFrameWnd
{
public:
	CCatchScreenBarFrameWnd(CCatchScreenDlg* dlg) 
		:CommonBarFrameWnd(dlg) {
	}

	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { };

	void Init();
	void Notify(DuiLib::TNotifyUI& msg);
	void SetRelatedWnd(HWND hWnd) { m_relatedWnd = hWnd; }

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	DuiLib::CButtonUI* m_pOkBtn;
	DuiLib::CButtonUI* m_pCancelBtn;
	DuiLib::CButtonUI* m_pSaveBtn;
	DuiLib::CButtonUI* m_pFontBtn;
	DuiLib::CButtonUI* m_pPenBtn;
	DuiLib::CButtonUI* m_pArrowBtn;
	DuiLib::CButtonUI* m_pCircleBtn;
	DuiLib::CButtonUI* m_pSquareBtn;
	DuiLib::CButtonUI* m_pRollback;

private:
	HWND m_relatedWnd;
};

#endif //CATCHSCREENBARFRAMEWND_H