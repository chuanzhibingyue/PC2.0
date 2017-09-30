#pragma once

#ifdef RECORDERSTARTINFODLL_EXPORTS
#define RECORDERSTARTINFOPORT __declspec(dllexport)
#else
#define RECORDERSTARTINFOPORT __declspec(dllimport)
#endif

namespace clt
{
    extern "C" RECORDERSTARTINFOPORT void* SessionDaemon_New();
    extern "C" RECORDERSTARTINFOPORT void SessionDaemon_Delete(void* pRecorder);

    extern "C" RECORDERSTARTINFOPORT HRESULT SessionDaemon_CreateViewMapping(void* pRecorder, LPCTSTR szMappingName);
    extern "C" RECORDERSTARTINFOPORT void SessionDaemon_DestroyViewMapping(void* pRecorder);

    extern "C" RECORDERSTARTINFOPORT void SessionDaemon_SetDaemonThreadID(void* pRecorder, DWORD dwDaemonThreadID);

    extern "C" RECORDERSTARTINFOPORT HWND SessionDaemon_GetClientWindowHandle(void* pRecorder);
    extern "C" RECORDERSTARTINFOPORT void SessionDaemon_DeleteClientWindowHandle(void* pRecorder);
}
