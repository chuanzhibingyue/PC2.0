/*
 * SearchService.h
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#ifndef SEARCHSERVICE_H_
#define SEARCHSERVICE_H_

#include "ISearchService.h"
#include "IUCEngine.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace uc {

typedef std::list<SearchMessageInfo*> SearchMessageList;

class SearchService: public ISearchService {
public:
	SearchService(IUCEngine *engine);
	virtual ~SearchService();

public:
	
	/**
	 * 消息页签搜索
	 * @param key 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @param scope 搜索scope
	 * @return 成功：0 失败：非0
	 */
	virtual int SearchServer(const string& key, int32_t page, int32_t count, const std::string& scope, int requestId);

	/**
	* 历史聊天页签搜索消息
	* @param type 会话类型
	* @param relateId 目标ID
	* @param key 搜索关键字
	* @param page 第几页
	* @param count 每页数量
	* @return 成功：0 失败：非0
	*/
	virtual int SearchMessage(ConversationType type, int64_t relateId, const string& key, int32_t page, int32_t count, int requestId);

	/**
	 * 从本地搜索通讯录
	 * @param key 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @param contacts 联系人搜索结果
	 * @param groups 讨论组搜索结果
	 * @return 成功：0 失败：非0
	 */
	virtual int SearchLocal(const string& key, int32_t page, int32_t count, const std::list<int32_t>& scope, int requestId);


private:
	int InitRequest(const string& key, int32_t page, int32_t count, const string& scope, HTTPRequest& request);
	int GetResponseCode(HTTPResponse* response, boost::property_tree::wptree& ptRoot);
	void GetContacts(const boost::property_tree::wptree& ptParse, list<ContactInfo*>& contacts);
	void GetGroups(const boost::property_tree::wptree& ptParse, list<GroupInfo>& groups);
	void GetConferences(const boost::property_tree::wptree& ptParse, list<Meeting*>& meetings);
	void GetGlobalMessages(const boost::property_tree::wptree& ptParse, SearchMessageList& messages);
	void GetMessages(const boost::property_tree::wptree& ptParse, list<MessageInfo*>& messages);
	UCID GetCurUser();

private:
	template<class T>
	void DelContainer(T& container) {
		for (typename T::iterator itor = container.begin(); itor != container.end();
			++itor){
			delete *itor;
			*itor = NULL;
		}
		container.clear();
	}




protected:
	int32_t OnServerSearched(int32_t errorCode, HTTPResponse* response, int requestId);
};

} /* namespace uc */
#endif /* SEARCHSERVICE_H_ */
