#pragma once
#include "GKCallback.h"
#include <string>
using namespace std;

#define   GKClientExport   __declspec(dllexport)

void		GKClientExport	GKClient_SetHost(const wstring& json);
void		GKClientExport	GKClient_SetProxy(const wstring& json);
void		GKClientExport	GKClient_SetCallback(GKCallback* callback);
wstring		GKClientExport	GKClient_Login(const wstring& json);
void		GKClientExport	GKClient_Logout();
void		GKClientExport	GKClient_Exit();
void		GKClientExport	GKClient_ConnectionStatus(const int & status);
wstring		GKClientExport	GKClient_GetMounts();
wstring		GKClientExport	GKClient_GetMountData(const wstring& json);
void		GKClientExport	GKClient_UserMessageResult(const wstring& json,int error);
wstring		GKClientExport	GKClient_CefFunction(const HWND&  hwnd, const wstring& fuc, const int& num, const wstring& content, const wstring& contentsecend);