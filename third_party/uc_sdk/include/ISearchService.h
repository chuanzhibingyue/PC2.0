/*
 * ISearchService.h
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#ifndef ISEARCHSERVICE_H_
#define ISEARCHSERVICE_H_

#include "UCSDK.h"
#include "common/event.h"
#include "IUCEngine.h"
#include "ContactInfo.h"
#include "GroupInfo.h"
#include "MessageInfo.h"
#include "SearchInfo.h"
#include "CalendarData.h"

namespace uc {

interface UC_API ISearchServiceEvent {
	virtual ~ISearchServiceEvent(){};

	/**
	 * 消息页签搜索返回事件
	 * @param errorCode 错误码
	 * @param contacts 联系人信息
	 * @param groups 讨论组信息
	 * @param conferences 会议信息
	 * @param messages 全局聊天信息
	 * @param chatMsgNum  某个会话聊天关键字总数
	 * @param messages 某个会话聊天信息
	 * @return 成功：0 失败：非0
	 */
	virtual void OnServerSearched(int32_t errorCode, int requestId, const list<ContactInfo*>& contacts, 
		const list<GroupInfo>& groups, const list<GroupInfo>& groupChats,const list<GroupInfo>& cloudGroups, const list<Meeting*>& meetings,
		const list<SearchMessageInfo*>& messages, int32_t chatMsgNum, const list<MessageInfo*>& chatMessages) = 0;

	/**
	 * 本地搜索搜索返回事件
	 * @param errorCode 错误码
	 * @param contacts 联系人信息
	 * @param groups 讨论组信息
	 * @return 成功：0 失败：非0
	 */
	virtual void OnLocalSearched(int32_t errorCode, int requestId, const list<ContactInfo*>& contacts, const list<GroupInfo>& groups) = 0;

};

interface UC_API ISearchService : public IUCService, Event<ISearchServiceEvent>, IUCEngineEvent {
	virtual ~ISearchService(){};
	
	/**
	 * 消息页签搜索
	 * @param key 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @return 成功：0 失败：非0
	 */
	virtual int SearchServer(const string& key, int32_t page, int32_t count, const std::string& scope, int requestId) = 0;


	/**
	* 历史聊天页签搜索消息
	* @param type 会话类型
	* @param relateId 目标ID
	* @param key 搜索关键字
	* @param page 第几页
	* @param count 每页数量
	* @return 成功：0 失败：非0
	*/
	virtual int SearchMessage(ConversationType type, int64_t relateId, const string& key, int32_t page, int32_t count, int requestId) = 0;

	/**
	 * 从本地搜索通讯录
	 * @param key 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @param contacts 联系人搜索结果
	 * @param groups 讨论组搜索结果
	 * @return 成功：0 失败：非0
	 */
	virtual int SearchLocal(const string& key, int32_t page, int32_t count, const std::list<int32_t>& scope, int requestId) = 0;


};

}

#endif /* ISEARCHSERVICE_H_ */
