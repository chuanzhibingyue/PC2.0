/*
 * GroupService.cpp
 *
 *  Created on: Jun 2, 2015
 *      Author: ethan
 */

#include "GroupService.h"
#include "UCEngine.h"
#include "client_service/ClientService.h"
#include "UCErrorCode.h"
#include "boost/property_tree/json_parser.hpp"
#include "boost/foreach.hpp"
#include "DBManager.h"
#include "message_service/MessageProcessor.h"
#include "jsonhelper.h"

namespace uc {

	GroupService::GroupService(IUCEngine *engine) :_batchUploadTimerId(-1){
	_engine = engine;
	_engine->registerListener(this);
	((UCEngine*)_engine)->RegisterQueueNetMgrListener(this);
}

GroupService::~GroupService() {
	StopService();
}

int32_t GroupService::StopService(){
	if (!_list_group_call_cache.empty())
		_list_group_call_cache.clear();

	return UC_SUCCEEDED;
}

void GroupService::OnLoginSucceeded(){
	// 登陆成功加载数据， 这样在UI 展示conversation list的时候，就可以拿到group info.
	theStore.GetGroupListStore()->LoadDataFromDB();

}

void GroupService::OnUCASConnected(int32_t errorCode){
	if (errorCode == UC_SUCCEEDED){
		theStore.GetGroupListStore()->ResetSync();
		//ucas连接成功去服务器获取更新的group list
		GetGroupList(ALL_GROUP_TYPE, 1, SYNC_GROUP_NUMBER, 0, true, true);

		//清除@消息缓存
		_atMsgCache.clear();
	}	
}

void GroupService::OnSendMessageResponse(const std::vector<MessageResponse> &messageResponses) {
	for (std::size_t i = 0; i < messageResponses.size(); ++i) {
		if (messageResponses[i].type == PROJECT_CHAT || messageResponses[i].type == GROUP_CHAT ||
			messageResponses[i].type == CONFERENCE_CHAT || messageResponses[i].type == CLOUD_CHAT) {
			if (UC_SUCCEEDED == messageResponses[i].code){
				theStore.GetMsgStore()->UpdateGroupMessageState(messageResponses[i].userId, messageResponses[i].msgId, STATE_SENDSUCCESS, messageResponses[i].msgSeq);

			}
			else{
				theStore.GetMsgStore()->UpdateGroupMessageState(messageResponses[i].userId, messageResponses[i].msgId, STATE_SENDFAIL, messageResponses[i].msgSeq);
			}

			triggerEvent(IGroupServiceEvent, OnMessageSent(messageResponses[i].code, messageResponses[i].type,
				messageResponses[i].userId, messageResponses[i].msgId, messageResponses[i].msgSeq, messageResponses[i].sendTime));
		}
	}
}

int32_t GroupService::UpdateGroupLogo(int64_t groupId, const std::string& localPath){
	return theStore.GetGroupListStore()->UpdateGroupLogo(groupId, localPath);
}

int32_t GroupService::GetGroupMemberCount(int64_t groupId){
	GroupInfo groupInfo;
	theStore.GetGroupListStore()->GetGroupInfo(groupId, groupInfo);
	
	return (groupInfo.GetMember().size() - 1);
}

UserIdList GroupService::GetGroupMemberList(int64_t groupId){
	UserIdList list;
	GroupInfo groupInfo;
	theStore.GetGroupListStore()->GetGroupInfo(groupId, groupInfo);
	for (auto memberInfo : groupInfo.GetMember()){
		if (memberInfo.userId == GetCurUser().GetUserId()){
			continue;
		}
		list.push_back(memberInfo.userId);
	}
	return list;
}

DocumentList* GroupService::GetDocumentListById(int64_t groupId){
	DocumentMap::iterator it = _documentMap.find(groupId);
	if (it != _documentMap.end()){
		return &(it->second);
	}

	return NULL;
}

void GroupService::AddDocument(int64_t groupId, DocumentInfo &document){
	DocumentList * documentList = GetDocumentListById(groupId);
	if (documentList == NULL){
		DocumentList newlist;
		newlist.push_back(document);
		_documentMap.insert(make_pair(groupId, newlist));
	}
	else{
		DocumentInfo* info = GetDocumentInfoById(groupId, document.GetFileId());
		if (NULL == info){
			documentList->push_back(document);
		}
		else{
			*info = document;
		}
	}
}

void GroupService::AddDocumentList(int64_t groupId, DocumentList& documents) {
	DocumentMap::iterator it = _documentMap.find(groupId);
	if (it != _documentMap.end()){
		DocumentList oldlist = it->second;
		for (DocumentInfo &infoItem : oldlist){
			DocumentList::iterator iter = std::find_if(documents.begin(), documents.end(),
				[&](DocumentInfo &info){return info.GetFileId() == infoItem.GetFileId(); });
			if (iter != documents.end()){
				infoItem = *iter;
				documents.erase(iter);
			}
		}		
		oldlist.splice(oldlist.end(), documents);
	}
	else{
		_documentMap.insert(make_pair(groupId, documents));
	}
}

void GroupService::RemoveDocumentListById(int64_t groupId){
	DocumentList* documentList= GetDocumentListById(groupId);
	if (documentList) {
		documentList->clear();
	}

	_documentMap.erase(groupId);
}

DocumentInfo* GroupService::GetDocumentInfoById(int64_t groupId, int64_t fileId){
	DocumentList* documentList = GetDocumentListById(groupId);
	if (documentList == NULL)
		return NULL;

	DocumentList::iterator it = documentList->begin();
	for (; it != documentList->end(); ++it){
		if ((*it).GetFileId() == fileId){
			return &*it;
		}	
	}

	return NULL;
}

void GroupService::RemoveDocumentInfoById(int64_t groupId, int64_t fileId){
	DocumentList* docList = GetDocumentListById(groupId);
	if (docList == NULL) 
		return;
	
	DocumentList::iterator it = docList->begin();
	for(; it != docList->end(); ++it){
		const DocumentInfo& documentInfo = *it;
		if (fileId == documentInfo.GetFileId()){
			docList->erase(it);
			break;
		}
	}
}

UCID GroupService::GetCurUser(){
	UCID userUid;

	if (_engine){
		userUid = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetCurUser();
	}
	return 	userUid;
}

std::string GroupService::GetCurUserName(){
	std::string displayName = "";
	if (_engine){
		displayName = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetDisplayName();
	}
	return 	displayName;
}

bool GroupService::IsCurUser(UCID userUid){

	if (userUid.GetUserId() == GetCurUser().GetUserId()
		&& userUid.GetResId() == GetCurUser().GetResId()
		&& userUid.GetSiteId() == GetCurUser().GetSiteId()){
			return true;
	}
	return false;
}

std::string GroupService::GetFileFormat(std::string fileName){

	std::string fileFormat= "";
	std::size_t nPos = fileName.find_last_of(".");
	if (nPos != string::npos){
		fileFormat = fileName.substr(nPos+1);
	}
	return fileFormat;
}

void GroupService::GetDocumentInfo(int32_t type, boost::property_tree::wptree jsonData, DocumentInfo& docInfo){

	if (0 == type) {
		docInfo.SetFileId(jsonData.get<int64_t>(L"contentId"));
	} else if (1 == type) {
		docInfo.SetFileId(jsonData.get<int32_t>(L"id"));
	}

	docInfo.SetCreatorId(jsonData.get<int32_t>(L"user_id"));
	docInfo.SetCreatorName(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"user_name", L"")));
	docInfo.SetFileName(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"title", L"")));
	docInfo.SetFileFormat(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"format", L"")));

	std::wstring url = jsonData.get<wstring>(L"file_url", L""); 
	if (url.empty()) {
		url = jsonData.get<wstring>(L"url", L"");
	}
	docInfo.SetFileUrl(StringConvert::ToUTF8Str(url));

	docInfo.SetPreview(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"preview", L"")));
	docInfo.SetTotal(jsonData.get<int32_t>(L"total", 0));
	docInfo.SetUploadTime(jsonData.get<int64_t>(L"file_upload_time", 0));
	docInfo.SetCreateTime(jsonData.get<int64_t>(L"created", 0));
	docInfo.SetModifyTime(jsonData.get<int64_t>(L"modify", 0));
	docInfo.SetFileSize(jsonData.get<int64_t>(L"size", 0));
	docInfo.SetContentType(jsonData.get<int32_t>(L"content_type"));
	docInfo.SetTotalLines(jsonData.get<int32_t>(L"total_lines", 0));
	docInfo.SetThumbnail(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"thumbnail", L"")));
	docInfo.SetCommentCount(jsonData.get<int32_t>(L"comment_count", 0));
	docInfo.SetDescription(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"description", L"")));
	docInfo.SetCloudType(DocumentCloudType(jsonData.get<int32_t>(L"cloud_type", CT_FILE_TRANSFER)));
	docInfo.SetMessageSeq(jsonData.get<int64_t>(L"seq", 0));

	if (docInfo.GetCloudType() != CT_FILE_TRANSFER) {
		docInfo.SetHashCode(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"hash_code", L"")));
		docInfo.SetMountId(jsonData.get<int32_t>(L"mount_id", 0));
		docInfo.SetDetailType(jsonData.get<int16_t>(L"‘detail_type", 0));
		docInfo.SetDetailContent(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"detail_content", L"")));
	}	
}

void GroupService::GetCommentInfo(boost::property_tree::wptree jsonData, CommentInfo& commentInfo){
	commentInfo.SetFileId(jsonData.get<int64_t>(L"file_id", 0));
	commentInfo.SetFileType(jsonData.get<int32_t>(L"file_type", -1));
	commentInfo.SetFileTitle(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"title", L"")));
	commentInfo.SetCommentId(jsonData.get<int64_t>(L"comments_id", 0));
	commentInfo.SetUserId(jsonData.get<int32_t>(L"from_commenter_id", 0));
	commentInfo.SetUserName(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"user_name", L"")));
	commentInfo.SetContent(StringConvert::URLDecode(StringConvert::ToUTF8Str(jsonData.get<wstring>(L"content", L""))));
	commentInfo.SetCreateTime(jsonData.get<int64_t>(L"create_time", 0));
}

void GroupService::ParseGroupInfo(const rapidjson::Value &data, GroupInfo& info, bool isSearch) {
	info.SetId(jsonhelper::getIntValue(data, "group_id", 0));
	isSearch ? info.SetGroupType(jsonhelper::getIntValue(data, "group_type", 0)) : info.SetGroupType(jsonhelper::getIntValue(data, "is_display", -1));
	info.SetSiteId(jsonhelper::getIntValue(data, "site_id", 0));
	info.SetName(jsonhelper::getStrValue(data, "group_name", ""));
	info.SetPinYinName(jsonhelper::getStrValue(data, "group_pinyin", ""));

	info.SetMemberCount(jsonhelper::getIntValue(data, "member_count", 0));
	info.SetCreateTime(jsonhelper::getIntValue(data, "created", 0));
	info.SetLogoLevel(jsonhelper::getIntValue(data, "logo_level", 0));
	info.SetGroupStatus(data.HasMember("group_status") ? jsonhelper::getIntValue(data, "group_status", 1) & 1 : 1);
	info.SetMemberType(data.HasMember("group_status") ? (jsonhelper::getIntValue(data, "group_status", 0) >> 2) & 1 : 0);
	info.SetModifyTime(jsonhelper::getIntValue(data, "modified", 0));
	info.SetStatusInfo(jsonhelper::getStrValue(data, "status_info", ""));
	
	// 讨论组头像;
	if (info.GetGroupType() == PROJECT_GROUP_TYPE || info.GetGroupType() == CLOUD_GROUP_TYPE) //项目组;
	{
		info.SetAvatarIndex(jsonhelper::getStrValue(data, "group_logo", ""));
	}
	else if (info.GetGroupType() == MUTILCHAT_GROUP_TYPE) // 群组;
	{
		std::string avatar = "";

		if (data.HasMember("group_logo")) {
			const rapidjson::Value &group_logos = data["group_logo"];
			if (group_logos.IsArray()) {
				for (rapidjson::SizeType i = 0; i < group_logos.Size(); i++) {
					if (i != 0)
						avatar += ",";
					avatar += jsonhelper::getStrValue(group_logos[i], "");
				}
			}
			else {
				avatar = jsonhelper::getStrValue(group_logos, "");
			}
		}

		info.SetAvatarIndex(avatar);
	}

	// 解析mount_id
	if (info.GetGroupType() == CLOUD_GROUP_TYPE) {
		info.SetMountId(jsonhelper::getIntValue(data, "mount_id", 0));
	}
	if (info.GetGroupType() == MUTILCHAT_GROUP_TYPE)	{
		info.SetExternalId(jsonhelper::getStrValue(data, "external_group_id", ""));
	}

	info.SetNameFlag(jsonhelper::getIntValue(data, "name_flag", 0));
	info.SetAdminId(jsonhelper::getIntValue(data, "admin_id", 0));
	info.SetLocalAvatar(DBManager::GetInstance().GetGroupLocalAvatar(info.GetId()));

	string memberIds = isSearch ? "member_ids" : "member_list";
	if (data.HasMember(memberIds.c_str()) && data[memberIds.c_str()].IsArray()) {
		MemberList memberList;
		const rapidjson::Value &group_members = data[memberIds.c_str()];
		if (group_members.IsArray()){
			for (rapidjson::SizeType i = 0; i < group_members.Size(); i++) {
				const rapidjson::Value &group_member = group_members[i];

				MemberInfo memberInfo;
				if (isSearch) {
					memberInfo.userId = std::atoi(group_member.GetString());
				}
				else {
					memberInfo.userId = jsonhelper::getIntValue(group_member, "user_id", 0);
					memberInfo.joinTime = jsonhelper::getIntValue(group_member, "join_time", 0);

					//current user insert info;
					if (GetCurUser().GetUserId() == memberInfo.userId){
						info.SetAlertRule(jsonhelper::getIntValue(group_member, "rule", 0));
						info.SetAlertTime(jsonhelper::getIntValue(group_member, "remind", 0));
						info.SetGroupDisturb(jsonhelper::getIntValue(group_member, "is_disturb", 0));
						info.SetGroupTop(jsonhelper::getIntValue(group_member, "is_top", 0));
						info.SetJoinState(jsonhelper::getIntValue(group_member, "join_state", -1));
						if (0 != info.GetJoinState()){
							continue;
						}
					}
					else {
						//other users insert info;
						if (0 != jsonhelper::getIntValue(group_member, "join_state", -1)){
							continue;
						}
					}
				}

				memberList.push_back(memberInfo);
			}
		}
		if (!memberList.empty()){
			info.AddMember(memberList);
		}
	}
}

bool GroupService::GetGroupInfoByExternalId(const string& externalId, GroupInfo& info){
	return theStore.GetGroupListStore()->GetGroupInfoByExternalId(externalId, info);
}

void GroupService::CacheAtMsg(int64_t groupId, AtMsgInfo info) {
	std::map<int64_t, std::vector<AtMsgInfo>>::iterator it = _atMsgCache.find(groupId);
	if (it != _atMsgCache.end()) {
		it->second.push_back(info);
	}
	else{
		std::vector<AtMsgInfo> infos;
		infos.push_back(info);
		_atMsgCache[groupId] = infos;
	}
}

void GroupService::DeleteAtMsgFromCache(int64_t groupId, const std::vector<int64_t>& seqList) {
	std::map<int64_t, std::vector<AtMsgInfo>>::iterator mapIt = _atMsgCache.begin();
	while(mapIt != _atMsgCache.end()) {
		if (mapIt->first == groupId)
		{
			std::vector<AtMsgInfo>::iterator it = mapIt->second.begin();
			while (it != mapIt->second.end()) {
				for (int64_t seq : seqList) {
					if (it->_seq == seq){
						it = mapIt->second.erase(it);
					}
					else{
						it++;
					}
				}
			}
		}
		mapIt++;
	}

}

bool GroupService::GetAtMsgFromCache(int64_t groupId, std::vector<AtMsgInfo>& info)
{
	std::map<int64_t, std::vector<AtMsgInfo>>::iterator mapIt = _atMsgCache.begin();
	while (mapIt != _atMsgCache.end()) {
		if (groupId == mapIt->first) {
			info = mapIt->second;
			return true;
		}
		mapIt++;
	}

	return false;
}

void GroupService::CacheAtMsgList(int64_t groupId, std::map<int64_t, int64_t> infos)
{
	std::vector<AtMsgInfo> vInfos;
	std::map<int64_t, int64_t>::iterator it_1 = infos.begin();
	while (it_1 != infos.end()) {
		vInfos.push_back(AtMsgInfo(it_1->second, it_1->first));
		it_1++;
	}

	if (!vInfos.empty()) {
		std::map<int64_t, std::vector<AtMsgInfo>>::iterator it_2 = _atMsgCache.begin();
		while (it_2 != _atMsgCache.end()) {
			if (groupId == it_2->first) {
				it_2->second.insert(it_2->second.end(), vInfos.begin(), vInfos.end());
				return;
			}
			it_2++;
		}
	}

	//空的也存下来，不用再向server要了
	_atMsgCache[groupId] = vInfos;

}

} /* namespace uc */
