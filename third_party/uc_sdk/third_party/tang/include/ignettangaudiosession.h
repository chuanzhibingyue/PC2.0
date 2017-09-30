#ifndef IGNETTANGAUDIOSESSION_H
#define IGNETTANGAUDIOSESSION_H

#include "ignettangbase.h"

/**
 *
 * @brief 电话用户信息类
 *
 */
struct PhoneCallNum
{
    unsigned int nUserID;
    char* sPhoneNum;
};

/**
 *
 * @brief 音频服务回调
 *
 */
class IGNetTangAudioSessionSink:public IGNetTangBaseSessionSink
{
public:
    virtual void onAudioSessionPropertyChanged(const char *propName,
                                      const CGNetTangVariant &oldPropValue,
                                      const CGNetTangVariant &newPropValue) = 0;
#if defined(WIN32) || defined(WIN64)
    virtual void onAudioNetWorkCallBack(unsigned int nJitterMax,
                                        unsigned int nJitterAverage,
                                        unsigned int nLostRate,
                                        unsigned int nUnsortRate,
                                        unsigned int nSendSuccessRate) = 0;
#endif
};

/**
 *
 * @brief 音频服务类，管理VoIP和PSTN服务
 *
 */
class IGNetTangAudioSession:public IGNetTangBaseSession
{
public:
#if defined(WIN32) || defined(WIN64)

    virtual int getMicCount(unsigned int& nCount) = 0;

    virtual int getSpeakerCount(unsigned int& nCount) = 0;

    virtual int getMicName(int nIndex, char* sDeviceName) = 0;

    virtual int getSpeakerName(int nIndex, char* sDeviceName) = 0;

    virtual int setMicDevice(int nIndex) = 0;

    virtual int setSpeakerDevice(int nIndex) = 0;

	virtual int startPlayFile(char* sWavFilePath) = 0;

	virtual int stopPlayFile() = 0;

	virtual int startLoopTest() = 0;
	
	virtual int stopLoopTest() = 0;
#else
    virtual int enableLoudSpeaker() = 0;

    virtual int disableLoudSpeaker() = 0;
#endif

    virtual int startVoip() = 0;

    virtual int stopVoip()  = 0;

    virtual int muteUser(const unsigned int *nUserID, unsigned int nNum)   = 0;

    virtual int unMuteUser(const unsigned int *nUserID, unsigned int nNum) = 0;

    virtual int call(const struct PhoneCallNum *sphoneCallNum, unsigned int nNum) = 0;

    virtual int hangUp(const unsigned int *nUserID, unsigned int nNum) = 0;

    virtual int bind(unsigned int nUserID, unsigned int nPhoneUserID) = 0;
};

#endif // IGNETTANGAUDIOSESSION_H

