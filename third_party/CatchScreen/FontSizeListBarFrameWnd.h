#ifndef FONTSIZELISTBARFRAMEWND_H
#define FONTSIZELISTBARFRAMEWND_H

#include "ControlEx.h"
#include "CatchScreenDlg.h"
#include "CommonFrameWnd.h"

class CFontSizeListBarFrameWnd : public CommonBarFrameWnd
{
public:
	CFontSizeListBarFrameWnd(CCatchScreenDlg* dlg)
		:CommonBarFrameWnd(dlg) {
		}
	LPCTSTR GetWindowClassName() const { return _T("CFontSizeListBarFrameWnd"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { };

	void Init();

	void Notify(DuiLib::TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void DrawMouseHoverEffect(Gdiplus::Graphics& graphics, DuiLib::CButtonUI* button, const CPoint& cursorPt);
	void DrawSelectedButton(Gdiplus::Graphics& graphics);
	void DrawEffect(Gdiplus::Graphics& graphics, DuiLib::CButtonUI* button, const Color& round, const Color& fill, const Color& textcolor, const WCHAR* filename);

private:
	DuiLib::CButtonUI* m_pSize9Btn;	
	DuiLib::CButtonUI* m_pSize10Btn;	
	DuiLib::CButtonUI* m_pSize11Btn; 
	DuiLib::CButtonUI* m_pSize12Btn;      
	DuiLib::CButtonUI* m_pSize13Btn;   
	DuiLib::CButtonUI* m_pSize14Btn;   
	DuiLib::CButtonUI* m_pSize18Btn;
	DuiLib::CButtonUI* m_pSize24Btn;	
	DuiLib::CButtonUI* m_pSize36Btn;
	DuiLib::CButtonUI* m_pSize48Btn;   
	DuiLib::CButtonUI* m_pSize72Btn;  
	DuiLib::CButtonUI* m_pSize96Btn;
};

#endif //FONTSIZELISTBARFRAMEWND_H