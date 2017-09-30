#ifndef SHAPECTRLBARFRAMEWND_H
#define SHAPECTRLBARFRAMEWND_H

#include "ControlEx.h"
#include "CatchScreenDlg.h"
#include "CommonFrameWnd.h"

class CShapeCtrlBarFrameWnd : public CommonBarFrameWnd
{
public:
	CShapeCtrlBarFrameWnd(CCatchScreenDlg* dlg)
		:CommonBarFrameWnd(dlg) {
	}

	LPCTSTR GetWindowClassName() const { return _T("CShapeCtrlBarFrameWnd"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { };

	void Init();

	void Notify(DuiLib::TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnBtnBkColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	DuiLib::CButtonUI* m_pColorListBtn;
	DuiLib::COptionUI* m_pSmallOpt;
	DuiLib::COptionUI* m_pMediumOpt;
	DuiLib::COptionUI* m_pLargeOpt;
};

#endif //CATCHSCREENBARFRAMEWND_H