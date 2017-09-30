#ifndef IAUDIOWIZARD_H
#define IAUDIOWIZARD_H

#ifdef AVWIZARD_EXPORTS
#define AUDIOWIZARD_EXPORTS extern "C" __declspec(dllexport)
#else
#define AUDIOWIZARD_EXPORTS extern "C" __declspec(dllimport)
#endif //AVWIZARD_EXPORTS

class IAudioWizard
{
public:
	virtual int GetMicrophoneNum(int& nCount) = 0;
	virtual int GetSpeakerNum(int& nCount) = 0;
	virtual int GetMicrophoneName(int nIndex, char* szDeviceName) = 0;
	virtual int GetSpeakerName(int nIndex, char* szDeviceName) = 0;
	virtual int SetMicrophone(int nIndex) = 0;
	virtual int SetSpeaker(int nIndex) = 0;
	virtual int StartPlayFile(char* szWavFileName) = 0;
	virtual int StopPlayFile() = 0;
	virtual int IsPlayFile(bool& bIsPlay) = 0;
	virtual int StartLoopAudio() = 0;
	virtual int StopLoopAudio() = 0;
	virtual int GetMicrophoneLevel(int& nLevel) = 0;
	virtual int GetSpeakerLevel(int& nLevel) = 0;
	virtual int GetMicrophoneVolume(int& nVolume) = 0;
	virtual int GetSpeakerVolume(int& nVolume) = 0;
	virtual int SetMicrophoneVolume(int nVolume) = 0;
	virtual int SetSpeakerVolume(int nVolume) = 0;
	virtual int GetAgc(bool& bIsEnable) = 0;
	virtual int SetAgc(bool bIsEnable) = 0;
};

AUDIOWIZARD_EXPORTS IAudioWizard* createAudioWizard();
AUDIOWIZARD_EXPORTS int deleteAudioWizard(IAudioWizard* pAudioWizard);

#endif //IAUDIOWIZARD_H