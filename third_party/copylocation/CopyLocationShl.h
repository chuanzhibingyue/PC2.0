// CopyLocationShl.h : Declaration of the CCopyLocationShl

#ifndef __COPYLOCATIONSHL_H_
#define __COPYLOCATIONSHL_H_

#include "resource.h"       // main symbols
#include <shlobj.h>
#include <comdef.h>

/////////////////////////////////////////////////////////////////////////////
// CCopyLocationShl
class ATL_NO_VTABLE CCopyLocationShl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCopyLocationShl, &CLSID_CopyLocationShl>,
	public IDispatchImpl<ICopyLocationShl, &IID_ICopyLocationShl, &LIBID_COPYLOCATIONLib>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CCopyLocationShl();

DECLARE_REGISTRY_RESOURCEID(IDR_COPYLOCATIONSHL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCopyLocationShl)
	COM_INTERFACE_ENTRY(ICopyLocationShl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

// ICopyLocationShl
public:
    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    STDMETHOD(GetCommandString)(UINT_PTR, UINT, UINT*, LPSTR, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);

protected:
	void CopyFileName(tchar_string &name, const tchar_string &fullpath);
	std::wstring GetBeeInstallPath();

protected:
    string_list m_lsFiles;
	HBITMAP     m_hCopyBmp;
};

#endif //__COPYLOCATIONSHL_H_
