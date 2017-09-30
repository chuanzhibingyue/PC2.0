#ifndef PLAT_FORM_H
#define PLAT_FORM_H

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




#endif // !PLAT_FORM_H
