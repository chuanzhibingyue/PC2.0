#ifndef MICROBLOGSERVICE_H_
#define MICROBLOGSERVICE_H_

#include "IMicroblogService.h"

namespace uc {

class MicroblogService : public IMicroblogService, IUCEngineEvent {
public:
	MicroblogService(IUCEngine *engine);
	virtual ~MicroblogService();

public:
	virtual int32_t GetRelation(int32_t userId);

	virtual std::string GetHomePageURL(int32_t userId);
	virtual std::string GetUserHomePageURL(int32_t selfId, int32_t userId);

	virtual void GetNewMsgCount();

	virtual void AddAttention(int32_t userId);

	virtual void CancelAttention(int32_t userId);

	virtual void ShareMeetingRecording(std::string urlVideo, std::string urlThumb, std::string fileName, int64_t fileSize, int64_t voiceTime, std::string comments);

	// IUCEngineEvent
	virtual void OnLoginSucceeded();

	virtual void OnLogoutSucceeded();

private:
	int32_t OnAddAttentionComplete(int32_t errorCode, HTTPResponse *response, int32_t userId);
	int32_t OnCancelAttentionComplete(int32_t errorCode, HTTPResponse *response, int32_t userId);
	int32_t OnGetNewMsgCountComplete(int32_t errorCode, HTTPResponse *response);
	void GetAttentionList(int32_t index, int32_t offset);
	int32_t OnGetAttentionListComplete(int32_t errorCode, HTTPResponse *response, int32_t index, int32_t offset);
	int32_t OnMeetingRecordingShared(int32_t errorCode, HTTPResponse *response);
	
private:
	std::map<int32_t, int32_t> _mapRelation; // 保存关注列表信息key:userId, value:relation
};

} 
#endif/* MICROBLOGSERVICE_H_*/

