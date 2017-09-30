/*++

Copyright (c) 2009 G-NET

Module Name:

    Debug.h

Abstract:

    This module contains definition and prototypes
    for code debugging and memory funciton wrapper etc.

Author:

    Yunfei Zuo (yunfei.zuo@gnetis.com) Oct/27/2009

Revision History:

    Yunfei Zuo (yunfei.zuo@gnetis.com) Created Oct/27/2009

--*/

#ifndef __DEBUG_H_
#define __DEBUG_H_

#include <WinDef.h>
#include <crtdbg.h>
#include <log.h>
#include <TangErrorCode.h>

#pragma warning(disable:4127)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define HRESULT_TO_SME(hr) (((UINT32)hr)& 0x0000FFFF)

// 
// _ASSERTE macro shows an assert failure dialog if the boolean expression evaluates false, 
// It give us 3 options to abort, retry and ignore the assert failure, plus where the 
// assert failure occurred. The retry option gives us a 2nd chance to debug into JIT debugger.
// Beside this, it also includes a string representation of the failed expression in the message. 
// 
#undef ASSERT
#define ASSERT _ASSERTE 

//
// To use the following macros,
// hr must be defined as the same type as _status_
//

#undef ERROR_REPORT
#define ERROR_REPORT(_status_) \
    { \
    } 

#undef ERROR_RETURN
#define ERROR_RETURN(_status_)   \
    { \
        const HRESULT expandOnce = _status_; \
        ERROR_REPORT(expandOnce); \
        return expandOnce; \
    }

#undef ERROR_LEAVE
#define ERROR_LEAVE(_status_)   \
    { \
        const HRESULT expandOnce = _status_; \
        ERROR_REPORT(expandOnce); \
        __leave; \
    }

#undef ERROR_EXIT
#define ERROR_EXIT(_status_)   \
    { \
        const HRESULT expandOnce = _status_; \
        hr = expandOnce; \
        ERROR_REPORT(expandOnce); \
        goto exit; \
    }

#undef ERROR_REPORT_AND_IGNORE
#define ERROR_REPORT_AND_IGNORE(_hr_)   \
    { \
        if (FAILED(_hr_)) \
        { \
            ERROR_REPORT(_hr_); \
            _hr_ = S_OK; \
        } \
    }

// ALLOC_CHECK is an easy pointer allocation checker for functions that return
// a PL_LPS_STATUS
#undef ALLOC_CHECK
#define ALLOC_CHECK(_var_) \
    { \
        if (_var_ == NULL) \
        { \
            ERROR_EXIT(ERROR_NOT_ENOUGH_MEMORY); \
        } \
    }

// ARG_CHECK is an easy argument pointer checker for functions that return
// a PL_LPS_STATUS
#undef ARG_CHECK
#define ARG_CHECK(_var_) \
    { \
        if (_var_ == NULL) \
        { \
            ERROR_EXIT(E_INVALIDARG); \
        } \
    }

// ARG_CHECK is an easy argument pointer checker for functions that return
// a PL_LPS_STATUS
#undef WEB_ARG_CHECK
#define WEB_ARG_CHECK(_var_) \
    { \
    if (_var_ == NULL) \
    { \
    ERROR_EXIT(E_COM_INVALID_PARAMS); \
} \
}

// HR_CHECK is a status checker for HRESULT's, where the calling function
// returns the same type
#undef HR_CHECK
#define HR_CHECK(_hr_) \
    { \
        const HRESULT expandOnce = _hr_; \
        hr = expandOnce; \
        if (FAILED(expandOnce)) \
        { \
            ERROR_REPORT(expandOnce); \
            goto exit; \
        } \
    }

// CHECK_EXIT is a status checker for any status type for which non-zero
// indicates failure (e.g. Win32 errors), where the calling function returns
// the same type
#undef CHECK_EXIT
#define CHECK_EXIT(_status_) \
    {\
        if (_status_ != 0) \
        { \
            ERROR_EXIT(_status_); \
        } \
    }

// Performs an ASSERT before the CHECK_IF.
#undef ASSERT_CHECK_IF
#define ASSERT_CHECK_IF(expr, hrError, ...) \
    if (expr)                               \
    {                                       \
        hr = (hrError);                  \
        ASSERT(FALSE);                      \
        ERROR_REPORT(hr);                \
        goto exit;                          \
    }

// Performs an ASSERT before the CHECK_IF_NULL.
#undef ASSERT_CHECK_IF_NULL
#define ASSERT_CHECK_IF_NULL(expr)            \
    {                                         \
        CONST BOOL isNull = ((expr) == NULL); \
                                              \
        if (isNull)                           \
        {                                     \
            ASSERT(FALSE);                    \
            ERROR_EXIT(E_FAIL);               \
        }                                     \
    }

// CHECK_IF is a expression checker. It sets the error code provided
// if the expression is true. 
// If the error code is a failure error code, then exits otherwise just
// adds the trace log.
#undef CHECK_IF
#define CHECK_IF(expr, hrError, ...) \
    { \
        if (expr) \
        { \
            HR_CHECK(hrError); \
        } \
    }

// CHECK_IF_NULL exits with E_FAIL if the expression is NULL.
#undef CHECK_IF_NULL
#define CHECK_IF_NULL(expr) \
    { \
        if ((expr) == NULL) \
        { \
            ERROR_EXIT(E_FAIL); \
        } \
    }

#undef UNREFERENCED
#define UNREFERENCED(x)     (x)

// BOOL_CHECK checks the boolean expression (0 = false, !0 = true).
// If the expression is false, will exit with an error code.  Variable hr
// will be set with the error code from HRESULT_FROM_WIN32(GetLastError())
// unless GetLastError() returns S_OK.  In that case hr is set to E_FAIL.
#undef BOOL_CHECK
#define BOOL_CHECK(expr) \
    { \
        if(!expr) \
        { \
            hr = HRESULT_FROM_WIN32(GetLastError()); \
            if(SUCCEEDED(hr)) \
            { \
                hr = E_FAIL; \
            } \
            ERROR_REPORT(hr); \
            goto exit; \
        } \
        else \
        { \
            hr = S_OK; \
        } \
    }

// HANDLE_CHECK is a status checker for Win 32 errors,where the calling function
// returns the same type
#define HANDLE_CHECK(expr) \
    do { \
        if (!(expr)) { \
            hr = HRESULT_FROM_WIN32(::GetLastError()); \
            do { if (hr == S_OK) hr = E_FAIL; } while(0); \
            goto exit; \
        } \
        else if( INVALID_HANDLE_VALUE == expr ) \
        { \
            hr = HRESULT_FROM_WIN32(::GetLastError()); \
            do { if (hr == S_OK) hr = E_FAIL; } while(0); \
            goto exit; \
        } \
    } while(0);

//
// "ALLOC" and "FREE" are the bases for other memory operation macros.
//
// Note:
// "size"  always means the size of the memory in BYTE.
// "count" always means the amount of elements in the array.
//

#define ALLOC(size)     malloc(size)
#define FREE(ptr) \
    { \
        if ((ptr) != NULL) \
        { \
            free((PVOID)ptr); \
            (ptr) = NULL; \
        } \
    }
//
// "ALLOC_EX" is an extention of "ALLOC", which allocates memory by
// specifying the type and count of the elemnts.
//

#define ALLOC_EX(type, count)       (type *) ALLOC(sizeof(type) * (count))

//
// "SAFE_ALLOC" is a macro, which checks the "ptr" before and after allocating the memory,
// and which allocates memory to "ptr" by specifying the type and count of the elemnts.
//

#define SAFE_ALLOC(ptr, type, count) \
    { \
        ASSERT((ptr) == NULL); \
        (ptr) = ALLOC_EX(type, (count)); \
        ALLOC_CHECK(ptr); \
    }

//
// "SAFE_DELETE" is a macro which safely deletes the memory.
//

#define SAFE_DELETE(ptr) \
    { \
        if ((ptr) != NULL) \
        { \
            delete (ptr); \
            (ptr) = NULL; \
        } \
    }

//
// "SAFE_DELETE_ARRAY" is a macro which safely deletes the array.
//

#define SAFE_DELETE_ARRAY(ptr) \
    { \
        if ((ptr) != NULL) \
        { \
            delete[] (ptr); \
            (ptr) = NULL; \
        } \
    }

//
// "SYSFREESTRING" is a macro which safely does a SysFreeString
//

#define SYSFREESTRING(ptr) \
    { \
        if ((ptr) != NULL) \
        { \
            SysFreeString((ptr)); \
            (ptr) = NULL; \
        } \
    }

//
// "SYSALLOCSTRING" is a macro which safely does a SysAllocString
//

#define SYSALLOCSTRING(ptr, str) \
    { \
        ASSERT((ptr) == NULL); \
        (ptr) = SysAllocString((str)); \
        ALLOC_CHECK((ptr)); \
    }

//
// "RELEASE" is a macro which safely does a Release()
//

#define RELEASE(ptr) \
    { \
        if ((ptr) != NULL) \
        { \
            ptr->Release(); \
            (ptr) = NULL; \
        } \
    }

//
// "CHECK_IF_NONZERO" is a macro which checks if the variable is greater then 0.
//

#define CHECK_IF_NONZERO(_var_) \
    { \
        if (_var_ <= 0) \
        { \
            ERROR_EXIT(E_FAIL); \
        } \
    }

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // __DEBUG_H_

