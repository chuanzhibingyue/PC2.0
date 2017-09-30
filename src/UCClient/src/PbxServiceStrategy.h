#ifndef PBX_SERVICE_STRATEGY_H_
#define PBX_SERVICE_STRATEGY_H_

#include "IPbxService.h"
#include "CefClient.h"
#include "assist/PbxServiceJSEventAssist.h"

namespace ucclient {

using namespace uc;

class PbxServiceStrategy : public IPbxServiceEvent {
public:
	PbxServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);
	virtual ~PbxServiceStrategy();

public:
	// 
	virtual int32_t CheckLogin(int &nRet);
	// VoIP呼叫
	virtual int32_t MakeCall(const std::string &userName, int userID, const std::string &number,
		int callType, const std::string &postValue, __int64 &timeStamp, int32_t &taskID);
	// 来电应答
	virtual int32_t AnswerCall(int32_t taskID, int callType);
	// 拒接来电
	virtual int32_t RejectCall(int32_t taskID);
	// 挂断通话
	virtual int32_t HangupCall(int32_t taskID);
	// 通话保持
	virtual int32_t HoldCall(int32_t taskID);
	// 解除通话保持
	virtual int32_t UnHoldCall(int32_t taskID);
	// 通话静音
	virtual int32_t MuteCall(int32_t taskID);
	// 解除通话静音
	virtual int32_t UnMuteCall(int32_t taskID);
	// 调整通话麦克风音量
	virtual int32_t AdjustMicLevel(int32_t taskID, int level); 
	// 刷新麦克风
	virtual int32_t RefreshMic(int &micState);
	// 发送DTMF
	virtual int32_t SendDTMF(int32_t taskID, const std::string &dtmf);
	// 获取当前PBX号码
	virtual int32_t GetCurrentPBXAccount(std::string &pbxAccount);

	virtual int32_t IncomingCallUserName(int32_t taskID, std::string &userName);

	virtual int32_t GetCallRecordLst(int32_t &count, int64_t timeStamp);

	virtual int32_t GetCallDetailsLst(const std::string &number, int userID, int64_t timeStamp, int count);

	virtual int32_t SearchCallRecord(const std::string &keyWord, int64_t timeStamp, int count);

	virtual int32_t SaveCallDetails(int64_t timeStamp, int taskID, const std::string &number,
		int userID, const std::string &name, int incoming, int type, int state, int druation);

	// 通话中开始或停止录音
	virtual int32_t SwitchRecordInCall(int recordSwitch, const std::string &callUUID);
	// 双向呼叫
	virtual int32_t MakeTwoWayCall(const std::string &toNumber, const std::string &fromNumber, int openRecord);
	// 获取所有录音列表目录
	virtual int32_t GetAllRecord(int recordType);
	// 获取单次通话录音
	virtual int32_t GetRecord(int recordType, const std::string &callUUID);

public:
	virtual void OnLoginPbx(int32_t errorCode);

	virtual void OnStatusChanged(__int64 timeStamp, int32_t taskID, int callState,
		const char* resultCode, const char* callUUID);

	virtual void OnMakeCall(int32_t errorCode, __int64 timeStamp, 
		int32_t taskID, int micState, const char* toNumber);

	virtual void OnCallIncoming(__int64 timeStamp, int32_t taskID, int callType,
		const char* fromNumber, const char* postValue);

	virtual void OnServiceStateChanged(int stateCode);

	virtual void OnCallHangup(const AC_PbxCallRecord &data);

	virtual void OnGetCallRecordLst(int32_t errorCode, const PbxCallRecordList &lstData);

	virtual void OnGetCallDetailsLst(int32_t errorCode, const PbxCallRecordList &lstData);

	virtual void OnSearchCallRecord(int32_t errorCode, const PbxCallRecordList &lstData);

private:
	cefIntegration::CefClient *_cefClient;
	IPbxService *_lpService;
	IUCEngine *_engine;
	PbxServiceJSEventAssist _jsEventAssist;
};

}

#endif  PBX_SERVICE_STRATEGY_H_ 
