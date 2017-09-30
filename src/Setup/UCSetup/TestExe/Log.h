/*++

  Copyright (c) 2008~2009 All Rights Resverved by
  G-Net Integrated Service co. Ltd. 


Module Name:

    Log.h
    
Abstract:

    This module implements Tang logging routines that logs/traces the program events.

Author:

    Xiaogang Yu (xiaogang.yu@gnetis.com) Nov/02/2009

Revision History:

    Xiaogang Yu (xiaogang.yu@gnetis.com) Created Nov/02/2009

Note:

    None.

--*/

#ifndef __LOG_LIB_H__
#define __LOG_LIB_H__

// include
#include <windows.h>

#ifdef LOG_DLL_API__
#define LOG_DLL_API__ extern "C" __declspec(dllimport)
#else
#define LOG_DLL_API__ extern "C" __declspec(dllexport)
#endif

/**
 * An enum. 
 * The description of log's level.
 */
enum LOG_LEVEL
{    
    LEVEL_NONE      = 0,
    LEVEL_EMERG     = 1,
    LEVEL_ERR       = 2,
    LEVEL_WARNING   = 3,
    LEVEL_INFO      = 4,
    LEVEL_DEBUG     = 5,
    LEVEL_ALL       = 6
} ;
/**
 * Invokes this fuction to write data to log file.
 * @param [in] pszSrc: the error file name.
 *        [in] uLine: the error line number.
 *        [in] level: log's level.
 *        [in] pszFormat: the formated string.
 *        [in] ... : the param list.
 * @return void.
 */
LOG_DLL_API__
void 
TraceToFile(
            __in LPCWSTR pszModuleName,
            __in LPCWSTR pszFileName,
            __in ULONG uLine, 
            __in LONG level, 
            __in LPCWSTR pszFormat, 
            ...
            );

// define
#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#ifdef _UNICODE
#define __TFILE__ __WFILE__
#else
#define __TFILE__ __FILE__
#endif

// definition for record the information 
// about module name and file name.
#define MODULE_NAME L"TangClient"
#define FILE_NAME   (__WFILE__)

/**
 * Init the log module.
 * @param [in] nFileSize: set the capability of log file, provide default value.
 *        [in] pszLogFileDir: set the store directory of log, provide default value.
 *        [in] enumLogLevel: set the log's level.
 *        [in] bProcessSafe: whether safe in different process.
 * @return HRESULT: return S_OK if successful.
 */
LOG_DLL_API__
HRESULT 
InitLog(
        );

/**
 * Close log module and release the resource.
 * @param none.
 * @return void.
 */
LOG_DLL_API__
void 
UninitLog(
         );

/**
 * Set log's level.
 * @param [in] enumLogLevel: this parameter set log's current level.
 * @return HRESULT: return S_OK if successful.
 */
LOG_DLL_API__
HRESULT
SetLogLevel(
            __in LOG_LEVEL enumLogLevel
            );

/**
 * Get current log's level.
 * @param none.
 * @return LOG_LEVEL: log's current level.
 */
LOG_DLL_API__
LOG_LEVEL 
GetCurLogLevel(
               );

/**
* Get the log file name.
* @param lpszFileName.
* @return HRESULT: return S_OK if successful, otherwise return error code.
*/
LOG_DLL_API__
HRESULT
GetLogFileName(
               __inout LPWSTR lpszFileName
               );


/**
 * These functions have same usage.
 * It define some macros that different from log's level.
 * @param [in] str: the formated string.
 *        [in] ...: the parameters list.
 * @see TraceToFile().
 */

#define LOG_EMERG(str, ...)        TraceToFile(MODULE_NAME, FILE_NAME, __LINE__, LEVEL_EMERG, str, ##__VA_ARGS__)
#define LOG_ERR(str, ...)        TraceToFile(MODULE_NAME, FILE_NAME, __LINE__, LEVEL_ERR, str, ##__VA_ARGS__)
#define LOG_WARNING(str, ...)    TraceToFile(MODULE_NAME, FILE_NAME, __LINE__, LEVEL_WARNING, str, ##__VA_ARGS__)
#define LOG_INFO(str, ...)        TraceToFile(MODULE_NAME, FILE_NAME, __LINE__, LEVEL_INFO , str, ##__VA_ARGS__)
#define LOG_DEBUG(str, ...)        TraceToFile(MODULE_NAME, FILE_NAME, __LINE__, LEVEL_DEBUG, str, ##__VA_ARGS__)

LOG_DLL_API__
void
TraceErrorReport(
    __in DWORD Status,
    __in LPCWSTR ModuleName,
    __in LPCWSTR FileName,
    __in DWORD LineNumber
    );

#endif //__LOG_LIB_H__

