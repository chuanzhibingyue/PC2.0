///////////////////////////////////////////////////////////
//  ContactDao.h
//  Implementation of the Class ContactDao
//  Created on:      22-五月-2015 10:35:26
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(CONTACTDAO_H_)
#define CONTACTDAO_H_

#include "AppDao.h"
#include "contact_service/ContactInfoImpl.h"
#include "ContactTag.h"

#include <list>

namespace uc {

using namespace std;

class ContactDao : public AppDao {
public:
	ContactDao();
	virtual ~ContactDao();

	int SaveContact(const ContactInfoImpl& data);
	int SaveContact(const list<ContactInfoImpl*>& contactList);
	int GetContact(int userId, ContactInfoImpl& data);
	int GetContact(const string& account, ContactInfoImpl& data);
	int GetContactListByType(int type, list<ContactInfoImpl*>& contactList);
	int GetContactListByName(const string& keyWord, int type, list<ContactInfoImpl*>& contactList);
	int GetContactListByName(const string& keyWord, int page, int count, list<ContactInfo*>& contactList);
	int GetContactListByUserID(const list<int>& userIDList, list<ContactInfoImpl*>& contactList);
	int UpdateContact(int userId, const ContactInfoImpl& data);
	int UpdateLastUpdateTime(int userId, int64_t timestamp);
	int UpdateLastUpdateTimeByTypes(int type, int64_t timestamp);
	int UpdateUserType(int userId, int type);
	//string GetContactLocalAvatar(int userId);

private:
	void SetContact(CppSQLite3Query& query, ContactInfoImpl& data);
	int SetContactList(CppSQLite3Query& query, list<ContactInfoImpl*>& contactList);
	int SetContactList(CppSQLite3Query& query, list<ContactInfo*>& contactList);

private:
	CppSQLite3DB* _db;
};

}
#endif // !defined(CONTACTDAO_H_)
