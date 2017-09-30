#ifndef SEARCH_SERVICE_STRATEGY_H_
#define SEARCH_SERVICE_STRATEGY_H_

#include "ISearchService.h"
#include "CefClient.h"

#include <string>

namespace ucclient {

using namespace std;
using namespace uc;

class SearchServiceStrategy : public ISearchServiceEvent  {
public:
	SearchServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);
	virtual ~SearchServiceStrategy();

	int32_t SearchServer(const wstring& key, int32_t page, int32_t count, const std::list<int32_t>& scope, int requestId);

	int32_t SearchMessage(ConversationType type, int64_t relateId, const wstring& key, int32_t page, int32_t count, int requestId);

	int32_t SearchLocal(const wstring& key, int32_t page, int32_t count, const std::list<int32_t>& scope, int requestId);

	virtual void OnServerSearched(int32_t errorCode, int requestId, const list<ContactInfo*>& contacts, 
		const list<GroupInfo>& groups, const list<GroupInfo>& groupChats, const list<GroupInfo>& cloudGroups, const list<Meeting*>& conferences, 
		const list<SearchMessageInfo*>& messages, int32_t chatMsgNum, const list<MessageInfo*>& chatMessages);

	virtual void OnLocalSearched(int32_t errorCode, int requestId, const list<ContactInfo*>& contacts, const list<GroupInfo>& groups);

	int64_t GetSearchRequestId();

private:
	cefIntegration::CefClient *_cefClient;
	ISearchService *_searchService;
	IUCEngine *_engine;
};

}

#endif //SEARCH_SERVICE_STRATEGY_H_