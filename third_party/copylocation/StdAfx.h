// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__A7847D34_09F3_11D4_8F6D_0080AD87DD41__INCLUDED_)
#define AFX_STDAFX_H__A7847D34_09F3_11D4_8F6D_0080AD87DD41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <string>
#include <list>
typedef std::list<std::basic_string<TCHAR> > string_list;
typedef std::basic_string<TCHAR> tchar_string;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A7847D34_09F3_11D4_8F6D_0080AD87DD41__INCLUDED)
