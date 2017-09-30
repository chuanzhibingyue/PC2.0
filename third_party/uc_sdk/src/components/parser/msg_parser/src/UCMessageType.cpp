#include "UCMessageType.h"

namespace uc {

///////////////////////////////////////
MessagePresenceMsg::MessagePresenceMsg() {
	this->_message = new MessagePresence();
}

MessagePresenceMsg::MessagePresenceMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new MessagePresence();
}

void MessagePresenceMsg::EncodeBody() {
	_body->__set_presence(*_message);
}

void MessagePresenceMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->presence);
}

/////////////////////////////////////////
MessagePingMsg::MessagePingMsg() {
	this->_message = new MessagePing();
}

MessagePingMsg::MessagePingMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new MessagePing();
}

void MessagePingMsg::EncodeBody() {
	_body->__set_ping(*_message);
}

void MessagePingMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->ping);
}

////////////////////////////////////////
AckSentMsg::AckSentMsg() {
	this->_message = new AckSent();
}

AckSentMsg::AckSentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AckSent();
}

void AckSentMsg::EncodeBody() {
	_body->__set_ackSent(*_message);
}

void AckSentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->ackSent);
}

////////////////////////////////////////
AckReceivedMsg::AckReceivedMsg() {
	this->_message = new AckReceived();
}

AckReceivedMsg::AckReceivedMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AckReceived();
}

void AckReceivedMsg::EncodeBody() {
	_body->__set_ackReceived(*_message);
}

void AckReceivedMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->ackReceived);
}

////////////////////////////////////////
AckReadMsg::AckReadMsg() {
	this->_message = new AckRead();
}

AckReadMsg::AckReadMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AckRead();
}

void AckReadMsg::EncodeBody() {
	_body->__set_ackRead(*_message);
}

void AckReadMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->ackRead);
}

/////////////////////////////////////////
BatchAckReadMsg::BatchAckReadMsg() {
	this->_message = new BatchAckRead();
}

BatchAckReadMsg::BatchAckReadMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new BatchAckRead();
}

void BatchAckReadMsg::EncodeBody() {
	_body->__set_batchAckRead(*_message);
}

void BatchAckReadMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->batchAckRead);
}

////////////////////////////////////////
AckPlayMsg::AckPlayMsg() {
	this->_message = new AckPlay();
}

AckPlayMsg::AckPlayMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AckPlay();
}

void AckPlayMsg::EncodeBody() {
	_body->__set_ackPlay(*_message);
}

void AckPlayMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->ackPlay);
}

////////////////////////////////////////
SleepSetMsg::SleepSetMsg() {
	this->_message = new SleepSet();
}

SleepSetMsg::SleepSetMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new SleepSet();
}

void SleepSetMsg::EncodeBody() {
	_body->__set_sleep(*_message);
}

void SleepSetMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->sleep);
}

////////////////////////////////////////
WakeupResultMsg::WakeupResultMsg() {
	this->_message = new WakeupResult();
}

WakeupResultMsg::WakeupResultMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new WakeupResult();
}

void WakeupResultMsg::EncodeBody() {
	_body->__set_wakeupConfirm(*_message);
}

void WakeupResultMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->wakeupConfirm);
}

////////////////////////////////////////
TextContentMsg::TextContentMsg() {
	this->_message = new TextContent();
}

TextContentMsg::TextContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new TextContent();
}

void TextContentMsg::EncodeBody() {
	_body->__set_text(*_message);
}

void TextContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->text);
}

///////////////////////////////////////////
MediaContentMsg::MediaContentMsg() {
	this->_message = new MediaContent();
}

MediaContentMsg::MediaContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new MediaContent();
}

void MediaContentMsg::EncodeBody() {
	_body->__set_media(*_message);
}

void MediaContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->media);
}

////////////////////////////////////////
ReportContentMsg::ReportContentMsg() {
	this->_message = new ReportContent();
}

ReportContentMsg::ReportContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ReportContent();
}

void ReportContentMsg::EncodeBody() {
	_body->__set_report(*_message);
}

void ReportContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->report);
}

///////////////////////////////////////////
MemberRoleUpdateContentMsg::MemberRoleUpdateContentMsg() {
	this->_message = new MemberRoleUpdateContent();
}

MemberRoleUpdateContentMsg::MemberRoleUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new MemberRoleUpdateContent();
}

void MemberRoleUpdateContentMsg::EncodeBody() {
	_body->__set_memberRole(*_message);
}

void MemberRoleUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->memberRole);
}

///////////////////////////////////////////
GroupMemberAddContentMsg::GroupMemberAddContentMsg() {
	this->_message = new GroupMemberAddContent();
}

GroupMemberAddContentMsg::GroupMemberAddContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupMemberAddContent();
}

void GroupMemberAddContentMsg::EncodeBody() {
	_body->__set_groupAdd(*_message);
}

void GroupMemberAddContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupAdd);
}

///////////////////////////////////////////
RevocationContentMsg::RevocationContentMsg() {
	this->_message = new RevocationContent();
}

RevocationContentMsg::RevocationContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new RevocationContent();
}

void RevocationContentMsg::EncodeBody() {
	_body->__set_revocation(*_message);
}

void RevocationContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->revocation);
}

///////////////////////////////////////////
GroupMemberDelContentMsg::GroupMemberDelContentMsg() {
	this->_message = new GroupMemberDelContent();
}

GroupMemberDelContentMsg::GroupMemberDelContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupMemberDelContent();
}

void GroupMemberDelContentMsg::EncodeBody() {
	_body->__set_groupDel(*_message);
}

void GroupMemberDelContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupDel);
}

///////////////////////////////////////////
GroupNameUpdateContentMsg::GroupNameUpdateContentMsg() {
	this->_message = new GroupNameUpdateContent();
}

GroupNameUpdateContentMsg::GroupNameUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupNameUpdateContent();
}

void GroupNameUpdateContentMsg::EncodeBody() {
	_body->__set_groupUpdate(*_message);
}

void GroupNameUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupUpdate);
}

///////////////////////////////////////////
GroupCreateContentMsg::GroupCreateContentMsg() {
	this->_message = new GroupCreateContent();
}

GroupCreateContentMsg::GroupCreateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupCreateContent();
}

void GroupCreateContentMsg::EncodeBody() {
	_body->__set_groupCreate(*_message);
}

void GroupCreateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupCreate);
}

///////////////////////////////////////////
GroupMemberKickContentMsg::GroupMemberKickContentMsg() {
	this->_message = new GroupMemberKickContent();
}

GroupMemberKickContentMsg::GroupMemberKickContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupMemberKickContent();
}

void GroupMemberKickContentMsg::EncodeBody() {
	_body->__set_groupKick(*_message);
}

void GroupMemberKickContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupKick);
}

///////////////////////////////////////////
GroupAvatarUpdateContentMsg::GroupAvatarUpdateContentMsg() {
	this->_message = new GroupAvatarUpdateContent();
}

GroupAvatarUpdateContentMsg::GroupAvatarUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupAvatarUpdateContent();
}

void GroupAvatarUpdateContentMsg::EncodeBody() {
	_body->__set_groupAvatarUpdate(*_message);
}

void GroupAvatarUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupAvatarUpdate);
}

///////////////////////////////////////////
GroupStateUpdateContentMsg::GroupStateUpdateContentMsg() {
	this->_message = new GroupStateUpdateContent();
}

GroupStateUpdateContentMsg::GroupStateUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupStateUpdateContent();
}

void GroupStateUpdateContentMsg::EncodeBody() {
	_body->__set_groupState(*_message);
}

void GroupStateUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupState);
}

///////////////////////////////////////////
GroupTopContentMsg::GroupTopContentMsg() {
	this->_message = new GroupTopContent();
}

GroupTopContentMsg::GroupTopContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupTopContent();
}

void GroupTopContentMsg::EncodeBody() {
	_body->__set_groupTop(*_message);
}

void GroupTopContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupTop);
}

///////////////////////////////////////////
SummaryCreateContentMsg::SummaryCreateContentMsg() {
	this->_message = new SummaryCreateContent();
}

SummaryCreateContentMsg::SummaryCreateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new SummaryCreateContent();
}

void SummaryCreateContentMsg::EncodeBody() {
	_body->__set_summaryCreate(*_message);
}

void SummaryCreateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->summaryCreate);
}

///////////////////////////////////////////
DocumentContentMsg::DocumentContentMsg() {
	this->_message = new DocumentContent();
}

DocumentContentMsg::DocumentContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new DocumentContent();
}

void DocumentContentMsg::EncodeBody() {
	_body->__set_docSend(*_message);
}

void DocumentContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->docSend);
}

///////////////////////////////////////////
CloudFileContentMsg::CloudFileContentMsg() {
	this->_message = new CloudFileContent();
}

CloudFileContentMsg::CloudFileContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CloudFileContent();
}

void CloudFileContentMsg::EncodeBody() {
	_body->__set_cloudFile(*_message);
}

void CloudFileContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->cloudFile);
}

///////////////////////////////////////////
DocumentDelContentMsg::DocumentDelContentMsg() {
	this->_message = new DocumentDelContent();
}

DocumentDelContentMsg::DocumentDelContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new DocumentDelContent();
}

void DocumentDelContentMsg::EncodeBody() {
	_body->__set_docDel(*_message);
}

void DocumentDelContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->docDel);
}

///////////////////////////////////////////
CommentCreateContentMsg::CommentCreateContentMsg() {
	this->_message = new CommentCreateContent();
}

CommentCreateContentMsg::CommentCreateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CommentCreateContent();
}

void CommentCreateContentMsg::EncodeBody() {
	_body->__set_commentCreate(*_message);
}

void CommentCreateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->commentCreate);
}

///////////////////////////////////////////
CommentDelContentMsg::CommentDelContentMsg() {
	this->_message = new CommentDelContent();
}

CommentDelContentMsg::CommentDelContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CommentDelContent();
}

void CommentDelContentMsg::EncodeBody() {
	_body->__set_commentDel(*_message);
}

void CommentDelContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->commentDel);
}

///////////////////////////////////////////
CodeCreateContentMsg::CodeCreateContentMsg() {
	this->_message = new CodeCreateContent();
}

CodeCreateContentMsg::CodeCreateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CodeCreateContent();
}

void CodeCreateContentMsg::EncodeBody() {
	_body->__set_codeCreate(*_message);
}

void CodeCreateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->codeCreate);
}

///////////////////////////////////////////
CodeDelContentMsg::CodeDelContentMsg() {
	this->_message = new CodeDelContent();
}

CodeDelContentMsg::CodeDelContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CodeDelContent();
}

void CodeDelContentMsg::EncodeBody() {
	_body->__set_codeDel(*_message);
}

void CodeDelContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->codeDel);
}

///////////////////////////////////////////
AlertRuleContentMsg::AlertRuleContentMsg() {
	this->_message = new AlertRuleContent();
}

AlertRuleContentMsg::AlertRuleContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AlertRuleContent();
}

void AlertRuleContentMsg::EncodeBody() {
	_body->__set_alertRule(*_message);
}

void AlertRuleContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->alertRule);
}

///////////////////////////////////////////
DiscussionCloseMsg::DiscussionCloseMsg() {
	this->_message = new DiscussionClose();
}

DiscussionCloseMsg::DiscussionCloseMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new DiscussionClose();
}

void DiscussionCloseMsg::EncodeBody() {
	_body->__set_groupClose(*_message);
}

void DiscussionCloseMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->groupClose);
}

///////////////////////////////////////////
MsgNotifyContentMsg::MsgNotifyContentMsg() {
	this->_message = new MsgNotifyContent();
}

MsgNotifyContentMsg::MsgNotifyContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new MsgNotifyContent();
}

void MsgNotifyContentMsg::EncodeBody() {
	_body->__set_msgNotify(*_message);
}

void MsgNotifyContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->msgNotify);
}

///////////////////////////////////////////
SystemWelcomeContentMsg::SystemWelcomeContentMsg() {
	this->_message = new SystemWelcomeContent();
}

SystemWelcomeContentMsg::SystemWelcomeContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new SystemWelcomeContent();
}

void SystemWelcomeContentMsg::EncodeBody() {
	_body->__set_welcome(*_message);
}

void SystemWelcomeContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->welcome);
}

///////////////////////////////////////////
ClientUpdateContentMsg::ClientUpdateContentMsg() {
	this->_message = new ClientUpdateContent();
}

ClientUpdateContentMsg::ClientUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ClientUpdateContent();
}

void ClientUpdateContentMsg::EncodeBody() {
	_body->__set_clientUpdate(*_message);
}

void ClientUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->clientUpdate);
}

///////////////////////////////////////////
CompleteInfoContentMsg::CompleteInfoContentMsg() {
	this->_message = new CompleteInfoContent();
}

CompleteInfoContentMsg::CompleteInfoContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CompleteInfoContent();
}

void CompleteInfoContentMsg::EncodeBody() {
	_body->__set_completeInfo(*_message);
}

void CompleteInfoContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->completeInfo);
}

///////////////////////////////////////////
PwdComplexUpdateContentMsg::PwdComplexUpdateContentMsg() {
	this->_message = new PwdComplexUpdateContent();
}

PwdComplexUpdateContentMsg::PwdComplexUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new PwdComplexUpdateContent();
}

void PwdComplexUpdateContentMsg::EncodeBody() {
	_body->__set_pwdComplexUpdate(*_message);
}

void PwdComplexUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->pwdComplexUpdate);
}

///////////////////////////////////////////
PwdExpireContentMsg::PwdExpireContentMsg() {
	this->_message = new PwdExpireContent();
}

PwdExpireContentMsg::PwdExpireContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new PwdExpireContent();
}

void PwdExpireContentMsg::EncodeBody() {
	_body->__set_pwdExpire(*_message);
}

void PwdExpireContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->pwdExpire);
}

///////////////////////////////////////////
PCClientGuideMsg::PCClientGuideMsg() {
	this->_message = new PCClientGuide();
}

PCClientGuideMsg::PCClientGuideMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new PCClientGuide();
}

void PCClientGuideMsg::EncodeBody() {
	_body->__set_pcGuide(*_message);
}

void PCClientGuideMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->pcGuide);
}

///////////////////////////////////////////
MobileClientGuideMsg::MobileClientGuideMsg() {
	this->_message = new MobileClientGuide();
}

MobileClientGuideMsg::MobileClientGuideMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new MobileClientGuide();
}

void MobileClientGuideMsg::EncodeBody() {
	_body->__set_mobileGuide(*_message);
}

void MobileClientGuideMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->mobileGuide);
}

///////////////////////////////////////////
PasswordModifyContentMsg::PasswordModifyContentMsg() {
	this->_message = new PasswordModifyContent();
}

PasswordModifyContentMsg::PasswordModifyContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new PasswordModifyContent();
}

void PasswordModifyContentMsg::EncodeBody() {
	_body->__set_pwdModify(*_message);
}

void PasswordModifyContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->pwdModify);
}

///////////////////////////////////////////
TagDelContentMsg::TagDelContentMsg() {
	this->_message = new TagDelContent();
}

TagDelContentMsg::TagDelContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new TagDelContent();
}

void TagDelContentMsg::EncodeBody() {
	_body->__set_tagDel(*_message);
}

void TagDelContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->tagDel);
}

///////////////////////////////////////////
GroupRemindContentMsg::GroupRemindContentMsg() {
	this->_message = new GroupRemindContent();
}

GroupRemindContentMsg::GroupRemindContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new GroupRemindContent();
}

void GroupRemindContentMsg::EncodeBody() {
	_body->__set_remind(*_message);
}

void GroupRemindContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->remind);
}

///////////////////////////////////////////
DeptUpdateContentMsg::DeptUpdateContentMsg() {
	this->_message = new DeptUpdateContent();
}

DeptUpdateContentMsg::DeptUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new DeptUpdateContent();
}

void DeptUpdateContentMsg::EncodeBody() {
	_body->__set_deptUpdate(*_message);
}

void DeptUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->deptUpdate);
}

///////////////////////////////////////////
DeptTransferContentMsg::DeptTransferContentMsg() {
	this->_message = new DeptTransferContent();
}

DeptTransferContentMsg::DeptTransferContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new DeptTransferContent();
}

void DeptTransferContentMsg::EncodeBody() {
	_body->__set_deptTransfer(*_message);
}

void DeptTransferContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->deptTransfer);
}

///////////////////////////////////////////
PositionUpdateContentMsg::PositionUpdateContentMsg() {
	this->_message = new PositionUpdateContent();
}

PositionUpdateContentMsg::PositionUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new PositionUpdateContent();
}

void PositionUpdateContentMsg::EncodeBody() {
	_body->__set_positionUpdate(*_message);
}

void PositionUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->positionUpdate);
}

///////////////////////////////////////////
EmployeeEntryContentMsg::EmployeeEntryContentMsg() {
	this->_message = new EmployeeEntryContent();
}

EmployeeEntryContentMsg::EmployeeEntryContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new EmployeeEntryContent();
}

void EmployeeEntryContentMsg::EncodeBody() {
	_body->__set_employeeEntry(*_message);
}

void EmployeeEntryContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->employeeEntry);
}

///////////////////////////////////////////
EmployeeLeaveContentMsg::EmployeeLeaveContentMsg() {
	this->_message = new EmployeeLeaveContent();
}

EmployeeLeaveContentMsg::EmployeeLeaveContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new EmployeeLeaveContent();
}

void EmployeeLeaveContentMsg::EncodeBody() {
	_body->__set_employeeLeave(*_message);
}

void EmployeeLeaveContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->employeeLeave);
}

///////////////////////////////////////////
EmployeeRightsChangeContentMsg::EmployeeRightsChangeContentMsg() {
	this->_message = new EmployeeRightsChangeContent();
}

EmployeeRightsChangeContentMsg::EmployeeRightsChangeContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new EmployeeRightsChangeContent();
}

void EmployeeRightsChangeContentMsg::EncodeBody() {
	_body->__set_employeeRights(*_message);
}

void EmployeeRightsChangeContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->employeeRights);
}

///////////////////////////////////////////
DeptParentChangeContentMsg::DeptParentChangeContentMsg() {
	this->_message = new DeptParentChangeContent();
}

DeptParentChangeContentMsg::DeptParentChangeContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new DeptParentChangeContent();
}

void DeptParentChangeContentMsg::EncodeBody() {
	_body->__set_deprtParentChange(*_message);
}

void DeptParentChangeContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->deprtParentChange);
}

///////////////////////////////////////////
CompanyJoinContentMsg::CompanyJoinContentMsg() {
	this->_message = new CompanyJoinContent();
}

CompanyJoinContentMsg::CompanyJoinContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CompanyJoinContent();
}

void CompanyJoinContentMsg::EncodeBody() {
	_body->__set_companyJoin(*_message);
}

void CompanyJoinContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->companyJoin);
}

///////////////////////////////////////////
DeptDeleteContentMsg::DeptDeleteContentMsg() {
	this->_message = new DeptDeleteContent();
}

DeptDeleteContentMsg::DeptDeleteContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new DeptDeleteContent();
}

void DeptDeleteContentMsg::EncodeBody() {
	_body->__set_deptDelete(*_message);
}

void DeptDeleteContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->deptDelete);
}

///////////////////////////////////////////
EmployeeEntryConfirmContentMsg::EmployeeEntryConfirmContentMsg() {
	this->_message = new EmployeeEntryConfirmContent();
}

EmployeeEntryConfirmContentMsg::EmployeeEntryConfirmContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new EmployeeEntryConfirmContent();
}

void EmployeeEntryConfirmContentMsg::EncodeBody() {
	_body->__set_entryConfirm(*_message);
}

void EmployeeEntryConfirmContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->entryConfirm);
}

///////////////////////////////////////////
EmployeeLeaveConfirmContentMsg::EmployeeLeaveConfirmContentMsg() {
	this->_message = new EmployeeLeaveConfirmContent();
}

EmployeeLeaveConfirmContentMsg::EmployeeLeaveConfirmContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new EmployeeLeaveConfirmContent();
}

void EmployeeLeaveConfirmContentMsg::EncodeBody() {
	_body->__set_leaveConfirm(*_message);
}

void EmployeeLeaveConfirmContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->leaveConfirm);
}

///////////////////////////////////////////
DeptTransferConfirmContentMsg::DeptTransferConfirmContentMsg() {
	this->_message = new DeptTransferConfirmContent();
}

DeptTransferConfirmContentMsg::DeptTransferConfirmContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new DeptTransferConfirmContent();
}

void DeptTransferConfirmContentMsg::EncodeBody() {
	_body->__set_deptConfirm(*_message);
}

void DeptTransferConfirmContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->deptConfirm);
}

///////////////////////////////////////////
InstantIQInviteContentMsg::InstantIQInviteContentMsg() {
	this->_message = new InstantIQInviteContent();
}

InstantIQInviteContentMsg::InstantIQInviteContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new InstantIQInviteContent();
}

void InstantIQInviteContentMsg::EncodeBody() {
	_body->__set_instantInvite(*_message);
}

void InstantIQInviteContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->instantInvite);
}

///////////////////////////////////////////
IQInviteContentMsg::IQInviteContentMsg() {
	this->_message = new IQInviteContent();
}

IQInviteContentMsg::IQInviteContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new IQInviteContent();
}

void IQInviteContentMsg::EncodeBody() {
	_body->__set_confInvite(*_message);
}

void IQInviteContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confInvite);
}

///////////////////////////////////////////
IQReceiveContentMsg::IQReceiveContentMsg() {
	this->_message = new IQReceiveContent();
}

IQReceiveContentMsg::IQReceiveContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new IQReceiveContent();
}

void IQReceiveContentMsg::EncodeBody() {
	_body->__set_confReceive(*_message);
}

void IQReceiveContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confReceive);
}

///////////////////////////////////////////
IQRejectContentMsg::IQRejectContentMsg() {
	this->_message = new IQRejectContent();
}

IQRejectContentMsg::IQRejectContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new IQRejectContent();
}

void IQRejectContentMsg::EncodeBody() {
	_body->__set_confReject(*_message);
}

void IQRejectContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confReject);
}

///////////////////////////////////////////
ConfCancelContentMsg::ConfCancelContentMsg() {
	this->_message = new ConfCancelContent();
}

ConfCancelContentMsg::ConfCancelContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ConfCancelContent();
}

void ConfCancelContentMsg::EncodeBody() {
	_body->__set_confCancel(*_message);
}

void ConfCancelContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confCancel);
}

///////////////////////////////////////////
ConfAlertContentMsg::ConfAlertContentMsg() {
	this->_message = new ConfAlertContent();
}

ConfAlertContentMsg::ConfAlertContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ConfAlertContent();
}

void ConfAlertContentMsg::EncodeBody() {
	_body->__set_confAlert(*_message);
}

void ConfAlertContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confAlert);
}

///////////////////////////////////////////
InstantIQReceiveContentMsg::InstantIQReceiveContentMsg() {
	this->_message = new InstantIQReceiveContent();
}

InstantIQReceiveContentMsg::InstantIQReceiveContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new InstantIQReceiveContent();
}

void InstantIQReceiveContentMsg::EncodeBody() {
	_body->__set_instantReceive(*_message);
}

void InstantIQReceiveContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->instantReceive);
}

///////////////////////////////////////////
InstantIQRejectContentMsg::InstantIQRejectContentMsg() {
	this->_message = new InstantIQRejectContent();
}

InstantIQRejectContentMsg::InstantIQRejectContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new InstantIQRejectContent();
}

void InstantIQRejectContentMsg::EncodeBody() {
	_body->__set_instantReject(*_message);
}

void InstantIQRejectContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->instantReject);
}

///////////////////////////////////////////
ConfChatContentMsg::ConfChatContentMsg() {
	this->_message = new ConfChatContent();
}

ConfChatContentMsg::ConfChatContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ConfChatContent();
}

void ConfChatContentMsg::EncodeBody() {
	_body->__set_confChat(*_message);
}

void ConfChatContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confChat);
}

///////////////////////////////////////////
ConfSummaryContentMsg::ConfSummaryContentMsg() {
	this->_message = new ConfSummaryContent();
}

ConfSummaryContentMsg::ConfSummaryContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ConfSummaryContent();
}

void ConfSummaryContentMsg::EncodeBody() {
	_body->__set_confSummary(*_message);
}

void ConfSummaryContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confSummary);
}

///////////////////////////////////////////
CallStateContentMsg::CallStateContentMsg() {
	this->_message = new CallStateContent();
}

CallStateContentMsg::CallStateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CallStateContent();
}

void CallStateContentMsg::EncodeBody() {
	_body->__set_callState(*_message);
}

void CallStateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->callState);
}

////////////////////////////////////////
ConfUploadContentMsg::ConfUploadContentMsg() {
	this->_message = new ConfUploadContent();
}

ConfUploadContentMsg::ConfUploadContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ConfUploadContent();
}

void ConfUploadContentMsg::EncodeBody() {
	_body->__set_confUpload(*_message);
}

void ConfUploadContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confUpload);
}

////////////////////////////////////////
ConfDelDocContentMsg::ConfDelDocContentMsg() {
	this->_message = new ConfDelDocContent();
}

ConfDelDocContentMsg::ConfDelDocContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ConfDelDocContent();
}

void ConfDelDocContentMsg::EncodeBody() {
	_body->__set_confDocDel(*_message);
}

void ConfDelDocContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confDocDel);
}

///////////////////////////////////////////
CardUpdateContentMsg::CardUpdateContentMsg() {
	this->_message = new CardUpdateContent();
}

CardUpdateContentMsg::CardUpdateContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new CardUpdateContent();
}

void CardUpdateContentMsg::EncodeBody() {
	_body->__set_cardUpdate(*_message);
}

void CardUpdateContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->cardUpdate);
}

///////////////////////////////////////////
ContacterAddContentMsg::ContacterAddContentMsg() {
	this->_message = new ContacterAddContent();
}

ContacterAddContentMsg::ContacterAddContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ContacterAddContent();
}

void ContacterAddContentMsg::EncodeBody() {
	_body->__set_contacterAdd(*_message);
}

void ContacterAddContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->contacterAdd);
}

///////////////////////////////////////////
ContacterDelContentMsg::ContacterDelContentMsg() {
	this->_message = new ContacterDelContent();
}

ContacterDelContentMsg::ContacterDelContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ContacterDelContent();
}

void ContacterDelContentMsg::EncodeBody() {
	_body->__set_contacterDel(*_message);
}

void ContacterDelContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->contacterDel);
}

///////////////////////////////////////////
FileTransmissionInviteContentMsg::FileTransmissionInviteContentMsg() {
	this->_message = new FileTransmissionInviteContent();
}

FileTransmissionInviteContentMsg::FileTransmissionInviteContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new FileTransmissionInviteContent();
}

void FileTransmissionInviteContentMsg::EncodeBody() {
	_body->__set_transInvite(*_message);
}

void FileTransmissionInviteContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->transInvite);
}

///////////////////////////////////////////
FileTransmissionReceiveContentMsg::FileTransmissionReceiveContentMsg() {
	this->_message = new FileTransmissionReceiveContent();
}

FileTransmissionReceiveContentMsg::FileTransmissionReceiveContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new FileTransmissionReceiveContent();
}

void FileTransmissionReceiveContentMsg::EncodeBody() {
	_body->__set_transReceive(*_message);
}

void FileTransmissionReceiveContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->transReceive);
}

///////////////////////////////////////////
FileTransmissionRejectContentMsg::FileTransmissionRejectContentMsg() {
	this->_message = new FileTransmissionRejectContent();
}

FileTransmissionRejectContentMsg::FileTransmissionRejectContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new FileTransmissionRejectContent();
}

void FileTransmissionRejectContentMsg::EncodeBody() {
	_body->__set_transReject(*_message);
}

void FileTransmissionRejectContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->transReject);
}

///////////////////////////////////////////
FileTransmissionStopContentMsg::FileTransmissionStopContentMsg() {
	this->_message = new FileTransmissionStopContent();
}

FileTransmissionStopContentMsg::FileTransmissionStopContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new FileTransmissionStopContent();
}

void FileTransmissionStopContentMsg::EncodeBody() {
	_body->__set_transStop(*_message);
}

void FileTransmissionStopContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->transStop);
}

///////////////////////////////////////////
AudioInviteContentMsg::AudioInviteContentMsg() {
	this->_message = new AudioInviteContent();
}

AudioInviteContentMsg::AudioInviteContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AudioInviteContent();
}

void AudioInviteContentMsg::EncodeBody() {
	_body->__set_audioInvite(*_message);
}

void AudioInviteContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->audioInvite);
}

///////////////////////////////////////////
AudioReceiveContentMsg::AudioReceiveContentMsg() {
	this->_message = new AudioReceiveContent();
}

AudioReceiveContentMsg::AudioReceiveContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AudioReceiveContent();
}

void AudioReceiveContentMsg::EncodeBody() {
	_body->__set_audioReceive(*_message);
}

void AudioReceiveContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->audioReceive);
}

///////////////////////////////////////////
AudioRejectContentMsg::AudioRejectContentMsg() {
	this->_message = new AudioRejectContent();
}

AudioRejectContentMsg::AudioRejectContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AudioRejectContent();
}

void AudioRejectContentMsg::EncodeBody() {
	_body->__set_audioReject(*_message);
}

void AudioRejectContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->audioReject);
}

///////////////////////////////////////////
AudioStopContentMsg::AudioStopContentMsg() {
	this->_message = new AudioStopContent();
}

AudioStopContentMsg::AudioStopContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AudioStopContent();
}

void AudioStopContentMsg::EncodeBody() {
	_body->__set_audioStop(*_message);
}

void AudioStopContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->audioStop);
}

///////////////////////////////////////////
AudioRecentCallContentMsg::AudioRecentCallContentMsg() {
	this->_message = new AudioRecentCallContent();
}

AudioRecentCallContentMsg::AudioRecentCallContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AudioRecentCallContent();
}

void AudioRecentCallContentMsg::EncodeBody() {
	_body->__set_callRecent(*_message);
}

void AudioRecentCallContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->callRecent);
}

///////////////////////////////////////////
AudioCancelCallContentMsg::AudioCancelCallContentMsg() {
	this->_message = new AudioCancelCallContent();
}

AudioCancelCallContentMsg::AudioCancelCallContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AudioCancelCallContent();
}

void AudioCancelCallContentMsg::EncodeBody() {
	_body->__set_callCancel(*_message);
}

void AudioCancelCallContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->callCancel);
}

///////////////////////////////////////////
AudioBlockCallContentMsg::AudioBlockCallContentMsg() {
	this->_message = new AudioBlockCallContent();
}

AudioBlockCallContentMsg::AudioBlockCallContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AudioBlockCallContent();
}

void AudioBlockCallContentMsg::EncodeBody() {
	_body->__set_callBlock(*_message);
}

void AudioBlockCallContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->callBlock);
}

///////////////////////////////////////////
AudioRejectCallContentMsg::AudioRejectCallContentMsg() {
	this->_message = new AudioRejectCallContent();
}

AudioRejectCallContentMsg::AudioRejectCallContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AudioRejectCallContent();
}

void AudioRejectCallContentMsg::EncodeBody() {
	_body->__set_callReject(*_message);
}

void AudioRejectCallContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->callReject);
}

///////////////////////////////////////////
ConfShareContentMsg::ConfShareContentMsg() {
	this->_message = new ConfShareContent();
}

ConfShareContentMsg::ConfShareContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new ConfShareContent();
}

void ConfShareContentMsg::EncodeBody() {
	_body->__set_confShare(*_message);
}

void ConfShareContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->confShare);
}

///////////////////////////////////////////
WBMessageContentMsg::WBMessageContentMsg() {
	this->_message = new WBMessageContent();
}

WBMessageContentMsg::WBMessageContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new WBMessageContent();
}

void WBMessageContentMsg::EncodeBody() {
	_body->__set_wbContent(*_message);
}

void WBMessageContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->wbContent);
}

///////////////////////////////////////////
RemoteAssistanceInviteContentMsg::RemoteAssistanceInviteContentMsg() {
	this->_message = new RemoteAssistanceInviteContent();
}

RemoteAssistanceInviteContentMsg::RemoteAssistanceInviteContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new RemoteAssistanceInviteContent();
}

void RemoteAssistanceInviteContentMsg::EncodeBody() {
	_body->__set_remoteAssistanceInvite(*_message);
}

void RemoteAssistanceInviteContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->remoteAssistanceInvite);
}

///////////////////////////////////////////
RemoteAssistanceAcceptContentMsg::RemoteAssistanceAcceptContentMsg() {
	this->_message = new RemoteAssistanceAcceptContent();
}

RemoteAssistanceAcceptContentMsg::RemoteAssistanceAcceptContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new RemoteAssistanceAcceptContent();
}

void RemoteAssistanceAcceptContentMsg::EncodeBody() {
	_body->__set_remoteAssistanceAccept(*_message);
}

void RemoteAssistanceAcceptContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->remoteAssistanceAccept);
}

///////////////////////////////////////////
RemoteAssistanceRejectContentMsg::RemoteAssistanceRejectContentMsg() {
	this->_message = new RemoteAssistanceRejectContent();
}

RemoteAssistanceRejectContentMsg::RemoteAssistanceRejectContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new RemoteAssistanceRejectContent();
}

void RemoteAssistanceRejectContentMsg::EncodeBody() {
	_body->__set_remoteAssistanceReject(*_message);
}

void RemoteAssistanceRejectContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->remoteAssistanceReject);
}

///////////////////////////////////////////
RemoteAssistanceEndContentMsg::RemoteAssistanceEndContentMsg() {
	this->_message = new RemoteAssistanceEndContent();
}

RemoteAssistanceEndContentMsg::RemoteAssistanceEndContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new RemoteAssistanceEndContent();
}

void RemoteAssistanceEndContentMsg::EncodeBody() {
	_body->__set_remoteAssistanceEnd(*_message);
}

void RemoteAssistanceEndContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->remoteAssistanceEnd);
}

///////////////////////////////////////////
SessionTopContentMsg::SessionTopContentMsg() {
	this->_message = new SessionTopContent();
}

SessionTopContentMsg::SessionTopContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new SessionTopContent();
}

void SessionTopContentMsg::EncodeBody() {
	_body->__set_sessionTop(*_message);
}

void SessionTopContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->sessionTop);
}



///////////////////////////////////////////
AtMessageReadedContentMsg::AtMessageReadedContentMsg() {
	this->_message = new AtMessageReadedContent();
}

AtMessageReadedContentMsg::AtMessageReadedContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new AtMessageReadedContent();
}

void AtMessageReadedContentMsg::EncodeBody() {
	_body->__set_atMessageReaded(*_message);
}

void AtMessageReadedContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->atMessageReaded);
}


///////////////////////////////////////////
UserLoginNotifyContentMsg::UserLoginNotifyContentMsg() {
	this->_message = new UserLoginNotifyContent();
}

UserLoginNotifyContentMsg::UserLoginNotifyContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new UserLoginNotifyContent();
}

void UserLoginNotifyContentMsg::EncodeBody() {
	_body->__set_loginNotify(*_message);
}

void UserLoginNotifyContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->loginNotify);
}

///////////////////////////////////////////
APITextContentMsg::APITextContentMsg() {
	this->_message = new APITextContent();
}

APITextContentMsg::APITextContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APITextContent();
}

void APITextContentMsg::EncodeBody() {
	_body->__set_apiText(*_message);
}

void APITextContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiText);
}

//////////////////////////////////////////
APIImageContentMsg::APIImageContentMsg() {
	this->_message = new APIImageContent();
}

APIImageContentMsg::APIImageContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APIImageContent();
}

void APIImageContentMsg::EncodeBody() {
	_body->__set_apiImage(*_message);
}

void APIImageContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiImage);
}

///////////////////////////////////////////
APIFileContentMsg::APIFileContentMsg() {
	this->_message = new APIFileContent();
}

APIFileContentMsg::APIFileContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APIFileContent();
}

void APIFileContentMsg::EncodeBody() {
	_body->__set_apiFile(*_message);
}

void APIFileContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiFile);
}

///////////////////////////////////////////
APIOAContentMsg::APIOAContentMsg() {
	this->_message = new APIOAContent();
}

APIOAContentMsg::APIOAContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APIOAContent();
}

void APIOAContentMsg::EncodeBody() {
	_body->__set_apiOA(*_message);
}

void APIOAContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiOA);
}

/////////////////////////////////////////
APIOAStatusChangeContentMsg::APIOAStatusChangeContentMsg() {
	this->_message = new APIOAStatusChangeContent();
}

APIOAStatusChangeContentMsg::APIOAStatusChangeContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APIOAStatusChangeContent();
}

void APIOAStatusChangeContentMsg::EncodeBody() {
	_body->__set_apiOAStatusChange(*_message);
}

void APIOAStatusChangeContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiOAStatusChange);
}

/////////////////////////////////////////
APIAppEventChangeNotifyMsg::APIAppEventChangeNotifyMsg() {
	this->_message = new APIAppEventChangeNotify();
}

APIAppEventChangeNotifyMsg::APIAppEventChangeNotifyMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APIAppEventChangeNotify();
}

void APIAppEventChangeNotifyMsg::EncodeBody() {
	_body->__set_apiEventNotify(*_message);
}

void APIAppEventChangeNotifyMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiEventNotify);
}

/////////////////////////////////////////
APIAppChangeNotifyContentMsg::APIAppChangeNotifyContentMsg(){
	this->_message = new APIAppChangeNotifyContent();
}

APIAppChangeNotifyContentMsg::APIAppChangeNotifyContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APIAppChangeNotifyContent();
}

void APIAppChangeNotifyContentMsg::EncodeBody() {
	_body->__set_apiChangeNotify(*_message);
}

void APIAppChangeNotifyContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiChangeNotify);
}

////////////////////////////////////////////
APIAckSentContentMsg::APIAckSentContentMsg() {
	this->_message = new APIAckSentContent(); 
}

APIAckSentContentMsg::APIAckSentContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APIAckSentContent();
}

void APIAckSentContentMsg::EncodeBody() {
	_body->__set_apiAckSent(*_message);
}

void APIAckSentContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiAckSent);
}

////////////////////////////////////////////
APICustomizedContentMsg::APICustomizedContentMsg() {
	this->_message = new APICustomizedContent();
}

APICustomizedContentMsg::APICustomizedContentMsg(MsgType type) {
	this->_msgType = type;
	this->_message = new APICustomizedContent();
}

void APICustomizedContentMsg::EncodeBody() {
	_body->__set_apiCustomized(*_message);
}

void APICustomizedContentMsg::DecodeBody(UcMessageBody *body) {
	swap(*this->_message, body->apiCustomized);
}

} //namespace uc