#ifndef MICROBLOG_SERVICE_STRATEHY_H_
#define MICROBLOG_SERVICE_STRATEHY_H_

#include "IMicroblogService.h"
#include "CefClient.h"

using namespace uc;

class MicroblogServiceStrategy : public IMicroblogServiceEvent {
public:
	MicroblogServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);
	virtual ~MicroblogServiceStrategy();

public:
	int32_t GetRelation(int32_t userId);

	std::wstring GetHomePageURL(int32_t userId);

	std::wstring GetUserHomePageURL(int32_t selfId, int32_t userId);

	void GetNewMsgCount();
	
	void AddAttention(int32_t userId);

	void CancelAttention(int32_t userId);

	void ShareMeetingRecording(std::wstring urlVideo, std::wstring urlThumb, std::wstring fileName, int64_t fileSize, int64_t voiceTime, std::wstring comments);
	
	// microblogservice event
	virtual void OnNewMsgCountGet(int32_t errorCode, int32_t countAll, int32_t relationCount, int32_t timenewsCount, int32_t pollCount, int32_t caseCount);

	virtual void OnAttentionAdd(int32_t errorCode, int32_t userId, int32_t relation);

	virtual void OnAttentionCancelled(int32_t errorCode, int32_t userId, int32_t relation);

	virtual void OnMeetingRecordingShared(int32_t errorCode, int64_t resourceID);

private:
	cefIntegration::CefClient *_cefClient;
	IMicroblogService *_microblogService;
	IUCEngine *_engine;
};

#endif /* MICROBLOG_SERVICE_STRATEHY_H_ */

