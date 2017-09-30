#ifndef SHARING_WND_H
#define SHARING_WND_H

#include "UIlib.h"
#include "ControlEx.h"

namespace ucclient {

	using namespace DuiLib;

	class CSharingWnd : public WindowImplBase
	{
	public:
		CSharingWnd() { };
		LPCTSTR GetWindowClassName() const { return _T("UISharingFrame"); };
		UINT GetClassStyle() const { return CS_DBLCLKS; };
		void OnFinalMessage(HWND /*hWnd*/) { delete this; };

		void Init();

		void OnPrepare() {}

		void Notify(TNotifyUI& msg);

		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) { return 0; }

		LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) { return 0; }

		LRESULT OnNcLButtonDBLCLK(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) { return 0; }

		LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		CDuiString GetSkinFile();

		CDuiString GetSkinFolder();

		void SetChildHwnd(HWND _hWnd);

	public:
		CPaintManagerUI m_pm;

	public:
		CButtonUI* m_pMaxBtn;
		CButtonUI* m_pRestoreBtn;
		CButtonUI* m_pMinBtn;
		CTextUI* m_pSharingName;
		HWND    _childWnd;
	};

}
#endif //SHARING_WND_H