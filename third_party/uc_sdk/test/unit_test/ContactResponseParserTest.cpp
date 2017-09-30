#include "gtest/gtest.h"
#include <string>

#include "contact_service/ContactResponseParser.h"

using namespace uc;

class ContactResponseParserTest : public ::testing::Test {
public:
	ContactResponseParserTest(){}
	virtual ~ContactResponseParserTest() {}

protected:
	virtual void SetUp() {

	}

	virtual void TearDown() {

	}

public:

};

static const std::string contactInclist = "{\"code\":0,\"timestamp\":\"1431584726581\",\"data\":[{\"user_id\":12212446,\"display_name\":\"\\u738b\\u521a\",\"personal_sign\":\"helo\",\"position\":\"\\u5458\\u5de5\",\"user_account\":\"gang.wang_3@quanshi.com\",\"email\":\"gang.wang_3@quanshi.com\",\"last_name\":\"\\u738b\\u521a\",\"first_name\":\"\",\"middle_name\":\"\",\"mobile\":\"18013483206\",\"name_pinyin\":\"WG\",\"fix_tel\":\"\",\"sex\":0,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTIyODY0Ni84L2F2YXRhcjEyMjg2NDZeXl50YW5naGRmc15eXjY1Njk4OTVmNGYzMzJjMWM0ZGUzYzY2YWIzZDM2NDY0Xl5edGFuZ2hkZnNeXl4yMDY5$&u=1228646\",\"card_version\":1431582565000,\"last_login_time\":0,\"add_time\":1427509885000,\"is_delete\":0,\"is_ecology_user\":0,\"status\":1,\"device\":3,\"site_id\":70804,\"department_id\":10,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\",\"leader_name\":\"\",\"leader_id\":0,\"colleague_num\":21},{\"user_id\":1382762,\"display_name\":\"\\u6768\\u743c\",\"personal_sign\":\"222+222\",\"position\":\"\\u5458\\u5de5\",\"user_account\":\"qiong.yang@quanshi.com\",\"email\":\"qiong.yang@quanshi.com\",\"last_name\":\"\\u6768\\u743c\",\"first_name\":\"\",\"middle_name\":\"\",\"mobile\":\"13399180877\",\"name_pinyin\":\"YQ\",\"fix_tel\":\"\",\"sex\":0,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTM4Mjc2Mi84L2F2YXRhcjEzODI3NjJeXl50YW5naGRmc15eXmNmYjFkODZmNTc0MWFhZDViNjBiYWQ1NDUzNTRiZjIwXl5edGFuZ2hkZnNeXl43NTg5$&u=1382762\",\"card_version\":1431582198000,\"last_login_time\":0,\"add_time\":1427509896000,\"is_delete\":0,\"is_ecology_user\":0,\"status\":1,\"device\":3,\"site_id\":70804,\"department_id\":10,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\",\"leader_name\":\"\",\"leader_id\":0,\"colleague_num\":21},{\"user_id\":1382766,\"display_name\":\"\\u5218\\u4f1f\",\"personal_sign\":\"22222222222123456794\",\"position\":\"\\u5458\\u5de5\",\"user_account\":\"wei.liu@quanshi.com\",\"email\":\"wei.liu@quanshi.com\",\"last_name\":\"\\u5218\\u4f1f\",\"first_name\":\"\",\"middle_name\":\"\",\"mobile\":\"15829204950\",\"name_pinyin\":\"LW\",\"fix_tel\":\"\",\"sex\":0,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTM4Mjc2Ni84L2F2YXRhcjEzODI3NjZeXl50YW5naGRmc15eXmY2ZDZkNGNlMDc4MjFlYzllOTg3ZDE1YjNjODM3OTBmXl5edGFuZ2hkZnNeXl41Mjgx$&u=1382766\",\"card_version\":1431584284000,\"last_login_time\":0,\"add_time\":1428041808000,\"is_delete\":0,\"is_ecology_user\":0,\"status\":1,\"device\":3,\"site_id\":70804,\"department_id\":10,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\",\"leader_name\":\"\",\"leader_id\":0,\"colleague_num\":9}],\"request_id\":\"1431584726.52298400.18657\"}";
static const std::string userProfile = "{\"code\":0,\"data\":[{\"user_id\":1221656,\"display_name\":\"\\u738b\\u521a\",\"personal_sign\":\"\\u7f16\\u8f91\\u4e2a\\u6027\\u7b7e\\u540d\",\"position\":\"\\u5458\\u5de5\",\"user_account\":\"gang.wang_3@quanshi.com\",\"email\":\"gang.wang_3@quanshi.com\",\"last_name\":\"\\u738b\\u521a\",\"first_name\":\"\",\"middle_name\":\"\",\"mobile\":\"18013483206\",\"name_pinyin\":\"WG\",\"fix_tel\":\"\",\"sex\":0,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTIyODY0Ni84L2Y1ZWU0ZDE4OTY2YjU5YzkyYjY3YWRiYzMyNzFhMWNkLnBuZ15eXnRhbmdoZGZzXl5eZmI4ZGQ1N2QxYzRkZmU3OGE5MzM0Mjk5YWQ1N2Y0YmNeXl50YW5naGRmc15eXjMwMTgy$&u=1228646\",\"is_ldap\":1,\"card_version\":1428977822000,\"last_login_time\":1431580524000,\"department_id\":10,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\",\"status\":5,\"device\":3,\"site_id\":70804,\"leader_name\":\"\",\"leader_id\":0,\"colleague_num\":\"21\"}],\"request_id\":\"1431580528.70738500.18667\"}";
static const std::string deleteContact = "{\"code\":0,\"data\":[],\"request_id\":\"1431580833.78097900.18067\"}";
static const std::string addContact = "{\"code\":0,\"data\":[{\"user_id\":1382772,\"display_name\":\"\\u5f90\\u6052\",\"personal_sign\":\"\",\"position\":\"\\u7cfb\\u7edf\\u67b6\\u6784\\u5e08\",\"user_account\":\"heng.xu@quanshi.com\",\"email\":\"heng.xu@quanshi.com\",\"last_name\":\"\\u5f90\\u6052\",\"first_name\":\"\",\"middle_name\":\"\",\"mobile\":\"18811034178\",\"name_pinyin\":\"XH\",\"fix_tel\":\"\",\"sex\":0,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTM4Mjc3Mi84LzE0Mjc4OTYyMjU1NDkuanBnXl5edGFuZ2hkZnNeXl5iMjU2OTllYzdiMGU3ZDQwMDEzNjJhNWI5NjI5MzdlZl5eXnRhbmdoZGZzXl5eMjQ2Mw$&u=1382772\",\"is_ldap\":0,\"card_version\":1429159276000,\"last_login_time\":1431473379000,\"department_id\":6,\"department\":\"UC\\u4ea7\\u54c1\\u7814\\u53d1\\u90e8\",\"status\":1,\"device\":3,\"site_id\":70804,\"leader_name\":\"\",\"leader_id\":0,\"colleague_num\":\"0\"}],\"request_id\":\"1431580756.16972400.18183\"}";
static const std::string userColleague = "{\"code\":0,\"data\":[{\"colleague_count\":\"9\",\"colleague\":[{\"user_id\":256965,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MjU2OTY1LzgvMzUyOTBjYjMzZjg2NDUxOWI4ZDBiM2Y1ODJmYjQzNDAucG5nXl5edGFuZ2hkZnNeXl41YWZmOTNiNDEwZTQ0NWI5NTBlODBmODM0MzE3N2I2MF5eXnRhbmdoZGZzXl5eMzcyNDE$&u=256965\",\"display_name\":\"\\u674e\\u6587\\u8f89\",\"position\":\"\\u5458\\u5de5\",\"card_version\":1427883271000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"},{\"user_id\":366197,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MzY2MTk3LzgvYXZhdGFyMzY2MTk3LmpwZ15eXnRhbmdoZGZzXl5eNWE1ZDE2Y2EyMGU5YjczZmE4NTcyNmU4NmU4ZjJiYmVeXl50YW5naGRmc15eXjc5MjI$&u=366197\",\"display_name\":\"\\u5218\\u4e9a\\u5e73\",\"position\":\"\\u5458\\u5de5\",\"card_version\":1429784133000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"},{\"user_id\":477828,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=NDc3ODI4LzgvYXZhdGFyNDc3ODI4LmpwZ15eXnRhbmdoZGZzXl5eNzMwMDdiYTYyMzk5YmJlNTEyMGY1ZWEwZWY0MWQ0NGNeXl50YW5naGRmc15eXjQ1MzU$&u=477828\",\"display_name\":\"\\u738b\\u5955\\u5a77\",\"position\":\"\\u5458\\u5de5\",\"card_version\":1430216757000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"},{\"user_id\":1168236,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTE2ODIzNi84LzE0Mjc5NTczNTYxOTUuanBnXl5edGFuZ2hkZnNeXl4yNzc2N2E1Y2EyMDQxOWRmODFiODg2Yjg2MjQ4ZTNmNV5eXnRhbmdoZGZzXl5eMjY4Mg$&u=1168236\",\"display_name\":\"\\u5f20\\u79c0\\u4e3e\",\"position\":\"\\u5458\\u5de5\",\"card_version\":1427957358000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"},{\"user_id\":1228644,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTIyODY0NC84LzE0Mjc1OTE5NzU3NTAuanBnXl5edGFuZ2hkZnNeXl45OTkxMTg4ZWIzZDg2NTY0ZmM4ODhlNGI3MDk4MTMwMl5eXnRhbmdoZGZzXl5eMjI2NA$&u=1228644\",\"display_name\":\"\\u6bd5\\u6c38\\u56fd\",\"position\":\"\\u4e3b\\u7ba1\",\"card_version\":1427951591000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"},{\"user_id\":1331346,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTMzMTM0Ni84LzE0Mjk2ODQwMTAxMDEuanBnXl5edGFuZ2hkZnNeXl43N2U1MjMwYjJhMTA1OGI5YzRmYjdmYTY4YWYzNDJmMV5eXnRhbmdoZGZzXl5eNDM5Mg$&u=1331346\",\"display_name\":\"\\u5b87\\u5fd7\\u4f1f\",\"position\":\"\\u5458\\u5de5\",\"card_version\":1429684010000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"},{\"user_id\":1382766,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTM4Mjc2Ni84L2F2YXRhcjEzODI3NjZeXl50YW5naGRmc15eXjcyYThhODU1YjA4NTdlMTZjOTFmNjNjYTNhZmNmMWRlXl5edGFuZ2hkZnNeXl41Mjgw$&u=1382766\",\"display_name\":\"\\u5218\\u4f1f\",\"position\":\"\\u5458\\u5de5\",\"card_version\":1429600117000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"},{\"user_id\":1382768,\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTM4Mjc2OC84L2F2YXRhcjEzODI3NjheXl50YW5naGRmc15eXmNhOTVmZjBjZmZjOGQ5NjQ2NjMzOWI0NDAwZWYxMTA2Xl5edGFuZ2hkZnNeXl43ODQ0$&u=1382768\",\"display_name\":\"\\u5b59\\u4e00\\u654f\",\"position\":\"\\u5458\\u5de5\",\"card_version\":1431308029000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"},{\"user_id\":1382770,\"avatar\":\"\",\"display_name\":\"\\u7a0b\\u6c38\\u5e05\",\"position\":\"\\u5458\\u5de5\",\"card_version\":1430380978000,\"department\":\"\\u5ba2\\u6237\\u7aef\\u56e2\\u961f\"}],\"leader_count\":0,\"leader\":[]}],\"request_id\":\"1431581128.65664600.18645\"}";
static const std::string orgList = "{\"code\":0,\"data\":[{\"dept_list\":{\"org_name\":\"\\u4ea7\\u54c1\\u53ca\\u8fd0\\u8425\\u90e8\",\"org_id\":46,\"parent_id\":4,\"node_code\":\"-2-4-46\",\"online\":1,\"count\":1,\"member_list\":[{\"user_account\":\"xxxx@quanshi.com\",\"email\":\"xxxx@quanshi.com\",\"user_id\":1759,\"display_name\":\"\\u5f20\\u6770\\u8bda\",\"avatar\":\"http:\\/\\/oncloud.quanshi.com\\/ucfserver\\/hddown?fid=MTc1OS84LzE0MzEzODgzNDE1NjQuanBnXl5edGFuZ2hkZnNeXl40MzIwNGFlOTJkYTU2MmY5ZTU0NDFhYTJjOTJmMGQ4MF5eXnRhbmdoZGZzXl5eMjY4Ng$&u=1759\",\"position\":\"\\u603b\\u76d1\",\"card_version\":1431388342000,\"last_login_time\":1431585614000,\"mobile\":\"18611625456\",\"is_admin\":0,\"type\":0,\"status\":1,\"device\":3,\"site_id\":70804,\"user_change_status\":[]}],\"dept_list\":[{\"org_id\":48,\"parend_id\":46,\"org_name\":\"\\u4ea7\\u54c1\\u90e8\",\"count\":2,\"node_code\":\"-2-4-46-48\",\"have_child\":1},{\"org_id\":50,\"parend_id\":46,\"org_name\":\"\\u65b9\\u6848\\u53ca\\u4ea4\\u4ed8\\u90e8\",\"count\":1,\"node_code\":\"-2-4-46-50\",\"have_child\":1},{\"org_id\":52,\"parend_id\":46,\"org_name\":\"\\u670d\\u52a1\\u90e8\",\"count\":1,\"node_code\":\"-2-4-46-52\",\"have_child\":1},{\"org_id\":54,\"parend_id\":46,\"org_name\":\"\\u4f01\\u4e1a\\u793e\\u533a\\u7528\\u6237\\u4f53\\u9a8c\\u90e8\",\"count\":3,\"node_code\":\"-2-4-46-54\",\"have_child\":1}],\"child_order\":\"\"}}],\"request_id\":\"1431586309.02431400.18709\"}";

TEST_F(ContactResponseParserTest, checkContactInclist) {
	/*
	ContactResponseParser parser;

	uint64_t timestamp;
	std::list<ContactInfoImpl*> contacts;
	std::list<ContactInfoImpl*> deletedContacts;
    
	int res = parser.ParseContactList(contactInclist, &timestamp, contacts, deletedContacts);

	ASSERT_EQ(0, res);
	EXPECT_EQ(3, contacts.size());
	ASSERT_TRUE((contacts.begin() != contacts.end()));
	EXPECT_EQ(12212446, ((*contacts.begin())->GetUserId()));
	EXPECT_STREQ("\u738b\u521a", ((*contacts.begin())->GetDisplayName().c_str()));*/
}

TEST_F(ContactResponseParserTest, checkGetProfile) {
	/*
	ContactResponseParser parser;
	std::list<ContactInfoImpl*> contacts;
	
	int res = parser.ParseGetProfile(userProfile,contacts);

	ASSERT_EQ(0, res);
	ASSERT_TRUE((contacts.begin() != contacts.end()));
	EXPECT_EQ(1221656, ((*contacts.begin())->GetUserId()));
	EXPECT_STREQ("\u738b\u521a", ((*contacts.begin())->GetDisplayName().c_str()));*/
}


TEST_F(ContactResponseParserTest, checkContactAdd) {
	/*
	ContactResponseParser parser;
	ContactInfoImpl* contact = NULL;

	int res = parser.ParseContactAdd(addContact, &contact);

	ASSERT_EQ(0, res);
	ASSERT_TRUE((contact != NULL));
	EXPECT_EQ(1382772, contact->GetUserId());
	EXPECT_STREQ(L"u5f90\u6052", contact->GetDisplayName().c_str());*/
}


TEST_F(ContactResponseParserTest, checkContactDelete) {
	ContactResponseParser parser;

	int res = parser.ParseContactDelete(deleteContact);
	ASSERT_EQ(0, res);
}

TEST_F(ContactResponseParserTest, checkUserColleague) {
	/*
	ContactResponseParser parser;

	int leaderNum, colleagueNum;
	std::list<ContactInfoImpl*> colleagues; 
	std::list<ContactInfoImpl*> leaders;
	
	int res = parser.ParseUserColleague(userColleague, &leaderNum, &colleagueNum, colleagues, leaders);

	ASSERT_EQ(0, res);
	EXPECT_EQ(0, leaderNum);
	EXPECT_EQ(9, colleagueNum);
	EXPECT_EQ(9, colleagues.size());
	EXPECT_EQ(0, leaders.size());

	ASSERT_TRUE(colleagues.begin() != colleagues.end());
	EXPECT_EQ(256965, ((*colleagues.begin())->GetUserId()));
	EXPECT_STREQ(L"\u674e\u6587\u8f89", ((*colleagues.begin())->GetDisplayName().c_str()));*/
}

TEST_F(ContactResponseParserTest, checkOrgList) {
	//DepartmentInfo departmentInfo, myDepartment;
	//ContactResponseParser parser;
	//bool res = parser.ParseOrgList(orgList, departmentInfo, myDepartment);
/*
	ASSERT_EQ(true, res);
	ASSERT_EQ(1, (*departmentInfo._contacts).size());
	ASSERT_TRUE(((*departmentInfo._contacts)[0] != NULL));
	EXPECT_STREQ(L"xxxx@quanshi.com", (*departmentInfo._contacts)[0]->_email.c_str());
*/
}
