#include "IMServiceTest.h"
#include "IMServiceMockClient.h"
#include "UCCommonFunctionMock.h"
#include "UCMockClient.h"
#include "UCErrorCode.h"

IMServiceTest::IMServiceTest(){

	_ucEngine = NULL;
}

void IMServiceTest::SetUp() {

	cout <<"\r\n"<<endl;
	_info.account = "testb_grape9@quanshi.com";
	_info.password = "11111111";
	_info.clientType = UCCLIENT_WIN;
	_info.mac = "6c:40:08:9f:ba:ca";
	_info.clientId = "3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba";
	_info.pinNumber = "E40D42D2-4C88-47D0-8029-03DEEC649312";
	_info.version = "1.9.590.16577";
	_info.storeType = "0";
	_info.phoneNumber = "1234567890";
	_info.newPass = "buildbuild";
	_info.confirmPass = "buildbuild";

	UCEngineConfig ucConfig;
	ucConfig.casServer = "https://testcloud.quanshi.com";
	CreateEngineConfig(ucConfig);		 
	_ucEngine = CreateUCEngine(ucConfig);
}

void IMServiceTest::TearDown() {	
	DestroyUCEngine();
	_ucEngine = NULL;
}

void IMServiceTest::SetUpTestCase(){



}

void IMServiceTest::TearDownTestCase(){


}

