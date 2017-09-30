#include "IClientService.h"
#include <boost/thread.hpp>
#include "LogManager.h"
#include "StringConvert.h"
#include "UCErrorCode.h"
#include "ClientInfo.h"
#include "UCMockClient.h"
#include "gtest/gtest.h"
#include "UCCommonFunctionMock.h"

using namespace uc;

UCMockClient::UCMockClient(const ClientInfo &clientinfo){
	_clientService = NULL;
	_clientinfo = clientinfo;
}

void UCMockClient::Init(IUCEngine *ucEngine) {
	_clientService = (IClientService*)ucEngine->QueryService(UCClientService);
	_clientService->registerListener(this);
}	
	
void UCMockClient::UnInit() {
	_clientService->removeListener(this);
}

void UCMockClient::Login() {
	_clientService->Login(_clientinfo.clientType, _clientinfo.account, _clientinfo.password, _clientinfo.clientId, _clientinfo.mac, _clientinfo.pinNumber, _clientinfo.version, _clientinfo.storeType, _clientinfo.phoneNumber);
}

void UCMockClient::Logout() {
	_clientService->Logout();
}

void UCMockClient::ConnectUCAS() {
	_clientService->ConnectUCAS();
}

void UCMockClient::DisconnectUCAS() {
	_clientService->DisconnectUCAS();
}

void UCMockClient::ResetPassword(){
	_clientService->ResetPassword(_clientinfo.newPass,_clientinfo.confirmPass);
}

void UCMockClient::OnLogin(int32_t errorCode) {

	NotifyResult(errorCode);

	cout << "OnLogin result: " << errorCode << endl;
}		
	
void UCMockClient::OnLogout(int32_t errorCode) {
	NotifyResult(errorCode);
	cout << "i'm logout bye:" << errorCode << endl;
}
	
void UCMockClient::OnPasswordReset(int32_t errorCode) {
	NotifyResult(errorCode);
	cout << "OnPasswordReset:" << errorCode << endl;
}
	
void UCMockClient::OnPasswordModified(const string &password) {
	LOG(INFO) << "OnPasswordModified:" << password;
}
	
void UCMockClient::OnPasswordRuleUpdated(int32_t type) {
	LOG(INFO) << "OnPasswordRulesUpdated:" << type;
}
	
void UCMockClient::OnPasswordExpired(int32_t dueDays) {
	LOG(INFO) << "OnPasswordExpired:" << dueDays;
}
	
void UCMockClient::OnUCASConnected(int32_t errorCode) {
	NotifyResult(errorCode);
	cout << "OnUCASConnected:" << errorCode << endl;
}

void UCMockClient::OnUCASDisconnected(int32_t errorCode) {
	cout << "OnUCASDisconnected:" << errorCode << endl;
}
	
void UCMockClient::OnConversationListReceived(int32_t errorCode, const std::list<ConversationInfo*> *conversations, bool clearCache) {
	LOG(INFO) << "OnConversationListReceived" << errorCode;
}
