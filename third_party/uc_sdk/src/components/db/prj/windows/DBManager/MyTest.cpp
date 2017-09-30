// test2.cpp : Defines the entry point for the console application.
//

#include <string>
#include <fstream>
#include <strstream>
#include <process.h>
#include "StringConvert.h"
#include "AccountInfo.h"

#include "CppSQLite3.h"

#include "Gzip.h"

using namespace std;

#include "DBManager.h"
#include "DBUpgrade.h"

using namespace uc;


#define buf_size 65535

bool ThreadAFinish = false;
bool ThreadBFinish = false;
bool ThreadCFinish = false;
bool ThreadDFinish = false;
bool ThreadEFinish = false;
static unsigned WINAPI threadFuncA(void *)
{
	DWORD start = GetTickCount();
	list<AccountInfo*> accountList1;
	for (int i = 1; i <= 500000; i++)
	{
		AccountInfo* pAccout1 = new AccountInfo;
		pAccout1->SetUserAccount(L"456@quanshi.com");
		pAccout1->SetUserName(L"456");
		pAccout1->SetAvatarUrl(L"a\\b\\c");
		pAccout1->SetLoginTime(i);
		pAccout1->SetPassword(L"123456");
		pAccout1->SetLastManualLoginTime(i);
		pAccout1->SetSavePassword(1);
		pAccout1->SetAutoLogin(0);
		accountList1.push_back(pAccout1);
		if (accountList1.size() == 500)
		{
			int ret = DBManager::GetInstance().SaveAccount(accountList1);
			assert(ret == 0);
			for (list<AccountInfo*>::iterator itor = accountList1.begin();
				itor != accountList1.end(); ++itor) {
					delete *itor;
					*itor = NULL;
			}
			accountList1.clear();
		}
	}
	ThreadAFinish = true;
	DWORD end = GetTickCount();
	printf("threadFuncA const:%d\n", (end - start));
	return 0;
}

static unsigned WINAPI threadFuncB(void *)
{
	DWORD start = GetTickCount();
	list<AccountInfo*> accountList1;
	for (int i = 500001; i <= 1000000; i++)
	{
		AccountInfo* pAccout1 = new AccountInfo;;
		pAccout1->SetUserAccount(L"456@quanshi.com");
		pAccout1->SetUserName(L"456");
		pAccout1->SetAvatarUrl(L"a\\b\\c");
		pAccout1->SetLoginTime(i);
		pAccout1->SetPassword(L"123456");
		pAccout1->SetLastManualLoginTime(i);
		pAccout1->SetSavePassword(1);
		pAccout1->SetAutoLogin(0);
		accountList1.push_back(pAccout1);
		if (accountList1.size() == 500)
		{
			int ret = DBManager::GetInstance().SaveAccount(accountList1);
			assert(ret == 0);
			for (list<AccountInfo*>::iterator itor = accountList1.begin();
				itor != accountList1.end(); ++itor) {
					delete *itor;
					*itor = NULL;
			}
			accountList1.clear();
		}
	}
	ThreadBFinish = true;
	DWORD end = GetTickCount();
	printf("threadFuncB const:%d\n", (end - start));
	return 0;
}

static unsigned WINAPI threadFuncC(void *)
{
	DWORD start = GetTickCount();
	vector<ConferenceInfo> conflist;
	for (int m = 1; m <= 1000000; m++) {
		ConferenceInfo conf;
		conf.SetConferenceId(m);
		conf.SetEventId(m);
		conf.SetHosterPassword(L"88888888");
		conf.SetAttendeePassword(L"77777777");
		conf.SetBillingCode(m);
		conf.SetCategory(0);
		conf.SetIsGnet(0);
		conf.SetStartTime(m);
		conf.SetDuration(m);
		conf.SetHostName(L"tom");
		conf.SetHostId(777);
		std::vector<Attendee> us;
		Attendee u1;
		u1.SetAccount(L"123@quanshi.com");
		u1.SetName(L"123");
		u1.SetStartTime(m);
		u1.SetStatus(1);
		u1.SetType(2);
		us.push_back(u1);
		Attendee u2;
		u2.SetAccount(L"abc@quanshi.com");
		u2.SetName(L"abc");
		u2.SetStartTime(m);
		u2.SetStatus(0);
		u2.SetType(1);
		us.push_back(u2);
		conf.SetAttendees(us);
		conf.SetTitle(L"会议测试");
		conf.SetSummary(L"conference test");
		conf.SetLocation(L"1号会议室");
		conf.SetTimeZone(L"北京");
		conf.SetLanguage(0);
		conf.SetEmailLanguage(1);
		conf.SetIsCycle(0);
		conflist.push_back(conf);
		if (conflist.size() == 100) {
			int ret = DBManager::GetInstance().SaveConference(conflist);
			conflist.clear();
		}
	}
	ThreadCFinish = true;
	DWORD end = GetTickCount();
	printf("threadFuncC const:%d\n", (end - start));
	return 0;
}

static unsigned WINAPI threadFuncD(void *)
{
	DWORD start = GetTickCount();
	list<MessageInfo*> messageList;
	wstring cotent = L"message 测试";
	for (int i = 1; i <= 2000000; i++) {
		MessageInfo* msg1  =new MessageInfo();
		msg1->SetMsgId(DBManager::GetInstance().GetMessageID());
		msg1->SetMsgSeq(i);
		msg1->SetMsgData(cotent);
		msg1->SetMsgTime(i);
		msg1->SetAppId(44);
		msg1->SetProtocolType(2);
		msg1->SetProtocolId(2);
		msg1->SetSendState(STATE_SENDSUCCESS);
		msg1->SetPlayedState(1000);
		msg1->SetConversation(2);
// 		msg1->SetFromId(2);
// 		msg1->SetFromSiteId(2);
// 		msg1->SetFromResId(2);
// 		msg1->SetToId(2);
// 		msg1->SetToSiteId(2);
// 		msg1->SetToResId(2);
		msg1->SetVersion(2);
//		msg1->SetLocalConversation(2);
		messageList.push_back(msg1);
		if (i%500 == 0) {
			int ret = DBManager::GetInstance().SaveMessage(messageList);
			for (list<MessageInfo*>::iterator it = messageList.begin(); it != messageList.end(); ++it) {
				delete *it;
				*it = NULL;
			}
			messageList.clear();
		}
	}
	ThreadDFinish = true;
	DWORD end = GetTickCount();
	printf("threadFuncD const:%d\n", (end - start));
	return 0;
}

static unsigned WINAPI threadFuncE(void *)
{
	DWORD start = GetTickCount();
	int ret = 0;
	for (int i = 1; i <= 500000; i++) {
		wstring value;
		ret = DBManager::GetInstance().GetConfigValue(L"chat", L"text_size", value);

		printf("text_size:%d, i:%d\n", atoi(StringConvert::ToUTF8Str(value).c_str()), i);
		Sleep(20);
	}
	ThreadEFinish = true;
	DWORD end = GetTickCount();
	printf("threadFuncE const:%d\n", (end - start));
	return 0;
}

void multiThreadA() 
{
	unsigned thread_id = 0;
	::_beginthreadex (0, 0, threadFuncA, 0, 0, &thread_id);
}
void multiThreadB() 
{
	unsigned thread_id = 0;
	::_beginthreadex (0, 0, threadFuncB, 0, 0, &thread_id);
}
void multiThreadC() 
{
	unsigned thread_id = 0;
	::_beginthreadex (0, 0, threadFuncC, 0, 0, &thread_id);
}
void multiThreadD() 
{
	unsigned thread_id = 0;
	::_beginthreadex (0, 0, threadFuncD, 0, 0, &thread_id);
}
void multiThreadE() 
{
	unsigned thread_id = 0;
	::_beginthreadex (0, 0, threadFuncE, 0, 0, &thread_id);
}


int main()
{
	//DB init
	wstring devDBPath = L"D:\\DB\\device.db";
	wstring userDBPath = L"D:\\DB\\userdata.db";
	int ret = DBManager::GetInstance().InitCommonDB(devDBPath);
	assert(ret == 0);
	ret = DBManager::GetInstance().InitUserDB(userDBPath);
	assert(ret == 0);

#if 1

	ret = sqlite3_threadsafe();
	//test thread
#if 0
	//multiThreadA();
	//multiThreadB();
	//multiThreadC();
	multiThreadD();
	//multiThreadE();
	while(/*!ThreadAFinish || !ThreadBFinish */
		/*|| !ThreadCFinish || */!ThreadDFinish /*|| !ThreadEFinish*/) {
			Sleep(60*1000);
	}
#endif
	///////////////////history account//////////////////////
	AccountInfo accout;
	accout.SetUserAccount(L"123@quanshi.com");
	accout.SetUserName(L"123");
	accout.SetAvatarUrl(L"a\\b\\c");
	accout.SetLoginTime(123);
	accout.SetPassword(L"123456");
	accout.SetLastManualLoginTime(999);
	accout.SetSavePassword(1);
	accout.SetAutoLogin(0);
	ret = DBManager::GetInstance().SaveAccount(accout);
	assert(ret == 0);

	accout.SetLoginTime(666);
	ret = DBManager::GetInstance().SaveAccount(accout);
	assert(ret == 0);

	ret = sqlite3_threadsafe();

	list<AccountInfo*> accountList1;
	AccountInfo* pAccout1 = new AccountInfo;;
	pAccout1->SetUserAccount(L"456@quanshi.com");
	pAccout1->SetUserName(L"456");
	pAccout1->SetAvatarUrl(L"a\\b\\c");
	pAccout1->SetLoginTime(456);
	pAccout1->SetPassword(L"123456");
	pAccout1->SetLastManualLoginTime(999);
	pAccout1->SetSavePassword(1);
	pAccout1->SetAutoLogin(0);
	accountList1.push_back(pAccout1);
	AccountInfo* pAccout2 = new AccountInfo;;
	pAccout2->SetUserAccount(L"789@quanshi.com");
	pAccout2->SetUserName(L"789");
	pAccout2->SetAvatarUrl(L"a\\b\\c");
	pAccout2->SetLoginTime(789);
	pAccout2->SetPassword(L"123456");
	pAccout2->SetLastManualLoginTime(999);
	pAccout2->SetSavePassword(1);
	pAccout2->SetAutoLogin(0);
	accountList1.push_back(pAccout2);
	ret = DBManager::GetInstance().SaveAccount(accountList1);
	assert(ret == 0);
	for (list<AccountInfo*>::iterator itor = accountList1.begin();
		itor != accountList1.end(); ++itor) {
			delete *itor;
			*itor = NULL;
	}
	accountList1.clear();

	AccountInfo result;
	ret = DBManager::GetInstance().GetAccount(L"123@quanshi.com", result);
	assert(ret == 0);
#if 0
	list<AccountInfo*> accountList;
	ret = DBManager::GetInstance().GetAccountList(accountList);
	assert(ret == 0);
	for (list<AccountInfo*>::iterator itor = accountList.begin();
		itor != accountList.end(); ++itor) {
			delete *itor;
			*itor = NULL;
	}
	accountList.clear();
#endif;

	///////////////////////////////contact////////////////////////////////
	ContactInfoImpl *data = ContactInfoImpl::CreateContactInfoImpl();
	data->SetUserId(123);
	data->SetAccount(L"tao.zhao@quanshi.com");
	data->SetDisplayName(L"zhaotao");
	data->SetPingyin(L"ZT");
	data->SetSex(ContactInfo::Sex::CS_MAN);
	data->SetDepartmentId(122);
	data->SetDepartmentName(L"ICL");
	data->SetPosition(L"ABC");
	data->SetWorkPhone(L"5236471");
	data->SetMobile(L"123456789");
	data->SetEmail(L"zt@163.com");
	data->SetSuperiorId(223);
	data->SetSuperiorName(L"zhangsan");
	data->SetSiteId(322);
	data->SetPersonalSign(L"dsafsdfds");
	data->SetLocalAvatar(L"C:\\");
	data->SetRemoteAvatarUrl(L"D:\\");
	data->SetType(ContactInfo::Type::CTI_USER);
	data->SetColleagueNnum(111);
	data->SetCardVersion(1430191840000);
 	ret = DBManager::GetInstance().SaveContact(*data);
	assert(ret == 0);

 	ret = DBManager::GetInstance().GetContact(123, *data);
	assert(ret == 0);

	list<ContactInfoImpl*> datas;
	ret = DBManager::GetInstance().GetContactListByType(1, datas);
	assert(ret == 0);
	for (list<ContactInfoImpl*>::iterator itor = datas.begin();
		itor != datas.end(); ++itor) {
		delete *itor;
		*itor = NULL;
	}
	datas.clear();

	wstring tt = L"中华人民共和国 China";
	data->SetUserId(123);
	data->SetAccount(L"tao.zhao");
	data->SetDisplayName(tt);
	data->SetPingyin(L"ZT");
	data->SetSex(ContactInfo::Sex::CS_MAN);
	data->SetDepartmentId(456);
	data->SetDepartmentName(L"ICL");
	data->SetPosition(L"ABC");
	data->SetWorkPhone(L"5236471");
	data->SetMobile(L"123456789");
	data->SetEmail(L"zt@163.com");
	data->SetSuperiorId(223);
	data->SetSuperiorName(L"lisi");
	data->SetSiteId(322);
	data->SetPersonalSign(L"dsafsdfds");
	data->SetLocalAvatar(L"C:\\");
	data->SetRemoteAvatarUrl(L"D:\\");
	data->SetType(ContactInfo::Type::CTI_COMMON);
	data->SetColleagueNnum(111);
	data->SetCardVersion(455);
	ret = DBManager::GetInstance().UpdateContact(123, *data);
	assert(ret == 0);

	ret = DBManager::GetInstance().UpdateContact(123, L"F:\\doc\\file", L"www.163.com");
	assert(ret == 0);

	ret = DBManager::GetInstance().UpdateUserType(123, 0);
	assert(ret == 0);
	
	ret = DBManager::GetInstance().GetContactListByName(L"ZT", 0, datas);
	assert(ret == 0);
	for (list<ContactInfoImpl*>::iterator itor = datas.begin();
		itor != datas.end(); ++itor) {
		delete *itor;
		*itor = NULL;
	}
	datas.clear();

	list<int> useridList;
	useridList.push_back(123);
	useridList.push_back(456);
	ret = DBManager::GetInstance().GetContactListByUserID(useridList, datas);
	assert(ret == 0);


	ContactTag tag;
	tag.SetUserId(123);
	tag.SetId(999);
	tag.SetName(L"country");
	tag.SetValue(L"china");
	tag.SetInputType(ContactTag::InputType::CTIT_MULTI_LINE_TEXT);
	tag.SetState(1);
	tag.SetScope(ContactTag::Scope::CTS_ADMIN_ONLY);
	tag.SetType(ContactTag::Type::CTT_BASIC);
	ret = DBManager::GetInstance().SaveTag(tag);
	assert(ret == 0);

	list<ContactTag*> tag2;
	ret = DBManager::GetInstance().GetTagList(123, tag2);
	assert(ret == 0);

	list<ContactTag*> tagList;
	list<int> userList;
	userList.push_back(123);
	userList.push_back(456);
	ret = DBManager::GetInstance().GetTagList(userList, tagList);
	assert(ret == 0);
	for (list<ContactTag*>::iterator itor = tagList.begin();
		itor != tagList.end(); ++itor) {
		delete *itor;
		*itor = NULL;
	}
	tagList.clear();

	
	ret = DBManager::GetInstance().GetTagList(tagList);
	assert(ret == 0);

	ret = DBManager::GetInstance().UpdateTag(123, 999, L"address", 2, 2, 2, 2);
	assert(ret == 0);

	ret = DBManager::GetInstance().DeleteTag(123, 999);
	assert(ret == 0);

	tag.SetUserId(456);
	tag.SetId(999);
	tag.SetName(L"country");
	tag.SetValue(L"china");
	tag.SetInputType(ContactTag::InputType::CTIT_MULTI_LINE_TEXT);
	tag.SetState(1);
	tag.SetScope(ContactTag::Scope::CTS_ADMIN_ONLY);
	tag.SetType(ContactTag::Type::CTT_BASIC);
	ret = DBManager::GetInstance().SaveTag(tag);
	assert(ret == 0);

	ret = DBManager::GetInstance().SaveConfigValue(L"chat", L"lan", L"en");
	assert(ret == 0);

	ret = DBManager::GetInstance().SaveConfigValue(L"chat", L"text_color", L"red");
	assert(ret == 0);

	ret = DBManager::GetInstance().SaveConfigValue(L"chat", L"text_size", L"10");
	assert(ret == 0);

	wstring value;
	ret = DBManager::GetInstance().GetConfigValue(L"chat", L"text_size", value);
	assert(ret == 0);

	map<wstring, wstring> valueMap;
	ret = DBManager::GetInstance().GetConfigValue(L"chat", valueMap);
	assert(ret == 0);

	ret = DBManager::GetInstance().UpdateConfigValue(L"chat", L"text_size", L"30");
	assert(ret == 0);

	map<wstring, wstring> values;
	values[L"lan"] = L"ch";
	values[L"text_color"] = L"green";
	values[L"text_size"] = L"100";
	ret = DBManager::GetInstance().UpdateConfigValue(L"chat", values);
	assert(ret == 0);

	///////////////////////////////message////////////////////////////////
	list<MessageInfo*> messageList;
	wstring cotent = L"fdsgfdgfdhdf dgfgfdhfghgf gfdgfdgfdg";
	MessageInfo* msg1  =new MessageInfo();
	msg1->SetMsgId(DBManager::GetInstance().GetMessageID());
	msg1->SetMsgSeq(GetTickCount());
	msg1->SetMsgData(cotent);
	msg1->SetMsgTime(2566);
	msg1->SetAppId(44);
	msg1->SetProtocolType(2);
	msg1->SetProtocolId(2);
	msg1->SetSendState(STATE_SENDSUCCESS);
	msg1->SetConversation(2);
// 	msg1->SetFromId(2);
// 	msg1->SetFromSiteId(2);
// 	msg1->SetFromResId(2);
// 	msg1->SetToId(2);
// 	msg1->SetToSiteId(2);
// 	msg1->SetToResId(2);
	msg1->SetVersion(2);
//	msg1->SetLocalConversation(2);
#if 0
 	for (int i = 19425000; i <= 20000000; i++) {
		MessageInfo* msg1  =new MessageInfo();
		msg1->SetMsgId(DBManager::GetInstance().GetMessageID());
		msg1->SetMsgSeq(i);
		msg1->SetMsgData(cotent);
		msg1->SetMsgTime(2566);
		msg1->SetAppId(44);
		msg1->SetProtocolType(2);
		msg1->SetProtocolId(2);
		msg1->SetSendState(STATE_SENDSUCCESS);
		msg1->SetPlayedState(1000);
		msg1->SetConversation(2);
		msg1->SetFromId(2);
		msg1->SetFromSiteId(2);
		msg1->SetFromResId(2);
		msg1->SetToId(2);
		msg1->SetToSiteId(2);
		msg1->SetToResId(2);
		msg1->SetVersion(2);
		msg1->SetLocalConversation(2);
		messageList.push_back(msg1);
		if (i%500 == 0) {
			ret = DBManager::GetInstance().SaveMessage(messageList);
			for (list<MessageInfo*>::iterator it = messageList.begin(); it != messageList.end(); ++it) {
				delete *it;
				*it = NULL;
			}
			messageList.clear();
		}
	}
#endif

	wstring cotent2 = L"ABCDG FFGDGGRRGR";
	MessageInfo* msg2  =new MessageInfo();
	msg2->SetMsgId(DBManager::GetInstance().GetMessageID());
	msg2->SetMsgSeq(GetTickCount());
	msg2->SetMsgData(cotent2);
	msg2->SetMsgTime(2566);
	msg2->SetAppId(44);
	msg2->SetProtocolType(2);
	msg2->SetProtocolId(2);
	msg2->SetSendState(STATE_SENDSUCCESS);
	msg2->SetConversation(2);
// 	msg2->SetFromId(2);
// 	msg2->SetFromSiteId(2);
// 	msg2->SetFromResId(2);
// 	msg2->SetToId(2);
// 	msg2->SetToSiteId(2);
// 	msg2->SetToResId(2);
	msg2->SetVersion(2);
//	msg2->SetLocalConversation(2);
	messageList.push_back(msg1);
	messageList.push_back(msg2);
	list<int> msgidlist;
	ret = DBManager::GetInstance().SaveMessage(messageList);
	assert(ret == 0);

	MessageInfo msg_data;
	ret = DBManager::GetInstance().GetMessage(msg2->GetMsgId(), msg_data);
	assert(ret == 0);

	ret = DBManager::GetInstance().SetMessageStatusById(msg2->GetMsgId(), 200);
	assert(ret == 0);

	ret = DBManager::GetInstance().SetMessageStatusBySeq(2, 666);
	assert(ret == 0);

	ret = DBManager::GetInstance().SetMessageSeq(2, 2566, 123);
	assert(ret == 0);

	ret  =DBManager::GetInstance().DeleteMessage(1);
	assert(ret == 0);

	ret = DBManager::GetInstance().UpdateMessageInfo(100, STATE_SENDFAIL, 9999, 88888, 77777);
	assert(ret == 0);

	DWORD start = GetTickCount();
	list<MessageInfo*> message;
	ret = DBManager::GetInstance().GetMessage(0, 2567, 999999999, 2, 100, message);
	assert(ret == 0);
	DWORD end = GetTickCount();
	printf("GetMessage, msg count:%d, const:%d\n", message.size(), (end - start));

	///////////////////////////////group////////////////////////////////
	GroupInfo grp;
	grp.SetId(111);
	grp.SetSiteId(1);
	grp.SetName(L"test");
	grp.SetPinYinName(L"TST");
	grp.SetCreateTime(123);
	grp.SetAvatarIndex(L"A\\D\\C");
	grp.SetJoinState(1);
	grp.SetMemberCount(3);
	grp.SetAdminId(0);
	grp.SetGroupAlert(1);
	grp.SetAlertRule(999);
	grp.SetAlertTime(888);
	grp.SetGroupTop(1);
	grp.SetGroupStatus(1);
	grp.SetGroupDisturb(0);
	grp.SetNameFlag(0);
	grp.SetGroupType(0);
	grp.SetLogoLevel(1);
	MemberInfo* member1 = new MemberInfo();
	member1->userId = 0;
	member1->joinTime = 100;
	MemberInfo* member2 = new MemberInfo();
	member2->userId = 1;
	member2->joinTime = 200;
	MemberInfo* member3 = new MemberInfo();
	member3->userId = 2;
	member3->joinTime = 300;
	list<MemberInfo*> memberList;
	memberList.push_back(member1);
	memberList.push_back(member2);
	memberList.push_back(member3);
	grp.AddMember(memberList);
	ret = DBManager::GetInstance().SaveGroup(grp);
	assert(ret == 0);
	for (list<MemberInfo*>::iterator it = memberList.begin(); it != memberList.end(); ++it) {
		delete *it;
		*it = NULL;
	}
	memberList.clear();


	list<GroupInfo*> grps;
	GroupInfo* group1 = new GroupInfo;
	group1->SetId(111);
	group1->SetSiteId(1);
	group1->SetName(L"test");
	group1->SetPinYinName(L"TST");
	group1->SetCreateTime(123);
	group1->SetAvatarIndex(L"A\\D\\C");
	group1->SetJoinState(1);
	group1->SetMemberCount(3);
	group1->SetAdminId(0);
	group1->SetGroupAlert(1);
	group1->SetAlertRule(999);
	group1->SetAlertTime(888);
	group1->SetGroupTop(1);
	group1->SetGroupStatus(1);
	group1->SetGroupDisturb(0);
	group1->SetNameFlag(0);
	group1->SetGroupType(0);
	group1->SetLogoLevel(1);
	MemberInfo* meb1 = new MemberInfo();
	meb1->userId = 0;
	meb1->joinTime = 100;
	MemberInfo* meb2 = new MemberInfo();
	meb2->userId = 1;
	meb2->joinTime = 200;
	MemberInfo* meb3 = new MemberInfo();
	meb3->userId = 2;
	meb3->joinTime = 300;
	list<MemberInfo*> memberList1;
	memberList1.push_back(meb1);
	memberList1.push_back(meb2);
	memberList1.push_back(meb3);
	group1->AddMember(memberList1);
	grps.push_back(group1);

	GroupInfo* group2 = new GroupInfo;
	group2->SetId(222);
	group2->SetSiteId(1);
	group2->SetName(L"test");
	group2->SetPinYinName(L"TST");
	group2->SetCreateTime(123);
	group2->SetAvatarIndex(L"A\\D\\C");
	group2->SetJoinState(1);
	group2->SetMemberCount(3);
	group2->SetAdminId(0);
	group2->SetGroupAlert(1);
	group2->SetAlertRule(999);
	group2->SetAlertTime(888);
	group2->SetGroupTop(1);
	group2->SetGroupStatus(1);
	group2->SetGroupDisturb(0);
	group2->SetNameFlag(0);
	group2->SetGroupType(0);
	group2->SetLogoLevel(1);
	MemberInfo* mb1 = new MemberInfo();
	mb1->userId = 0;
	mb1->joinTime = 100;
	MemberInfo* mb2 = new MemberInfo();
	mb2->userId = 1;
	mb2->joinTime = 200;
	MemberInfo* mb3 = new MemberInfo();
	mb3->userId = 2;
	mb3->joinTime = 300;
	list<MemberInfo*> memberList2;
	memberList2.push_back(mb1);
	memberList2.push_back(mb2);
	memberList2.push_back(mb3);
	group2->AddMember(memberList2);
	grps.push_back(group2);

 	ret = DBManager::GetInstance().SaveGroup(grps);
	assert(ret == 0);
	for (list<GroupInfo*>::iterator it = grps.begin(); it != grps.end(); ++it) {
		list<MemberInfo*> ms = (*it)->GetMember();
		for (list<MemberInfo*>::iterator itor = ms.begin(); itor != ms.end(); ++itor) {
			delete *itor;
			*itor = NULL;
		}
		ms.clear();
		delete *it;
		*it = NULL;
	}
	grps.clear();
#if 0
	for (int n = 99500; n <= 200000; n++) {
		GroupInfo* group = new GroupInfo;
		group->SetId(n);
		group->SetSiteId(n);
		group->SetName(L"test");
		group->SetPinYinName(L"TST");
		group->SetCreateTime(123);
		group->SetAvatarIndex(L"A\\D\\C");
		group->SetJoinState(1);
		group->SetMemberCount(3);
		group->SetAdminId(0);
		group->SetGroupAlert(1);
		group->SetAlertRule(999);
		group->SetAlertTime(888);
		group->SetGroupTop(1);
		group->SetGroupStatus(1);
		group->SetGroupDisturb(0);
		group->SetNameFlag(0);
		group->SetGroupType(0);
		group->SetLogoLevel(1);
		MemberInfo* meb1 = new MemberInfo();
		meb1->userId = 0;
		meb1->joinTime = 100;
		MemberInfo* meb2 = new MemberInfo();
		meb2->userId = 1;
		meb2->joinTime = 200;
		MemberInfo* meb3 = new MemberInfo();
		meb3->userId = 2;
		meb3->joinTime = 300;
		list<MemberInfo*> memberList1;
		memberList1.push_back(meb1);
		memberList1.push_back(meb2);
		memberList1.push_back(meb3);
		group->AddMember(memberList1);
		grps.push_back(group);
		if (n%500 == 0) {
			ret = DBManager::GetInstance().SaveGroup(grps);
			for (list<GroupInfo*>::iterator it = grps.begin(); it != grps.end(); ++it) {
				list<MemberInfo*> ms = (*it)->GetMember();
				for (list<MemberInfo*>::iterator itor = ms.begin(); itor != ms.end(); ++itor) {
					delete *itor;
					*itor = NULL;
				}
				ms.clear();
				delete *it;
				*it = NULL;
			}
			grps.clear();
		}
	}
#endif

	GroupInfo grpInfo;
	ret = DBManager::GetInstance().GetGroup(222, grpInfo);
	assert(ret == 0);

	list<MemberInfo*> members;
	ret = DBManager::GetInstance().GetMember(222, members);
	assert(ret == 0);
	for (list<MemberInfo*>::iterator it = members.begin(); it != members.end(); ++it) {
		delete *it;
		*it = NULL;
	}
	members.clear();

	grpInfo.SetCreateTime(999999999);
	ret = DBManager::GetInstance().SetGroup(grpInfo);
	assert(ret == 0);

	ret = DBManager::GetInstance().SetJoinStatus(222,5);
	assert(ret == 0);

	ret = DBManager::GetInstance().DeleteGroup(222);
	assert(ret == 0);

	ret = DBManager::GetInstance().SetGroupName(111, L"ABC", L"CDF");
	assert(ret == 0);

	ret = DBManager::GetInstance().SetGroupAvatar(111, L"CCCCCCCCCCCCC");
	assert(ret == 0);

	ret = DBManager::GetInstance().SetGroupAlertRule(111, 1, 55, 66);
	assert(ret == 0);

	ret = DBManager::GetInstance().SetGroupTop(111, 0);
	assert(ret == 0);

	ret = DBManager::GetInstance().SetGroupDisturb(111, 1);
	assert(ret == 0);

	MemberInfo m;
	m.userId = 100;
	m.joinTime = 200;
	ret = DBManager::GetInstance().AddMember(111, m);
	assert(ret == 0);

	list<MemberInfo*> ms;
	MemberInfo* pm1 = new MemberInfo();
	pm1->userId = 222;
	pm1->joinTime = 500;
	ms.push_back(pm1);
	MemberInfo* pm2 = new MemberInfo();
	pm2->userId = 333;
	pm2->joinTime = 600;
	ms.push_back(pm2);
	ret = DBManager::GetInstance().AddMember(111, ms);
	assert(ret == 0);
	for (list<MemberInfo*>::iterator it = ms.begin(); it != ms.end(); ++it) {
		delete *it;
		*it = NULL;
	}
	ms.clear();

	ret = DBManager::GetInstance().DeleteMember(111, 222);
	assert(ret == 0);
	
	vector<int> userIdList;
	userIdList.push_back(333);
	ret = DBManager::GetInstance().DeleteMember(111, userIdList);
	assert(ret == 0);

	////////////////////////conference//////////////////////////
	ConferenceInfo conf;
	conf.SetConferenceId(123);
	conf.SetEventId(1);
	conf.SetHosterPassword(L"111111");
	conf.SetAttendeePassword(L"222222");
	conf.SetBillingCode(3);
	conf.SetCategory(0);
	conf.SetIsGnet(0);
	conf.SetStartTime(1230);
	conf.SetDuration(2);
	conf.SetHostName(L"赵涛");
	conf.SetHostId(999);
	std::vector<Attendee> users;
	Attendee user1;
	user1.SetAccount(L"tao.zhao@quanshi.com");
	user1.SetName(L"zhaotao");
	user1.SetStartTime(1230);
	user1.SetStatus(1);
	user1.SetType(2);
	users.push_back(user1);
	Attendee user2;
	user2.SetAccount(L"test@quanshi.com");
	user2.SetName(L"test");
	user2.SetStartTime(1900);
	user2.SetStatus(0);
	user2.SetType(1);
	users.push_back(user2);
	conf.SetAttendees(users);
	conf.SetTitle(L"会议测试");
	conf.SetSummary(L"conference test");
	conf.SetLocation(L"1号会议室");
	conf.SetTimeZone(L"北京");
	conf.SetLanguage(0);
	conf.SetEmailLanguage(1);
	conf.SetIsCycle(0);
	ret = DBManager::GetInstance().SaveConference(conf);
	assert(ret == 0);

	vector<ConferenceInfo> conflist;
	conflist.push_back(conf);
	ConferenceInfo conf1;
	conf1.SetConferenceId(666);
	conf1.SetEventId(2);
	conf1.SetHosterPassword(L"88888888");
	conf1.SetAttendeePassword(L"77777777");
	conf1.SetBillingCode(3);
	conf1.SetCategory(0);
	conf1.SetIsGnet(0);
	conf1.SetStartTime(1230);
	conf1.SetDuration(2);
	conf1.SetHostName(L"tom");
	conf1.SetHostId(777);
	std::vector<Attendee> us;
	Attendee u1;
	u1.SetAccount(L"123@quanshi.com");
	u1.SetName(L"123");
	u1.SetStartTime(1230);
	u1.SetStatus(1);
	u1.SetType(2);
	us.push_back(u1);
	Attendee u2;
	u2.SetAccount(L"abc@quanshi.com");
	u2.SetName(L"abc");
	u2.SetStartTime(1900);
	u2.SetStatus(0);
	u2.SetType(1);
	us.push_back(u2);
	conf1.SetAttendees(us);
	conf1.SetTitle(L"会议测试");
	conf1.SetSummary(L"conference test");
	conf1.SetLocation(L"1号会议室");
	conf1.SetTimeZone(L"北京");
	conf1.SetLanguage(0);
	conf1.SetEmailLanguage(1);
	conf1.SetIsCycle(0);
	conflist.push_back(conf1);
	ret = DBManager::GetInstance().SaveConference(conflist);
	assert(ret == 0);

	ret = DBManager::GetInstance().SetConferenceToOld();
	assert(ret == 0);

#if 0
	for (int m = 1; m <= 1000000; m++) {
		ConferenceInfo conf;
		conf.SetConferenceId(m);
		conf.SetEventId(m);
		conf.SetHosterPassword(L"88888888");
		conf.SetAttendeePassword(L"77777777");
		conf.SetBillingCode(m);
		conf.SetCategory(0);
		conf.SetIsGnet(0);
		conf.SetStartTime(m);
		conf.SetDuration(m);
		conf.SetHostName(L"tom");
		conf.SetHostId(777);
		std::vector<Attendee> us;
		Attendee u1;
		u1.SetAccount(L"123@quanshi.com");
		u1.SetName(L"123");
		u1.SetStartTime(m);
		u1.SetStatus(1);
		u1.SetType(2);
		us.push_back(u1);
		Attendee u2;
		u2.SetAccount(L"abc@quanshi.com");
		u2.SetName(L"abc");
		u2.SetStartTime(m);
		u2.SetStatus(0);
		u2.SetType(1);
		us.push_back(u2);
		conf.SetAttendees(us);
		conf.SetTitle(L"会议测试");
		conf.SetSummary(L"conference test");
		conf.SetLocation(L"1号会议室");
		conf.SetTimeZone(L"北京");
		conf.SetLanguage(0);
		conf.SetEmailLanguage(1);
		conf.SetIsCycle(0);
		conflist.push_back(conf);
		if (m%500 == 0) {
			ret = DBManager::GetInstance().SaveConference(conflist);
			conflist.clear();
		}
	}
#endif

	int conferenceId = 123;
	int64_t startTime = 1230;
	ConferenceInfo confData;
	ret = DBManager::GetInstance().GetConference(conferenceId, 1, startTime, confData);
	assert(ret == 0);

	vector<ConferenceInfo> confinfolist;
	ret = DBManager::GetInstance().GetConferenceById(conferenceId, 1230, 8888, confinfolist);
	assert(ret == 0);

	startTime = 0;
	int64_t endTime = 2000;
	int32_t limit = 200;
	vector<ConferenceInfo> confs;
	ret = DBManager::GetInstance().GetConferenceList(startTime, endTime, 1, limit, confs);
	assert(ret == 0);

	ret = DBManager::GetInstance().DeletConference(123, 123, 123);
	assert(ret == 0);

	ret = DBManager::GetInstance().DeletConferenceList(200, 0, 8888);
	assert(ret == 0);

	ret = DBManager::GetInstance().DeletConferenceList(3000, 5000, 2, 200);
	assert(ret == 0);

	AccreditInfo accredit;
	accredit.SetStatus(1);
	accredit.SetAccreditUserId(123);
	ret = DBManager::GetInstance().SaveAccreditUser(accredit);
	assert(ret == 0);

	AccreditInfo accredit1;
	accredit1.SetStatus(2);
	accredit1.SetAccreditUserId(456);
	ret = DBManager::GetInstance().SaveAccreditUser(accredit1);


	vector<AccreditInfo> accreditInfo;
	ret = DBManager::GetInstance().GetAccreditUserList(accreditInfo);
	assert(ret == 0);

	///////////////////////////conversation//////////////////////////
	ConversationInfo con;
	con.SetLocalConversation(123456);
	con.SetConversation(7890);
	con.SetType(SINGLECHAT_MSG);
	con.SetRelateId(99);
	con.SetNewMsgCount(100);
	con.SetLastMsgTime(1830);
	con.SetLastMsgSeq(20);
	con.SetHidden(true);
	ret = DBManager::GetInstance().UpdateConversationInfo(con);
	//assert(ret == 0);

#if 0
	for (int j = 1; j < 100000; j++) {
		ConversationInfo con;
		con.SetLocalConversation(j);
		con.SetConversation(j);
		con.SetType(SINGLECHAT_MSG);
		con.SetRelateId(j);
		con.SetNewMsgCount(j);
		con.SetLastMsgTime(j);
		con.SetLastMsgSeq(j);
		con.SetHidden(true);
		ret = DBManager::GetInstance().UpdateConversationInfo(con);
	}
#endif

	ConversationInfo con2;
	con2.SetLocalConversation(9999);
	con2.SetConversation(7890);
	con2.SetType(SINGLECHAT_MSG);
	con2.SetRelateId(99);
	con2.SetNewMsgCount(100);
	con2.SetLastMsgTime(1830);
	con2.SetLastMsgSeq(20);
	con2.SetHidden(true);
	ret = DBManager::GetInstance().UpdateConversationInfo(con2);
	//assert(ret == 0);

	ConversationInfo con1;
	ret = DBManager::GetInstance().GetConversationById(123456, con1);
	assert(ret == 0);

	ret = DBManager::GetInstance().UpdateConversationUnreadMsgNumber(123456, 200);
	assert(ret == 0);

	vector<ConversationInfo*> conList;
	ret = DBManager::GetInstance().LoadConversationsByTime(1900, 5, conList);
	assert(ret == 0);
	for (vector<ConversationInfo*>::iterator it = conList.begin(); it != conList.end(); ++it) {
		delete *it;
		*it = NULL;
	}
	conList.clear();

	ret = DBManager::GetInstance().LoadLatestConversations(4, conList);
	assert(ret == 0);
	for (vector<ConversationInfo*>::iterator it = conList.begin(); it != conList.end(); ++it) {
		delete *it;
		*it = NULL;
	}
	conList.clear();

	int64_t maxSeq;
	ret = DBManager::GetInstance().GetMaxMessageSeqData(maxSeq);
	assert(ret == 0);

	ret = DBManager::GetInstance().RemoveConversation(123456);
	assert(ret == 0);

	ret = DBManager::GetInstance().RemoveAllConversations();
	assert(ret == 0);

#else
	////////////////////////////////////////////////////////////////////
	ret = DBUpgrade::GetInstance().Upgrade("D:\\DB\\device.db", "D:\\DB\\userdata.db");
	assert(ret == 0);
#endif

	return 0;
}

