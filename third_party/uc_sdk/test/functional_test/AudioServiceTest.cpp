#include "AudioServiceTest.h"
#include "AudioServiceMockClient.h"
#include "UCErrorCode.h"
#include "LogManager.h"
#include "IUCEngine.h"
#include "UCCommonFunctionMock.h"

#include "UCMockClient.h"

AudioServiceTest::AudioServiceTest() { 
	_ucEngine = NULL;
	_info = NULL;
	_mockClient = NULL;
	_service = NULL;
}

void AudioServiceTest::SetUp() {
	cout <<"\r\n"<<endl;
	_info = new ClientInfo();
	_info->account = "testb_grape9@quanshi.com";
	_info->password = "11111111";
	_info->clientType = UCCLIENT_WIN;
	_info->mac = "6c:40:08:9f:ba:ca";
	_info->clientId = "3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba";
	_info->pinNumber = "E40D42D2-4C88-47D0-8029-03DEEC649312";
	_info->version = "1.9.590.16577";
	_info->storeType = "0";
	_info->phoneNumber = "1234567890";
	_info->newPass = "buildbuild";
	_info->confirmPass = "buildbuild";

	UCEngineConfig ucConfig;
	ucConfig.casServer = "https://testcloud.quanshi.com";
	CreateEngineConfig(ucConfig);		 
	_ucEngine = CreateUCEngine(ucConfig);

	_mockClient = new UCMockClient(*_info);
	_mockClient->Init(_ucEngine);

	_service = new 	AudioServiceMockClient();
	_service->Init(_ucEngine);

	_mockClient->Login();
	_mockClient->WaitAssertEQ(UC_SUCCEEDED);
}

void AudioServiceTest::TearDown() {	
	_mockClient->Logout();
	_mockClient->WaitAssertEQ(UC_SUCCEEDED);
}

void AudioServiceTest::SetUpTestCase(){

}

void AudioServiceTest::TearDownTestCase(){
	// 	if (NULL != _service)
	// 	{
	// 		_service->UnInit();
	// 		delete _service;
	// 		_service = NULL;
	// 	}
	// 
	// 	if (NULL != _mockClient)
	// 	{
	// 		_mockClient->Logout();
	// 		_mockClient->WaitAssertEQ(UC_SUCCEEDED);
	// 		_mockClient->UnInit();
	// 		delete _mockClient;
	// 		_mockClient = NULL;
	// 	}

	//  	DestroyUCEngine();
	//  	_ucEngine = NULL;
	// 
	// 	if (NULL != _info)
	// 	{
	// 		delete _info;
	// 		_info = NULL;
	// 	}
}

// TEST_F(AudioServiceTest, CreateCallSuccess) {
// 	int64_t dd = 11111111111111111;
// 	std::string s = StringConvert::Convert2String(dd);
// 	_service->CreateCall();
// 	
// 	_service->WaitAssertEQ(UC_SUCCEEDED);
// }


// TEST_F(AudioServiceTest, JoinCallSuccess) {	
// 
// 	_service->CreateCall();
// 	_service->WaitAssertEQ(UC_SUCCEEDED);
// 
// 	if (0 == _service->_tempConferenceId)
// 	{
// 		return;
// 	}
// 	_service->JoinCall(_service->_tempConferenceId, _service->_reserveCode);
// 	_service->WaitAssertEQ(UC_SUCCEEDED);
// }
// 
// TEST_F(AudioServiceTest, LeaveCallSuccess) {	
// 	_mockClient->Login();
// 	_mockClient->WaitAssertEQ(UC_SUCCEEDED);
// 
// 	_service->CreateCall();
// 	_service->WaitAssertEQ(UC_SUCCEEDED);
// 
// 	if (0 == _service->_tempConferenceId)
// 	{
// 		return;
// 	}
// 	_service->LeaveCall(_service->_tempConferenceId, _service->_tempUserId);
// 	_service->WaitAssertEQ(UC_SUCCEEDED);
// }

TEST_F(AudioServiceTest, JoinConferenceSuccess) {
	int64_t dd = 11111111111111111;
	std::string s = StringConvert::Convert2String(dd);
	_service->CreateCall();

	_service->WaitAssertEQ(UC_SUCCEEDED);

	int ret = _service->JoinConference(_service->_token);
	_service->WaitAssertEQ(UC_SUCCEEDED);
}