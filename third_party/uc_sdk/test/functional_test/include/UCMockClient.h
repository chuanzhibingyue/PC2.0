#ifndef UC_MOCK_CLIENT_H_
#define UC_MOCK_CLIENT_H_

#include "IClientService.h"
#include "ClientInfo.h"
#include "StringConvert.h"
#include "UCMockBase.h"

using namespace uc;

class UCMockClient : public IClientServiceEvent, public UCMockBase {
public:
	UCMockClient(const ClientInfo &clientinfo);

	//UCMockClient::~UCMockClient(){}
	~UCMockClient(){}

	void Init(IUCEngine *ucEngine); 
	void UnInit();
	void Login();
	void Logout();
	void ConnectUCAS();
	void DisconnectUCAS();
	void ResetPassword();


	virtual void OnLogin(int32_t errorCode);
	virtual void OnLogout(int32_t errorCode);	
	virtual void OnPasswordReset(int32_t errorCode) ;		
	virtual void OnPasswordModified(const string &password) ;	
	virtual void OnPasswordRuleUpdated(int32_t type) ;	
	virtual void OnPasswordExpired(int32_t dueDays) ;	
	virtual void OnUCASConnected(int32_t errorCode) ;	
	virtual void OnUCASDisconnected(int32_t errorCode) ;
	virtual void OnConversationListReceived(int32_t errorCode, const std::list<ConversationInfo*> *conversations, bool clearCache);
public:
	ClientInfo _clientinfo;
private:
	IClientService *_clientService;
};

#endif