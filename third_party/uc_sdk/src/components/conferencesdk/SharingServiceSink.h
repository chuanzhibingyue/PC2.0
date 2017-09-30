#ifndef SHARING_SERVICE_SINK_H
#define SHARING_SERVICE_SINK_H

#include "ISharingService.h"
#include "IGNetTangDesktopSession.h"
#include "IGNetTangConfMgr.h"

using namespace uc;

class SharingServiceSink :public IGNetTangDesktopSessionSink
{
public:
	SharingServiceSink(int conferenceId);
	~SharingServiceSink();
	virtual void onSessionErrorHandle(CGNetTangSessionErrorInfo *pErrorInfo);
	virtual void onDesktopShared();
	virtual void onDesktopShareStoped();
	virtual void onDesktopViewerStarted();
	virtual void onDesktopViewerStopped();

	void SetConference(IGNetTangConference *conference) { _conference = conference; }
	void setSession(IGNetTangDesktopSession *session) { m_psession = session; }

#if defined(WIN32) || defined(WIN64)
	virtual void onDesktopCommentCreated(int status) {}
	virtual void onDesktopCommentShowed(int status) {}
	virtual void onDesktopCommentClosed() {}
#endif   //defined(WIN32) || defined(WIN64)

	void SetDelegate(ISharingServiceEvent *delegate) { m_pDelegate = delegate; }

private:
	int getShareUserID();

private:
	int m_conferenceId;
	ISharingServiceEvent *m_pDelegate;
	IGNetTangDesktopSession *m_psession;
	IGNetTangConference *_conference;
};

#endif //SHARING_SERVICE_SINK_H