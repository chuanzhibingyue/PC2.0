#include "SearchServiceStrategy.h"
#include "ServiceObjectConversion.h"
#include "UCDefs.h"

#include <limits>

namespace ucclient {

#undef max

SearchServiceStrategy::SearchServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient) 
	: _engine(engine), _cefClient(cefClient) {
	_searchService = (ISearchService*)_engine->QueryService(UCSearchService);
}

SearchServiceStrategy::~SearchServiceStrategy() {

}

int32_t SearchServiceStrategy::SearchServer(const wstring& key, int32_t page, int32_t count, const std::list<int32_t>& scope, int requestId) {
	std::string searchKey = StringConvert::ToUTF8Str(key);
	std::stringstream searchScope;
	searchScope << "[";

	std::list<int32_t>::const_iterator last = --scope.end();
	for (std::list<int32_t>::const_iterator itor = scope.begin(); 
		itor != scope.end(); ++itor) {
		searchScope << *itor;
		if (itor != last)
			searchScope << ", ";
	}

	searchScope << "]";
	return _searchService->SearchServer(searchKey, page, count, searchScope.str(), requestId);
}

int32_t SearchServiceStrategy::SearchMessage(ConversationType type, int64_t relateId, const wstring& key, int32_t page, int32_t count, int requestId){
	
	std::string searchKey = StringConvert::ToUTF8Str(key);
	return _searchService->SearchMessage(type, relateId, searchKey, page, count, requestId);
}

int32_t SearchServiceStrategy::SearchLocal(const wstring& key, int32_t page, int32_t count, const std::list<int32_t>& scope, int requestId) {
	std::string searchKey = StringConvert::ToUTF8Str(key);
	return _searchService->SearchLocal(searchKey, page, count, scope, requestId);
}

void SearchServiceStrategy::OnServerSearched(int32_t errorCode, int requestId, const list<ContactInfo*>& contacts, 
	const list<GroupInfo>& groups, const list<GroupInfo>& groupChats, const list<GroupInfo>& cloudGroups, 
	const list<Meeting*>& conferences, const list<SearchMessageInfo*>& messages, int32_t chatMsgNum, const list<MessageInfo*>& chatMessages) {
		std::wstringstream jsScript;
		jsScript<<L"var e = new CustomEvent(\"" << SERVER_SEARCH_RESULT_RECEIVED
			<< "\",{detail:" << "{errorCode: " << errorCode << ", requestId: " << requestId
            << ", chatMsgNum: " << chatMsgNum;

		//contacts
		jsScript << ", contacts: [";
		jsScript << ServiceObjectConversion::ConvertContactToJSON(contacts);

		//groups
		jsScript << "], groups: [";
		jsScript << ServiceObjectConversion::ConvertGroupInfoToJSON(groups);

		//group chat
		jsScript << "], groupChats: [";
		jsScript << ServiceObjectConversion::ConvertGroupInfoToJSON(groupChats);

		//cloud groups
		jsScript << "], cloudGroups: [";
		jsScript << ServiceObjectConversion::ConvertGroupInfoToJSON(cloudGroups);

		//conferenceInfos
		jsScript << "], conferenceInfos: [";
		jsScript << ServiceObjectConversion::ConvertMeetingToJSON(conferences);

		//messages
		jsScript << "], messages: [";
		jsScript << ServiceObjectConversion::ConvertSearchMessageInfosToJSON(messages);

		//chat history messages
		jsScript << "], chatMessages: [";
		jsScript << ServiceObjectConversion::ConvertMessageInfosToJSON(chatMessages);


		jsScript << "]},});document.dispatchEvent(e);";

		_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

		UC_LOG(INFO) << "dispatch OnAllSearched event to JS:" << jsScript.str();
}

void SearchServiceStrategy::OnLocalSearched(int32_t errorCode, int requestId, const list<ContactInfo*>& contacts, const list<GroupInfo>& groups) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << LOCAL_SEARCH_RESULT_RECEIVED
		<< "\",{detail:" << "{errorCode: " << errorCode << ", requestId: " << requestId;

	//contacts
	jsScript << ", contacts: [";
	jsScript << ServiceObjectConversion::ConvertContactToJSON(contacts);

	//groups
	jsScript << "], groups: [";
	jsScript << ServiceObjectConversion::ConvertGroupInfoToJSON(groups);

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAddressBookSearched event to JS:" << jsScript.str();
}

//生成搜索请求id，不存在多线程访问的场景
int64_t SearchServiceStrategy::GetSearchRequestId() {
	static int64_t searchId = 0;
	if (searchId == std::numeric_limits<int64_t>::max()) {
		searchId = 0;
	}
	return searchId++;
}

}