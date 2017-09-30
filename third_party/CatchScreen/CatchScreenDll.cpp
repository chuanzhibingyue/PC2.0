
#include "stdafx.h"
#include "CatchScreenDll.h"
#include "CatchScreenDlg.h"

#include <GdiPlus.h>
#pragma comment(lib, "Gdiplus.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCatchScreenDlg *g_pMainDlg = NULL;
CRect g_deflateRect(0, 0, 0, 0);

CATCHSCREE_API int CaptureScreen(void* userData, const wstring& path, CaptureScreenCallBack callBack)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	
	if (g_pMainDlg != NULL && ::IsWindowVisible(g_pMainDlg->GetSafeHwnd()))
		return 0;

	if (g_pMainDlg != NULL)
	{
		if (::IsWindow(g_pMainDlg->GetSafeHwnd()))
			g_pMainDlg->DestroyWindow();

		delete g_pMainDlg;
		g_pMainDlg = NULL;
	}

	g_pMainDlg = new CCatchScreenDlg(userData, path, callBack);
	g_pMainDlg->Create(IDD_CATCHSCREEN_DIALOG, NULL);
	g_pMainDlg->SetDeflateParam(g_deflateRect);
	g_pMainDlg->ShowWindow (SW_SHOWNORMAL);
	g_pMainDlg->UpdateWindow();

	return 0;
}

CATCHSCREE_API void CaptureScreen_SetDeflateParam(int left_deflate, int top_deflate, int right_deflate, int bottom_deflate)
{
	g_deflateRect = CRect(LogicalToDevice(left_deflate), LogicalToDevice(top_deflate), LogicalToDevice(right_deflate), LogicalToDevice(bottom_deflate));

	if (g_pMainDlg != NULL)
	{
		g_pMainDlg->SetDeflateParam(g_deflateRect);
	}
}
