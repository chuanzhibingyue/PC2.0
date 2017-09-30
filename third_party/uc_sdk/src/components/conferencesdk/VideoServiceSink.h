#ifndef VIDEO_SERVICE_SINK_H
#define VIDEO_SERVICE_SINK_H

#include "IConferenceEvent.h"
#include "IGNetTangVideoSession.h"

using namespace uc;

class VideoServiceSink : public IGNetTangVideoSessionSink
{
public:
	VideoServiceSink(int conferenceId);
	~VideoServiceSink();

	virtual void onSessionErrorHandle(CGNetTangSessionErrorInfo *pErrorInfo);
	virtual void onVideoPropertyChanged(const char *propName,
		const CGNetTangVariant &oldPropValue,
		const CGNetTangVariant &newPropValue);
	virtual void onVideoInstanceAdded(const IGNetTangVideoInstance* pInst);
	virtual void onVideoInstanceRemoved(const IGNetTangVideoInstance* pInst);
	virtual void onVideoInstancePropertyChanged(const char*propName,
		const CGNetTangVariant &oldPropValue,
		const CGNetTangVariant &newPropValue,
		const IGNetTangVideoInstance* pInst);
	virtual void  onVideoFrameRate(unsigned int fps, unsigned int resample_fps,
		unsigned int number_of_streams);

	void SetDelegate(IVideoEvent *delegate) { m_pDelegate = delegate; }

private:
	int m_conferenceId;
	IVideoEvent *m_pDelegate;
};

#endif//VIDEO_SERVICE_SINK_H