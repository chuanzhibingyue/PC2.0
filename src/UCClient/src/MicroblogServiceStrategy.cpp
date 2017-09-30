#include "MicroblogServiceStrategy.h"
#include "UCDefs.h"


MicroblogServiceStrategy::MicroblogServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient)
	: _engine(engine), _cefClient(cefClient) {
	_microblogService = (IMicroblogService*)_engine->QueryService(UCMicroblogService);
}


MicroblogServiceStrategy::~MicroblogServiceStrategy() {
}

int32_t MicroblogServiceStrategy::GetRelation(int32_t userId) {
	return _microblogService->GetRelation(userId);
}

std::wstring MicroblogServiceStrategy::GetHomePageURL(int32_t userId) {
	return StringConvert::FromUTF8Str(_microblogService->GetHomePageURL(userId));
}

std::wstring MicroblogServiceStrategy::GetUserHomePageURL(int32_t selfId, int32_t userId){
	return StringConvert::FromUTF8Str(_microblogService->GetUserHomePageURL(selfId, userId));
}

void MicroblogServiceStrategy::GetNewMsgCount() {
	_microblogService->GetNewMsgCount();
}

void MicroblogServiceStrategy::AddAttention(int32_t userId) {
	_microblogService->AddAttention(userId);
}

void MicroblogServiceStrategy::CancelAttention(int32_t userId) {
	_microblogService->CancelAttention(userId);
}


void MicroblogServiceStrategy::ShareMeetingRecording(std::wstring urlVideo, std::wstring urlThumb, std::wstring fileName, int64_t fileSize, int64_t voiceTime, std::wstring comments)
{
	std::string utf8UrlVideo = StringConvert::ToUTF8Str(urlVideo);
	std::string utf8UrlThumb = StringConvert::ToUTF8Str(urlThumb);
	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);
	std::string utf8Comments = StringConvert::ToUTF8Str(comments);

	_microblogService->ShareMeetingRecording(utf8UrlVideo, utf8UrlThumb, utf8FileName, fileSize, voiceTime, utf8Comments);
}


// microblogservice event
void MicroblogServiceStrategy::OnNewMsgCountGet(int32_t errorCode, int32_t countAll, int32_t relationCount, int32_t timenewsCount, int32_t pollCount, int32_t caseCount) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << MICROBLOG_SERVICE_GETNEWMSGCOUNT_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", countAll: " << countAll
		<< ", relationCount: " << relationCount << ", timenewsCount: " << timenewsCount
		<< ", pollCount: " << pollCount << ", caseCount: " << caseCount
		<< "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnNewMsgCountGet event to JS, errorCode: " << errorCode;
}

void MicroblogServiceStrategy::OnAttentionAdd(int32_t errorCode, int32_t userId, int32_t relation) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << MICROBLOG_SERVICE_ADDATTENTION_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", userId: " << userId
		<< ", relation: " << relation << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAttentionAdd event to JS, errorCode: " << errorCode;
}

void MicroblogServiceStrategy::OnAttentionCancelled(int32_t errorCode, int32_t userId, int32_t relation) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << MICROBLOG_SERVICE_CANCELATTENTION_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", userId: " << userId
		<< ", relation: " << relation << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAttentionCancelled event to JS, errorCode: " << errorCode;
}

void MicroblogServiceStrategy::OnMeetingRecordingShared(int32_t errorCode, int64_t resourceID)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << MICROBLOG_SERVICE_RECORDINGSHARED_EVENT
		<< "\",{detail:" << "{errorCode: " << errorCode << ", resourceId: " << resourceID << "},});"
		<< "document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMeetingRecordingShared event to JS, errorCode: " << errorCode << ", resourceId = " << resourceID;
}
