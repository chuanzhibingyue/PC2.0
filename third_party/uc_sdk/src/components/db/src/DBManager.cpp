///////////////////////////////////////////////////////////
//  DBManager.cpp
//  Implementation of the Class DBManager
//  Created on:      22-五月-2015 10:34:22
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "DBManager.h"
#include "AppDao.h"
#include "ContactDao.h"
#include "AccountDao.h"
#include "MessageDao.h"
#include "ConversationDao.h"
#include "ConferenceDao.h"
#include "GroupDao.h"
#include "ConfigDao.h"
#include "SQLConfig.h"
#include "SQLiteManager.h"
#include "LogManager.h"
#include "FileTransferDao.h"
#include "ProxyInfoDao.h"
#include "KeyValueDao.h"
#include "ExternalDao.h"
#ifdef PBX_SERVICE
#include "PbxDao.h"
#endif // PBX_SERVICE

namespace uc {

DBManager::DBManager() {

}

DBManager::~DBManager() {

}

DBManager& DBManager::GetInstance() {
	static DBManager instance;
	return instance;
}

int DBManager::InitCommonDB(const string& dbPath) {
	return SQLiteManager::GetInstance().InitCommonDB(dbPath);
}

int DBManager::InitUserDB(const string& dbPath) {
	int result = SQLiteManager::GetInstance().InitUserDB(dbPath);
	if (result == DBError::DB_SUCCESS) {
		DBManager::InitMessageID();
	}
	return result;
}

int DBManager::InitExternalDB(const string& dbPath) {
	return SQLiteManager::GetInstance().InitExternalDB(dbPath);
}

#ifdef PBX_SERVICE
int DBManager::InitPbxDB(const std::string& dbPath, const std::string &strVer) {
	return SQLiteManager::GetInstance().InitPbxDB(dbPath, strVer);
}
#endif // PBX_SERVICE

/////////////////////历史账户/////////////////////
int DBManager::SaveAccount(const AccountInfo& data) {
	return AccountDao().SaveAccount(data);
}

int DBManager::SaveAccount(const list<AccountInfo>& accountList) {
	return AccountDao().SaveAccount(accountList);
}
int DBManager::GetAccount(const string& userAccount, AccountInfo& accountInfo) {
	return AccountDao().GetAccount(userAccount, accountInfo);
}

int DBManager::GetAccountList(list<AccountInfo>& accountList) {
	return AccountDao().GetAccountList(accountList);
}

int DBManager::DeleteAccount(const string& userAccount) {
	return AccountDao().DeleteAccount(userAccount);
}

int DBManager::SetAutoLogin(const std::string& userAccount, bool autoLogin) {
	return AccountDao().SetAutoLogin(userAccount, autoLogin);
}

int DBManager::RememberPassword(const string& userAccount, bool remember) {
	return AccountDao().RememberPassword(userAccount, remember);
}

int DBManager::UpdateAccountAvatar(const std::string &userAccount, const std::string &localAvatar) {
	return AccountDao().UpdateAvatar(userAccount, localAvatar);
}

//////////////////////Proxy//////////////////////
int DBManager::SaveProxyAuthInfo(const ProxyInfo& proxyInfo)
{
	return ProxyInfoDao().SaveProxyAuthInfo(proxyInfo);
}

int DBManager::GetAuthInfo(ProxyInfo& proxyInfo)
{
	return ProxyInfoDao().GetAuthInfo(proxyInfo);
}

int DBManager::DeleteAuthInfo(const std::string& proxy_url)
{
	return ProxyInfoDao().DeleteAuthInfo(proxy_url);
}

//////////////////////消息///////////////////////
void DBManager::InitMessageID() {
	return MessageID::GetInstance().InitMessageID();
}

int32_t DBManager::GetMessageID() {
	return MessageID::GetInstance().GetMsgId();
}

int DBManager::SaveMessage(MessageStoreItem& data) {
	return MessageDao().SaveMsg(data);
}

int DBManager::SaveMessage(list<MessageStoreItem*>& messageList) {
	return MessageDao().SaveMsg(messageList);
}

int DBManager::SyncMsg(list<MessageStoreItem*>& messageList) {
	return MessageDao().SyncMsg( messageList );
}

int DBManager::GetMessage(int messageId, MessageStoreItem& data) {
	return MessageDao().GetMsg(messageId, data);
}

int DBManager::GetMessage(int32_t conversation_type, int64_t relate_id, int64_t seq, MessageStoreItem& message){
	return MessageDao().GetMsg(conversation_type, relate_id, seq, message ); 
}

int DBManager::GetMessage(int32_t conversationType, int64_t relateId, int64_t startTime, int64_t endTime, int count, list<MessageStoreItem*>& message) {
	return MessageDao().GetMsgContentList(conversationType, relateId, startTime, endTime, count, message);
}

int DBManager::SetMessageStatusById(int messageId, int state) {
	return MessageDao().SetMessageStatusById(messageId, state);
}

int DBManager::SetMessageStatusBySeq(int64_t messageSeq, int state) {
	return MessageDao().SetMessageStatusBySeq(messageSeq, state);
}

int DBManager::SetMessageSeq(int msgId, int64_t time, int64_t seq) {
	return MessageDao().SetMessageSeq(msgId, time, seq);
}

int DBManager::GetMaxMessageSeq(int32_t conversation_type, int64_t relate_id, int64_t& max_seq)
{
	return MessageDao().GetMaxMessageSeq(conversation_type, relate_id, max_seq);
}

int DBManager::UpdateMessageInfo(int msgId, MsgSendState state, int64_t timeStamp, int64_t seq, int32_t conversation) {
	return MessageDao().UpdateMessageInfo(msgId, state, timeStamp, seq, conversation);
}

int DBManager::DeleteMessage(int msgId) {
	return MessageDao().DeleteMessage(msgId);
}

int DBManager::UpdateMessageFlag(int msgId) {
	return MessageDao().UpdateMessageFlag(msgId);
}

int32_t DBManager::UpdateRevocationMsgState(int32_t type, int64_t seqId, int64_t relateId, int32_t state)
{
	return MessageDao().UpdateRevocationMsgState(type, seqId, relateId, state);
}

/////////////////////会话////////////////////////

int DBManager::LoadLatestConversations(int32_t number, std::list<ConversationInfo*> &conversations) {
	return ConversationDao().LoadLatestConversations(number, conversations);
}

int DBManager::SaveConversations(std::list<ConversationInfo*>& conversations) {
	return ConversationDao().SaveConversations( conversations );
}

int DBManager::DeleteConversation(ConversationType type, int64_t relateId){
	return ConversationDao().DeleteConversation(type, relateId);
}

int DBManager::GetDeleteTime(ConversationType type, int64_t relateId, int64_t & removeTime)
{
	return ConversationDao().GetDeleteTime(type, relateId, removeTime);
}

int32_t DBManager::SaveConversationInfo(int32_t type, int64_t msgSeq, int64_t msgTime) {
	return ConversationDao().SaveConversationInfo(type, msgSeq, msgTime);
}

int DBManager::GetMaxMessageSeqData(int64_t &msgSeq) {
	return ConversationDao().GetMaxMessageSeqData(msgSeq);
}

int DBManager::RemoveConversation(int32_t type, int64_t relateId) {
	return ConversationDao().RemoveConversation(type, relateId);
}

int DBManager::RemoveAllConversations() {
	return ConversationDao().RemoveAllConversations();
}

/////////////////////联系人///////////////////////
int DBManager::SaveContact(const ContactInfoImpl& data) {
	return ContactDao().SaveContact(data);
}

int DBManager::SaveContact(const list<ContactInfoImpl*>& contactList) {
	return ContactDao().SaveContact(contactList);
}
int DBManager::GetContact(int userId, ContactInfoImpl& data) {
	return ContactDao().GetContact(userId, data);
}

int DBManager::GetContact(const string& account, ContactInfoImpl& data){
	return ContactDao().GetContact(account, data);
}

int DBManager::GetContactListByType(int type, list<ContactInfoImpl*>& data) {
	return ContactDao().GetContactListByType(type, data);
}

int DBManager::GetContactListByName(const string& keyWord, int type, list<ContactInfoImpl*>& contactList) {
	return ContactDao().GetContactListByName(keyWord, type, contactList);
}

int DBManager::GetContactListByUserID(const list<int>& userIDList, list<ContactInfoImpl*>& contactList) {
	return ContactDao().GetContactListByUserID(userIDList, contactList);
}

int DBManager::UpdateContact(int userId, const ContactInfoImpl& data) {
	return ContactDao().UpdateContact(userId, data);
}

int DBManager::UpdateLastUpdateTime(int userId, int64_t timestamp) {
	return ContactDao().UpdateLastUpdateTime(userId, timestamp);
}

int DBManager::UpdateLastUpdateTimeByTypes(int type, int64_t timestamp) {
	return ContactDao().UpdateLastUpdateTimeByTypes(type, timestamp);
}

int DBManager::UpdateUserType(int userId, int type) {
	return ContactDao().UpdateUserType(userId, type);
}

// string DBManager::GetContactLocalAvatar(int userId) {
// 	return ContactDao().GetContactLocalAvatar(userId);
// }

//////////////////////会议///////////////////////
int DBManager::SaveMeeting(const Meeting& data) {
	return ConferenceDao().SaveMeeting(data);
}

int DBManager::SaveMeetings(const vector<Meeting>& data) {
	return ConferenceDao().SaveMeetings(data);
}

int DBManager::GetMeeting(Meeting& meeting, int64_t eventId, int64_t startTime/* = 0*/, int32_t authorizerId/* = 0*/) {
	return ConferenceDao().GetMeetingByEventId(eventId, startTime, authorizerId, meeting);
}

int DBManager::GetMeeting(Meeting& meeting, const std::string& confId) {
	return ConferenceDao().GetMeetingByConfId(confId, meeting);
}

int DBManager::UpdateMeeting(const Meeting& data, int64_t eventId, int64_t oriStart /*= 0*/, int32_t authorizerId /*= 0*/) {
	ConferenceDao().DeleteMeetingByEventId(eventId, oriStart, authorizerId);
	return ConferenceDao().SaveMeeting(data);
}

int DBManager::UpdateAttendees(const std::vector<Attendee>& attendees, int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/) {
	return ConferenceDao().UpdateAttendees(eventId, startTime, authorizerId, attendees);
}

int DBManager::AddAttendees(const std::vector<Attendee>& attendees, int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/) {
	return ConferenceDao().SaveAttendees(eventId, startTime, authorizerId, attendees);
}


int DBManager::UpdateMeetingStatus(uc::MeetingStatus status, int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/) {
	return ConferenceDao().UpdateMeetingStatus(status, eventId, startTime, authorizerId);
}

int DBManager::UpdateAttendeeStatus(uc::AttendeeStatus status, const std::string& account, int64_t eventId, int64_t startTime/*= 0*/, int32_t authorizerId /*= 0*/) {
	return ConferenceDao().UpdateAttendeeStatus(status, account, eventId, startTime, authorizerId);
}

int DBManager::GetMeetingByParentId(int64_t parentId, int64_t oriTime, int32_t authorizerId, Meeting& data) {
	return ConferenceDao().GetConferenceByParentId(parentId, oriTime, authorizerId, data);
}

int DBManager::GetMeetingByIds(vector<Meeting>& data, const vector<int64_t>& eventIds, int64_t startTime, int64_t endTime, int32_t authorizerId/* = 0*/) {
	return ConferenceDao().GetMeetingByIds(eventIds, startTime, endTime, authorizerId, data);
}
int DBManager::GetMeetingList(vector<Meeting>& data, int64_t startTime, int64_t endTime, int32_t limit/* = 200*/, int32_t authorizerId/* = 0*/, int64_t eventIdFilter /*= 0*/, bool bFilterAsc/* = true*/) {
	return ConferenceDao().GetMeetingList(authorizerId, startTime, endTime, limit, eventIdFilter, bFilterAsc, data);
}

int DBManager::GetOldMeetingList(vector<Meeting>& data, int64_t endTime, int32_t limit /*= 200*/, int32_t authorizerId /*= 0*/, int64_t eventIdFilter /*= 0*/) {
	return ConferenceDao().GetOldMeetingList(authorizerId, endTime, limit, eventIdFilter, data);
}

int DBManager::GetNewMeetingList(vector<Meeting>& data, int64_t startTime, int32_t limit /*= 200*/, int32_t authorizerId /*= 0*/, int64_t eventIdFilter /*= 0*/) {
	return ConferenceDao().GetNewMeetingList(authorizerId, startTime, limit, eventIdFilter, data);
}

int DBManager::DeleteMeetingByEventId(int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/) {
	return ConferenceDao().DeleteMeetingByEventId(eventId, startTime, authorizerId);
}

int DBManager::DeleteMeetings(const vector<Meeting>& data) {
	return ConferenceDao().DeleteMeetings(data);
}

int DBManager::DeleteMeetingsWithCommonEventId(int64_t eventId, int64_t startTime, int64_t endTime) {
	return ConferenceDao().DeleteMeetingsWithCommonEventId(eventId, startTime, endTime);
}

int DBManager::DeleteMeetingsWithTimeRange(int64_t startTime, int64_t endTime, int32_t limit /*= 200*/, int64_t eventIdFilter/* = 0*/, bool bfilterAsc /*= true*/, int32_t authorizerId /*= 0*/) {
	return ConferenceDao().DeleteMeetingsWithTimeRange(startTime, endTime, limit, eventIdFilter, bfilterAsc, authorizerId);
}

int DBManager::SaveAccreditUser(const Accrediting& data) {
	return ConferenceDao().SaveAccreditUser(data);
}

int DBManager::GetAccreditUserList(vector<Accrediting>& data) {
	return ConferenceDao().GetAccreditUserList(data);
}

int DBManager::CleanupMeetings() {
    return ConferenceDao().Cleanup();
}

int DBManager::SetAllMeetingOld() {
	return ConferenceDao().InvalidateData();
}
///////////////////////讨论组///////////////////////
// int DBManager::SaveGroup(const GroupInfo& data) {
// 	return GroupDao().SaveGroup(data);
// }

int DBManager::SaveGroup(const list<GroupInfo>& data) {
	return GroupDao().SaveGroup(data);
}

int DBManager::GetAllGroup(OUT list<GroupInfo>&data)
{
	return GroupDao().GetAllGroup(data);
}

int DBManager::GetGroup(int64_t groupId, GroupInfo& data) {
	return GroupDao().GetGroup(groupId, data);
}


int DBManager::SetGroupLocalAvatar(int64_t groupId, const string& avatar){
	return GroupDao().SetGroupLocalAvatar(groupId, avatar);
}

int DBManager::SetGroupStatus(int64_t groupId, int state) {
	return GroupDao().SetGroupStatus(groupId, state);
}

string DBManager::GetGroupLocalAvatar(int64_t groupId) {
	return GroupDao().GetGroupLocalAvatar(groupId);
}

///////////////////////配置参数///////////////////////
int DBManager::SaveValueByKey(const string& application, const string& key, const string& value)
{
	return KeyValueDao().SaveValueByKey(application, key, value);
}

int DBManager::GetValueByKey(const string& application, const string& key, string& value)
{
	return KeyValueDao().GetValueByKey(application, key, value);
}

///////////////////////配置参数//////////////////////
int DBManager::SaveConfigValue(const string& application, const string& key, const string& value) {
	return ConfigDao().SaveConfigValue(application, key, value);
}

int DBManager::SaveConfigValue(const string& application, const map<string, string>& value) {
	return ConfigDao().SaveConfigValue(application, value);
}

int DBManager::GetConfigValue(const string& application, const string& key, string& value) {
	return ConfigDao().GetConfigValue(application, key, value);
}

int DBManager::GetConfigValue(const string& application, map<string, string>& valueMap) {
	return ConfigDao().GetConfigValue(application, valueMap);
}

int DBManager::UpdateConfigValue(const string& application, const string& key, const string& value) {
	return ConfigDao().UpdateConfigValue(application, key, value);
}

int DBManager::UpdateConfigValue(const string& application, const map<string, string>& value) {
	return ConfigDao().UpdateConfigValue(application, value);
}

///////////////////////搜索//////////////////////
int DBManager::SearchContact(const string& keyWord, int32_t page, int32_t count, list<ContactInfo*>& contactList) {
	return ContactDao().GetContactListByName(keyWord, page, count, contactList);
}

int DBManager::SearchGroup(const string& keyWord, int32_t page, int32_t count, list<GroupInfo>& groupList) {
	return GroupDao().GetGroup(keyWord, page, count, groupList);
}

//////////////////////FileTransfer本地路径信息///////////////////////
int32_t DBManager::GetFSLocalPath(const string &fileURL, string &filePath, bool ExcludeIfNotExist) {
	return FileTransferDao().GetFSLocalPath(fileURL, filePath, ExcludeIfNotExist);
}

int32_t DBManager::GetFSLocalPath(const std::list<string> &fileURLs, std::map<string, string> &filePaths, bool ExcludeIfNotExist) {
	return FileTransferDao().GetFSLocalPath(fileURLs, filePaths, ExcludeIfNotExist);
}

int32_t DBManager::UpdateFSLocalPath(const string &fileURL, const string &filePath) {
	return FileTransferDao().UpdateFSLocalPath(fileURL, filePath);
}

int32_t DBManager::GetFSLocalPathAndSize(const std::string &fileURL, std::string &filePath, int64_t &fileSize) {
	return FileTransferDao().GetFSLocalPathAndSize(fileURL, filePath, fileSize);
}

int32_t DBManager::UpdateFSLocalPathAndSize(const string &fileURL, const string &filePath, int64_t fileSize) {
	return FileTransferDao().UpdateFSLocalPathAndSize(fileURL, filePath, fileSize);
}

int DBManager::GetConferenceIdByEventId(int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/ ) {
	return ConferenceDao().GetConferenceIdByEventId(eventId, startTime, authorizerId);
}

int64_t DBManager::GetEventIdByGroupId(int64_t groupId){
	return ConferenceDao().GetEventIdByGroupId(groupId);
}

int64_t DBManager::GetEventIdByConferenceId(int32_t conferenceId, int32_t authorizerId) {
	return ConferenceDao().GetEventIdByConferenceId(conferenceId, authorizerId);
}

int32_t DBManager::SaveCallPhoneNum(const string& phoneNum) {
	return ConferenceDao().SaveCallPhoneNum(phoneNum);
}

int32_t DBManager::GetHistoryPhoneNum(list<string>& phoneNums) {
	return ConferenceDao().GetHistoryPhoneNum(phoneNums);
}
int32_t DBManager::HaveData(string externalId)
{
	return ExternalDao().HaveData(externalId);
}

int32_t DBManager::GetExternalMessage(int32_t conversationType, string externalId, int64_t startTime,
	int64_t endTime, int count, list<ExternalMessage*>& message){
	return ExternalDao().GetMsgContentList(conversationType, externalId, startTime, endTime, count, message);
}

#ifdef PBX_SERVICE

int32_t DBManager::GetPbxCallRecordLst(int64_t timeStamp, int count, int selMask, PbxCallRecordList &lstData) {
	return PbxDao().GetCallRecordLst(timeStamp, count, selMask, lstData);
}

int32_t DBManager::SelectPbxCallDetails(const AC_PbxCallRecord &cond,
	int64_t timeStamp, int count, int selMask, PbxCallRecordList &lstData) {
	return PbxDao().SelectCallDetails(cond, timeStamp, count, selMask, lstData);
}

int32_t DBManager::SearchPbxCallRecord(const string& keyWord, int64_t timeStamp,
	int32_t count, PbxCallRecordList &lstData) {
	return PbxDao().SearchCallRecord(keyWord, timeStamp, count, lstData);
}

int32_t DBManager::InsertPbxCallDetails(const AC_PbxCallRecord &obj) {
	return PbxDao().InsetCallDetails(obj);
}

int32_t DBManager::UpdatePbxCallDetails(const AC_PbxCallRecord &cond, const AC_PbxCallRecord &data) {
	return PbxDao().UpdateCallDetails(cond, data);
}

#endif // PBX_SERVICE

} //namespace uc
