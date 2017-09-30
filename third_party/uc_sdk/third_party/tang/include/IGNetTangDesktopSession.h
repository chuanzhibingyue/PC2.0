#ifndef IGNETTANGDESKTOPSERVICE_H
#define IGNETTANGDESKTOPSERVICE_H

#include "ignettangbase.h"


/**
 *
 * @brief 桌面共享回调接口
 *
 */
class IGNetTangDesktopSessionSink:public IGNetTangBaseSessionSink
{
public:
    virtual void onDesktopShared() = 0;
    virtual void onDesktopShareStoped() = 0;
    virtual void onDesktopViewerStarted() = 0;
    virtual void onDesktopViewerStopped() = 0;
    
#if defined(WIN32) || defined(WIN64)
    virtual void onDesktopCommentCreated(int status) = 0;
    virtual void onDesktopCommentShowed(int status) = 0;
    virtual void onDesktopCommentClosed() = 0;
  #endif   //defined(WIN32) || defined(WIN64)
};


/**
 *
 * @brief 桌面共享接口
 *
 */
class IGNetTangDesktopSession:public IGNetTangBaseSession
{
public:

#if defined(WIN32) || defined(WIN64)
    //sharing only is support in Windows, in Android/Ios, sharing is not support now
	//for host
    /**
     * @brief 开始共享桌面
     * @return 返回0成功否则失败
     */
    virtual int startShare()=0;

    /**
     * @brief 停止共享桌面
     * @return 返回0成功否则失败
     */
    virtual int stopShare()=0;

    virtual int createComment() = 0;

    virtual int closeComment() = 0;

    virtual int showComment(void *pRenderer) = 0;

#endif   //defined(WIN32) || defined(WIN64)

    //for attendee
    /**
     * @brief 开始观看桌面
     * @param [in] pView 如果是windows系统 pView 是HWND，如果是IOS pView是UIImageView*，如果是Android pView是jobject，如果使用QML pView是Image object
     * @return 返回0成功否则失败
     */
    virtual int startView(TangImageViewHandle pView) = 0;

    /**
     * @brief 停止观看桌面
     * @return 返回0成功否则失败
     */
    virtual int stopView() = 0;

    /**
     * @brief 缩放桌面
     * @param [in] scale 缩放比例
     * @return 返回0成功否则失败
     */
    virtual int zoomView(double scale) = 0;

    /**
     * @brief 判断是否可以滚动到某个位置
     * @param [in] x X坐标，y Y坐标
     * @return 返回true可以否则不行
     */
    virtual bool canScroll(int x, int y) = 0;

    /**
     * @brief 滚动桌面
     * @param [in] x X坐标，y Y坐标
     * @return 返回0成功否则失败
     */
    virtual int scroll(int x, int y) = 0;

    /**
     * @brief 旋转桌面
     * @param [in] angle 旋转角度
     * @return 返回0成功否则失败
     */
    virtual int rotate(double angle) = 0;

};

#endif // IGNETTANGDESKTOPSERVICE_H

