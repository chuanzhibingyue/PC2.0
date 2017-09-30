/*
 * IExternalService.h
 *
 *  Created on: Jan 27, 2016
 *      Author: yongshuai.cheng
 */

#ifndef IEXTERNALSERVICE_H_
#define IEXTERNALSERVICE_H_

#include "UCSDK.h"
#include "IUCEngine.h"
#include "ExternalInfo.h"

namespace uc {

	interface UC_API IExternalServiceEvent {
		virtual ~IExternalServiceEvent() {};

		/**
		* 聊天历史列表;
		* @param result  	     错误信息;
		* @param type			 会话类型,单人/讨论组;
		* @param relateId	     用户ID/讨论组ID;
		* @param msgCount		 message count;
		* @param min_timestamp   min timestamp;
		*/
		virtual void OnChatHistoryReceived(int32_t result, ConversationType type, int64_t relateId, int32_t msgCount, int64_t min_timestamp, const list<ExternalMessage*>& msgList) = 0;
	};

	interface UC_API IExternalService : public IUCService, Event<IExternalServiceEvent>, IUCEngineEvent {
		virtual ~IExternalService() {};
		/**
		* 获取历史聊天内容接口;
		* @param type               聊天类型;
		* @param externalId         请求目标ID(P2P:用户账号/讨论组:外部ID);
		* @param count              请求聊天内容数量，默认20，不能超过200;
		* @param startTime          请求聊天内容的开始时间;
		* @param endTime            请求聊天内容的结束时间;
		* @return					错误码;
		*/
		virtual int32_t GetChatHistory(ConversationType type, const string& externalId, int32_t count, int64_t startTime, int64_t endTime) = 0;

		/**
		* 是否显示外部消息;
		* @param externalId		请求目标ID(P2P:用户账号/讨论组:外部ID);
		* @return					 0：不显示/1：显示;
		*/
		virtual int32_t ShowExternalMessage(const string& externalId) = 0;

	};
}

#endif /* IEXTERNALSERVICE_H_ */
