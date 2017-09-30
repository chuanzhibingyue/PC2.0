#if !defined(WINFILTER__H__)
#define WINFILTER__H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>

class CSCWinFilter
{
public:
    static BOOL IsFilterWindow(HWND hWnd)
    {
		if (!::IsWindowVisible(hWnd))
			return TRUE;

		HWND hRootParent = ::GetParent(hWnd);
		
		int recursive_depth = 5;

		do
		{
			CRect rectParent(0, 0, 0, 0);
			if (::GetWindowRect(hRootParent, &rectParent) && rectParent.Width() * rectParent.Height() == 0)
			{
				return TRUE;
			}

		} while((hRootParent = ::GetParent(hRootParent)) && recursive_depth-- > 0);

		return FALSE;
    }

	static BOOL IsInCurrentProcess(HWND hWnd)
	{
		if (hWnd == NULL)
			return FALSE;

		DWORD dwProcessID = GetCurrentProcessId();
		if (hWnd != NULL && IsWindow(hWnd))
		{
			DWORD dwWinProcessId(0);
			GetWindowThreadProcessId(hWnd, &dwWinProcessId);
			if (dwProcessID == dwWinProcessId)
			{
				return TRUE;
			}
		}

		return FALSE;
	}
    
    static DWORD GetIncludeStyle()
    {
        return WS_VISIBLE;
    }
    
    static DWORD GetExcludeStyleEx()
    {
        return  WS_EX_TRANSPARENT;
    }
    
    static BOOL IsTargetPopupWindow()
    {
        return FALSE;
    }
};

class CSCWinInfo
{
public:
    HWND m_hWnd;    
    CRect m_rtWin;    //window rect
                    
    INT m_nLevel;    // 1 - pop up window  ;  2N - child window
};

//pop up win 1 (level 1).. first Z order
//        child11 (level 2)
//        child12 (level 2)
//                chilld121 (level 3)
//                chilld122 (level 3)
//                
//        child3 (level 2)
//pop up win2
//        child21 (level 2)
//        child21 (level 2)
// .
// .
//pop up winN . last Z order
template<typename CWinFilterTraits = CSCWinFilter, int max_window_depth = 6>
class CSCWinSpy
{
public:
	CSCWinSpy() { NULL; }
	~CSCWinSpy() { ClearData(); }

    BOOL SnapshotAllWinRect()
    {
        ClearData();

        // cache current window Z order when call this function
        EnumWindows(EnumWindowsSnapshotProc, 1); 
        
        return TRUE;
    }

	static VOID ClearData()
	{
		int nCount = m_arSnapshot.size();
		for (int i = 0; i < nCount; ++i)
		{
			delete m_arSnapshot[i];
		}

		m_arSnapshot.clear();
	}
    
    //get from current Z order of desktop
    HWND GetHWNDByPoint(CPoint pt)
    {
        m_hWndTarget = NULL;
        
        EnumWindows(EnumWindowsRealTimeProc, MAKELPARAM(pt.x, pt.y));
        
        return m_hWndTarget;
    }

	BOOL IsInCurrentProcess(CPoint ptHit, BOOL bGetInRealTime = FALSE)
	{
		HWND hWndTarget = NULL;
		if (bGetInRealTime) //get from current Z order
		{
			hWndTarget = GetHWNDByPoint(ptHit);
		}
		else //get from snapshot cache
		{
			CRect rtRect(0, 0, 0, 0);
			GetRectByPointFromSnapshot(ptHit, rtRect, hWndTarget);
		}

		return CWinFilterTraits::IsInCurrentProcess(hWndTarget);
	}
    
    CRect GetWinRectByPoint(CPoint ptHit, BOOL bGetInRealTime = FALSE)
    {
		BOOL bGet = FALSE;
		HWND hWndTarget = NULL;
        CRect rtRect(0, 0, 0, 0);
        if(bGetInRealTime) //get from current Z order
        {
            hWndTarget = GetHWNDByPoint(ptHit);
            if(hWndTarget != NULL )
            {
				bGet = GetWindowRect(hWndTarget, &rtRect);
            }
        }
        else //get from snapshot cache
        {
			bGet = GetRectByPointFromSnapshot(ptHit, rtRect, hWndTarget);
        }

		if (bGet)
		{
			if (CWinFilterTraits::IsInCurrentProcess(hWndTarget))
			{
				rtRect.DeflateRect(&m_deflateRect);
			}

			CRect rcDesktop;
			::GetWindowRect(::GetDesktopWindow(), &rcDesktop);
			if (rcDesktop.left >= rtRect.left)
				rtRect.left = rcDesktop.left + 2;
			if (rcDesktop.top >= rtRect.top)
				rtRect.top = rcDesktop.top + 2;
			if (rcDesktop.right <= rtRect.right)
				rtRect.right = rcDesktop.right - 2;
			if (rcDesktop.bottom <= rtRect.bottom)
				rtRect.bottom = rcDesktop.bottom - 2;
		}
        
        return rtRect;
    }

	void SetDeflateParam(const CRect& deflateRect)
	{
		m_deflateRect = deflateRect;
	}
    
protected:
    static BOOL CALLBACK EnumWindowsRealTimeProc(HWND hwnd, LPARAM lParam)
    {
        if(!PtInWinRect(hwnd, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))) return TRUE;
        
        if(ShouldWinBeFiltered(hwnd))  return TRUE;
        
        m_hWndTarget = hwnd;
        
        if(CWinFilterTraits::IsTargetPopupWindow()) return FALSE; //this is the target window, exit search
        
        EnumChildWindows(hwnd, EnumChildRealTimeProc, lParam);
        
        return FALSE;
    }
    
    static BOOL CALLBACK EnumChildRealTimeProc(HWND hwnd, LPARAM lParam)
    {
        if(!PtInWinRect(hwnd, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))) return TRUE;
        
        if(ShouldWinBeFiltered(hwnd)) return TRUE;
        
        m_hWndTarget = hwnd;
        EnumChildWindows(hwnd, EnumChildRealTimeProc, lParam);
        
        return FALSE;
    }
    
protected:
    static BOOL CALLBACK EnumWindowsSnapshotProc(HWND hwnd, LPARAM lParam)
    {
        INT nLevel = lParam;
        if(ShouldWinBeFiltered(hwnd))  return TRUE;
        
        SaveSnapshotWindow(hwnd, nLevel);
        
        if(!CWinFilterTraits::IsTargetPopupWindow())
        {
            ++nLevel;
            EnumChildWindows(hwnd, EnumChildSnapshotProc, nLevel);
        }
        
        return TRUE;
    }
    
    static BOOL CALLBACK EnumChildSnapshotProc(HWND hwnd, LPARAM lParam)
    {
        INT nLevel = lParam;
        
        if(ShouldWinBeFiltered(hwnd)) return TRUE;
        
        SaveSnapshotWindow(hwnd, nLevel);
        
		if (nLevel > max_window_depth)
			return FALSE;

        ++nLevel;
        EnumChildWindows(hwnd, EnumChildSnapshotProc, nLevel);
        
        return TRUE;
    }
    
protected:
    static BOOL PtInWinRect(HWND hWnd, CPoint pt)
    {
        CRect rtWin(0, 0, 0, 0);
        GetWindowRect(hWnd, &rtWin);
        return PtInRect(&rtWin, pt);
    }
    
    static BOOL ShouldWinBeFiltered(HWND hWnd)
    {
        if(CWinFilterTraits::IsFilterWindow(hWnd)) return TRUE;
        
        DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
        DWORD dwStyleMust = CWinFilterTraits::GetIncludeStyle();
        if((dwStyle & dwStyleMust) != dwStyleMust) return TRUE;
        
        DWORD dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
        DWORD dwStyleMustNot = CWinFilterTraits::GetExcludeStyleEx();
        if((dwStyleMustNot & dwStyleEx) != 0) return TRUE;
        
        return FALSE;
    }
    
    //find the first window that level is biggest
    static BOOL  GetRectByPointFromSnapshot(CPoint ptHit, CRect& rtRet, HWND &hWndTarget)
    {
		hWndTarget = NULL;
        int nCount = m_arSnapshot.size();
		if (nCount == 0)
			return FALSE;

        CSCWinInfo* pInfo = NULL;
        CSCWinInfo* pTarget = NULL; 
        
        for(int i=0; i<nCount; ++i)
        {
            pInfo = m_arSnapshot[i];
            _ASSERTE(pInfo != NULL);
            
            //target window is found 
            //and level is not increasing, 
            //that is checking its sibling or parent window, exit search
            if(pTarget != NULL
                && pInfo->m_nLevel <= pTarget->m_nLevel)
            {
                break;
            }
            
            if(PtInRect(&pInfo->m_rtWin, ptHit))
            {
                if(pTarget == NULL)
                {
                    pTarget = pInfo;
                }
                else
                {
                    if( pInfo->m_nLevel > pTarget->m_nLevel)
                    {
                        pTarget = pInfo;
                    }
                }
            }
        }
        
        if(pTarget != NULL)
        {
#ifdef _DEBUG
			HWND hWnd = pTarget->m_hWnd;
			TCHAR szText[128] = { 0 };
			_sntprintf(szText, 127, _T("GetRectByPointFromSnapshot: pt(%d, %d), hWnd(%x)"),
				ptHit.x, ptHit.y, (UINT)(pInfo->m_hWnd));
			OutputDebugString(szText);
#endif

            rtRet.CopyRect(&pTarget->m_rtWin);
			hWndTarget = pTarget->m_hWnd;
            return TRUE;
        }
        
        return FALSE;
    }
    
    static VOID SaveSnapshotWindow(HWND hWnd, INT nLevel)
    {
        _ASSERTE(hWnd != NULL && IsWindow(hWnd));
        CRect rtWin(0, 0, 0, 0);
        GetWindowRect(hWnd, &rtWin);
        if(rtWin.IsRectEmpty()) return;
        
        CSCWinInfo* pInfo = new CSCWinInfo;
        if(pInfo == NULL) return;
        
        pInfo->m_hWnd = hWnd;
        pInfo->m_nLevel = nLevel;
        pInfo->m_rtWin = rtWin;
        
        m_arSnapshot.push_back(pInfo);
    }
    
private:
    static HWND m_hWndTarget;
    static std::vector<CSCWinInfo*> m_arSnapshot;
	static CRect m_deflateRect;
};

#endif
