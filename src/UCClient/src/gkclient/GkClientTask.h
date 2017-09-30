#ifndef GK_CLIENT_TASK_H_
#define GK_CLIENT_TASK_H_

#include "GkClientMessage.h"
#include "IGroupService.h"

namespace ucclient {

enum GkClientTaskState {
	GKTS_INITIAL,				// task is created, group id is still not received;

	GKTS_WAITING_GROUP_INFO,	// waiting for group info from server;
	GKTS_GROUP_NOT_EXIST,		// didn't find the group;
	GKTS_GROUPID_RECEIVED,		// group id is received;

	GKTS_CREATING_GROUP,		// creating a group;
	GKTS_WAITING_GROUP_CREATED,	// waiting for group to be created;
	GKTS_GROUP_CREATED,			// group is created;

	GKTS_WAITING,				// task is waiting for some http calls;

	GKTS_DONE,					// task is finished;
	GKTS_FAILED					// task failed;
};

enum GkClientTaskType{
	GKTT_NONE,
	GKTT_CREATE_GROUP,
	GKTT_UPDATE_URL,
	GKTT_UPDATE_NAME,
	GKTT_END_GROUP,
	GKTT_ADD_MEMBER,
	GKTT_KICK_MEMBER,
	GKTT_FILE,
};

class GkClientTask {
public:
	GkClientTask(GkClientMessage& message, const std::wstring& jsonMsg, int32_t mountId, uc::UCID groupUid = uc::UCID(),
		GkClientTaskState state = GKTS_INITIAL, GkClientTaskType type = GKTT_NONE) : 
		message(std::move(message)), jsonMsg(jsonMsg), mountId(mountId), groupUid(groupUid),
		state(state), type(type) {
	}


	GkClientTaskType type;
	uc::UCID groupUid;
	GkClientMessage message;
	std::wstring jsonMsg;
	int32_t mountId;
	GkClientTaskState state;
	int32_t res;
	int64_t delayedRunTime;
};

}

#endif