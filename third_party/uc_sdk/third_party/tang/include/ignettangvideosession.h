#ifndef IGNetTangVideoSession_H
#define IGNetTangVideoSession_H

#include "ignettangbase.h"
/**
 * @brief 视频共享用户�?
 *
 *  提供视频共享用户信息，视频共享用户ID
 *
 */
class IGNetTangVideoInstance
{
public:        
    virtual unsigned int getVideoUserID() = 0;
    virtual CGNetTangVariant getProperty(const char* propName) = 0;
};

/**
 *
 * @brief 视频服务回调
 *
 */
class IGNetTangVideoSessionSink:public IGNetTangBaseSessionSink
{
public:
	virtual void onVideoPropertyChanged(const char *propName,
									const CGNetTangVariant &oldPropValue,
									const CGNetTangVariant &newPropValue)              = 0;
    virtual void onVideoInstanceAdded(const IGNetTangVideoInstance* pInst)             = 0;
    virtual void onVideoInstanceRemoved(const IGNetTangVideoInstance* pInst)           = 0;
    virtual void onVideoInstancePropertyChanged(const char*propName,
                                       const CGNetTangVariant &oldPropValue,
                                       const CGNetTangVariant &newPropValue,
                                       const IGNetTangVideoInstance* pInst)            = 0;

#if defined(WIN32) || defined(WIN64)
	virtual void  onVideoFrameRate(unsigned int fps, unsigned int resample_fps,
		unsigned int number_of_streams) = 0;
#endif
};


/**
 *
 * @brief 视频服务类，管理视频服务
 *
 */
class IGNetTangVideoSession:public IGNetTangBaseSession
{
public:

#if defined(WIN32) || defined(WIN64) 
    
    virtual bool isDeviceOccupied(int nDevicIndex) = 0;

    virtual int resetViewWindow(unsigned int nUserID, TangVideoViewHandle pWindow) = 0;

    virtual int resetPreviewWindow(TangVideoViewHandle pWindow) = 0;

    virtual int resetViewWindowForJS(unsigned int nUserID, TangVideoViewHandle pWindow) = 0;

    virtual int resetPreviewWindowForJS(TangVideoViewHandle pWindow) = 0;

    virtual int startPreviewForJS(int nDevicIndex, TangVideoViewHandle pWindow) = 0;

    virtual int stopPreviewForJS() = 0;

    virtual int startViewForJS(unsigned int nUserID, TangVideoViewHandle pWindow) = 0;

    virtual int startForwardToMcu(unsigned int nUserID) = 0;

    virtual int stopForwardToMcu() = 0;

    virtual int enterMcuMode() = 0;

    virtual int leaveMcuMode() = 0;

    virtual int getSipAddress(char* strSipAddr) = 0;
    
#else
    virtual int changeCameraOrientation(int orientation) = 0;
    virtual int resetRenderWindow(unsigned int nUserID, TangVideoViewHandle pWindow) = 0;
    
#endif

    virtual unsigned int getCameraCount() = 0;
    
    virtual int getCameraName(int nIndex, char* sDeviceName) = 0;
    
    virtual int startPreview(int nDevicIndex, TangVideoViewHandle pWindow) = 0;
    
    virtual int stopPreview() = 0;
    
    virtual int startView(unsigned int nUserID,TangVideoViewHandle pWindow) = 0;
    
    virtual int stopView(unsigned int nUserID) = 0;
    
    virtual int startShare(unsigned int nUserID, int nDevicIndex, unsigned int nWidth, unsigned int nHeight) = 0;
    
    virtual int stopShare(unsigned int nUserID) = 0;
    
    virtual int requestResolution(unsigned int nUserID, unsigned int nWidth, unsigned int nHeight) = 0;
    
    virtual int changeShareCamera(unsigned int nUserID, int nDevicIndex) = 0;

	virtual int snapshot(TangVideoViewHandle pWindow, const char *sFileName) = 0;

    virtual unsigned int getVideoInstanceCount()                                   = 0;

    virtual IGNetTangVideoInstance* getVideoInstanceByIndex(unsigned int nIndex)   = 0;

    virtual IGNetTangVideoInstance* getVideoInstanceByUserID(unsigned int nUserID) = 0;

    virtual IGNetTangVideoInstance* getVideoInstanceMyself()                       = 0;
    
};

#endif // IGNetTangVideoSession_H

