#include "UCMockClient.h"
#include "UCErrorCode.h"
#include "IFileTransferService.h"
#include <boost/thread.hpp>
#include "LogManager.h"
#include "StringConvert.h"
#include "ClientInfo.h"
#include "UCCommonFunctionMock.h"
#include "FileTransferServiceTest.h"
#include "FileTransferServiceMockClient.h"


FileTransferServiceTest::FileTransferServiceTest() { 
	_ucEngine = NULL;
}

void FileTransferServiceTest::SetUp() {
	cout <<"\r\n"<<endl;
	_info.account = "jie.shen@quanshi.com";
	_info.password = "redapple";
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
	CreateEngineConfig(ucConfig);		 
	_ucEngine = CreateUCEngine(ucConfig);
}

void FileTransferServiceTest::TearDown() {	
	DestroyUCEngine();
	_ucEngine = NULL;
}

void FileTransferServiceTest::SetUpTestCase(){

}
void FileTransferServiceTest::TearDownTestCase(){

}


TEST_F(FileTransferServiceTest, UploadFileSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	FileTransferServiceMockClient fileTransferMock;
	fileTransferMock.Init(_ucEngine);
	std::wstring filePath = L"D:\\test\\upload\\16.pdf";
	int32_t taskId = 0;
	fileTransferMock.UploadFile(StringConvert::ToUTF8Str(filePath), taskId);
	fileTransferMock.WaitAssertEQ(UC_SUCCEEDED);
	fileTransferMock.UnInit();
	mockClient.UnInit();
}