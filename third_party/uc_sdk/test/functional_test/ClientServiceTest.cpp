#include "gtest/gtest.h"
#include "UCMockClient.h"
#include "UCErrorCode.h"
#include "IClientService.h"
#include <boost/thread.hpp>
#include "LogManager.h"
#include "StringConvert.h"
#include "ClientInfo.h"
#include "UCCommonFunctionMock.h"
#include "ClientServiceTest.h"


ClientServiceTest::ClientServiceTest() { 
	ucEngine = NULL;
}

void ClientServiceTest::SetUp() {
	cout <<"\r\n"<<endl;
	info.account = "jie.shen@quanshi.com";
	info.password = "redapple";
	info.clientType = UCCLIENT_WIN;
	info.mac = "6c:40:08:9f:ba:ca";
	info.clientId = "3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba";
	info.pinNumber = "E40D42D2-4C88-47D0-8029-03DEEC649312";
	info.version = "1.9.590.16577";
	info.storeType = "0";
	info.phoneNumber = "1234567890";
	info.newPass = "buildbuild";
	info.confirmPass = "buildbuild";
	
	UCEngineConfig ucConfig;
	CreateEngineConfig(ucConfig);		 
	ucEngine = CreateUCEngine(ucConfig);
}

void ClientServiceTest::TearDown() {	
	DestroyUCEngine();
	ucEngine = NULL;
}

void ClientServiceTest::SetUpTestCase(){

}
void ClientServiceTest::TearDownTestCase(){

}


//Reset Password
/*
TEST_F(ClientServiceTest, ResetPassword) {

	UCMockClient mockClient(info);
	mockClient.Init(ucEngine);
	mockClient.Login();
	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);
	ASSERT_EQ(UC_SUCCEEDED, mockClient.resultCode);

	mockClient.ResetPassword();

	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);
	wcout << L"***********get resultCode from server:" << info.newPass<<endl;
//wait
	//EXPECT_EQ(UC_SUCCEEDED, mockClient.resultCode);
}
*/

/*
//UCASConnect Module
TEST_F(ClientServiceTest, ConnectUCAS) {
	
	UCMockClient mockClient(info);
	mockClient.Init(ucEngine);
	mockClient.Login();

	//wait login
	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);

	// wait ucas
	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);
	
}

<<<<<<< .mine
=======
*/
/*
>>>>>>> .r54197
//UCASDisconnect Module
TEST_F(ClientServiceTest, DisconnectUCAS) {
	
	UCMockClient mockClient(info);
	mockClient.Init(ucEngine);
	mockClient.Login();

	//wait login
	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);

	// wait ucas
	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);
}
*/

/*
//Logout Module
TEST_F(ClientServiceTest, LogOut) {
	
	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);
	mockClient.Login();

	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);
	cout <<"have login......"<<endl;
	mockClient.Logout();
   UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);
	cout <<"have logout......"<<endl;
}
*/

/*
TEST_F(ClientServiceTest, LoginSucceeded) {	
	UCMockClient mockClient(info);
	mockClient.Init(ucEngine);
	mockClient.Login();

   UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, UC_SUCCEEDED, mockClient.resultCode);
 }

*/

TEST_F(ClientServiceTest, LoginSucceeded) {	
	
	UCMockClient mockClient(info);
	mockClient.Init(ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);
	
	mockClient.ConnectUCAS();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	mockClient.DisconnectUCAS(); //这个函数是同步操作,不能用当前的方式来判断

	mockClient.Logout();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	mockClient.UnInit();
}

TEST_F(ClientServiceTest, PasswordEmpty) {
	
	info.password="";
	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);		
	mockClient.Login();

	mockClient.WaitAssertEQ(EMPTY_USER_PASS);
}

TEST_F(ClientServiceTest, AccountEmpty) {		
	info.account="";
	UCMockClient mockClient(info);
	mockClient.Init(ucEngine);
	mockClient.Login();

	mockClient.WaitAssertEQ(EMPTY_PASSWORD);
}

TEST_F(ClientServiceTest, AccountAndPasswordEmpty) {
	//
	info.account="";
	info.password="";

	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);
	mockClient.Login();

	mockClient.WaitAssertEQ(EMPTY_PASSWORD);
}

/*
TEST_F(ClientServiceTest, Android_ClientIdEmpty) {
	//

const ClientInfo info = {L"jian.xue@quanshi.com", L"11111111",UCCLIENT_ANDROID, L"6c:40:08:9f:ba:ca",
		L"", 
		L"E40D42D2-4C88-47D0-8029-03DEEC649312", L"1.9.590.16577", L"0", L"1234567890"};

	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);
	mockClient.Login();

	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, CLIENT_ID_MAC, mockClient.resultCode);
}


TEST_F(ClientServiceTest, IOS_ClientIdEmpty) {	
	const ClientInfo info = {L"jian.xue@quanshi.com", L"11111111", UCCLIENT_IOS, L"6c:40:08:9f:ba:ca",
		L"", 
		L"E40D42D2-4C88-47D0-8029-03DEEC649312", L"1.9.590.16577", L"0", L"1234567890"};
	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);
	mockClient.Login();

UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, CLIENT_ID_MAC, mockClient.resultCode);
}


TEST_F(ClientServiceTest, PC_MacEmpty) {
	//
	const ClientInfo info = {L"jie.shen@quanshi.com", L"redapple", UCCLIENT_WIN, L"",
		L"3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba", 
		L"E40D42D2-4C88-47D0-8029-03DEEC649312", L"1.9.590.16577", L"0", L"1234567890"};

	UCMockClient mockClient(info);
	
	mockClient.Init(ucEngine);
	mockClient.Login();

	UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, CLIENT_ID_MAC, mockClient.resultCode);
}




TEST_F(ClientServiceTest, ClientTypeEmpty) {
    const ClientInfo info = {L"jie.shen@quanshi.com", L"redapple",NULL,L"6c:40:08:9f:ba:ca",
		L"3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba", 
		L"E40D42D2-4C88-47D0-8029-03DEEC649312", L"1.9.590.16577", L"0", L"1234567890"};

	UCMockClient mockClient(info);
	mockClient.Init(ucEngine);	
	mockClient.Login();

UC_EXPECT_EQ(mockClient.methodMtx, mockClient.methodCond, EMPTY_CLIENT_TYPE, mockClient.resultCode);
}


/*

TEST_F(ClientServiceTest, ClientTypeError) {
	//
	ClientInfo info = {L"jian.xue@quanshi.com", L"11111111", 88, L"6c:40:08:9f:ba:ca",
		L"3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba", 
		L"E40D42D2-4C88-47D0-8029-03DEEC649312", L"1.9.590.16577", L"0", L"1234567890"};

	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);
	//info.clientType =88;
	mockClient.Login();

	boost::unique_lock<boost::recursive_mutex> lock(mockClient.mthodMtx);
	mockClient.methodCond.wait(lock);
	lock.unlock();

	cout <<"***********get resultCode from server:"<<mockClient.resultCode<<endl;
	//wait
	EXPECT_EQ(EMPTY_CLIENT_TYPE, mockClient.resultCode);
}



TEST_F(ClientServiceTest, PasswordError) {

	ClientInfo info = {L"jian.xue@quanshi.com", L"222222", UCCLIENT_WIN, L"6c:40:08:9f:ba:ca",
		L"3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba", 
		L"E40D42D2-4C88-47D0-8029-03DEEC649312", L"1.9.590.16577", L"0", L"1234567890"};

	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);
	//info.password = L"123";
	mockClient.Login();

	boost::unique_lock<boost::recursive_mutex> lock(mockClient.mthodMtx);
	mockClient.methodCond.wait(lock);
	lock.unlock();
	cout <<"***********get resultCode from server:"<<mockClient.resultCode<<endl;

	//wait
	EXPECT_EQ(EMPTY_USER_PASS, mockClient.resultCode);
}


TEST_F(ClientServiceTest, AccountError) {
	ClientInfo info = {L"jian.xue1@quanshi.com", L"11111111", UCCLIENT_WIN, L"6c:40:08:9f:ba:ca",
		L"3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba", 
		L"E40D42D2-4C88-47D0-8029-03DEEC649312", L"1.9.590.16577", L"0", L"1234567890"};

	//
	UCMockClient mockClient(info);

	mockClient.Init(ucEngine);
	//info.account = L"11123";
	mockClient.Login();

	boost::unique_lock<boost::recursive_mutex> lock(mockClient.mthodMtx);
	mockClient.methodCond.wait(lock);
	lock.unlock();
cout <<"***********get resultCode from server:"<<mockClient.resultCode<<endl;
	//wait
	EXPECT_EQ(USER_ACCOUNT_ERROR, mockClient.resultCode);
}
*/

