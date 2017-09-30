#pragma once
#include "includes.h"
#include "ThreadPool.h"

#define FT_CLTWND "FT_CLTWND"
#define WM_USER_CONN (WM_USER + 0x100)
#define WM_USER_SEND (WM_USER + 0x101)
#define WM_USER_ACCEPT (WM_USER + 0x102)

namespace uc {

	class IWndMsgSink{
	public:
		virtual void onWndMsgProc(UINT message, WPARAM wParam, LPARAM lParam, bool &ret) = 0;
	};

	class UCWndMsg
	{
	public:
		inline void setSink(IWndMsgSink *lp) { m_lpSink = lp; }
		inline IWndMsgSink* getSink() { return m_lpSink; }

		void openWndMsg(){

			_threadPool.Init();
			_threadPool.Schedule(MakeCallback(this, &UCWndMsg::threadProc));
		}

		void closeWndMsg(){

			PostMessage(m_hWnd, WM_QUIT, (WPARAM)1, (LPARAM)1);

			_threadPool.Wait();
			_threadPool.Shutdown();
		}

		bool postMsg(UINT Msg, WPARAM wParam, LPARAM lParam){

			return (TRUE == PostMessage(m_hWnd, Msg, wParam, lParam));
		}

		// inline HWND GetWnd() { return m_hWnd; }

	public:
		UCWndMsg(void) : _threadPool(1){
			m_lpSink = NULL;
			m_hWnd = NULL;
		}

		virtual ~UCWndMsg(void){

			DestoryWnd();
		}

	protected:
		void threadProc(){

			RegisterWindowClass(FT_CLTWND, NULL);
			bool ret = CreateWnd(NULL, FT_CLTWND, FT_CLTWND);

			MSG msg;
			while (GetMessage(&msg, 0, 0, 0))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			DestoryWnd();
			UnregisterWindowClass(FT_CLTWND, NULL);
		}

	protected:
		static ATOM RegisterWindowClass(const TCHAR * lpClassName, HINSTANCE hInst,
			UINT style = CS_HREDRAW | CS_VREDRAW, int cbClsExtra = 0, int cbWndExtra = 0, 
			HICON hIcon = NULL, HCURSOR hCursor = NULL, HBRUSH hbrBackground = NULL,
			const TCHAR * lpszMenuName = NULL, HICON hIconSm = NULL){

				WNDCLASSEX wcex;
				wcex.cbSize = sizeof(WNDCLASSEX); 
				wcex.style			= style;
				wcex.lpfnWndProc	= (WNDPROC)WndProc;
				wcex.cbClsExtra		= cbClsExtra;
				wcex.cbWndExtra		= cbWndExtra;
				wcex.hInstance		= hInst;
				wcex.hIcon			= hIcon;
				wcex.hCursor		= hCursor;
				wcex.hbrBackground	= hbrBackground;
				wcex.lpszMenuName	= lpszMenuName;
				wcex.lpszClassName	= lpClassName;
				wcex.hIconSm		= hIconSm;

				return RegisterClassEx(&wcex);
		}

		bool CreateWnd(HINSTANCE hInst, const TCHAR * lpClassName, const TCHAR * lpszWndName, 
			DWORD dwStyle = WS_OVERLAPPEDWINDOW, int x = 0, int y = 0, int nWidth = 0, int nHeight = 0, 
			HWND hParent = NULL, HMENU hMenu = NULL, LPVOID lpParam = NULL) {

				if (NULL == m_hWnd) {
					m_hWnd = CreateWindow(lpClassName, lpszWndName, dwStyle,
						x, y, nWidth, nHeight, hParent, hMenu, hInst, this);
				}

				return (NULL != m_hWnd);
		}

		bool DestoryWnd(){

			if (NULL == m_hWnd) {
				DestroyWindow(m_hWnd);
				m_hWnd = NULL;
				return true;
			}

			return false;
		}

		static BOOL UnregisterWindowClass(const TCHAR * lpClassName, HINSTANCE hInst){

			return UnregisterClass(lpClassName, hInst);
		}

	protected:
		bool CreateWndEx(HINSTANCE hInst, DWORD dwExStyle, const TCHAR * lpClassName, const TCHAR * lpszWndName, 
			DWORD dwStyle = WS_OVERLAPPEDWINDOW, int x = 0, int y = 0, int nWidth = 0, int nHeight = 0, 
			HWND hParent = NULL, HMENU hMenu = NULL, LPVOID lpParam = NULL){

				if (NULL == m_hWnd) {
					m_hWnd = CreateWindowEx(dwExStyle, lpClassName, lpszWndName, dwStyle,
						x, y, nWidth, nHeight, hParent, hMenu, hInst, this);
				}

				return (m_hWnd != NULL);
		}

	protected:
		virtual bool WindowProc(UINT message, WPARAM wParam, LPARAM lParam){ 

			bool ret = false;
			if (NULL != m_lpSink) {
				m_lpSink->onWndMsgProc(message, wParam, lParam, ret);
			}

			return ret;
		}

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

			UCWndMsg* pThis = NULL;
			if( message == WM_NCCREATE ) {
				LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
				pThis = (UCWndMsg*)lpcs->lpCreateParams;
				pThis->m_hWnd = hWnd;
				::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LPARAM)pThis);
			} else {
				pThis = (UCWndMsg*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
				if (message == WM_NCDESTROY && NULL != pThis) {
					LRESULT lRes = ::DefWindowProc(hWnd, message, wParam, lParam);
					::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
					pThis->m_hWnd = NULL;
					return lRes;
				}
			}

			if (NULL != pThis && pThis->WindowProc(message, wParam, lParam)) {
				return TRUE;
			}

			return ::DefWindowProc(hWnd, message, wParam, lParam);
		}

	private:
		ThreadPool _threadPool;
		HWND m_hWnd;
		IWndMsgSink *m_lpSink;
	};

}
