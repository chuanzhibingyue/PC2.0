#ifndef AUDIO_SERVICE_SINK_H
#define AUDIO_SERVICE_SINK_H

#include "IConferenceEvent.h"
#include "IGNetTangAudioSession.h"

using namespace uc;

class AudioServiceSink : public IGNetTangAudioSessionSink
{
public:
	AudioServiceSink(int conferenceId);
	~AudioServiceSink();

	virtual void onSessionErrorHandle(CGNetTangSessionErrorInfo *pErrorInfo);
	virtual void onAudioSessionPropertyChanged(const char *propName,
		const CGNetTangVariant &oldPropValue,
		const CGNetTangVariant &newPropValue);
	virtual void onAudioNetWorkCallBack(UINT32 nJitterMax, UINT32 nJitterAverage, UINT32 nLostRate, UINT32 nUnsortRate, UINT32 nSendSuccessRate);

	void SetDelegate(IConferenceEvent *delegate) { m_pDelegate = delegate; }

private:
	bool DoesCallPhoneError(const std::string &errorDescription);

private:
	IConferenceEvent *m_pDelegate;
	int m_conferenceId;
};
#endif //AUDIO_SERVICE_SINK_H