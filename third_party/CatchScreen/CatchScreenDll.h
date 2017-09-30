#ifndef CATCHSCREENDLL_H
#define CATCHSCREENDLL_H

#include <string>
using namespace std;

#ifdef CATCHSCREEDLL_EXPORTS
#define CATCHSCREE_API  extern "C" __declspec(dllexport)
#else
#define CATCHSCREE_API  extern "C" __declspec(dllimport)
#endif

/**
 * 截图回调
 * @param userData 调用方上下文数据
 * @param type 回调类型 0:截图完成 1:截图取消 2:截图保存
 * @param imagePath 截图图片路径
 * @return 成功：0 失败：非0
 */
typedef void (CALLBACK * CaptureScreenCallBack)(void *userData, int type, const wstring& imagePath);

/**
 * 截图接口
 * @param userData 调用方上下文数据
 * @param path 指定的截图路径
 * @param callBack 回调处理
 * @return 成功：0 失败：非0
 */
CATCHSCREE_API int CaptureScreen(void* userData, const wstring& path, CaptureScreenCallBack callBack);

CATCHSCREE_API void CaptureScreen_SetDeflateParam(int left_deflate, int top_deflate, int right_deflate, int bottom_deflate);

#endif //CATCHSCREENDLL_H