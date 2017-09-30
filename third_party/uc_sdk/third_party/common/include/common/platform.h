#ifndef __platform_h__
#define __platform_h__

#include <stddef.h>
#include <stdio.h>

#if defined(_MSC_VER) && (_MSC_VER < 1600)
	typedef signed char int8_t;
	typedef unsigned char uint8_t;
	typedef short int16_t;  // NOLINT
	typedef unsigned short uint16_t;  // NOLINT
	typedef int int32_t;
	typedef unsigned int uint32_t;
	typedef __int64 int64_t;
	typedef unsigned __int64 uint64_t;
	// intptr_t and friends are defined in crtdefs.h through stdio.h.
#else
#include <stdint.h>
#endif

#if defined(__INTEL_COMPILER)
#	define PLATFORM_INTEL			__INTEL_COMPILER
#	define PLATFORM_COMPILER_DESC	"Intel C/C++"
#elif defined(__ICC)
#	define PLATFORM_INTEL			__ICC
#	define PLATFORM_COMPILER_DESC	"Intel C/C++"
#elif defined(__ECC)
#	define PLATFORM_INTEL			__ECC
#	define PLATFORM_COMPILER_DESC	"Intel C/C++"
#elif defined(__ICL)
#	define PLATFORM_INTEL			__ICL
#	define PLATFORM_COMPILER_DESC	"Intel C/C++"
#elif defined(__GNUC__)
#	define PLATFORM_GNUC				__GNUC__
#	define PLATFORM_COMPILER_DESC	"GNU GCC"
#elif defined(__GNUG__)
#	define PLATFORM_GNUC				__GNUG__
#	define PLATFORM_COMPILER_DESC	"GNU G++"
#elif defined(_MSC_VER)
#	define PLATFORM_MSC				_MSC_VER
#	define PLATFORM_COMPILER_DESC	"Microsoft Visual C++"
#elif defined(__MWERKS__)
#	define PLATFORM_MWERKS   		__MWERKS__
#	define PLATFORM_COMPILER_DESC	"MetroWerks CodeWarrior"
#elif defined(__IBMC__)
#	define PLATFORM_IBM				__IBMC__
#	define PLATFORM_COMPILER_DESC	"IBM C"
#elif defined(__IBMCPP__)
#	define PLATFORM_IBM				__IBMCPP__
#	define PLATFORM_COMPILER_DESC	"IBM C++"
#elif defined(__DECC)
#	define PLATFORM_COMPAQ			__DECC
#	define PLATFORM_COMPILER_DESC	"Compaq/DEC C"
#elif defined(__DECCXX)
#	define PLATFORM_COMPAQ			__DECCXX
#	define PLATFORM_COMPILER_DESC	"Compaq/DEC C++"
#elif defined(__BORLANDC__)
#	define PLATFORM_BORLAND			__BORLANDC__
#	define PLATFORM_COMPILER_DESC	"Borland C/C++"
#elif defined(__WATCOMC__)
#	define PLATFORM_WATCOM			__WATCOMC__
#	define PLATFORM_COMPILER_DESC	"Watcom C/C++"
#elif defined(__SUNPRO_C)
#	define PLATFORM_SUN				__SUNPRO_C
#	define PLATFORM_COMPILER_DESC	"Sun Pro C"
#elif defined(__SUNPRO_CC)
#	define PLATFORM_SUN				__SUNPRO_CC
#	define PLATFORM_COMPILER_DESC	"Sun Pro C++"
#elif defined(__APPLE_CC__)
#	define PLATFORM_APPLE_CC			__APPLE_CC__
#	define PLATFORM_COMPILER_DESC	"Apple C/C++"
#else
#	error "can't detect target compiler"
#endif

 // android
#if defined(ANDROID) || defined(android) || defined(__android__) || defined(__ANDROID__)
#	define PLATFORM_ANDROID			1
// windows
#elif defined(WINAPI_FAMILY) || defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
#	define PLATFORM_WIN      		1
#	if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#		define PLATFORM_WIN32			1
#	endif
#	if defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
#		define PLATFORM_WIN64			1
#	endif
#   if defined(WINAPI_FAMILY)
#       include <winapifamily.h>
#       if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#           define PLATFORM_WIN_PHONE        1
#       elif WINAPI_FAMILY_ONE_PARTITION(WINAPI_FAMILY, WINAPI_PARTITION_APP)
#           define PLATFORM_WIN_STORE        1
#       else
#           define PLATFORM_WIN_DESKTOP      1
#       endif
#   else
#       define PLATFORM_WIN_DESKTOP      1
#   endif
// mac & ios
#elif defined(__APPLE__)
#	define PLATFORM_APPLE		1
#	include <TargetConditionals.h>
#	if TARGET_OS_IPHONE == 1
#		define PLATFORM_IOS				1
#	elif TARGET_OS_MAC == 1
#		define PLATFORM_MAC				1
#		if defined(__LP64__) || defined(__64BIT__) || defined(_LP64) || (__WORDSIZE == 64)
#			define PLATFORM_MAC64			1
#		else
#			define PLATFORM_MAC32			1
#		endif
#	else
#		error: not support this platform
#	endif
// linux
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	define PLATFORM_LINUX			1
// bsd
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#	define PLATFORM_BSD				1
// solaris
#elif defined(sun) || defined(__sun) || defined(__sun__) || defined(__solaris__)
#	if defined(__SVR4) || defined(__svr4__) || defined(__solaris__)
#		define PLATFORM_SOLARIS			1
#	else
#		define PLATFORM_SUNOS			1
#	endif
// others
#elif defined(__CYGWIN__)
#	define PLATFORM_CYGWIN			1
#else
#	error "can't determine target platform"
#endif
 
#if !(defined(PLATFORM_WIN))
#	define PLATFORM_POSIX	1
#endif
    
// handle calling conventions: cdecl, stdcall, fastcall  
#if defined(PLATFORM_GNUC)   
#   if !defined(PLATFORM_POSIX) && defined(PLATFORM_ANDROID)   
#	    define PLATFORM_CDECL   
#	    define PLATFORM_STDCALL   
#	    define PLATFORM_FASTCALL   
#   else  
#	    define PLATFORM_CDECL	__attribute__((cdecl))  
#	    define PLATFORM_STDCALL	__attribute__((stdcall))    
#	    define PLATFORM_FASTCALL __attribute__((fastcall)) 
#   endif 
#elif defined(PLATFORM_MSC) || defined(PLATFORM_WATCOM) || defined(PLATFORM_BORLAND) || defined(PLATFORM_MWERKS)  
#	define PLATFORM_CDECL    __cdecl  
#	define PLATFORM_STDCALL  __stdcall  
#	define PLATFORM_FASTCALL __fastcall
#else  
#	define PLATFORM_CDECL 
#	define PLATFORM_STDCALL
#	define PLATFORM_FASTCALL 
#endif
    
// handle import/export directives 
#if (defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64) || defined(PLATFORM_WINCE)) 
// __declspec works in BC++ , Watcom C++ as well as VC++ and gcc/g++ 
#	if (defined(PLATFORM_MSC) || defined(PLATFORM_GNUC) || defined(PLATFORM_BORLAND) || defined(PLATFORM_WATCOM))  
#		define PLATFORM_IMPORT	__declspec(dllimport)  
#		define PLATFORM_EXPORT	__declspec(dllexport)  
#	endif 
#elif (defined(PLATFORM_MAC32) || defined(PLATFORM_MAC64) || defined(PLATFORM_IOS)) 
#	if (defined(PLATFORM_MWERKS)) 
#		define PLATFORM_IMPORT	__declspec(import)   
#		define PLATFORM_EXPORT	__declspec(export)  
#	endif
#endif
     
// for other platforms/compilers we don't anything  
#if !defined(PLATFORM_EXPORT)  
#    define PLATFORM_EXPORT   
#    define PLATFORM_IMPORT
#endif
    
#if defined(PLATFORM_LIB_STATIC)
#	 undef PLATFORM_EXPORT
#    undef PLATFORM_IMPORT
#    define PLATFORM_EXPORT   
#    define PLATFORM_IMPORT
#endif

// define PLATFORM_API
#if defined(PLATFORM_API)   
#	undef PLATFORM_API 
#endif
    
#if defined(PLATFORM_EXPORTS) 
#	define PLATFORM_API	extern "C" PLATFORM_EXPORT
#	define PLATFORM_CLASS PLATFORM_EXPORT
#else  
#	define PLATFORM_API	extern "C" PLATFORM_IMPORT
#	define PLATFORM_CLASS PLATFORM_IMPORT
#endif

#if defined(__arm__) || defined(__ARM_EABI__) || defined(_M_ARM_FP)
#	define PLATFORM_ARM			1
#	define PLATFORM_CPU_DESC		"ARM"
#elif defined(__i386__) || defined(_M_IX86)
#	define PLATFORM_X86			1
#	define PLATFORM_CPU_DESC		"Intel x86"
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(_M_IA64) || defined(__x86_64__) || defined(_M_X64)
#	define PLATFORM_IA86			1
#	define PLATFORM_CPU_DESC		"Intel x64"
#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || defined(__ppc__) || defined(_M_PPC)
#	define PLATFORM_POWERPC		1
#	if defined(__powerpc64__)
#       define PLATFORM_CPU_DESC	"PowerPC64"
#	else
#       define PLATFORM_CPU_DESC	"PowerPC"
#	endif
#else
#	error cannot determine target CPU
#endif

#if PLATFORM_ARM
#	if defined(__ARMEB__)
#		define PLATFORM_CPU_BE		1
#	else
#		define PLATFORM_CPU_LE		1
#	endif
#elif PLATFORM_X86 || PLATFORM_X64
#		define PLATFORM_CPU_LE		1
#endif

#if PLATFORM_WIN
    /*  multi byte string (mbs) && wide char string (wcs, unicode) */
    #if defined ( _UNICODE ) || defined ( UNICODE )
    #else
    #   define PLATFORM_MBS
    #endif /* end of #ifndef _UNICODE */
#endif


#ifndef INT32_MAX
#define INT32_MAX	(2147483647)
#endif

#if defined INLINE_PREFER
#	define PLATFORM_FUNC inline
#else
#	define PLATFORM_FUNC static
#endif

	/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef PLATFORM_ANDROID
#   ifndef PATH_MAX
#       define PATH_MAX MAX_PATH
#   endif
#endif

typedef int                                PBool;
#define PTrue                              1
#define PFalse                             0

#ifdef PLATFORM_WIN
#ifdef wchar_t
typedef wchar_t							PCharL;
#else
typedef char							PCharL;
#endif
#elif defined (PLATFORM_APPLE)
typedef char							PCharL;
#else
typedef char							PCharL;
#endif // PLATFORM_WIN

#if defined(PLATFORM_WIN)
#   define WIN32_LEAN_AND_MEAN
#	include "objbase.h"
#else
#   define interface struct
#endif

#if defined(COMMON_EXPORTS) || defined(UC_EXPORTS)
#   define UC_API_C	extern "C" PLATFORM_EXPORT
#   define UC_API PLATFORM_EXPORT
#else
#   define UC_API_C	extern "C" PLATFORM_IMPORT
#   define UC_API PLATFORM_IMPORT
#endif

#endif	//__platform_h__
