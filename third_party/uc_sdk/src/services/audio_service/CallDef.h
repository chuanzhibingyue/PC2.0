/*
 * CallDef.h
 *
 *  Created on: 2015年8月3日
 *      Author: hualong.zhang
 */


#ifndef CALLDEF_H_
#define CALLDEF_H_

#include <string>

#define CALL_WIN		(2)
#define CALL_MOBILE		(6)

namespace uc {

	//加入呼的方式
	enum JoinType {
		Create_Call = 0,
		Join_Call = 1
	};

	//呼类型
	enum CallType {
		PROJECT_CALL = 1,	//项目组call
		GROUP_CALL = 2,		//讨论组call
		SINGLE_CALL = 3		//1对1呼
	};

	//呼基本信息
	struct CallInfo {
		CallType callType; //呼类型
		JoinType JoinType; //加入类型
		int conferenceId; //call会议ID
		int callId; //一对一为对端用户ID；群呼为群ID
		int tangUserId; //tang 用户ID
		CallInfo() :callType(SINGLE_CALL),
			JoinType(Create_Call),
			conferenceId(0), callId(0), 
			tangUserId(0) {
		}
	};

	struct UserCallStatus {
		int userId;
		int status;
		UserCallStatus() 
			:userId(0), 
			status(0) {
		}
	};

}

#endif //CALLDEF_H_