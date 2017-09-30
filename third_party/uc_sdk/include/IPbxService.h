/*
 * IPbxService.h
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#ifndef IPBXSERVICE_H_
#define IPBXSERVICE_H_
#pragma once

#include "UCSDK.h"
#include "IUCEngine.h"
#include "base/HTTPRequestEvent.h"
#include "AC_PbxCallRecord.h"

namespace uc {
	interface UC_API IPbxServiceEvent;
	interface UC_API IPbxService : public IUCService, Event<IPbxServiceEvent> {
		virtual ~IPbxService(){};

		virtual void ConnectPbx(int32_t userID, const std::string &token, int expiresIn) = 0;
		virtual void SetProxyUrl(const std::string& proxyUrl) = 0;
		virtual void SetProxyInfo(int32_t auth_type, const std::string& auth_account, const std::string& auth_passwd) = 0;

		virtual int32_t CheckLogin(int &nRet) = 0;
		// VoIP呼叫
		virtual int32_t MakeCall(const std::string &userName, int userID, const std::string &number,
			int callType, const std::string &postValue, __int64 &timeStamp, int32_t &taskID) = 0;
		// 来电应答
		virtual int32_t AnswerCall(int32_t taskID, int callType) = 0;
		// 拒接来电
		virtual int32_t RejectCall(int32_t taskID) = 0;
		// 挂断通话
		virtual int32_t HangupCall(int32_t taskID) = 0;
		// 通话保持
		virtual int32_t HoldCall(int32_t taskID) = 0;
		// 解除通话保持
		virtual int32_t UnHoldCall(int32_t taskID) = 0;
		// 通话静音
		virtual int32_t MuteCall(int32_t taskID) = 0;
		// 解除通话静音
		virtual int32_t UnMuteCall(int32_t taskID) = 0;
		// 调整通话麦克风音量
		virtual int32_t AdjustMicLevel(int32_t taskID, int level) = 0;
		// 刷新麦克风
		virtual int32_t RefreshMic(int &micState) = 0;
		// 发送DTMF
		virtual int32_t SendDTMF(int32_t taskID, const std::string &dtmf) = 0;
		// 获取当前PBX号码
		virtual int32_t GetCurrentPBXAccount(std::string &pbxAccount) = 0;
		//////////////////////////////////////////////////////////////////////////
		virtual int32_t IncomingCallUserName(int32_t taskID, std::string &userName) = 0;

		virtual int32_t GetCallRecordLst(int32_t &count, int64_t timeStamp) = 0;

		virtual int32_t GetCallDetailsLst(const std::string &number, int userID, int64_t timeStamp, int count) = 0;

		virtual int32_t SearchCallRecord(const std::string &keyWord, int64_t timeStamp, int count) = 0;

		virtual int32_t SaveCallDetails(int64_t timeStamp, int taskID, const std::string &number,
			int userID, const std::string &name, int incoming, int type, int state, int druation) = 0;

		// 双向呼叫
		virtual int32_t MakeTwoWayCall(const std::string &toNumber, 
			const std::string &fromNumber, int openRecord) = 0;
		// 通话中开始或停止录音
		virtual int32_t SwitchRecordInCall(int recordSwitch, const std::string &callUUID) = 0;
		// 获取所有录音列表目录
		virtual int32_t GetAllRecord(int recordType) = 0;
		// 获取单次通话录音
		virtual int32_t GetRecord(int recordType, const std::string &callUUID) = 0;
	};

	// class PbxServiceStrategy : public IPbxServiceEvent
	interface UC_API IPbxServiceEvent {
		virtual ~IPbxServiceEvent(){};

		virtual void OnLoginPbx(int32_t errorCode) = 0;

		virtual void OnStatusChanged(__int64 timeStamp, int32_t taskID,
			int callState, const char* resultCode, const char* callUUID) = 0;

		virtual void OnMakeCall(int32_t errorCode, __int64 timeStamp, 
			int32_t taskID, int micState, const char* toNumber) = 0;

		virtual void OnCallIncoming(__int64 timeStamp, int32_t taskID, int callType,
			const char* fromNumber, const char* postValue) = 0;

		virtual void OnServiceStateChanged(int stateCode) = 0;

		virtual void OnCallHangup(const AC_PbxCallRecord &data) = 0;

		virtual void OnGetCallRecordLst(int32_t errorCode, const PbxCallRecordList &lstData) = 0;

		virtual void OnGetCallDetailsLst(int32_t errorCode, const PbxCallRecordList &lstData) = 0;

		virtual void OnSearchCallRecord(int32_t errorCode, const PbxCallRecordList &lstData) = 0;
	};
}

#endif /* IPBXSERVICE_H_ */
