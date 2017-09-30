#ifndef CGNETTANG_CROSSPLATFORM
#define CGNETTANG_CROSSPLATFORM

#if defined(ANDROID)
#include <jni.h>
typedef jobject TangImageViewHandle; //for desktopshare and docshare service
typedef jobject TangVideoViewHandle; //for video service
#else
typedef void* TangImageViewHandle;
typedef const void* TangVideoViewHandle;
#endif


#endif // CGNETTANG_CROSSPLATFORM

