//============================================================================
// Name        : UC_SDK.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "IUCEngine.h"
#include "IClientService.h"
#include "IContactService.h"
#include "IImService.h"
#include "IGroupService.h"
#include "gtest/gtest.h"
#include "LogManager.h"
#include "StringConvert.h"
#include "Gzip.h"
#include "UCMessageType.h"
#include "MessageParser.h"
#include "UCMessage.h"
#include "ContactInfo.h"
#include "IGroupService.h"
#include "ICalendarService.h"
#include "IFileTransferService.h"
#include "ISearchService.h"
#include "UCErrorCode.h"
#include "UCTime.h"

using namespace std;
using namespace uc;

/**
 * 创建程序的工作目录
 */
#define APP_DIRECTORY_NAME "Bee";

static string g_AppDir = "";
static IUCEngine *g_ucEngine = NULL;

class UCMockClient : public IClientServiceEvent {
public:
	UCMockClient() {
		_clientService = NULL;
		_contactService = NULL;
	}

	virtual ~UCMockClient() {
		UnInit();
	}

public:
	virtual void OnLogin(int32_t errorCode) {
		cout << "OnLogin: " << errorCode << endl;

		if (errorCode == UC_SUCCEEDED) {
			// 登录UCAS
			_clientService->ConnectUCAS();
		}
	}

	virtual void OnLogout(int32_t errorCode) {
		cout << "OnLogout: " << errorCode << endl;
	}

	virtual void OnPasswordModified(int32_t errorCode) {
		UC_LOG(INFO) << "OnPasswordModified:" << errorCode;
	}

	virtual void OnPasswordReset(int32_t errorCode) {
		UC_LOG(INFO) << "OnPasswordReset:" << errorCode;
	}

	virtual void OnPasswordModifyMessageReceived(const string &password) {
		UC_LOG(INFO) << "OnPasswordModifyMessageReceived:" << password;
	}

	virtual void OnPasswordRuleUpdateMessageReceived(int32_t type) {
		UC_LOG(INFO) << "OnPasswordRuleUpdateMessageReceived:" << type;
	}

	virtual void OnPasswordExpiredMessageReceived(int32_t dueDays) {
		UC_LOG(INFO) << "OnPasswordExpiredMessageReceived:" << dueDays;
	}

	virtual void OnUCASConnected(int32_t errorCode) {
		cout << "OnUCASConnect: " << errorCode << endl;
	}

	virtual void OnUCASDisconnected(int32_t errorCode) {
		cout << "OnUCASDisconnect:" << errorCode << endl;
	}

	virtual void OnAccountVerified(int32_t errorCode) {
		cout << "OnAccountVerified:" << errorCode << endl;
	}

	virtual void OnCodeVerified(int32_t errorCode) {
		cout << "OnCodeVerified:" << errorCode << endl;
	}

	virtual void OnForgetPassword(int32_t errorCode) {
		cout << "OnForgetPassword:" << errorCode << endl;
	}

	virtual void OnConversationListReceived(int32_t errorCode, const std::list<ConversationInfo*> *conversations, bool clearCache) {
		cout << "OnConversationListReceived: " << errorCode << endl;
		if (errorCode != UC_SUCCEEDED)
			return;

		int32_t count = conversations->size();
		ConversationInfo *conversation = NULL;

		for ( std::list<ConversationInfo*>::const_iterator iter = conversations->begin();
			  iter != conversations->end(); ++iter ) {
			conversation = (ConversationInfo*)(*iter);

			cout << "Conversation Type: ";
			switch (conversation->GetType()) 
			{
			case SINGLE_CHAT:
				{
					cout << "p2p Chat" << endl;
					break;
				}
			case GROUP_CHAT:
				{
					cout << "Group Chat" << endl;
					break;
				}
			case CONFERENCE_CHAT:
				{
					cout << "Conference Chat" << endl;
					break;
				}
				/*
			case SYSTEM_MSG:
				{
					cout << "System" << endl;
					break;
				}
			case ORGANIZE_MSG:
				{
					cout << "Organize" << endl;
					break;
				}
			case APP_MSG:
				{
					cout << "Application" << endl;
					break;
				}
			case SHARE_MSG:
				{
					cout << "Share" << endl;
					break;
				}
			case AUDIO_MSG:
				{
					cout << "Audio" << endl;
					break;
				}*/
			default:
				{
					cout << conversation->GetType() << endl;
					break;
				}
			}

			//	cout << "avatar: " << conversation->GetAvatar() << endl;
			cout << "TimeStamp: " << conversation->GetLastMsgTime() << endl;
		//	cout << "Content: " << conversation->GetContent() << endl;
		//	cout << "From: " << conversation->GetFromId() << endl;
			cout << "To: " << conversation->GetRelateId() << endl;
			cout << "=============================================" << endl;
		}
	}

public:
	void Init() {
		_clientService = (IClientService*)g_ucEngine->QueryService(UCClientService);
		_clientService->registerListener(this);

		_contactService = (IContactService*)g_ucEngine->QueryService(UCContactService);
	}

	void UnInit() {
		_clientService->removeListener(this);
	}

	/**
	 *
	 */
	void Login() {

		//string account = "testc_camel35@quanshi.com";
		string account = "testb_grape0@quanshi.com";
		string password = "11111111";
		uint16_t clientType = UCCLIENT_WIN;
		string mac = "b0:83:fe:56:12:05";
		string clientId = "3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba";
		string pinNumber = "E40D42D2-4C88-47D0-8029-03DEEC649312";
		string version = "1.0.171.52615";
		string storeType = "0";
		string phoneNumber = "1234567890";

		_clientService->Login(clientType, account, password, clientId, mac, pinNumber, version, storeType, phoneNumber);
	}

	/**
	 *
	 */
	void Logout() {
		_clientService->DisconnectUCAS();
		_clientService->Logout();
	}

	void RunTests() {
		// list new conversation命令
		// 用户登陆时调用GetNewConversationList
		//
		LoadHistoryAccount();
		_clientService->GetNewConversationList(2);
	}

	void LoadHistoryAccount() {
		std::list<AccountInfo> accounts;
		std::list<AccountInfo>::iterator itor;

		_clientService->LoadHistoryAccounts(accounts);

		cout << "LoadHistoryAccount()" << endl;
		itor = accounts.begin();
		while (itor != accounts.end() )
		{
			AccountInfo& account = *itor;
			cout << "AccountName: " << account.GetUserAccount() << endl;
			cout << "UserName: " << account.GetDisplayName() << endl;
			cout << "AvatarURL: " << account.GetAvatarUrl() << endl;
			cout << "LoginTime: " << account.GetLoginTime() << endl;
			cout << "LastManualLoginTime: " << account.GetLastManualLoginTime() << endl;
			itor ++;
		}
		cout << "=================================" << endl;
	}

protected:
	IClientService* _clientService;
	IContactService* _contactService;
};

class UCMockIMChat : public IIMServiceEvent {
public:
	UCMockIMChat() {
		_imService = NULL;
	}

	~UCMockIMChat() {

	}

	bool Init() {
		_imService = (IIMService*)g_ucEngine->QueryService(UCIMService);
		_imService->registerListener(this);
		return true;
	}

	bool UnInit() {
		_imService->removeListener(this);
		return true;
	}

	void SendIMMessage() {
		UCID toUser;
		int8_t textType = 1;
	//	string textData = "测试消息，，的大幅度"; // 普通文本
		string textData = ":^)"; // 表情
		int32_t msgId;

		toUser.SetUserId(62007835);
		toUser.SetSiteId(71759);

		toUser.SetResId(0);

		_imService->SendTextMessage(toUser, textType, "", textData, msgId);
		cout << "======== SendTextMessage =============" << endl;
		cout << "To UserId: " << toUser.GetUserId() << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "======================================" << endl;
	}
	void SendImage() {
		UCID toUser;
	//	string textData = "测试消息，，的大幅度"; // 普通文本
		string imageName = "1234.jpg"; // 表情
		int32_t imageSize = 100; // 这个值好像没有用
		string imageURL = "http://oncloud.quanshi.com/ucfserver/hddown?fid=MTIyODY1MC84LzE0MzY3Njc5NDU3OTBiaWcuanBnXl5edGFuZ2hkZnNeXl5mOGZkM2MwMTM5ZjU3YzQzZmIwNDc2ZmM1ODE0NmZmZl5eXnRhbmdoZGZzXl5eODY4OTc$&u=1228650";
		string imageData = "";
		int32_t msgId;

		toUser.SetUserId(62007835);
		toUser.SetSiteId(71759);

		toUser.SetResId(0);

		_imService->SendImageMessage(toUser, imageName, imageSize, imageURL, imageData, msgId);
		cout << "======== SendImage =============" << endl;
		cout << "To UserId: " << toUser.GetUserId() << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "======================================" << endl;
	}
	void SendAudioMessage() {
		UCID toUser;
	//	string textData = "测试消息，，的大幅度"; // 普通文本
		string audioName = "1436773758048.amr"; // 表情
		int32_t audioSize = 23; // 这个值好像没有用
		string audioURL = "http://oncloud.quanshi.com/ucfserver/hddown?fid=MTIyODY1MC84LzE0MzY3NzM3NTgwNDguYW1yXl5edGFuZ2hkZnNeXl43NDc1MTIwODRhZWRmNGNhZTZiNjM2NjI1NTE4ZmY4YV5eXnRhbmdoZGZzXl5eMzk0Mg$&u=1228650";
		int32_t audioDuration = 2; // 语音的持续时间
		int32_t msgId;

		toUser.SetUserId(62007835);
		toUser.SetSiteId(71759);

		toUser.SetResId(0);

		_imService->SendAudioMessage(toUser, audioName, audioSize, audioURL, audioDuration, msgId);
		cout << "======== SendAudioMessage =============" << endl;
		cout << "To UserId: " << toUser.GetUserId() << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "======================================" << endl;
	}
	void SendFileMessage() {
		UCID toUser;
		string fileName = "1436773758048.amr"; // 表情
		int32_t fileSize = 23; // 这个值好像没有用
		string fileURL = "http://oncloud.quanshi.com/ucfserver/hddown?fid=MTIyODY1MC84LzE0MzY3NzM3NTgwNDguYW1yXl5edGFuZ2hkZnNeXl43NDc1MTIwODRhZWRmNGNhZTZiNjM2NjI1NTE4ZmY4YV5eXnRhbmdoZGZzXl5eMzk0Mg$&u=1228650";
		int32_t fileId = 2;
		int32_t msgId;

		toUser.SetUserId(62007835);
		toUser.SetSiteId(71759);

		toUser.SetResId(0);

		_imService->SendFileMessage(toUser, fileName, fileSize, fileURL, fileId, msgId);
		cout << "======== SendFileMessage =============" << endl;
		cout << "To UserId: " << toUser.GetUserId() << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "======================================" << endl;
	}
	void ShareFileMessage() {
		UCID toUser;
		string fileName = "1436773758048.amr"; // 表情
		int32_t fileSize = 23; // 这个值好像没有用
		string fileURL = "http://oncloud.quanshi.com/ucfserver/hddown?fid=MTIyODY1MC84LzE0MzY3NzM3NTgwNDguYW1yXl5edGFuZ2hkZnNeXl43NDc1MTIwODRhZWRmNGNhZTZiNjM2NjI1NTE4ZmY4YV5eXnRhbmdoZGZzXl5eMzk0Mg$&u=1228650";
		int32_t fileId = 2;
		int32_t msgId;

		toUser.SetUserId(62007835);
		toUser.SetSiteId(71759);

		toUser.SetResId(0);

		_imService->ShareFileMessage(toUser, fileName, fileSize, fileURL, fileId, msgId);
		cout << "======== ShareFileMessage =============" << endl;
		cout << "To UserId: " << toUser.GetUserId() << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "======================================" << endl;
	}
	void SendMessageRead() {
		UCID toUser;
		int32_t msgId = 149;
		int64_t msgSeq = 1234567890;

		toUser.SetUserId(62007835);
		toUser.SetSiteId(71759);

		toUser.SetResId(0);

		_imService->SendMessageRead(toUser, msgId, msgSeq);
		cout << "======== SendMessageRead =============" << endl;
		cout << "To UserId: " << toUser.GetUserId() << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "======================================" << endl;
	}
	void SendMessagesRead() {
		UCID toUser;
		vector<int32_t> chatId;

		toUser.SetUserId(62007835);
		toUser.SetSiteId(71759);
		toUser.SetResId(0);

		//chatId[0] = 1;
		//chatId[1] = 2;

		/*
		_imService->SendMessagesRead(toUser, chatId);
		cout << "======== SendMessagesRead =============" << endl;
		cout << "To UserId: " << toUser.GetUserId() << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "======================================" << endl;
		*/
	}
	void SendMessagePlayed() {
		UCID toUser;
		int32_t msgId = 149;
		int64_t msgSeq = 1234567890;

		toUser.SetUserId(62007835);
		toUser.SetSiteId(71759);

		toUser.SetResId(0);

		_imService->SendMessagePlayed(toUser, msgId, msgSeq);
		cout << "======== SendMessagePlayed =============" << endl;
		cout << "To UserId: " << toUser.GetUserId() << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "======================================" << endl;
	}
public:
	/**
	* 聊天历史列表;
	* @param vcInfo		   聊天内容列表;
	*/
	virtual void OnChatHistoryReceived(int32_t result, int64_t userId) {
		UC_LOG(INFO) << "UCMockIMChat::OnChatHistoryReceived";
	}

	/**
	* 消息发送结果;
	* @param errorCode	   错误码;
	* @param msgId         消息ID;
	*/
	virtual void OnMessageSent(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {


		cout << "======== OnMessageSent result ======= " << endl;
		cout << "Result: " << result << endl;
		cout << "To UserId: " << userId << endl;
	//	cout << "To UserName: " << userName << endl;
		cout << "Msg id: " << msgId << endl;
		cout << "Msg seq: " << msgSeq << endl;
		cout << "===================================== " << endl;
	}

	/**
	* 接收文本聊天消息;
	*/
	virtual void OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message) {
		cout << "=======UCMockIMChat::OnTextMessageReceived========" << endl;
		cout << "Channel: " << channel << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "TextType: " << message->GetTextType() << endl;
		cout << "Content: " << message->GetTextContent() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;
		cout << "==================================================" << endl;
	}

	/**
	* 接收图片消息;
	*/
	virtual void OnImageMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {

		cout << "=====UCMockIMChat::OnImageMessageReceived==============" << endl;
		cout << "Channel: " << channel << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;

		cout << "imageName: " << message->GetFileName() << endl;
		cout << "imageSize: " << message->GetFileSize() << endl;
		cout << "imageURL: " << message->GetFileUrl() << endl;
		cout << "imageData: " << message->GetThumbnails() << endl;
		cout << "========================================================" << endl;
	}

	/**
	* 接收音频消息;;
	*/
	virtual void OnAudioMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
		cout << "=====UCMockIMChat::OnAudioMessageReceived==============" << endl;
		cout << "Channel: " << channel << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;

		cout << "audioName: " << message->GetFileName() << endl;
		cout << "audioSize: " << message->GetFileSize() << endl;
		cout << "audioURL: " << message->GetFileUrl() << endl;
		cout << "audioDuration: " << message->GetDuration() << endl;
		cout << "========================================================" << endl;
	}

	/**
	* 接收视频消息;
	*/
	virtual void OnVideoMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
		cout << "=====UCMockIMChat::OnAudioMessageReceived==============" << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;

		cout << "videoName: " << message->GetFileName() << endl;
		cout << "videoSize: " << message->GetFileSize() << endl;
		cout << "videoURL: " << message->GetFileUrl() << endl;
		cout << "videothumbnails: " << message->GetThumbnails() << endl;
		cout << "========================================================" << endl;
	}

	virtual void OnMessageRead(int32_t result, int32_t userId, int32_t readType, int32_t msgId, int64_t msgSeq) {
		UC_LOG(INFO) << "UCMockIMChat::OnMessageRead";
	}

	virtual void OnMessagePlayed(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq) {
		UC_LOG(INFO) << "UCMockIMChat::OnMessagePlayed";
	}

	virtual void OnFileMessageReceived(MessageChannel channel, const FileMessageInfo *message) {
		cout << "=========UCMockIMChat::OnFileReceived==================" << endl;
		cout << "Channel: " << channel << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;

		cout << "FileName: " << message->GetFileName() << endl;
		cout << "FileSize: " << message->GetFileSize() << endl;
		cout << "FileURL: " << message->GetFileUrl() << endl;
		cout << "LocalPath: " << message->GetFilePath() << endl;
	//	cout << "ContentId: " << message->GetFileId() << endl;
		cout << "==================================================" << endl;
	}

	/**
	* 文件分享响应时间;
	* @param userId          发送者ID;
	* @param fileName        文件名称;
	* @param fileSize        文件大小;
	* @param fileURL         文件下载地址;
	* @param contentId       文档记录的内容ID;
	*/
	virtual void OnFileShared(UCID userId, int32_t msgId, int64_t msgSeq, const std::string& fileName, int32_t fileSize, const std::string& fileURL, int64_t contentId) {
		UC_LOG(INFO) << "UCMockIMChat::OnFileShared";
	}

	/**
	* 文件删除响应事件;
	* @param userId          发送者ID;
	* @param fileName        文件名称;
	* @param contentId       文档记录的内容ID;
	*/
	virtual void OnFileDeleted(UCID userId, int32_t msgId, int64_t msgSeq, const std::string& fileName, int64_t contentId) {
		UC_LOG(INFO) << "UCMockIMChat::OnFileDeleted";
	}


	virtual void OnCodeMessageReceived(MessageChannel channel, FileSendType optType, const CodeMessageInfo& message){
		UC_LOG(INFO) << "UCMockIMChat::OnCodeMessageReceived";

	}

	/**
	* 音频通话邀请响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param confId          会议ID;
	* @param confPwd         会议密码;
	* @param hostId          主持人ID;
	*/
	virtual void OnCallInvited(int32_t result, int32_t userId, const std::string confId, const std::string confPwd, int32_t hostId) {
		UC_LOG(INFO) << "UCMockIMChat::OnCallInvited";
	}

	/**
	* 音频通话接受响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param confId          会议ID;
	*/
	virtual void OnCallAccepted(int32_t result, int32_t userId, const std::string confId) {
		UC_LOG(INFO) << "UCMockIMChat::OnCallAccepted";
	}

	/**
	* 音频通话拒绝响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param confId          会议ID;
	*/
	virtual void OnCallRejected(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, const std::string confId) {
		UC_LOG(INFO) << "UCMockIMChat::OnCallRejected";
	}

	/**
	* 音频通话结束响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param confId          会议ID;
	*/
	virtual void OnCallStopped(int32_t result, int32_t userId, const std::string confId) {
		UC_LOG(INFO) << "UCMockIMChat::OnCallStopped";
	}

	/**
	* 音频通话取消响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	*/
	virtual void OnCallCanceled(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq) {
		UC_LOG(INFO) << "UCMockIMChat::OnCallCanceled";
	}

	/**
	* 音频通话未接通响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	*/
	virtual void OnCallNotResponded(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq) {
		UC_LOG(INFO) << "UCMockIMChat::OnCallNotResponded";
	}

	/**
	* 音频通话接通时长响应事件;
	* @param result 	     错误信息;
	* @param userId          用户ID;
	* @param duration        通话时间;
	*/
	virtual void OnCallConnected(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int32_t duration) {
		UC_LOG(INFO) << "UCMockIMChat::OnCallConnected";
	}
private:
	IIMService* _imService;

};

class UCMockGroupChat : public IGroupServiceEvent{
public:
	UCMockGroupChat() {
		_groupService = NULL;
	}

	virtual ~UCMockGroupChat() {
		_groupService = NULL;
	}

	bool Init() {
		_groupService = (IGroupService*)g_ucEngine->QueryService(UCGroupService);
		_groupService->registerListener(this);
		return true;
	}

	bool UnInit() {
		_groupService->removeListener(this);
		return true;
	}

	void CreateGroup(GroupStyle groupType){

		std::string groupName = "test";
		UserInfo info;
		info.userId = 61371652;
		info.userName = "camel0";
		UserInfo info1;
		info1.userId = 61371737;
		info1.userName = "camel16";
		UserInfo info2;
		info2.userId = 61371827;
		info2.userName = "camel2";

		vector<UserInfo> vcInfo;
		vcInfo.push_back(info);
		vcInfo.push_back(info1);
		vcInfo.push_back(info2);


		_groupService->CreateGroup(groupType, groupName, vcInfo, 0);		
	}
	
	void ExitGroup(){
		_groupService->ExitGroup(40000501);
	}

	void EndGroup(){
		_groupService->EndGroup(40000499);
	}

	void GetGroupList(GroupStyle groupType){
		_groupService->GetGroupList(groupType);
	}

	void GetGroupInfo(){
		GroupInfo group;

		_groupService->GetGroupInfo(40000665, group);
	}

	void AddMember(){
		std::vector<UserInfo> user;
		UserInfo info;
		info.userId = 62000059;
		info.userName = "camel0";
		UserInfo info1;
		info1.userId = 62000075;
		info1.userName = "camel16";
		user.push_back(info);
		user.push_back(info1);

		_groupService->AddMember(40001257, user);
	}

	void RemoveMember(){
		std::vector<int32_t> user;
		user.push_back(62000061);

		_groupService->RemoveMember(40000463, user);
	}

	void SetGroupName(){
		_groupService->SetGroupName(40000501, "测试一");
	}

	void SetGroupLogo(){
	//	_groupService->SetGroupLogo(40000501, "123\4122254\66666666661\5122");
	}

	void SetTopChat(){
		_groupService->SetTopChat(40000501, 1);
	}

	void SetReminder(){
		_groupService->SetReminder(40000665, 1, 1, 551551);
	}
	void SetDoNotDisturb(){
		_groupService->SetDoNotDisturb(40000501, 1);
	}

	void GetDocumentList(){
		_groupService->GetDocumentList(40000501);
	}

	void GetDocumentInfo(){
		_groupService->GetDocumentInfo(62000061, 4573);
	}

	void UploadDocument(){
	//	_groupService->UploadDocument(40000663, "123213.doc", 123124124, "123123123\4124124\123213.doc", uc::Time::GetSysTime());
	}

	void ShareDocument(){
	//	_groupService->ShareDocument(40000501, "123213.doc", 123124124, "123123123\4124124\123213.doc", 123124);
	}
	void DeleteDocument(){
		_groupService->DeleteDocument(40000501, 4577);
	}

	void SendText(){

 		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		std::string wstrData = "测试";
 		_groupService->SendTextMessage(PROJECT_CHAT ,cid, 0x01, "", wstrData, msgId);
	}

	void SendImage(){

		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		std::string imageName = "123123.png";
		int32_t imageSize = 1233;
		std::string imageUrl = "123123.png";
		std::string imageData = "";
		_groupService->SendImageMessage(PROJECT_CHAT ,cid, imageName, imageSize, imageUrl, imageData, msgId);
	}

	void SendAudio(){

		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		std::string audioName = "123123.mp3";
		int32_t audioSize = 1233;
		std::string audioUrl = "123123.mp3";
		int32_t audioDuration = 331;
		_groupService->SendAudioMessage(PROJECT_CHAT ,cid, audioName, audioSize, audioUrl, audioDuration, msgId);
	}

	void Sendvideo(){

		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		std::string videoName = "123123.wma";
		int32_t videoSize = 1233;
		std::string videoUrl = "123123.wma";
		std::string videoData = "";
		int32_t videoDuration = 331;
		_groupService->SendVideoMessage(PROJECT_CHAT ,cid, videoName, videoSize, videoUrl, videoData, videoDuration, msgId);
	}

	void SendInvateCall(){

		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		std::string confId = "123123515212312";
		std::string confPwd = "12312fssfasf";
		int32_t hostId = 123123;
		_groupService->SendInviteCallMessage(PROJECT_CHAT ,cid, confId, confPwd, hostId, msgId);
	}

	void SendAppectCall(){

		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		std::string confId = "123123515212312";
		_groupService->SendAcceptCallMessage(PROJECT_CHAT ,cid, confId, msgId);
	}

	void SendRejectCall(){

		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		std::string confId = "123123515212312";
		_groupService->SendRejectCallMessage(PROJECT_CHAT ,cid, confId, msgId);
	}
	
	void SendFinishCall(){

		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		std::string confId = "123123515212312";
		_groupService->SendFinishCallMessage(PROJECT_CHAT ,cid, confId, msgId);
	}

	void SendConnectCallRecord(){

		UCID cid(40000665, 71640, 0);
		int32_t msgId = 0;
		int32_t duration = 123;
		_groupService->SendConnectedCallRecordMessage(PROJECT_CHAT ,cid, duration, msgId);
	}

	void SendMessageReaded(){

		UCID cid(40000665, 71640, 0);
		_groupService->SendMessageRead(PROJECT_CHAT ,cid, 1231234, 51525);
	}

	void SendMessagesReaded(){

		UCID cid(40000665, 71640, 0);
		vector<int32_t> chatId;
		chatId.push_back(123123);
		chatId.push_back(33123);
		_groupService->SendMessagesRead(40000665,  chatId );
	}

	void SendMessagesPalyed(){

		UCID cid(40000665, 71640, 0);
		_groupService->SendMessagePlayed(PROJECT_CHAT ,cid, 1234, 4124);
	} 


	/**
	* 创建讨论组回调函数;
	* @param operatorId		 操作者ID;
	* @param info            讨论组信息;
	*/
	virtual void OnGroupCreated(int32_t result, int32_t operatorId, const GroupInfo& info) {
		cout << "UCMockGroupChat::OnGroupCreated" << endl;
		cout << "result:" << result << endl;
		if (0 == result)
		{
			cout << "GroupId:" << info.GetId() << endl;
			cout << "GroupName:" << info.GetName() << endl;
			cout << "PinyinName:" << info.GetPinYinName() << endl;
			cout << "MemberCount:" << info.GetMemberCount() << endl;
			cout << "AdminId:" << info.GetAdminId() << endl;
			cout << "CreateTime:" << info.GetCreateTime() << endl;
			cout << "LogoLevel:" << info.GetLogoLevel() << endl;
			cout << "AvatarIndex:" << info.GetAvatarIndex() << endl;
			cout << "GroupType:" << info.GetGroupType() << endl;
			cout << "SiteId:" << info.GetSiteId() << endl;
			cout << "GroupTop:" << info.GetGroupTop() << endl;
			cout << "GroupDisturb:" << info.GetGroupDisturb() << endl;
			cout << "GroupAlert:" << info.GetGroupAlert() << endl;
			cout << "AlertRule:" << info.GetAlertRule() << endl;
			cout << "AlertTime:" << info.GetAlertTime() << endl;
			cout << "JoinState:" << info.GetJoinState() << endl;
			cout << "GroupStatus:" << info.GetGroupStatus() << endl;
			MemberList list = info.GetMember();
			for (MemberList::iterator iter = list.begin(); iter != list.end(); ++iter)
			{
				const MemberInfo &memberInfo = *iter;
				cout <<"userId:"  << memberInfo.userId << endl;
				cout <<"joinTime:"  << memberInfo.joinTime << endl;
			}
		}
		
	}

	/**
	* 退出讨论组回调函数;
	* @param groupId		 讨论组ID;
	* @param userId  		 退出成员的ID;
	*/
	virtual void OnGroupExited(int32_t result, int64_t groupId, int32_t userId) {
		cout << "UCMockGroupChat::OnGroupExited" << endl;
		cout << "result:" << result << endl;
		cout << "GroupId:" << groupId << endl;
		cout << "userId:" << userId << endl;
	}

	/**
	* 结束讨论组回调函数;
	* @param
	* @param
	*/
	virtual void OnGroupEnded(int32_t result, int64_t groupId, int32_t operatorId) {
		cout << "UCMockGroupChat::OnGroupEnded" << endl;
		cout << "result:" << result << endl;
		cout << "GroupId:" << groupId << endl;
		cout << "operatorId:" << operatorId << endl;
	}

	/**
	* 讨论组列表回调函数;
	* @param vcinfo			 讨论组列表;
	*/
	virtual void OnGroupListReceived(int32_t result, GroupStyle groupType, const std::list<GroupInfo>& groupList) {
		cout << "UCMockGroupChat::OnGroupListReceived" << endl;
		cout << "result:" << result << endl;
		if (0 == result)
		{
			cout << "groupType:" << groupType << endl;
			cout << "count" << groupList.size() << endl;
			std::list<GroupInfo>::const_iterator it = groupList.begin();
			for (; it != groupList.end(); ++it)	{
				const GroupInfo& info = *it;
				cout << "================================================Begin" << endl;
				cout << "GroupId:" << info.GetId() << endl;
				cout << "GroupName:" << info.GetName() << endl;
				cout << "PinyinName:" << info.GetPinYinName() << endl;
				cout << "MemberCount:" << info.GetMemberCount() << endl;
				cout << "AdminId:" << info.GetAdminId() << endl;
				cout << "CreateTime:" << info.GetCreateTime() << endl;
				cout << "LogoLevel:" << info.GetLogoLevel() << endl;
				cout << "AvatarIndex:" << info.GetAvatarIndex() << endl;
				cout << "GroupType:" << info.GetGroupType() << endl;
				cout << "SiteId:" << info.GetSiteId() << endl;
				cout << "GroupTop:" << info.GetGroupTop() << endl;
				cout << "GroupDisturb:" << info.GetGroupDisturb() << endl;
				cout << "GroupAlert:" << info.GetGroupAlert() << endl;
				cout << "AlertRule:" << info.GetAlertRule() << endl;
				cout << "AlertTime:" << info.GetAlertTime() << endl;
				cout << "JoinState:" << info.GetJoinState() << endl;
				cout << "GroupStatus:" << info.GetGroupStatus() << endl;
				MemberList list = info.GetMember();
				for (MemberList::iterator iter = list.begin(); iter != list.end(); ++iter)
				{
					const MemberInfo& memberInfo = *iter;
					cout <<"userId:"  << memberInfo.userId << endl;
					cout <<"joinTime:"  << memberInfo.joinTime << endl;
				}
				cout << "================================================End" << endl;
			}
		}
	}

	/**
	* 讨论组信息回调函数;
	* @param groupId		 讨论组ID;
	* @param info  		     讨论组信息;
	*/
	virtual void OnGroupInfoReceived(int32_t result, int64_t groupId, const GroupInfo& info) {
		cout << "UCMockGroupChat::OnGroupInfoReceived" << endl;
		cout << "result:" << result << endl;
		if (0 == result)
		{
			cout << "GroupId:" << info.GetId() << endl;
			cout << "GroupName:" << info.GetName() << endl;
			cout << "PinyinName:" << info.GetPinYinName() << endl;
			cout << "MemberCount:" << info.GetMemberCount() << endl;
			cout << "AdminId:" << info.GetAdminId() << endl;
			cout << "CreateTime:" << info.GetCreateTime() << endl;
			cout << "LogoLevel:" << info.GetLogoLevel() << endl;
			cout << "AvatarIndex:" << info.GetAvatarIndex() << endl;
			cout << "GroupType:" << info.GetGroupType() << endl;
			cout << "SiteId:" << info.GetSiteId() << endl;
			cout << "GroupTop:" << info.GetGroupTop() << endl;
			cout << "GroupDisturb:" << info.GetGroupDisturb() << endl;
			cout << "GroupAlert:" << info.GetGroupAlert() << endl;
			cout << "AlertRule:" << info.GetAlertRule() << endl;
			cout << "AlertTime:" << info.GetAlertTime() << endl;
			cout << "JoinState:" << info.GetJoinState() << endl;
			cout << "GroupStatus:" << info.GetGroupStatus() << endl;
			MemberList list = info.GetMember();
			for (MemberList::iterator iter = list.begin(); iter != list.end(); ++iter)
			{
				const MemberInfo& memberInfo = *iter;
				cout <<"userId:"  << memberInfo.userId << endl;
				cout <<"joinTime:"  << memberInfo.joinTime << endl;
			}
		}
	}

	/**
	* 讨论组成员增加回调函数;
	* @param
	* @param
	*/
	virtual void OnMemberAdded(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberList) {
		cout << "UCMockGroupChat::OnMemeberAdded" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			for(unsigned int i = 0; i < memberList.size(); ++i){
				cout << "userId:" << memberList[i] << endl;
			}
		}
	}

	/**
	* 讨论组成员减少回调函数;
	* @param
	* @param
	*/
	virtual void OnMemberRemoved(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberList) {
		cout << "UCMockGroupChat::OnMemberRemoved" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			for(unsigned int i = 0; i < memberList.size(); ++i){
				cout << "userId:" << memberList[i] << endl;
			}
		}
	}

	/**
	* 讨论组名称更新回调函数;
	* @param groupId         讨论组ID;
	* @param operatorId      操作者ID(如果为零，不提示名字变化);
	* @param groupName       新讨论组名称;
	* @param pinYinName      新讨论组拼音;
	*/
	virtual void OnGroupNameUpdated(int32_t result, int64_t groupId, int32_t operatorId, const std::string& groupName, const std::string& pinYinName) {
		cout << "UCMockGroupChat::OnGroupNameUpdated" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
			cout << "operatorId:" << operatorId << endl;
			cout << "groupName:" << groupName << endl;
			cout << "pinYinName:" << pinYinName << endl;
		}
	}

	/**
	* 讨论组Logo更新回调函数;
	* @param groupId         讨论组ID;
	* @param operatorId      操作者ID;
	* @param logoURL         logo路径;
	*/
	virtual void OnGroupLogoUpdated(int32_t result, int64_t groupId, const std::string& logoURL) {
		cout << "UCMockGroupChat::OnGroupLogoUpdated" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
			cout << "logoURL:" << logoURL << endl;
		}
	}

	/**
	* 讨论组免打扰设置回调函数;
	* @param operatorId      操作者ID;
	* @param isDisturb       是否免打扰;
	*/
	virtual void OnDisturbSet(int32_t result, int64_t groupId, int32_t isDisturb) {
		cout << "UCMockGroupChat::OnDisturbSet" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
			cout << "isDisturb:" << isDisturb << endl;
		}
	}

	/**
	* 讨论组设置回调函数;
	* @param operatorId      操作者ID;
	* @param isAlert         是否提醒;
	* @param remindRule      提醒规则;
	* @param remindTime      提醒时间;
	*/
	virtual void OnReminderSet(int32_t result, int64_t groupId, int32_t isAlert, int32_t remindRule, int32_t remindTime){
		cout << "UCMockGroupChat::OnRemindSet" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
			cout << "isAlert:" << isAlert << endl;
			cout << "remindRule:" << remindRule << endl;
			cout << "remindTime:" << remindTime << endl;
		}
	}

	virtual void OnCommentCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const CommentInfo& commentInfo) {
	}

	virtual void OnCommentListReceived(int32_t result, int64_t groupId, int64_t fileId, const std::list<CommentInfo>& info) {

	}

	virtual void OnCommentCreated(int32_t result, int64_t groupId, const CommentInfo& info) {

	}

	/**
	* 讨论组置顶显示回调函数;
	* @param result  		 错误信息;
	* @param operatorId      操作者ID;
	* @param isDisturb       是否置顶;
	*/
	virtual void OnTopChatSet(int32_t result, int64_t groupId, int32_t isTop) {
		cout << "UCMockGroupChat::OnTopChatSet" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
			cout << "isTop:" << isTop << endl;
		}
	}

	/**
	* 讨论组文档列表回调函数;
	* @param groupId		 讨论组ID;
	* @param vcInfo  		 讨论组文档列表;
	*/
	virtual void OnDocListReceived(int32_t result, int64_t groupId, const std::list<DocumentInfo>& info) {
		cout << "UCMockGroupChat::OnDocListReceived" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
		}
	}

	/**
	* 讨论组文档信息回调函数;
	* @param groupId		 讨论组ID;
	* @param vcInfo  		 讨论组文档列表;
	*/
	virtual void OnDocInfoReceived(int32_t result, int64_t groupId, const DocumentInfo& info) {
		cout << "UCMockGroupChat::OnDocInfoReceived" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
		}
	}

	/**
	* 上传讨论组文档响应事件;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param operatorId		 操作者ID;
	* @param DocumentInfo	 文档信息;
	*/
	virtual void OnDocumentUploaded(int32_t result, int64_t groupId, int32_t operatorId, const DocumentInfo& info){
		cout << "UCMockGroupChat::OnDocumentUploaded" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
		}
	}

	/**
	* 分享讨论组文档响应事件;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param operatorId		 操作者ID;
	* @param DocumentInfo	 文档信息;
	*/
	virtual void OnDocumentShared(int32_t result, int64_t groupId, int32_t operatorId, const DocumentInfo& info){
		cout << "UCMockGroupChat::OnDocumentShared" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
		}
	}

	/**
	* 删除讨论组文档响应事件sefdD;
	* @param result  		 错误信息;
	* @param groupId		 讨论组ID;
	* @param operatorId		 操作者ID;
	* @param docName         文档名称;
	* @param fileId          文档ID;
	*/
	virtual void OnDocumentDeleted(int32_t result, int64_t groupId, int32_t operatorId, 
		const std::string& docName, int64_t fileId){
		cout << "UCMockGroupChat::OnDocumentDeleted" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
			cout << "operatorId:" << operatorId << endl;
			cout << "docName:" << docName << endl;
			cout << "fileId:" << fileId << endl;
		}
	}
	/**
	* 接收消息已读消息;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;k
	* @param isAll           消息已读类型:0 单条已读/1 批量已读;
	* @param msgId           本地消息ID（单条已读时有效）;
	* @param msgSeq          服务器消息ID（单条已读时有效）;
	*/
	virtual void OnMessageRead(int32_t result, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq) {
		cout << "UCMockGroupChat::OnMessageRead" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
		}
	}

	
	/**
	* 聊天历史列表;
	* @param groupId	   项目组ID/群ID;
	* @param vcInfo		   聊天内容列表;
	*/
	virtual void OnChatHistoryReceived(int32_t result, ConversationType type, int64_t groupId) {
		UC_LOG(INFO) << "UCMockGroupChat::OnChatHistoryReceived";
	}

	/**
	 * 消息发送结果;
	 * @param errorCode	   错误码;
	 * @param groupId	   项目组ID/群ID;
	 * @param msgId        消息ID;
	 */
	virtual void OnMessageSent(int32_t result, ConversationType type, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
		cout << "UCMockGroupChat::OnMessageSent" << endl;
		cout << "result:" << result << endl;
		if (0 == result){
			cout << "groupId:" << groupId << endl;
			cout << "msgId:" << msgId << endl;
			cout << "msgSeq:" << msgSeq << endl;
		}
	}

	/**
	* 接收文本聊天消息;
	*/
	virtual void OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message) {
		cout << "=======UCMockGroupChat::OnTextMessageReceived========" << endl;
		cout << "Channel: " << channel << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "TextType: " << message->GetTextType() << endl;
		cout << "Content: " << message->GetTextContent() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;
		cout << "==================================================" << endl;
	}

	/**
	* 接收图片消息;
	*/
	virtual void OnImageMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {

		cout << "=====UCMockGroupChat::OnImageMessageReceived==============" << endl;
		cout << "Channel: " << channel << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;

		cout << "imageName: " << message->GetFileName() << endl;
		cout << "imageSize: " << message->GetFileSize() << endl;
		cout << "imageURL: " << message->GetFileUrl() << endl;
		cout << "imageData: " << message->GetThumbnails() << endl;
		cout << "========================================================" << endl;

	}

	/**
	* 接收音频消息;;
	*/
	virtual void OnAudioMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
		cout << "=====UCMockGroupChat::OnAudioMessageReceived==============" << endl;
		cout << "Channel: " << channel << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;

		cout << "audioName: " << message->GetFileName() << endl;
		cout << "audioSize: " << message->GetFileSize() << endl;
		cout << "audioURL: " << message->GetFileUrl() << endl;
		cout << "audioDuration: " << message->GetDuration() << endl;
		cout << "========================================================" << endl;

	}

	/**
	* 接收视频消息;
	*/
	virtual void OnVideoMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
		cout << "=====UCMockGroupChat::OnAudioMessageReceived==============" << endl;
		cout << "Channel: " << channel << endl;
		cout << "From: " << message->GetFrom().GetUserId()
				<< "_" << message->GetFrom().GetResId()
				<< "_" << message->GetFrom().GetSiteId() << endl;

		cout << "To: " << message->GetTo().GetUserId()
				<< "_" << message->GetTo().GetResId()
				<< "_" << message->GetTo().GetSiteId() << endl;

		cout << "MsgId: "  << message->GetMsgId() << endl;
		cout << "MsgSeq: "  << message->GetMsgSeq() << endl;
		cout << "SendTime: " << message->GetMsgTime() << endl;

		cout << "videoName: " << message->GetFileName() << endl;
		cout << "videoSize: " << message->GetFileSize() << endl;
		cout << "videoURL: " << message->GetFileUrl() << endl;
		cout << "videothumbnails: " << message->GetThumbnails() << endl;
		cout << "========================================================" << endl;

	}

	/**
	* 接收消息已播放消息;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param msgId           本地消息ID;
	* @param msgSeq          服务器消息ID;
	*/
	virtual void OnMessagePlayed(int32_t result, int64_t groupId, int32_t msgId, int64_t msgSeq) {
		cout << "UCMockGroupChat::OnMessagePlayed" << endl;
		cout << "result:" << result << endl;
	}

	virtual void OnGroupCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const GroupInfo& group) {
		cout << "UCMockGroupChat::OnGroupCreateMessageReceived" << endl;
	}

	virtual void OnGroupCloseMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId) {
		cout << "UCMockGroupChat::OnGroupCloseMessageReceived" << endl;
	}

	virtual void OnGroupMemberDelMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
				int32_t memberId, const std::string &avatar, const std::string &groupName, const std::string &namePinYin, int8_t nameFlag) {
		cout << "UCMockGroupChat::OnGroupMemberDelMessageReceived" << endl;
	}

	virtual void OnGroupMemberAddMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
				const std::vector<int32_t> &memberList, const std::string &avatar, const std::string &groupName,
				const std::string &namePinYin, int8_t nameFlag) {
		cout << "UCMockGroupChat::OnGroupMemberAddMessageReceived" << endl;
	}

	virtual void OnGroupMemberKickMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
				const std::vector<int32_t> &memberList, const std::string &avatar, const std::string &groupName, const std::string &namePinYin,
				int8_t nameFlag) {
		cout << "UCMockGroupChat::OnGroupMemberKickMessageReceived" << endl;
	}

	virtual void OnGroupNameUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
				const std::string &groupName, const std::string &namePinYin, int8_t nameFlag) {
		cout << "UCMockGroupChat::OnGroupNameUpdateMessageReceived" << endl;
	}

	virtual void OnGroupDisturbSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId, int8_t state ) {
		cout << "UCMockGroupChat::OnGroupDisturbSetMessageReceived" << endl;
	}

	virtual void OnGroupTopMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
				int8_t isTop) {
		cout << "UCMockGroupChat::OnGroupTopMessageReceived" << endl;
	}

	virtual void OnGroupAvatarUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
				const std::string &avatar, int32_t groupId) {
		cout << "UCMockGroupChat::OnGroupAvatarUpdateMessageReceived" << endl;
	}

	void OnGroupReminderSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId,
				int8_t state, int32_t rule, int32_t time) {
		cout << "UCMockGroupChat::OnGroupReminderSetMessageReceived" << endl;
	}

	virtual void OnDocumentUploadMessageReceived(MessageChannel channel, const MessageInfo &message, const DocumentInfo &doc) {
		cout << "UCMockGroupChat::OnDocumentUploadMessageReceived" << endl;
	}

	virtual void OnDocumentShareMessageReceived(MessageChannel channel, const MessageInfo &message, const DocumentInfo &doc) {
		cout << "UCMockGroupChat::OnDocumentShareMessageReceived" << endl;
	}

	virtual void OnDocumentDeleteMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
				int64_t contentId, const std::string &docName) {
		cout << "UCMockGroupChat::OnDocumentDeleteMessageReceived" << endl;
	}

	/**
	* 音频通话邀请响应事件;
	* @param result 	     	错误信息;
	* @param ConversationType   聊天类型;
	* @param groupId         	讨论组ID;
	* @param confId          	会议ID;
	* @param confPwd         	会议密码;
	* @param hostId          	主持人ID;
	*/
	virtual void OnCallInvited(int32_t result, ConversationType type, int64_t groupId, int32_t msgId, int64_t msgSeq, const std::string confId, const std::string confPwd, int32_t hostId) {
		cout << "UCMockGroupChat::OnCallInvited" << endl;
		cout << "result:" << result << endl;
	}

	virtual void OnCallAccepted(int32_t result, ConversationType type, int64_t groupId, const std::string confId) {
		cout << "UCMockGroupChat::OnCallAccepted" << endl;
		cout << "result:" << result << endl;
	}

	virtual void OnCallRejected(int32_t result, ConversationType type, int64_t groupId, const std::string confId) {
		cout << "UCMockGroupChat::OnCallRejected" << endl;
		cout << "result:" << result << endl;
	}

	virtual void OnCallStopped(int32_t result, ConversationType type, int64_t groupId, const std::string confId) {
		cout << "UCMockGroupChat::OnCallStopped" << endl;
		cout << "result:" << result << endl;
	}

	virtual void OnCallConnected(int32_t result, ConversationType type, int64_t groupId, int32_t duration) {
		cout << "UCMockGroupChat::OnCallConnected" << endl;
		cout << "result:" << result << endl;
	}
private:
	IGroupService *_groupService;
};

class UCMockCalendar : public ICalendarServiceEvent {
public:
	UCMockCalendar() {
		_summaryId = 0;
		_calendarService = NULL;
	}
	virtual ~UCMockCalendar() {

	}

private:
	ConferenceInfo _conferenceInfo;
	int32_t _summaryId;

public:
	bool Init() {
		_calendarService = (ICalendarService*)g_ucEngine->QueryService(UCCalendarService);
		assert(_calendarService);
		_calendarService->registerListener(this);
		return true;
	}

	bool UnInit() {
		_calendarService->removeListener(this);
		return true;
	}

	/**
	* 创建会议结果的返回事件
	* @param errorCode				错误码
	* @param conferenceId			会议ID
	* @param info					会议信息
	* @return
	*/
	virtual void OnConferenceCreated( int32_t errorCode, ConferenceInfo& info) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceCreated";
		if (errorCode == 0) {
			UC_LOG(INFO) << "UCMockCalendar::OnConferenceCreated successed!";
			_conferenceInfo = info;
		}else {
			UC_LOG(ERROR) << "UCMockCalendar::OnConferenceCreated  create conference failed!"<<"  errorCode= " << errorCode;
		}
	}


	/**
	* 更新会议结果的返回事件
	* @param errorCode				错误码
	* @param info					会议信息
	*/
	virtual void OnConferenceUpdated(int32_t errorCode, int64_t eventId, int64_t startTime, ConferenceInfo& info) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceUpdated";
		if (errorCode == 0) {
			UC_LOG(INFO) << "UCMockCalendar::OnConferenceUpdated successed!";
			_conferenceInfo = info;
		}else {
			UC_LOG(ERROR) << "UCMockCalendar::OnConferenceUpdated  update conference failed!"<<"  errorCode= " << errorCode;
		}
	}

	/**
	* 取消会议结果的返回事件
	* @param errorCode				错误码
	* @param conferenceId			会议ID
	* @param eventId				日程事件ID
	* @param startTime				会议开始时间，UTC Unix时间戳
	* @param authorizerId			授权人ID
	*/
	virtual void OnConferenceCanceled(int32_t errorCode, int32_t conferenceId, int64_t eventId,  int64_t startTime, int32_t authorizerId = 0) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceCanceled" << ",  errorCode = " << errorCode << ",  conferenceId =" << conferenceId;
	}


	/**
	* 获取会议调起串结果的返回事件
	* @param errorCode				错误码
	* @param eventId				日程事件ID
	* @param startUrl				调起串
	*/
	virtual void OnConferenceStartUrlReceived(int32_t errorCode, int64_t eventId, const std::string& startUrl) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceStartUrlReceived"<<",  errorCode = " << errorCode << ",  eventId =" << eventId;
	}


	/**
	* 通过邮件获取会议调起串结果的返回事件
	* @param errorCode				错误码
	* @param eventId				日程事件ID
	* @param startUrl				调起串
	*/
	virtual void OnConferenceWebStartUrlReceived(int32_t errorCode, int64_t eventId, const std::string& startUrl) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceWebStartUrlReceived"<<",  errorCode = " << errorCode << ",  eventId =" << eventId;
	}


	/**
	* 更新授权调协结果的返回事件
	* @param errorCode				错误码
	* @param errorMsg				错误描述
	* @param authorizedPersonId		授权或取消授权的人的ID
	* @param status					授权状态 1：授权, 0：取消授权
	*/
	virtual void OnAccreditSettingUpdated(int32_t errorCode = -1, int32_t authorizedPersonId = 0, int32_t status = 1) {
		UC_LOG(INFO) << "UCMockCalendar::OnAccreditSettingUpdated";
	}


	/**
	* 获取授权用户列表结果的返回事件
	* @param errorCode				错误码
	* @param accreditUserList		日程共享用户列表
	*/
	virtual void OnAccreditUserListReceived(int32_t errorCode, std::vector<AccreditInfo>& accreditUserList) {
		UC_LOG(INFO) << "UCMockCalendar::OnAccreditUserListReceived";
	}


	/**
	* 获取会议列表结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param infos					会议信息列表
	*/
	virtual void OnConferenceListReceived(int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int32_t page, std::vector<ConferenceInfo>& infos) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceListReceived";
	}


	/**
	* 获取会议详情结果的返回事件
	* @param errorCode				错误码
	* @param info					会议信息
	*/
	virtual void OnConferenceInfoReceivedById(int32_t errorCode,int32_t conferenceId, std::vector<ConferenceInfo>& infos) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceInfoReceivedById";
	}


	/**
	* 批量获取会议详情结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param infos					会议信息列表
	*/
	virtual void OnConferenceInfoReceivedByIds(int32_t errorCode, int32_t requesetId, std::vector<ConferenceInfo>& infos) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceInfoReceivedByIds";
	}


	/**
	* 接受会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param info					会议信息
	*/
	virtual void OnConferenceInvitationAccepted(int32_t errorCode, ConferenceInfo& info) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceInvitationAccepted";
	}


	/**
	* 拒绝会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param info					会议信息
	*/
	virtual void OnConferenceInvitationDenied(int32_t errorCode, ConferenceInfo& info) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceInvitationDenied";
	}


	/**
	* 转发会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param info					会议信息
	*/
	virtual void OnConferenceForwarded(int32_t errorCode, ConferenceInfo& info) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceForwarded";
	}

	/**
	* 获取用户日程列表结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param schedules				日程信息列表 
	*/
	virtual void OnUserScheduleListReceived(int32_t errorCode, int32_t requestId, std::vector<UserSchedule>& schedules) {
		UC_LOG(INFO) << "UCMockCalendar::OnUserScheduleListReceived";
	}


	/**
	* 创建会议纪要结果的返回事件
	* @param errorCode				错误码
	* @param conferenceId			会议ID
	* @param summaryId				会议纪要ID
	* @param creatorId				创建人ID
	* @param conferenceName			会议名称
	* @param summaryName			会议纪要名称
	* @param startTime				会议开始时间
	* @param createTime				会议纪要发布时间
	*/
	virtual void OnConferenceSummaryCreated(
											int32_t errorCode,
											int32_t conferenceId,
											int32_t summaryId,
											int32_t creatorId,
											const std::string& conferenceName,
											const std::string& summaryName,
											int64_t startTime,
											int64_t createTime
											) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceSummaryCreated";
		if (errorCode == 0) {
			_summaryId = summaryId;
		} else {
			UC_LOG(ERROR) << "UCMockCalendar::OnConferenceSummaryCreated Failed, errorCode = "<< errorCode;
		}
		
	}

	/**
	* 更新会议纪要结果的返回事件
	* @param errorCode				错误码
	* @param conferenceId			会议ID
	* @param summaryId				会议纪要ID
	* @param operatorId				操作人ID
	* @param conferenceName			会议名称
	* @param summaryName			会议纪要名称
	* @param startTime				会议开始时间
	* @param updateTime				会议纪要更新时间
	*/
	virtual void OnConferenceSummaryUpdated(
											int32_t errorCode,
											int32_t conferenceId,
											int32_t summaryId,
											int32_t operatorId,
											const std::string& conferenceName,
											const std::string& summaryName,
											int64_t startTime,
											int64_t updateTime
											) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceSummaryUpdated, errorCode = "<< errorCode;
	}

	/**
	* 获取会议纪要详情结果的返回事件
	* @param errorCode				错误码
	* @param conferenceId			会议ID	
	* @param summaryId				会议纪要ID
	* @param groupId				讨论组ID
	* @param createTime				会议纪要创建时间
	* @param summary				会议纪要讨论的问题
	* @param startTime				会议开始时间
	* @param location				会议地点
	*/
	virtual void OnConferenceSummaryInfoReceived(
												int32_t errorCode = -1,
												int32_t conferenceId = 0,
												int32_t summaryId = 0,
												int32_t creatorId = 0,
												int64_t groupId = 0,
												int32_t createTime = 0,
												const std::string& summary = "",
												int32_t startTime = 0,
												const std::string& location = ""
												) {
		UC_LOG(INFO) << "UCMockCalendar::OnConferenceSummaryInfoReceived, errorCode = "<< errorCode;
	}

	/**
	* 根据会议ID获取会议纪要列表结果的返回事件
	* @param errorCode				错误码
	* @param conferenceId			会议ID	
	* @param summaryIds				会议纪要ID列表
	*/
	virtual void OnSummaryListReceivedByConferenceId(
													int32_t errorCode,
													int32_t conferenceId,
													std::vector<int32_t> summaryIds
													) {
		UC_LOG(INFO) << "UCMockCalendar::OnSummaryListReceivedByConferenceId, errorCode = "<< errorCode;
	}

private:
	ICalendarService *_calendarService;

public:
	int32_t CreateConference() {
		//参会人
		std::vector<Attendee> attendees;
		Attendee attendee;
		attendee.SetAccount("1228650");
		attendees.push_back(attendee);
		attendee.SetAccount("1189278");
		attendees.push_back(attendee);
		attendee.SetAccount("399610");
		attendees.push_back(attendee);
		attendee.SetAccount("217705");
		attendees.push_back(attendee);
		//会议标题
		std::string title = "这是会议title";
		//会议地点
		std::string location = "这是会议location";
		//会议摘要
		std::string summary = "这是会议summary";
		//时区
		std::string timezone = "8";
		int64_t startTime =1436995000;
		int32_t length = 60;
		return _calendarService->CreateConference(attendees, title, location, summary, timezone, startTime, length, 1, 0);
	}

	int32_t UpdateConference() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		int64_t eventId = _conferenceInfo.GetEventId();
		//参会人
		std::vector<Attendee> attendees = _conferenceInfo.GetAttendees();
		//会议标题
		std::string title = "这是会议title2";
		//会议地点
		std::string location = "这是会议location2";
		//会议摘要
		std::string summary = "这是会议summary2";
		//时区
		std::string timezone = "8";
		int64_t startTime = 1435695000;
		int64_t oriStart = 1435695000;
		int32_t length = 120;
		return _calendarService->UpdateConference(conferenceId, eventId, oriStart, attendees, title, location, summary, timezone, startTime, length, 1, 0);
	}


	int32_t ForwardConference() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		int64_t eventId = _conferenceInfo.GetEventId();
		std::vector<Attendee> attendees = _conferenceInfo.GetAttendees();
		Attendee attendee;
		attendee.SetAccount("399610");
		attendees.push_back(attendee);
		return _calendarService->ForwardConference(conferenceId, eventId, attendees, 0);
	}

	int32_t CancelConference() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		int64_t eventId = _conferenceInfo.GetEventId();
		return _calendarService->CancelConference(conferenceId, eventId);
	}
	int32_t GetConferenceStartUrl() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		std::string password = _conferenceInfo.GetAttendeePassword();
		return _calendarService->GetConferenceStartUrl(conferenceId, password);
	}

	int32_t GetConferenceWebStartUrl() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		std::string mail = "abcdef@qq.com";
		std::string password = _conferenceInfo.GetAttendeePassword();
		std::string displayName = "abcdef";
		return _calendarService->GetConferenceWebStartUrl(conferenceId, mail, password, displayName);
	}

	int32_t UpdateAccreditSetting() {
		int32_t authorizedPersonId = 1189278;
		int32_t status  = 1;
		return _calendarService->UpdateAccreditSetting(authorizedPersonId, status);
	}

	int32_t GetAccreditUserList() {
		vector<AccreditInfo> infos;
		return _calendarService->GetAccreditUserList(infos);
	}

	int32_t GetConferenceList() {
		int32_t authorizedId = 1189278;
		int64_t startTime  = 1435615000;
		int64_t endTime = 1437995000;
		int32_t requestId = 0;
		int32_t limit = 0;
		int32_t page = 0;
		int32_t refresh = 0;
		vector<ConferenceInfo> infos;
		return _calendarService->GetConferenceList(authorizedId, startTime, endTime, limit, page, refresh, requestId, infos);
	}

	int32_t AcceptConferenceInvitation() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		int64_t eventId = _conferenceInfo.GetEventId();
		return _calendarService->AcceptConferenceInvitation(conferenceId, eventId, 0);
	}

	int32_t DenyConferenceInvitation() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		int64_t eventId = _conferenceInfo.GetEventId();
		return _calendarService->DenyConferenceInvitation(conferenceId, eventId, 0);
	}
	
	int32_t GetConferenceInfoById() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		int64_t startTime  = 1435615000;
		int64_t endTime = 1435795000;
		vector<ConferenceInfo> infos;
		return _calendarService->GetConferenceInfoById(conferenceId, startTime, endTime, 0, infos);
	}

	int32_t GetConferenceInfoByIds() {
		vector<int64_t> eventIds;
		eventIds.push_back(_conferenceInfo.GetEventId());
		eventIds.push_back(456789);
		int32_t requestId = 0;
		vector<ConferenceInfo> infos;
		return _calendarService->GetConferenceInfoByIds(eventIds, 0,0,0, requestId,infos);
	}

	int32_t GetUserScheduleList() {
		std::vector<int32_t> userIdList;
		userIdList.push_back(1189278);
		userIdList.push_back(217705);
		int64_t startTime = 1435615000;
		int64_t endTime = 1435795000;
		int32_t timeInterval = 5;
		int32_t requestId = 0;
		return _calendarService->GetUserScheduleList(userIdList, startTime, endTime, timeInterval, requestId);
	}

	int32_t CreateConferenceSummary() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId(); 
		int64_t groupId = 456789; 
		int64_t startTime = 1435615000;
		std::string conferenceLocation = "xian";
		std::vector<int32_t> attendees;
		attendees.push_back(1189278);
		attendees.push_back(217705);
		std::string summary = "summary : do something";
		int32_t status  = 1;
		return _calendarService->CreateConferenceSummary(conferenceId, groupId, startTime, conferenceLocation, attendees, summary, status);
	}

	int32_t UpdateConferenceSummary() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		int32_t summaryId = _summaryId;
		int64_t groupId = 456789; 
		int64_t startTime = 1435615000;
		std::string conferenceLocation = "xian2";
		std::vector<Attendee> attendees;
		Attendee attendee;
		attendee.SetAccount("1189278");
		attendees.push_back(attendee);
		attendee.SetAccount("217705");
		attendees.push_back(attendee);
		std::string summary = "summary : do something2";
		int32_t status  = 1;
		return _calendarService->UpdateConferenceSummary(conferenceId, summaryId, groupId, startTime, summary, status);
	}


	int32_t GetConferenceSummaryInfo() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		int32_t summaryId = _summaryId;
		return _calendarService->GetConferenceSummaryInfo(conferenceId, summaryId);
	}

	int32_t GetSummaryListByConferenceId() {
		int32_t conferenceId = _conferenceInfo.GetConferneceId();
		return _calendarService->GetSummaryListByConferenceId(conferenceId);
	}
	
};

class UCMockContact : public IContactServiceEvent {
public:
	UCMockContact() {
		_contactService = NULL;
	}

	virtual ~UCMockContact() {

	}

	bool Init() {
		_contactService = (IContactService*)g_ucEngine->QueryService(UCContactService);
		_contactService->registerListener(this);
		return true;
	}

	bool UnInit() {
		_contactService->removeListener(this);
		return true;
	}

	/**
	 * 登陆用户或常用联系人状态变更
	 * @param userId 联系人用户id
	 * @param status ，状态
	 */
	virtual void OnContactPresenceReceived(int32_t userId, ContactInfo::Status status) {
		UC_LOG(INFO) << "UCMockContact::OnContactPresenceReceived";
	}

	/**
	 * 获取联系人信息的回调，
	 * @param contacts 联系人信息列表。该容器只保证在OnContactInfo作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnContactInfoReceived(int32_t result, const std::list<int32_t>* userIds, const std::list<ContactInfo*>* contacts) {
		UC_LOG(INFO) << "UCMockContact::OnContactInfoReceived";
	}

	/**
	 * 获取联系人基本信息的回调，
	 * @param contacts 联系人信息列表。该容器只保证在OnContactInfo作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnBasicContactInfoReceived(int32_t result, const std::list<int32_t>* userIds, const std::list<BasicContactInfo>* contacts) {
		UC_LOG(INFO) << "UCMockContact::OnBasicContactInfoReceived";
	}

	/**
	 * 添加常用联系人的回调，
	 * @param result 操作的结果
	 * @param userId 联系人的用户ID
	 * 勿在外部引用
	 */
	virtual void OnContactAdded(int32_t result, int32_t userId) {
		UC_LOG(INFO) << "UCMockContact::OnContactAdded";
	}

	/**
	 * 删除常用联系人的回调，触发场景：（1）在contact/inclist接口响应中，有cantact被delete；
	 * （2）用户主动调删除常用联系人接口；（3）ucas中接收到其他终端删除常用联系人的消息
	 * @param result 操作的结果
	 * @param userId 联系人的用户ID
	 */
	virtual void OnContactDeleted(int32_t result, int32_t userId) {
		UC_LOG(INFO) << "UCMockContact::OnContactDeleted";
	}

	/**
	 * 更新常用联系人的回调，触发场景：（1）主动调contactservice的updateCommonContact接口；
	 * （2）处理ucas发来的常用联系人信息变更消息;
	 * @param contacts 联系人信息列表。该容器只保证在OnContactUpdated作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnContactUpdated(std::list<ContactInfo*> *contact) {
		UC_LOG(INFO) << "UCMockContact::OnContactUpdated";
	}

	/**
	 * 登陆用户信息修改的回调
	 * @param result 操作的结果
	 * @param updateField 更新的字段
	 */
	virtual void OnUserInfoUpdated(int32_t result, const ContactInfo *info) {
		UC_LOG(INFO) << "UCMockContact::OnUserInfoUpdated";
	}

	/**
	 * 更新常用联系人标签的回调
	 * @param result 操作的结果
	 * @param addedTags 新增加的标签
	 * @param updatedTags 更新的标签
	 * @param deletedTags 删除的标签
	 */
	virtual void OnContactTagsUpdated(int32_t result, std::list<ContactTag*> *addedTags, std::list<ContactTag*> *updatedTags,
			std::list<ContactTag*> *deletedTags) {
		UC_LOG(INFO) << "UCMockContact::OnContactTagsUpdated";
	}

	/**
	 * 用户标签更新的回调
	 * @param result 操作的结果
	 * @param updateField 更新的字段
	 */
	virtual void OnUserTagsUpdated(int32_t result, const std::list<ContactTag*>* fieldList) {
		UC_LOG(INFO) << "UCMockContact::OnUserTagsUpdated";
	}

	/**
	 * 后台管理员增加标签的回调
	 * @param tags 增加的标签
	 */
	virtual void OnSystemTagsAdded(std::vector<ContactTag*>* tags) {
		UC_LOG(INFO) << "UCMockContact::OnSystemTagsAdded";
	}

	/**
	 * 后台管理员修改标签的回调
	 * @param tags 更新的标签
	 */
	virtual void OnSystemTagsUpdated(std::vector<ContactTag*>* tags) {
		UC_LOG(INFO) << "UCMockContact::OnSystemTagsUpdated";
	}

	/**
	 * 后台管理员删除标签的回调
	 * @param tagIds 删除的标签
	 */
	virtual void OnSystemTagsDeleted(std::vector<int32_t>& tagIds) {
		UC_LOG(INFO) << "UCMockContact::OnSystemTagsDeleted";
	}

	/**
	 * 获取同事列表的回调，
	 * @param result 操作的结果
	 * @param userId 联系人的用户ID
	 * @param page 分页获取的页数
	 * @param colNum 同事数量
	 * @param leaderNum 上级数量
	 * @param colleagues 同事列表。该容器只保证在onColleagueReceived作用域中有效，
	 * 勿在外部引用
	 * @param leaders 上级列表。该容器只保证在onColleagueReceived作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnColleagueReceived(int32_t result, int32_t userId, int32_t page,
			int32_t colNum, int32_t leaderNum, std::list<ContactInfo*> *colleagues,
			std::list<ContactInfo*> *leaders) {
		UC_LOG(INFO) << "UCMockContact::OnColleagueReceived";
	}

	/**
	 * 获取组织通讯录的回调，
	 * @param result 操作的结果
	 * @param department 部门信息。只保证在onOrgAddrListReceived作用域中有效，
	 * 勿在外部引用
	 * @param myDepartment 用户自己的部门信息。只保证在onOrgAddrListReceived作用域中有效，
	 * 勿在外部引用
	 */
	virtual void OnOrgAddressBookReceived(int32_t result, DepartmentInfo *department, DepartmentInfo *myDepartment) {
		UC_LOG(INFO) << "UCMockContact::OnOrgAddressBookReceived";
	}

	virtual void OnOrgAddrMembersReceived(int32_t result, int orgId, int userCount,
	        std::vector<int32_t> *memberIds) {
		UC_LOG(INFO) << "UCMockContact::OnOrgAddrMembersReceived";
	}

	/**
	 * 部门名称变更的回调
	 * @param operatorId 操作发起人用户ID
	 * @param deptId 部门ID
	 * @param oldDeptName 原部门名称
	 * @param newDeptName 新部门名称
	 * @param desc 消息描述
	 */
	virtual void OnDeptUpdated(int32_t operatorId, int32_t deptId, const std::string &oldDeptName, const std::string &newDeptName,
			const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnDeptUpdated";
	}

	/**
	 * 员工部门调动的回调
	 * @param operatorId 操作发起人用户ID
	 * @param userId :用户ID
	 * @param userName 用户姓名
	 * @param deptId 新部门ID
	 * @param oldDeptName 旧部门名称
	 * @param newDeptName 新部门名称
	 * @param desc 消息描述
	 */
	virtual void OnDeptTransfered(int32_t operatorId, int32_t userId, const std::string &userName, int32_t deptId,
			const std::string &oldDeptName, const std::string &deptName, const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnDeptTransfered";
	}

	/**
	 * 员工职位调整的回调
	 * @param operatorId 操作发起人用户ID
	 * @param userId :用户ID
	 * @param userName 用户姓名
	 * @param newPosition 新职位名称
	 * @param oldPosition 旧职位名称
	 * @param deptName 职位所在部门名称
	 * @param desc 消息描述
	 */
	virtual void OnPositionUpdated(int32_t operatorId, int32_t userId, const std::string &userName, const std::string &newPosition,
			const std::string &oldPosition, const std::string& deptName, const std::string& desc) {
		UC_LOG(INFO) << "UCMockContact::OnPositionUpdated";
	}

	/**
	 * 员工入职的回调
	 * @param operatorId 操作发起人用户ID
	 * @param userId :员工用户ID
	 * @param deptId 入职部门ID
	 * @param userName 员工姓名
	 * @param deptName 员工部门名称
	 * @param position 员工职位名称
	 * @param desc 消息描述
	 */
	virtual void OnEmployeeEntry(int32_t operatorId, int32_t userId, int32_t deptId, const std::string &userName,
			const std::string &deptName, const std::string &position, const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnEmployeeEntry";
	}

	/**
	 * 员工离职的回调
	 * @param operatorId 操作发起人用户ID
	 * @param userId :员工用户ID
	 * @param userName 员工姓名
	 * @param deptName 员工部门名称
	 * @param desc 消息描述
	 */
	virtual void OnEmployeeLeaved(int32_t operatorId, int32_t userId, const std::string &userName, const std::string &deptName,
			const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnEmployeeLeaved";
	}

	/**
	 * 员工权限变更的回调
	 * @param operatorId 操作发起人用户ID
	 * @param userId :员工用户ID
	 * @param userName 员工姓名
	 * @param deptName 员工部门名称
	 * @param deptId 用户权限变更部门ID
	 * @param desc 消息描述
	 */
	virtual void OnEmployeeRightsUpdated(int32_t operatorId, int32_t userId, const std::string &userName,
			const std::string &deptName, int32_t deptId, const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnEmployeeRightsUpdated";
	}

	/**
	 * 部门整体移动的回调
	 * @param operatorId 操作发起人用户ID
	 * @param deptId 移动的部门ID
	 * @param deptName 移动的部门名称
	 * @param oldDeptName 原上级部门名称
	 * @param newDeptName 新上级部门名称
	 * @param desc 消息描述
	 */
	virtual void OnDeptParentChanged(int32_t operatorId, int32_t deptId, const std::string &deptName,
			const std::string &oldDeptName, const std::string &newDeptName, const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnDeptParentChanged";
	}

	/**
	 * 员工被指定为生态企业员工的回调
	 * @param operatorId 操作发起人用户ID
	 * @param companyId 生态企业ID
	 * @param userId 用户ID
	 * @param companyName 生态企业名称
	 * @param userName 用户姓名
	 * @param desc 消息描述
	 */
	virtual void OnCompanyJoined(int32_t operatorId, int32_t companyId, int32_t userId, const std::string &companyName,
			const std::string &userName, const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnCompanyJoined";
	}

	/**
	 * 部门删除的回调
	 * @param operatorId 操作发起人用户ID
	 * @param deptId 删除的部门ID
	 * @param deptName 删除的部门名称
	 * @param parentDeptName 所在父级部门名称
	 * @param desc 消息描述
	 */
	virtual void OnDeptDeleted(int32_t operatorId, int32_t deptId, const std::string &deptName, const std::string &parentDeptName,
			const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnDeptDeleted";
	}

	/**
	 * 管理员确认员工入职消息的回调
	 * @param operatorId 操作发起人用户ID
	 * @param deptId 入职部门ID
	 * @param userName 员工姓名
	 * @param deptName 员工部门名称
	 * @param userId 入职员工ID
	 * @param desc 消息描述
	 */
	virtual void OnEmployeeEntryConfirmed(int32_t operatorId, int32_t deptId, const std::string &userName, const std::string &deptName,
			int32_t userId, std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnEmployeeEntryConfirmed";
	}

	/**
	 * 管理员确认员工离职消息的回调
	 * @param operatorId 操作发起人用户ID
	 * @param userId 员工用户ID
	 * @param deptId 入职部门ID
	 * @param userName 员工姓名
	 * @param deptName 员工部门名称
	 * @param desc 消息描述
	 */
	virtual void OnEmployeeLeaveConfirmed(int32_t operatorId, int32_t userId, int32_t deptId, const std::string &userName,
			const std::string &deptName, const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnEmployeeLeaveConfirmed";
	}

	/**
	 * 管理员确认员工调岗(部门变更)消息的回调
	 * @param operatorId 操作发起人用户ID
	 * @param userId 员工用户ID
	 * @param userName 员工姓名
	 * @param deptName 员工部门名称
	 * @param desc 消息描述
	 */
	virtual void OnDeptTransferConfirmed(int32_t operatorId, int32_t userId, const std::string &userName, const std::string &deptName,
			const std::string &desc) {
		UC_LOG(INFO) << "UCMockContact::OnDeptTransferConfirmed";
	}

	virtual void OnContactLoaded(int32_t result) {
		UC_LOG(INFO) << "UCMockContact::OnContactLoaded";
	}

private:
	IContactService *_contactService;
};

class UCMockFileTransfer : public IFileTransferServiceEvent {
public:
	UCMockFileTransfer() {
	}
	virtual ~UCMockFileTransfer() {

	}

private:

	IFileTransferService* _filetransferService;

public:
	bool Init() {
		_filetransferService = (IFileTransferService*)g_ucEngine->QueryService(UCFileTransferService);
		assert(_filetransferService);
		_filetransferService->registerListener(this);

		return true;
	}

	bool UnInit() {
		_filetransferService->removeListener(this);
		return true;
	}

protected:
	virtual void OnFileUploading(int32_t errorCode, int32_t taskId, int32_t percent = 0) {

		cout << "MockFileTransfer::OnFileUploading"  << " errorCode: " << errorCode << "; taskId: " << taskId << "; percent: " << percent << endl;
	}

		/**
		* 文件上传成功的返回事件
		* @param errorCode				错误码
		* @param taskId					上传任务ID
		* @param fileName				文件名称
		* @param fileSize				文件大小
		* @param operatorId				操作者ID
		* @param operatorTime			操作时间
		* @param downloadURL			文件的下载链接地址
		* @param convertDownloadURL		文件转换后的下载链接地址
		*/
		virtual void OnFileUploaded(
			int32_t errorCode, 
			int32_t taskId,
			const std::string& fileName,
			int64_t fileSize,
			int32_t operatorId,
			int64_t opreatorTime,
			const std::string& downloadURL,
			const std::string& convertDownloadURL
			) {
			cout << "MockFileTransfer::OnFileUploaded errCode:" << errorCode <<"; taskId:" << taskId << "; downloadURL:" << downloadURL  << "; cvtDownloadURL:" << convertDownloadURL<< endl;

		}
		
		/**
		* 文件下载结果的返回事件
		* @param errorCode				错误码
		* @param taskId					下载任务ID
		* @param percent				下载进度，取值0--100
		*/
		virtual void OnFileDownloading(int32_t errorCode, int32_t taskId, int32_t percent = 0) {
			cout << "MockFileTransfer::OnFileDownloading errorCode:" << errorCode << "; taskId:" << taskId << "; percent:" << percent << endl;
		}

		/**
		* 文件下载结果的返回事件
		* @param errorCode				错误码
		* @param taskId					下载任务ID
		* @param savePath				文件下载成功后本地保存路径
		*/
		virtual void OnFileDownloaded(int32_t errorCode, int32_t taskId, const std::string& savePath) {
			cout << "MockFileTransfer::OnFileDownloaded errorCode:" << errorCode << "; taskId:" << taskId << "; path:" << savePath << endl;
		}

public:
	int32_t UploadFile() {
		std::wstring filePath = L"D:\\test\\upload\\12.pdf";
		int32_t taskId = 0;
		return _filetransferService->UploadFile(StringConvert::ToUTF8Str(filePath), taskId);
	}

	int32_t DownloadFile() {

		std::string fileURL = "http://www.sina.com.cn";
		std::string filePath = "c:\\sina.html";
		
		_filetransferService->UpdateLocalFilePath(fileURL, filePath);
		filePath = "";
		_filetransferService->GetLocalFilePath(fileURL, filePath);
		cout << "GetLocalFilePath:: " << filePath << endl;

		std::string url = "http://testcloudb.quanshi.com/ucfserver/hddown?fid=NjIwMDc4MzQvOC8xMjQ0MzIxMC5wZGZeXl50YW5naGRmc15eXmFiY2RlZmcxMjM0NTZeXl50YW5naGRmc15eXjE1OTg5MDA2$";
		std::string savePath = "D:\\test\\download\\2.pdf";
		int32_t taskId = 0;
		return _filetransferService->DownloadFile(url, savePath, taskId);
	}
};

class UCMockSearch : public ISearchServiceEvent {
public:
	UCMockSearch() {
	}
	virtual ~UCMockSearch() {

	}

private:
	ISearchService* _searchService;

public:
	bool Init() {
		_searchService = (ISearchService*)g_ucEngine->QueryService(UCSearchService);
		assert(_searchService);
		_searchService->registerListener(this);
		return true;
	}

	bool UnInit() {
		_searchService->removeListener(this);
		return true;
	}

protected:
	/**
	 * 消息页签搜索返回时间
	 * @param errorCode 错误码
	 * @param contacts 联系人信息
	 * @param groups 讨论组信息
	 * @param conferences 会议信息
	 * @param messages 聊天信息
	 * @return 成功：0 失败：非0
	 */
	virtual void OnAllSearched(int32_t errorCode, const list<ContactInfo*>& contacts,
		const list<GroupInfo>& groups, const list<ConferenceInfo*>& conferences, const list<MessageInfo*>& messages) {
		cout << "OnAllSearched, contact:" << contacts.size() << " group:" << groups.size() 
			<< " conference:" << conferences.size() << " messages:" << messages.size() << endl;
	}

	/**
	 * 通讯录页签搜索返回时间
	 * @param errorCode 错误码
	 * @param contacts 联系人信息
	 * @param groups 讨论组信息
	 * @return 成功：0 失败：非0
	 */
	virtual void OnAddressBookSearched(int32_t errorCode, const list<ContactInfo*>& contacts, const list<GroupInfo>& groups) {
		cout << "OnAddressBookSearched, contact:" << contacts.size() << " group:" << groups.size() << endl;
	}

	/**
	 * 通讯录页签搜索返回时间
	 * @param errorCode 错误码
	 * @param contacts 联系人信息
	 * @return 成功：0 失败：非0
	 */
	virtual void OnContactSearched(int32_t errorCode, const list<ContactInfo*>& contacts) {
		cout << "OnContactSearched, contact:" << contacts.size() << endl;
	}

public:
	int SearchAll() {
		string key = "z";
		int32_t page = 1;
		int32_t count = 5;
		return _searchService->SearchAll(key, page, count);
	}

	/**
	 * 从本地搜索通讯录
	 * @param key 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @param contacts 联系人搜索结果
	 * @param groups 讨论组搜索结果
	 * @return 成功：0 失败：非0
	 */
	int SearchLocalAddressBook() {
		string key = "z";
		int32_t page = 1;
		int32_t count = 5;
		list<ContactInfo*> contacts;
		list<GroupInfo> groups;
		int ret = _searchService->SearchLocalAddressBook(key, page, count, contacts, groups);
		cout << "SearchLocalAddressBook, contact:" << contacts.size() << " groups:" << groups.size() << endl;
		return ret;
	}

	/**
	 * 从server搜索通讯录
	 * @param key 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @return 成功：0 失败：非0
	 */
	int SearchRemoteAddressBook() {
		string key = "z";
		int32_t page = 1;
		int32_t count = 5;
		return _searchService->SearchRemoteAddressBook(key, page, count);
	}

	/**
	 * 从本地搜索联系人
	 * @param key 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @param contacts 联系人搜索结果
	 * @return 成功：0 失败：非0
	 */
	int SearchLocalContact() {
		string key = "z";
		int32_t page = 1;
		int32_t count = 5;
		list<ContactInfo*> contacts;
		int ret = _searchService->SearchLocalContact(key, page, count, contacts);
		cout << "SearchLocalContact, contact:" << contacts.size() << endl;
		return ret;
	}

	/**
	 * 从server搜索联系人
	 * @param key 搜索关键字
	 * @param page 第几页
	 * @param count 每页数量
	 * @return 成功：0 失败：非0
	 */
	int SearchRemoteContact() {
		string key = "z";
		int32_t page = 1;
		int32_t count = 5;
		return _searchService->SearchRemoteContact(key, page, count);
	}

};

void InitAppDirectory() {
	char path[PATH_MAX] = {0};

	getcwd(path, PATH_MAX);

	g_AppDir.append(path);
	g_AppDir.append("/Bee");
};

void PrintHelpInfo() {
	cout << "======================UC help======================" << endl;
	cout << "input:" << endl;
	cout << "1: login" << endl;
	cout << "0: logout" << endl;
	cout << "2: run imservice test" << endl;
	cout << "3: run groupservice test" << endl;
	cout << "4: run calendarservice test" << endl;
	cout << "5: run contactservice test" << endl;
	cout << "6: run clientservice test" << endl;
	cout << "c: clear console log" << endl;
	cout << "q: exit" << endl;
	cout << "h: help" << endl;
	cout << "7: SearchAll" << endl;
	cout << "8: SearchRemoteAddressBook" << endl;
	cout << "9: SearchRemoteContact" << endl;
	cout << "A: SearchLocalAddressBook" << endl;
	cout << "B: SearchLocalContact" << endl;
	cout << "===================================================" << endl;
};

int main(int argc, char** argv) {

//	std::cout.imbue(std::locale("chs"));

	InitAppDirectory();

	string logPath = g_AppDir + "/Log";

#ifdef PLATFORM_WIN
	cout << "mkdir: " << mkdir(logPath.c_str()) << endl;
#else
	cout << "mkdir: " << mkdir(logPath.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) << endl;
#endif

	UCEngineConfig ucConfig;

	ucConfig.casServer = "https://testcloud.quanshi.com";
	ucConfig.appPath = g_AppDir;
	ucConfig.logConfig.logPath = logPath;
	ucConfig.logConfig.logToFile = true;
	ucConfig.logConfig.programName = "全时蜜蜂";
	ucConfig.logConfig.logFileName = "sdkclient.log";

	g_ucEngine = CreateUCEngine(ucConfig);

	UC_LOG(INFO) << "UC start";
	UCMockClient mockClient;
	UCMockContact contact;
	UCMockIMChat imChat;
	UCMockGroupChat groupChat;
	//UCMockCalendar calendar;
	UCMockFileTransfer filetransfer;
	UCMockSearch search;

	mockClient.Init();
	contact.Init();
	imChat.Init();
	groupChat.Init();
	//calendar.Init();
	filetransfer.Init();
	search.Init();

	// 1:login
	// 0:logout
	// q:exit
	// h:help
	char input;
	bool run = true;

	PrintHelpInfo();

	while(run) {
		cin >> input;
		switch (input) {
		case '1': {
			mockClient.Login();
			break;
		}
		case '0': {
			mockClient.Logout();
			break;
		}
		case '2': {
			imChat.SendIMMessage();
			/*
			imChat.SendImage();
			imChat.SendAudioMessage();
			imChat.SendFileMessage();
			imChat.ShareFileMessage();
			imChat.SendMessagesRead();
			imChat.SendMessagePlayed();*/

			break;
		}
		case '3': {
			groupChat.CreateGroup(PROJECT_GROUP_TYPE);
// 			groupChat.CreateGroup(MUTILCHAT_GROUP_TYPE);
// 			groupChat.ExitGroup();
// 			groupChat.EndGroup();
//			groupChat.GetGroupList(PROJECT_GROUP_TYPE);
//			groupChat.GetGroupList(MUTILCHAT_GROUP_TYPE);
//			groupChat.GetGroupInfo();
//			groupChat.AddMember();
//			groupChat.RemoveMember();
//			groupChat.SetGroupName();
//			groupChat.SetGroupLogo();
//			groupChat.SetTopChat();
//			groupChat.SetReminder();
//			groupChat.SetDoNotDisturb();
//			groupChat.GetDocumentList();
//			groupChat.GetDocumentInfo();
//			groupChat.UploadDocument();
//			groupChat.ShareDocument();
//			groupChat.DeleteDocument();
//     		groupChat.SendText();
//			groupChat.SendImage();
//			groupChat.SendAudio();
//			groupChat.Sendvideo();
//			groupChat.SendInvateCall();
//			groupChat.SendAppectCall();
//			groupChat.SendRejectCall();
//			groupChat.SendFinishCall();
//			groupChat.SendConnectCallRecord();
//			groupChat.SendMessageReaded();
//			groupChat.SendMessagesReaded();
//			groupChat.SendMessagesPalyed();
			break;
		}
// 		case '4': {
// 			calendar.CreateConferenceSummary();
// 			break;
// 		}
// 		case 'u': {
// 			filetransfer.UploadFile();
// 			break;
// 		}
// 		case 'g': {
// 			calendar.GetSummaryListByConferenceId();
// 			break;
// 				  }
// 		case 'i': {
// 			calendar.GetConferenceSummaryInfo();
// 			break;
// 				  }
// 		case 'f': {
// 			calendar.ForwardConference();
// 			break;
// 		}
// 		case 'a': {
// 			calendar.AcceptConferenceInvitation();
// 			break;
// 		}
 		case 'd': {
 			filetransfer.DownloadFile();
 			break;
 		}
// 		case 'l': {
// 			calendar.GetConferenceList();
// 			break;
// 		}
// 		case '5': {
// 			calendar.GetConferenceWebStartUrl();
// 			break;
// 		}
		case '6': {
			mockClient.RunTests();
			break;
		}
		case 'h': {
			PrintHelpInfo();
			break;
		}
		case 'q': {
			run = false;
			break;
		}
		case 'c': {
			system("reset");
			break;
		}
		case '7': {
			search.SearchAll();
			break;
		}
		case '8': {
			search.SearchRemoteAddressBook();
			break;
		}
		case '9': {
			search.SearchRemoteContact();
			break;
		}
		case 'A': {
			search.SearchLocalAddressBook();
			break;
		}
		case 'B': {
			search.SearchLocalContact();
			break;
		}
		}
	}

	mockClient.UnInit();
	contact.UnInit();
	imChat.UnInit();
	groupChat.UnInit();
	//calendar.UnInit();
	filetransfer.UnInit();
	search.UnInit();

	UC_LOG(INFO) << "UC exit";

	LogManager::GetInstance().UnInit();
	DestroyUCEngine();
	return 0;
}

