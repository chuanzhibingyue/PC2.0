#include "GroupServiceTest.h"
#include "GroupServiceMockClient.h"
#include "UCCommonFunctionMock.h"
#include "UCMockClient.h"
#include "UCErrorCode.h"

GroupServiceTest::GroupServiceTest(){

	_ucEngine = NULL;
}

void GroupServiceTest::SetUp() {
 
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

void GroupServiceTest::TearDown() {	
	DestroyUCEngine();
	_ucEngine = NULL;
}

void GroupServiceTest::SetUpTestCase(){



}

void GroupServiceTest::TearDownTestCase(){


}

#if 0
TEST_F(GroupServiceTest, CreateProjectGroupSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	std::string groupName = "test";
	UserInfo info;
	info.userId = 62007843;
	info.userName = "PT9";
	UserInfo info1;
	info1.userId = 62007834;
	info1.userName = "PT0";
	UserInfo info2;
	info2.userId = 62007835;
	info2.userName = "PT1";
	UserInfo info3;
	info2.userId = 62007836;
	info2.userName = "PT2";
	UserInfo info4;
	info2.userId = 62007837;
	info2.userName = "PT3";

	vector<UserInfo> vcInfo;
	vcInfo.push_back(info);
	vcInfo.push_back(info1);
	vcInfo.push_back(info2);
	vcInfo.push_back(info3);
	vcInfo.push_back(info4);
	groupMock.GetChatHistory(GROUP_CHAT, 1111, 20, 0, 0);

	//groupMock.CreateGroup(PROJECT_GROUP_TYPE, groupName, vcInfo, 1);
	//groupMock.WaitAssertEQ(UC_SUCCEEDED);

	//groupMock.UnInit();
	mockClient.UnInit();
}



TEST_F(GroupServiceTest, CreateMutilChatGroupSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	std::string groupName = "test";
	UserInfo info;
	info.userId = 62007843;
	info.userName = "PT9";
	UserInfo info1;
	info1.userId = 62007834;
	info1.userName = "PT0";
	UserInfo info2;
	info2.userId = 62007835;
	info2.userName = "PT1";
	UserInfo info3;
	info2.userId = 62007836;
	info2.userName = "PT2";
	UserInfo info4;
	info2.userId = 62007837;
	info2.userName = "PT3";

	vector<UserInfo> vcInfo;
	vcInfo.push_back(info);
	vcInfo.push_back(info1);
	vcInfo.push_back(info2);
	vcInfo.push_back(info3);
	vcInfo.push_back(info4);

	groupMock.CreateGroup(MUTILCHAT_GROUP_TYPE, groupName, vcInfo, 0);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, GetProjectGroupListSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.GetGroupList(PROJECT_GROUP_TYPE);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, GetMutilChatGroupListSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.GetGroupList(MUTILCHAT_GROUP_TYPE);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}



TEST_F(GroupServiceTest, GetProjectGroupInfoSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.GetGroupInfo(80000081);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, GetMutilChatGroupInfoSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.GetGroupInfo(3123);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, EndProjectGroupSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.EndGroup(3123);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}


TEST_F(GroupServiceTest, EndMutilChatGroupSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.EndGroup(3123);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, ExitProjectGroupSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.ExitGroup(3123);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, ExitMutilChatGroupSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.ExitGroup(3123);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, AddProjectGroupMemberSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UserInfo info;
	info.userId = 62007839;
	info.userName = "PT5";
	UserInfo info1;
	info1.userId = 62007840;
	info1.userName = "PT5";
	UserInfo info2;
	info2.userId = 62007841;
	info2.userName = "PT6";

	vector<UserInfo> vcInfo;
	vcInfo.push_back(info);
	vcInfo.push_back(info1);
	vcInfo.push_back(info2);

	groupMock.AddMember(3123, vcInfo);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, AddMutilChatGroupMemberSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UserInfo info;
	info.userId = 62007839;
	info.userName = "PT5";
	UserInfo info1;
	info1.userId = 62007840;
	info1.userName = "PT5";
	UserInfo info2;
	info2.userId = 62007841;
	info2.userName = "PT6";

	vector<UserInfo> vcInfo;
	vcInfo.push_back(info);
	vcInfo.push_back(info1);
	vcInfo.push_back(info2);

	groupMock.AddMember(3123, vcInfo);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, RemoveProjectGroupMemberSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	vector<int32_t> vcInfo;
	vcInfo.push_back(62007839);
	vcInfo.push_back(62007840);

	groupMock.RemoveMember(3123, vcInfo);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, RemoveMutilChatGroupMemberSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	vector<int32_t> vcInfo;
	vcInfo.push_back(62007839);
	vcInfo.push_back(62007840);

	groupMock.RemoveMember(3123, vcInfo);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SetProjectGroupNameSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	std::string groupName = "TEST_Modify";

	groupMock.SetGroupName(3123, groupName);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SetMutilChatGroupNameSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	std::string groupName = "TEST_Modify";

	groupMock.SetGroupName(3123, groupName);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SetProjectGroupLogoSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	std::string logoUrl = "";

	groupMock.SetGroupLogo(3123, logoUrl);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SetProjectGroupReminderSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);
	
	int32_t state = 1;
	int32_t alertRule = 7;
	int32_t alertTime = 1200;
	groupMock.SetReminder(3123, state, alertRule, alertTime);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SetProjectGroupTopChatSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	int32_t state = 1;
	groupMock.SetTopChat(3123, state);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SetMutilChatGroupTopChatSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	int32_t state = 1;
	groupMock.SetTopChat(3123, state);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SetProjectGroupDoNotDistrabSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	int32_t state = 1;
	groupMock.SetDoNotDisturb(3123, state);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SetMutilChatGroupDoNotDistrabSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	int32_t state = 1;
	groupMock.SetDoNotDisturb(3123, state);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, GetProjectGroupDocumentListSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.GetDocumentList(3123);
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, GetProjectGroupDocumentInfoSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.GetDocumentInfo(3123, 1234);
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, UploadProjectGroupDocumentSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	std::string fileName = "";
	int32_t fileSize = 0;
	std::string fileUrl  = "";
	int64_t uploadTime = 0;
	groupMock.UploadDocument(3123, fileName, fileSize, fileUrl, uploadTime);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, ShareProjectGroupDocumentSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	std::string fileName = "";
	int32_t fileSize = 0;
	std::string fileUrl  = "";
	int64_t uploadTime = 0;
	groupMock.ShareDocument(3123, fileName, fileSize, fileUrl, uploadTime);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, DeleteProjectGroupDocumentSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	groupMock.DeleteDocument(3123, 123123);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupTextSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int8_t textType = 1;
	std::string textData = "≤‚ ‘";
	int32_t msgID = 0;
	groupMock.SendText(PROJECT_CHAT, groupGid, textType, textData, msgID);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupTextSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int8_t textType = 1;
	std::string textData = "≤‚ ‘";
	int32_t msgID = 0;
	groupMock.SendText(GROUP_CHAT, groupGid, textType, textData, msgID);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMeetingGroupTextSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int8_t textType = 1;
	std::string textData = "≤‚ ‘";
	int32_t msgID = 0;
	groupMock.SendText(CONFERENCE_CHAT, groupGid, textType, textData, msgID);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupImageSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string imageName = "";
	int32_t imageSize = 0;
	std::string imageUrl = "";
	std::string imageData = "";
	int32_t msgId = 0;
	groupMock.SendImage(PROJECT_CHAT, groupGid, imageName, imageSize, imageUrl, imageData, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupTImageSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string imageName = "";
	int32_t imageSize = 0;
	std::string imageUrl = "";
	std::string imageData = "";
	int32_t msgId = 0;
	groupMock.SendImage(GROUP_CHAT, groupGid, imageName, imageSize, imageUrl, imageData, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMeetingGroupImageSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string imageName = "";
	int32_t imageSize = 0;
	std::string imageUrl = "";
	std::string imageData = "";
	int32_t msgId = 0;
	groupMock.SendImage(CONFERENCE_CHAT, groupGid, imageName, imageSize, imageUrl, imageData, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupAudioSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string audioName = "";
	int32_t audioSize = 0;
	std::string audioUrl = "";
	int32_t audioDuration= 0;
	int32_t msgId = 0;
	groupMock.SendAudio(PROJECT_CHAT, groupGid, audioName, audioSize, audioUrl, audioDuration, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupAudioSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string audioName = "";
	int32_t audioSize = 0;
	std::string audioUrl = "";
	int32_t audioDuration= 0;
	int32_t msgId = 0;
	groupMock.SendAudio(GROUP_CHAT, groupGid, audioName, audioSize, audioUrl, audioDuration, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMeetingGroupAudioSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string audioName = "";
	int32_t audioSize = 0;
	std::string audioUrl = "";
	int32_t audioDuration= 0;
	int32_t msgId = 0;
	groupMock.SendAudio(CONFERENCE_CHAT, groupGid, audioName, audioSize, audioUrl, audioDuration, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupVedioSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string videoName = "";
	int32_t videoSize = 0;
	std::string videoUrl = "";
	std::string videoData = "";
	int32_t videoDuration= 0;
	int32_t msgId = 0;
	groupMock.SendVideo(PROJECT_CHAT, groupGid, videoName, videoSize, videoUrl, videoData, videoDuration, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupVideoSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string videoName = "";
	int32_t videoSize = 0;
	std::string videoUrl = "";
	std::string videoData = "";
	int32_t videoDuration= 0;
	int32_t msgId = 0;
	groupMock.SendVideo(GROUP_CHAT, groupGid, videoName, videoSize, videoUrl, videoData, videoDuration, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMeetingGroupVideoSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string videoName = "";
	int32_t videoSize = 0;
	std::string videoUrl = "";
	std::string videoData = "";
	int32_t videoDuration= 0;
	int32_t msgId = 0;
	groupMock.SendVideo(CONFERENCE_CHAT, groupGid, videoName, videoSize, videoUrl, videoData, videoDuration, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupInviteCallSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string confId = "";
	std::string confPwd = "";
	int32_t hostId = 0;
	int32_t msgId = 0;
	groupMock.SendInvateCall(PROJECT_CHAT, groupGid, confId, confPwd, hostId, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupInviteCallSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string confId = "";
	std::string confPwd = "";
	int32_t hostId = 0;
	int32_t msgId = 0;
	groupMock.SendInvateCall(GROUP_CHAT, groupGid, confId, confPwd, hostId, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupAcceptCallSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string confId = "";
	int32_t msgId = 0;
	groupMock.SendAppectCall(PROJECT_CHAT, groupGid, confId, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupAcceptCallSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string confId = "";
	int32_t msgId = 0;
	groupMock.SendAppectCall(GROUP_CHAT, groupGid, confId, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupRejectCallSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string confId = "";
	int32_t msgId = 0;
	groupMock.SendRejectCall(PROJECT_CHAT, groupGid, confId, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupRejectCallSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string confId = "";
	int32_t msgId = 0;
	groupMock.SendRejectCall(GROUP_CHAT, groupGid, confId, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupFinishCallSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	std::string confId = "";
	int32_t msgId = 0;
	groupMock.SendFinishCall(PROJECT_CHAT, groupGid, confId, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupFinishCallSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);


	UCID groupGid;
	std::string confId = "";
	int32_t msgId = 0;
	groupMock.SendFinishCall(GROUP_CHAT, groupGid, confId, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupConnectedCallRecordSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int32_t duration = 0;
	int32_t msgId = 0;
	groupMock.SendConnectCallRecord(PROJECT_CHAT, groupGid, duration, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupConnectedCallRecordSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int32_t duration = 0;
	int32_t msgId = 0;
	groupMock.SendConnectCallRecord(PROJECT_CHAT, groupGid, duration, msgId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupMessageReadSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int32_t msgId = 0;
	int64_t msgSeq = 0;
	groupMock.SendMessageRead(PROJECT_CHAT, groupGid, msgId, msgSeq);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupMessageReadSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int32_t msgId = 0;
	int64_t msgSeq = 0;
	groupMock.SendMessageRead(GROUP_CHAT, groupGid, msgId, msgSeq);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMeetingGroupMessageReadSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int32_t msgId = 0;
	int64_t msgSeq = 0;
	groupMock.SendMessageRead(CONFERENCE_CHAT, groupGid, msgId, msgSeq);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupMessagesReadSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	vector<int32_t> chatId;
	groupMock.SendMessagesRead(3123, chatId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupMessagesReadSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	vector<int32_t> chatId;
	groupMock.SendMessagesRead(3123, chatId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMeetingGroupMessagesReadSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	vector<int32_t> chatId;
	groupMock.SendMessagesRead(3123, chatId);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendProjectGroupMessagePlayedSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int32_t msgId = 0;
	int64_t msgSeq = 0;
	groupMock.SendMessagesPalyed(PROJECT_CHAT, groupGid, msgId, msgSeq);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMutilChatGroupMessagePlayedSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int32_t msgId = 0;
	int64_t msgSeq = 0;
	groupMock.SendMessagesPalyed(GROUP_CHAT, groupGid, msgId, msgSeq);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}

TEST_F(GroupServiceTest, SendMeetingGroupMessagePlayedSuccessed){

	UCMockClient mockClient(_info);
	mockClient.Init(_ucEngine);

	mockClient.Login();
	mockClient.WaitAssertEQ(UC_SUCCEEDED);

	GroupServiceMockClient groupMock;
	groupMock.Init(_ucEngine);

	UCID groupGid;
	int32_t msgId = 0;
	int64_t msgSeq = 0;
	groupMock.SendMessagesPalyed(GROUP_CHAT, groupGid, msgId, msgSeq);
	groupMock.WaitAssertEQ(UC_SUCCEEDED);

	groupMock.UnInit();
	mockClient.UnInit();
}
#endif