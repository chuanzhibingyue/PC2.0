#include "IClientService.h"
#include <boost/thread.hpp>
#include "LogManager.h"
#include "StringConvert.h"
#include "UCErrorCode.h"
#include "ClientInfo.h"
#include "ContactServiceMockClient.h"


using namespace uc;

void ContactServiceMockClient::Init(IUCEngine *ucEngine) {
	_contactSerivce = (IContactService*)ucEngine->QueryService(UCContactService);
	_contactSerivce->registerListener(this);
}

void ContactServiceMockClient::UnInit() {
	_contactSerivce->removeListener(this);
}

void ContactServiceMockClient::GetUpdatedContacts() {
	_contactSerivce->GetUpdatedContacts();
}

/*
void ContactServiceMockClient::GetContactInfo(int32_t userId) {
	//_contactSerivce->GetContactInfo(userId);
}
*/

void ContactServiceMockClient::OnContactUpdated(std::list<ContactInfo*> *contact) {
	cout <<"OnContactUpdated response:"<<resultCode<<endl;
}

void ContactServiceMockClient::OnContactInfoReceived(int32_t result, const std::list<int32_t>* userIds,
        const std::list<ContactInfo*>* contacts) {

		std::list<int32_t>::const_iterator iter = userIds->begin();
		if (iter != userIds->end()) {
			int32_t userId = *iter;
			cout <<"OnContactInfoReceived userId: "<<userId<<endl;
		}

		std::list<ContactInfo*>::const_iterator iterContact = contacts->begin();
		if (iterContact != contacts->end()) {
			(*iterContact)->GetAccount();
			(*iterContact)->GetPingyin();
			//cout <<"OnContactInfoReceived userId: "<<userId<<endl;
		}

		this->mthodMtx.lock();
		//resultCode = result;
		this->methodCond.notify_one();
		this->mthodMtx.unlock();

}
