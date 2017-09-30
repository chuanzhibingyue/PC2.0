#ifndef COLORLISTBARFRAMEWND_H
#define COLORLISTBARFRAMEWND_H

#include "ControlEx.h"
#include "CatchScreenDlg.h"
#include "CommonFrameWnd.h"

class ColorListBarFrameWnd : public CommonBarFrameWnd
{
public:
	ColorListBarFrameWnd(CCatchScreenDlg* dlg, int type) : CommonBarFrameWnd(dlg){
		m_colorType = type; 
	}
	LPCTSTR GetWindowClassName() const { return _T("ColorListBarFrameWnd"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { };

	void Init();

	void Notify(DuiLib::TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void DrawMouseHoverEffect(DuiLib::CButtonUI* button, const CPoint& cursorPt);

private:
	int m_colorType; //0: shape 1:font
	DuiLib::CButtonUI* m_pBrowNBtn;		//BROWN ÑÕÉ«button
	DuiLib::CButtonUI* m_pWhiteBtn;		//WHITE ÑÕÉ«button
	DuiLib::CButtonUI* m_pLawngreenBtn; //LAWNGREEN
	DuiLib::CButtonUI* m_pNavyBtn;      //NAVY
	DuiLib::CButtonUI* m_pAzureBtn;     //AZURE
	DuiLib::CButtonUI* m_pYelloWBtn;    //YELLOW
	DuiLib::CButtonUI* m_pCyanBtn;		//CYAN
	DuiLib::CButtonUI* m_pPinkBtn;		//PINK
	DuiLib::CButtonUI* m_pRedBtn;		//RED
	DuiLib::CButtonUI* m_pBisqueBtn;    //BISQUE
	DuiLib::CButtonUI* m_pBlackBtn;      //Black
	DuiLib::CButtonUI* m_pDarkgreenBtn; //DARKGREEN
};

#endif //COLORLISTBARFRAMEWND_H