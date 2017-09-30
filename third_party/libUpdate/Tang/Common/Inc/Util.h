/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file Util.h
* @brief Implemente common function for tang client,such as string parse,
* window sontrol creation and so on.
* 
* @author hongwei hu (hongwei.hu@gnetis.com)
*
* @date March. 8 2010
*
* @version 1.0.0
* 
* @warning 
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug
******************************************************************************/

#ifndef GNET_TANG_CLIENT_COMMON_UTILITY_H
#define GNET_TANG_CLIENT_COMMON_UTILITY_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <list>
#include <string>
#include <comdef.h>
#include <conferencedef.h>
#include "Commonlib.h"
using namespace std;

/**
* @brief parse the command line string to identify whether application is start normally.
* @param [in] lpstrCmdLine. command line string.
*
* @return BOOL return True if successful,otherwise return False.
*/
BOOL IsAppStartNormal(
                      LPTSTR cmdLineString,
                      TCHAR** ppStrMappingName,
					  UINT32 &uInvokeMode
                      );

/**
* @brief parse tang client/conference run mode.
* @param [in] lpstrCmdLine. the command line string.
* @param [in] confMode. the parsing result.
* @return int return S_OK if successful,otherwise return error code.
*/
HRESULT ParseConferenceMode(
                            LPTSTR cmdLineString,
                            AppExecuteMode &confMode
                            );

/**
* @brief parse the login url of the login mode.
* @param [in] lpstrCmdLine. the command line string. like tang:// /run_mode:loginmode /login_url:www.quanshi.com /
* @param [in] strloginUrl. the parsing result.
* @return int return S_OK if successful,otherwise return error code.
*/
HRESULT ParseLoginUrl(
                      LPTSTR cmdLineString,
                      TCHAR** ppStrloginUrl
                      );


/**
* @brief parse the user's id from input string.
* @param [in] bstrUserID. the input string.
* @param [in out] userIDList. user id list. the parse result.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT ParseUint32ByString(
							__in BSTR bstrUserID,
							__inout  std::vector<UINT32> &userIDList
							);

/**
* @brief parse the user's id from input string.
* @param [in] bstrUserID. the input string.
* @param [in out] userIDList. user id list. the parse result.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT ParseUint32ByJSArray(
						   __in IDispatch *pDispatch,
						   __inout  std::vector<UINT32> &userIDList
						   );

/**
* @brief create a windows object on a position(div or other html container).
* Invoker can get the hwnd of the object created just now.
* @param [in] pUnknown. the position of html UI page.
* @param [in out] pHwnd. the hwnd of create windows object.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT RegisterWinObject(
						  __in void *pComObj,
						  __inout IUnknown **ppUnknown,
						  __inout DWORD *pCookie
						);

/**
* @brief create a windows object on a position(div or other html container).
* Invoker can get the hwnd of the object created just now.
* @param [in] pUnknown. the position of html UI page.
* @param [in out] pHwnd. the hwnd of create windows object.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT CreateWinObject(
						__in IUnknown *pUnknown,
						__inout HWND *pHwnd,
                        __in LPCTSTR szID = NULL,
						__inout HWND * pActiveWin = NULL,
						__inout ITangWinControl ** pTangWin = NULL,
						__in BOOL bCreateChildWindow = TRUE
						);

/**
* @brief this method used to change the size of created window control.
* @param [in] lHeight. the height value changed to.
* @param [in] lWidth. the new width value.
* @param [in] hWnd. the hwnd of create windows object.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT UpdateWinObjectSize(
							__in long lHeight,
							__in long lWidth,
							__in HWND hWnd
							);

/**
* @brief convert base64 string to unicode string.
* @param [in] b64String. the base64 string.
* @param [in] UnicodeString. result of unicode string.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT Base64ToString(
					   __in TCHAR *b64String,
					   __inout TCHAR **unicodeString
					   );

/**
* @brief convert unicode transformation string to unicode string.
* @param [in] pstrUtf. the UTF-8 string.
* @param [in][out] ppstrUnicode. result of unicode string.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT UTF8ToUnicode(
					   __in LPCSTR pstrUtf,
					   __inout LPWSTR *ppstrUnicode
					   );

/**
* @brief convert unicode string to UTF-8 string.
* @param [in] pstrUnicode. the unicode string.
* @param [in][out] ppstrUtf. result of UTF-8 string.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT UnicodeToUTF8(
					  __in LPWSTR pstrUnicode,
					  __inout LPSTR *ppstrUtf,
                      __inout UINT32 *pLength = NULL
					  );

/**
* @brief Dump the meory and cpu status;
* @return void
*/
void DumpCpuAndMemoryStatus();

/// 获取当前进程的cpu使用率，返回-1失败   
static int get_cpu_usage();   


/// 获取当前进程内存和虚拟内存使用量，返回-1失败，0成功   
static int get_memory_usage(UINT64 * mem, UINT64* vmem);   


/// 获取当前进程总共读和写的IO字节数，返回-1失败，0成功   
static int get_io_bytes(UINT64* read_bytes, UINT64* write_bytes);   

static int get_processor_number();

static int get_memory_usage(UINT64* mem, UINT64* vmem);

string& ReplaceAllStrDistinct(string& sourceStr,const  string&  old_value,const  string&  new_value);
/**
    * @brief	<b>Summary:</b>	 get the temp folder directory of the current system user 
    */
HRESULT GetCurrentSystemUserTempFolderDir(std::wstring & refStrTempDir);

/**
* @brief query xml node value.
* @param [in] lpstrXmlPath. xml document path.
* @param [in] lpstrNodePath. node path of xml.
* @param [in][out] ppstrNodeValue. result of node value.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT QueryXmlNode(
                      __in LPCTSTR lpstrXmlPath,
                      __in LPCTSTR lpstrNodePath,
                      __inout BSTR* ppstrNodeValue
                      );

char* Encode64(const char* buffer, int buflen);
char* Decode64(const char* buffer, int& Length);

#endif //GNET_TANG_CLIENT_COMMON_UTILITY_H
