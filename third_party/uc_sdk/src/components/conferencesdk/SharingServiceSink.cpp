#include "SharingServiceSink.h"
#include "LogManager.h"

using namespace uc;

SharingServiceSink::SharingServiceSink(int conferenceId)
:m_conferenceId(conferenceId) {

}

SharingServiceSink::~SharingServiceSink() {

}

void SharingServiceSink::onSessionErrorHandle(CGNetTangSessionErrorInfo *pErrorInfo) {
	UC_LOG(ERROR) << "Desktop error: messageID:" << pErrorInfo->messageID
		<< ",description:" << pErrorInfo->description
		<< ",statusCode:" << pErrorInfo->statusCode;
	m_pDelegate->OnErrorHandle(std::atoi(pErrorInfo->statusCode));
}

void SharingServiceSink::onDesktopShared() {
	UC_LOG(INFO) << "onDesktopShared";
	int userId = getShareUserID();
	m_pDelegate->OnDesktopShared(m_conferenceId, userId);
	UC_LOG(INFO) << "onDesktopShared, conferenceId:" << m_conferenceId << ", userId:" << userId;
}

void SharingServiceSink::onDesktopShareStoped() {
	UC_LOG(INFO) << "onDesktopShareStoped";
	int userId = getShareUserID();
	m_pDelegate->OnDesktopShareStoped(m_conferenceId, userId);
	UC_LOG(INFO) << "onDesktopShareStoped, conferenceId:" << m_conferenceId << ", userId:" << userId;
}

void SharingServiceSink::onDesktopViewerStarted() {
	UC_LOG(INFO) << "onDesktopViewerStarted, conferenceId:" << m_conferenceId;
	m_pDelegate->OnDesktopViewerStarted(m_conferenceId);
}

void SharingServiceSink::onDesktopViewerStopped() {
	UC_LOG(INFO) << "onDesktopViewerStopped, conferenceId:" << m_conferenceId;
	m_pDelegate->OnDesktopViewerStopped(m_conferenceId);
}

int SharingServiceSink::getShareUserID() {
	VARIANT var;
	VariantInit(&var);
	m_psession->getPropertyValue("shareUserID", var);
	int tangUserId = _wtof(var.bstrVal);
	IGNetTangUser *pUser = _conference->getUserByID(tangUserId);
	if (pUser == NULL) {
		UC_LOG(ERROR) << "getUserByID return null, tangUserId:" << tangUserId;
		return 0;
	}

	char *userName = pUser->getUserName();
	int userId = std::atoi(userName);

	UC_LOG(INFO) << "getshareUserID! tangUserId:" << tangUserId << ", userId:" << userId;
	return userId;
}