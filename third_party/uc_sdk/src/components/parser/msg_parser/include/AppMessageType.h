#ifndef APPMESSAGETYPE_H
#define APPMESSAGETYPE_H

#include <map>
#include <vector>
#include "UCMessageType.h"

namespace uc {

using namespace std;

////////////////////////////////////////////
typedef UCMsg* (*NewFunc)(MsgType type);
struct MsgInst {
	int msgId;
	MsgType type;
	NewFunc func;
	MsgInst():msgId(0), type(UCInvalidTMsgype), func(NULL) {
	}
};

////////Application message type define////////////
class AppMsgType {
public:
	static AppMsgType& Instance();
	UCMsg* GetMsg(const UcMessageHead *head);
	int GetMsgID(MsgType type, int& appId, int& protocoltype, int& protocolId);
private:
	AppMsgType();
	AppMsgType(const AppMsgType&);
	AppMsgType & operator = (const AppMsgType &);
private:
	void Init();
	void InitTimeNews();
	void InitSleep();
	void InitAddressbook();
	void InitAudio();
	void InitAck();
	void InitOrganization();
	void InitMeeting();
	void InitCalendar();
	void InitFiletransfer();
	void InitNotify();
	void InitPresence();
	void InitHeartbeat();
	void InitChat();
	void InitAPI();
	void InitAssist();
	void InitSyncMessage();
private:
	// map<appid, map<prototype, vector<MsgInst> > >
	map<int, map<int, vector<MsgInst> > > _appMsgMap;
};

///////////////business message creator////////////
class CreateMsg {
public:
	static UCMsg* CreateNull(MsgType type){ return new NullMsg(type); }
	static UCMsg* CreatePresence(MsgType type){ return new MessagePresenceMsg(type); }
	static UCMsg* CreatePing(MsgType type){ return new MessagePingMsg(type); }
	static UCMsg* CreateAckSent(MsgType type){ return new AckSentMsg(type); }
	static UCMsg* CreateAckRecv(MsgType type){ return new AckReceivedMsg(type); }
	static UCMsg* CreateAckRead(MsgType type){ return new AckReadMsg(type); }
	static UCMsg* CreateBatchAckRead(MsgType type){ return new BatchAckReadMsg(type); }
	static UCMsg* CreateAckPlay(MsgType type){ return new AckPlayMsg(type); }
	static UCMsg* CreateSleep(MsgType type){ return new SleepSetMsg(type); }
	static UCMsg* CreateWakeUp(MsgType type){ return new WakeupResultMsg(type); }
	static UCMsg* CreateText(MsgType type){ return new TextContentMsg(type); }
	static UCMsg* CreateMedia(MsgType type){ return new MediaContentMsg(type); }
	static UCMsg* CreateReport(MsgType type){ return new ReportContentMsg(type); }
	static UCMsg* CreateDocumentDel(MsgType type){ return new DocumentDelContentMsg(type); }
	static UCMsg* CreateCodeCreate(MsgType type){ return new CodeCreateContentMsg(type); }
	static UCMsg* CreateCodeDel(MsgType type){ return new CodeDelContentMsg(type); }
	static UCMsg* CreateComment(MsgType type){ return new CommentCreateContentMsg(type); }
	static UCMsg* CreateDelComment(MsgType type){ return new CommentDelContentMsg(type); }
	static UCMsg* CreateMemberRoleUpdate(MsgType type){ return new MemberRoleUpdateContentMsg(type); }
	static UCMsg* CreateGroupAdd(MsgType type){ return new GroupMemberAddContentMsg(type); }
	static UCMsg* CreateGroupDel(MsgType type){ return new GroupMemberDelContentMsg(type); }
	static UCMsg* CreateGroupNameUpdate(MsgType type){ return new GroupNameUpdateContentMsg(type); }
	static UCMsg* CreateGroupCreate(MsgType type){ return new GroupCreateContentMsg(type); }
	static UCMsg* CreateGroupMemberKick(MsgType type){ return new GroupMemberKickContentMsg(type); }
	static UCMsg* CreateGroupAvatarUpdate(MsgType type){ return new GroupAvatarUpdateContentMsg(type); }
	static UCMsg* CreateGroupStateUpdate(MsgType type){ return new GroupStateUpdateContentMsg(type); }
	static UCMsg* CreateGroupTop(MsgType type){ return new GroupTopContentMsg(type); }
	static UCMsg* CreateRevocation(MsgType type){ return new RevocationContentMsg(type); }
	static UCMsg* CreateSummaryCreate(MsgType type){ return new SummaryCreateContentMsg(type); }
	static UCMsg* CreateDocument(MsgType type){ return new DocumentContentMsg(type); }
	static UCMsg* CreateCloudFileContent(MsgType type){ return new CloudFileContentMsg(type); }
	static UCMsg* CreateAlertrule(MsgType type){ return new AlertRuleContentMsg(type); }
	static UCMsg* CreateDiscussionClose(MsgType type){ return new DiscussionCloseMsg(type); }
	static UCMsg* CreateMsgNotify(MsgType type){ return new MsgNotifyContentMsg(type); }
	static UCMsg* CreateSystemWelcome(MsgType type){ return new SystemWelcomeContentMsg(type); }
	static UCMsg* CreateClientUpdate(MsgType type){ return new ClientUpdateContentMsg(type); }
	static UCMsg* CreateCompleteInfo(MsgType type){ return new CompleteInfoContentMsg(type); }
	static UCMsg* CreatePwdComplexUpdate(MsgType type){ return new PwdComplexUpdateContentMsg(type); }
	static UCMsg* CreatePwdExpire(MsgType type){ return new PwdExpireContentMsg(type); }
	static UCMsg* CreatePcClientGuide(MsgType type){ return new PCClientGuideMsg(type); }
	static UCMsg* CreateMobileClientGuide(MsgType type){ return new MobileClientGuideMsg(type); }
	static UCMsg* CreatePwdModify(MsgType type){ return new PasswordModifyContentMsg(type); }
	static UCMsg* CreateTagDel(MsgType type){ return new TagDelContentMsg(type); }
	static UCMsg* CreateGroupRemind(MsgType type){ return new GroupRemindContentMsg(type); }
	static UCMsg* CreateDeptUpdate(MsgType type){ return new DeptUpdateContentMsg(type); }
	static UCMsg* CreateDeptTransfer(MsgType type){ return new DeptTransferContentMsg(type); }
	static UCMsg* CreatePositionUpdate(MsgType type){ return new PositionUpdateContentMsg(type); }
	static UCMsg* CreateEmployeeEntry(MsgType type){ return new EmployeeEntryContentMsg(type); }
	static UCMsg* CreateEmployeeLeave(MsgType type){ return new EmployeeLeaveContentMsg(type); }
	static UCMsg* CreateEmployeeRightsChange(MsgType type){ return new EmployeeRightsChangeContentMsg(type); }
	static UCMsg* CreateDeptparentChange(MsgType type){ return new DeptParentChangeContentMsg(type); }
	static UCMsg* CreateCompanyJoin(MsgType type){ return new CompanyJoinContentMsg(type); }
	static UCMsg* CreateDeptDelete(MsgType type){ return new DeptDeleteContentMsg(type); }
	static UCMsg* CreateEmployeeEntryConfirm(MsgType type){ return new EmployeeEntryConfirmContentMsg(type); }
	static UCMsg* CreateEmployeeLeaveConfirm(MsgType type){ return new EmployeeLeaveConfirmContentMsg(type); }
	static UCMsg* CreateDeptTransferConfirm(MsgType type){ return new DeptTransferConfirmContentMsg(type); }
	static UCMsg* CreateInstantIQInvite(MsgType type){ return new InstantIQInviteContentMsg(type); }
	static UCMsg* CreateIQInvite(MsgType type){ return new IQInviteContentMsg(type); }
	static UCMsg* CreateIQReceive(MsgType type){ return new IQReceiveContentMsg(type); }
	static UCMsg* CreateIQReject(MsgType type){ return new IQRejectContentMsg(type); }
	static UCMsg* CreateConfCancel(MsgType type){ return new ConfCancelContentMsg(type); }
	static UCMsg* CreateConfAlert(MsgType type){ return new ConfAlertContentMsg(type); }
	static UCMsg* CreateInstantIQReceive(MsgType type){ return new InstantIQReceiveContentMsg(type); }
	static UCMsg* CreateInstantIQReject(MsgType type){ return new InstantIQRejectContentMsg(type); }
	static UCMsg* CreateConfChat(MsgType type){ return new ConfChatContentMsg(type); }
	static UCMsg* CreateConfSummary(MsgType type){ return new ConfSummaryContentMsg(type); }
	static UCMsg* CreateCallState(MsgType type){ return new CallStateContentMsg(type); }
	static UCMsg* CreateConfUpload(MsgType type){ return new ConfUploadContentMsg(type); }
	static UCMsg* CreateConfDelDoc(MsgType type) { return new ConfDelDocContentMsg(type); }
	static UCMsg* CreateCardUpdate(MsgType type){ return new CardUpdateContentMsg(type); }
	static UCMsg* CreateContacterAdd(MsgType type){ return new ContacterAddContentMsg(type); }
	static UCMsg* CreateContacterDel(MsgType type){ return new ContacterDelContentMsg(type); }
	static UCMsg* CreateFsInvite(MsgType type){ return new FileTransmissionInviteContentMsg(type); }
	static UCMsg* CreateFsReceive(MsgType type){ return new FileTransmissionReceiveContentMsg(type); }
	static UCMsg* CreateFsReject(MsgType type){ return new FileTransmissionRejectContentMsg(type); }
	static UCMsg* CreateFsStop(MsgType type){ return new FileTransmissionStopContentMsg(type); }
	static UCMsg* CreateAudioInvite(MsgType type){ return new AudioInviteContentMsg(type); }
	static UCMsg* CreateAudioReceive(MsgType type){ return new AudioReceiveContentMsg(type); }
	static UCMsg* CreateAudioReject(MsgType type){ return new AudioRejectContentMsg(type); }
	static UCMsg* CreateAudioStop(MsgType type){ return new AudioStopContentMsg(type); }
	static UCMsg* CreateAudioCancelCall(MsgType type){ return new AudioCancelCallContentMsg(type); }
	static UCMsg* CreateAudioRecentCall(MsgType type){ return new AudioRecentCallContentMsg(type); }
	static UCMsg* CreateAudioBlockCall(MsgType type){ return new AudioBlockCallContentMsg(type); }
	static UCMsg* CreateAudioRejectCall(MsgType type){ return new AudioRejectCallContentMsg(type); }
	static UCMsg* CreateConfShare(MsgType type){ return new ConfShareContentMsg(type); }
	static UCMsg* CreateWebMessage(MsgType type){ return new WBMessageContentMsg(type); }
	static UCMsg* CreateAssistInviteMessage(MsgType type){ return new RemoteAssistanceInviteContentMsg(type); }
	static UCMsg* CreateAssistAcceptMessage(MsgType type){ return new RemoteAssistanceAcceptContentMsg(type); }
	static UCMsg* CreateAssistRejectMessage(MsgType type){ return new RemoteAssistanceRejectContentMsg(type); }
	static UCMsg* CreateAssistEndMessage(MsgType type){ return new RemoteAssistanceEndContentMsg(type); }
	static UCMsg* CreatSessionTopContent(MsgType type){ return new SessionTopContentMsg(type); }
	static UCMsg* CreatAtMessageReadedContent(MsgType type){ return new AtMessageReadedContentMsg(type); }
	static UCMsg* CreateUserLoginNotify(MsgType type){ return new UserLoginNotifyContentMsg(type); }
	static UCMsg* CreateAPIText(MsgType type){ return new APITextContentMsg(type); }
	static UCMsg* CreateAPIImage(MsgType type){ return new APIImageContentMsg(type); }
	static UCMsg* CreateAPIFile(MsgType type){ return new APIFileContentMsg(type); }
	static UCMsg* CreateAPIOA(MsgType type){ return new APIOAContentMsg(type); }
	static UCMsg* CreateAPIAck(MsgType type){ return new APIAckSentContentMsg(type); }
	static UCMsg* CreateAPIOAStatusChange(MsgType type){ return new APIOAStatusChangeContentMsg(type); }
	static UCMsg* CreateAPIAppEvnentChange(MsgType type){ return new APIAppEventChangeNotifyMsg(type); }
	static UCMsg* CreateAPIAppChangeNotify(MsgType type){ return new APIAppChangeNotifyContentMsg(type); }
	static UCMsg* CreateAPICustomizedContent(MsgType type){ return new APICustomizedContentMsg(type); }
};

} //namespace uc

#endif //APPMESSAGETYPE_H
