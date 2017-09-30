#ifndef CLIENT_DEFS_H_
#define CLIENT_DEFS_H_

#include <cassert>

namespace cefIntegration {

#ifdef CEF_STATIC
#define CEF_CLIENT_API
#else
#define CEF_CLIENT_API __declspec(dllexport)
#endif

enum CEF_CLIENT_ERROR_CODE {
	CEF_SUCCESS = 0,
	CEF_INITIALIZE_ERROR,
	CEF_SUB_PROCESS_COMPLETE,
	CEF_ARGUMENT_ERROR,
	CEF_MODULE_NOTFOUND,
	CEF_METHOD_NOTFOUND,
	CEF_CROP_IMAGE_ERROR,
	CEF_SAVE_CROPED_IMAGE_ERROR,
	CEF_OPEN_FILE_ERROR,
	CEF_DROPPED_FILE_NOT_FOUND,
	CEF_PARAMETER_ERROR,
	CEF_FILE_NOT_FOUND
};

#define REQUIRE_UI_THREAD()   	assert(CefCurrentlyOn(TID_UI))
#define REQUIRE_IO_THREAD()   	assert(CefCurrentlyOn(TID_IO))
#define REQUIRE_FILE_THREAD() 	assert(CefCurrentlyOn(TID_FILE))

#define PROCESS_MESSAGE_RETURN(WndProc, ret) 			{		\
		if ((WndProc)) {									\
			return ret;									\
		}												\
		break;											\
	}

#define PROCESS_MESSAGE(WndProc)		PROCESS_MESSAGE_RETURN((WndProc), 0)

#define UCCLIENT_CLASS_NAME											L"QUANSHI UC CLIENT CLASS"

}


#endif