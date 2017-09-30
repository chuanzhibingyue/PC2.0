#include "gtest/gtest.h"
#include "UCMockClient.h"
#include "ContactServiceMockClient.h"
#include "UCErrorCode.h"
#include "IClientService.h"
#include <boost/thread.hpp>
#include "LogManager.h"
#include "StringConvert.h"
#include "ClientInfo.h"
#include "UCCommonFunctionMock.h"
#include "utils/callback/ExecuteCallback.h"

class ContactServiceTest : public testing::Test {
public:
	ContactServiceTest() {
		ucEngine = NULL;
	}
	virtual ~ContactServiceTest() {}
public:
	void SetUp() {
		ExecuteCallback(this, &ContactServiceTest::CreateUCEngine);
		boost::unique_lock<boost::recursive_mutex> lock(_Mtx);
		_methodCond.wait(lock);
	}

	void CreateUCEngine() {
		UCEngineConfig ucConfig;
		CreateEngineConfig(ucConfig);
		ucEngine = ::CreateUCEngine(ucConfig);

		_Mtx.lock();
		_methodCond.notify_all();
		_Mtx.unlock();
	}

	void TearDown() {
		ExecuteCallback(this, &ContactServiceTest::DestroyUCEngine);
		boost::unique_lock<boost::recursive_mutex> lock(_Mtx);
		_methodCond.wait(lock);
	}

	void DestroyUCEngine() {
		::DestroyUCEngine();
		ucEngine = NULL;

		_Mtx.lock();
		_methodCond.notify_all();
		_Mtx.unlock();
	}
public:
	boost::recursive_mutex _Mtx;
	boost::condition_variable_any _methodCond;
	IUCEngine *ucEngine;
};

/*
TEST_F(ContactServiceTest, GetUpdatedContacts) {

	const ClientInfo info = {L"jie.shen@quanshi.com", L"redapple", UCCLIENT_WIN, L"6c:40:08:9f:ba:ca",
		L"3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba", 
		L"E40D42D2-4C88-47D0-8029-03DEEC649312", L"1.9.590.16577", L"0", L"1234567890"};

	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);
	mockClient.Login();

	boost::unique_lock<boost::recursive_mutex> lock(mockClient.methodMtx);
	mockClient.methodCond.wait(lock);
	lock.unlock();
	cout <<"***********get resultCode from server:"<<mockClient.resultCode<<endl;
	
	//	ASSERT_EQ(UC_SUCCEEDED, mockClient.resultCode);
	EXPECT_EQ(UC_SUCCEEDED, mockClient.resultCode);
	//wait
	
	ContactServiceMockClient contactclient;
	contactclient.Init(ucEngine);
	//contactclient.GetUpdatedContacts();
	//contactclient.GetContactInfo(1382756);

	//boost::unique_lock<boost::recursive_mutex>	lock(mockClient.mthodMtx);
	lock.lock();
	contactclient.methodCond.wait(lock);
	lock.unlock();
	cout <<"***********get resultCode from contactserver:"<<mockClient.resultCode<<endl;

	EXPECT_EQ(UC_SUCCEEDED, contactclient.resultCode);
}*/