/*
 * ConversationDao.h
 *
 *  Created on: 2015年6月19日
 *      Author: jianxue
 */

#ifndef CONVERSATIONDAO_H_
#define CONVERSATIONDAO_H_

#include <vector>
#include "ConversationInfo.h"
#include "AppDao.h"
#include "client_service/ConversationInfoInternal.h"

namespace uc {

class ConversationDao : public AppDao {
public:
	ConversationDao();
	virtual ~ConversationDao();

public:

	/**
	 * 从当前数据库中获取最新conversation列表
	 * @param number 请求的数量
	 * @param conversations 返回的实际数量有可能小于请求的number数量
	 * @return 成功: 0 失败: 非0
	 */
	int32_t LoadLatestConversations(int32_t number, std::list<ConversationInfo*> &conversations);

	/**
	 * Save conversation list in DB
	 * @param conversations conversation list data
	 * @return true: 0 false: 非0
	 */
	int32_t SaveConversations(std::list<ConversationInfo*>& conversations);

	int32_t DeleteConversation(ConversationType type, int64_t relateId); //与remove conversation不同，这里是标记在界面上删除了

	int32_t GetDeleteTime(ConversationType type, int64_t relateId, int64_t & removeTime);

	int32_t SaveConversationInfo(int32_t type, int64_t msgSeq, int64_t msgTime);

	/**
	 * 从表中获取last_msgseq的最大值
	 * @param msgSeq
	 * @return
	 */
	int32_t GetMaxMessageSeqData(int64_t &msgSeq);

	/**
	 * 根据指定的type和relateId从数据库中删除
	 * @param conversationId
	 * @return
	 */
	int32_t RemoveConversation(int32_t type, int64_t relateId);

	/**
	 * 清空所有的conversation对象
	 * @return
	 */
	int32_t RemoveAllConversations();

private:
	int32_t SaveConversationInfo(const ConversationInfoInternal &conversation);
	void SetConversation(CppSQLite3Query& query, ConversationInfoInternal& conversation);

private:
	CppSQLite3DB* _db;
};

} /* namespace uc */

#endif /* CONVERSATIONDAO_H_ */
