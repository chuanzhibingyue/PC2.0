#pragma once
#include <list>

#ifdef RECORDERSTARTINFODLL_EXPORTS
#define RECORDERSTARTINFOPORT __declspec(dllexport)
#else
#define RECORDERSTARTINFOPORT __declspec(dllimport)
#endif

namespace clt
{
    struct SessionInfo
    {
        BOOL bIsValid;
        DWORD dwSessionProcessID;
        DWORD dwSessionThreadID;

        SessionInfo()
            : bIsValid(FALSE)
            , dwSessionProcessID(0)
            , dwSessionThreadID(0)
        {
        }
    };

    /**
    * @brief	<b>Summary:</b>	创建HeadInfo内存映射与视图
    * @detail   只包含Client端的windows句柄和Client端isAlive的标识
    * @caller   Daemon进程调用
    * @param [in] szMappingName: 内存映射块的名称
    * @return   S_OK:成功；E_FAIL:失败
    */
    extern "C" RECORDERSTARTINFOPORT HRESULT OpenViewMappingOfBaseInfo(LPCTSTR szMappingName);

    /**
    * @brief	<b>Summary:</b>	从内存块中获取Client进程的窗口句柄
    * @detail   
    * @caller   Daemon进程调用
    * @param 
    * @return   Client进程的窗口句柄
    */
    extern "C" RECORDERSTARTINFOPORT HWND GetClientWindowHandle();

    /**
    * @brief	<b>Summary:</b>	将内存块的Client进程的窗口句柄清零
    * @detail   
    * @caller   Daemon进程调用
    * @param 
    * @return  
    */
    extern "C" RECORDERSTARTINFOPORT void DeleteClientWindowHandle();

    /**
    * @brief	<b>Summary:</b>	从内存块中获取更新客户端的命令行
    * @detail   
    * @caller   Daemon进程调用
    * @param 
    * @return   命令行
    */
    extern "C" RECORDERSTARTINFOPORT HRESULT GetUpdateInfo(LPTSTR pBatPath,size_t & len_b,LPTSTR pCommLine,size_t& len_c );

    /**
    * @brief	<b>Summary:</b>	从内存块中获取Session的进程句柄
    * @detail   
    * @caller   Daemon进程调用
    * @param 
    * @return   Session进程句柄
    */
    extern "C" RECORDERSTARTINFOPORT HRESULT GetClientSessionInfo(std::list<SessionInfo>& listSessionProcess);

    /**
    * @brief	<b>Summary:</b>	从内存块中清理Session的进程句柄
    * @detail   
    * @caller   Daemon进程调用
    * @param 
    * @return   Session进程句柄
    */
    extern "C" RECORDERSTARTINFOPORT HRESULT ClearClientSessionInfo();
}
