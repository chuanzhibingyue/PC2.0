#include "GkClientMessage.h"

namespace ucclient {

GkClientMessage::GkClientMessage(GkClientMessage&& other) {
	this->_doc = std::move(other._doc);
}

GkClientMessage& GkClientMessage::operator=(GkClientMessage&& other) {
	if (this != &other) {
		this->_doc = std::move(other._doc);
	}

	return *this;
}

void GkClientMessage::CreateMessage(int32_t mountId, const std::wstring& type, const std::wstring& subType) {
	WValue typeVal(rapidjson::kStringType);
	typeVal.SetString(type.c_str(), _doc.GetAllocator());
	_doc.SetObject().AddMember(L"type", typeVal, _doc.GetAllocator());

	WValue subTypeVal(rapidjson::kStringType);
	subTypeVal.SetString(subType.c_str(), _doc.GetAllocator());
	
	WValue bodyVal;
	bodyVal.SetObject().AddMember(L"type", subTypeVal, _doc.GetAllocator());
	
	if (mountId != 0) {
		WValue mountIdVal(mountId);
		bodyVal.AddMember(L"mount_id", mountIdVal, _doc.GetAllocator());
	}

	_doc.AddMember(L"body", bodyVal, _doc.GetAllocator());
}

CreateGroupMessage::CreateGroupMessage(int32_t mountId, const std::wstring& name, const std::wstring& url,
	const std::vector<GKClientUserInfo>& userInfo) {
	CreateMessage(mountId, L"member", L"add");

	WValue countVal(1);
	_doc[L"body"].AddMember(L"count", countVal, _doc.GetAllocator());

	WValue listVal;
	listVal.SetArray();

	for (std::size_t i = 0; i < userInfo.size(); ++i) {
		WValue value;
		value.SetObject();

		WValue MemberIdVal(userInfo[i].userId);
		value.AddMember(L"member_id", MemberIdVal, _doc.GetAllocator());

		WValue nameValue(rapidjson::kStringType);
		nameValue.SetString(uc::StringConvert::FromUTF8Str(userInfo[i].userName).c_str(), _doc.GetAllocator());
		value.AddMember(L"member_name", nameValue, _doc.GetAllocator());

		WValue permissionsValue(rapidjson::kStringType);
		permissionsValue.SetString(userInfo[i].role.c_str(), _doc.GetAllocator());
		value.AddMember(L"permissions", permissionsValue, _doc.GetAllocator());

		listVal.PushBack(value, _doc.GetAllocator());
	}

	_doc[L"body"].AddMember(L"list", listVal, _doc.GetAllocator());
}

CreateGroupMessage::CreateGroupMessage(WDocument& document) {
	CreateMessage(0, L"member", L"add");

	WValue countVal(1);
	_doc[L"body"].AddMember(L"count", countVal, _doc.GetAllocator());

	for (WValue::MemberIterator iter = document.MemberBegin();
		iter != document.MemberEnd(); ++iter) {
		_doc[L"body"].AddMember(iter->name, iter->value, _doc.GetAllocator());
	}
}

UpdateNameMessage::UpdateNameMessage(int32_t mountId, const std::wstring& name) {
	CreateMessage(mountId, L"mount", L"update_name");

	WValue nameValue(rapidjson::kStringType);
	nameValue.SetString(name.c_str(), _doc.GetAllocator());
	_doc[L"body"].AddMember(L"name", nameValue, _doc.GetAllocator());
}

UpdateUrlMessage::UpdateUrlMessage(int32_t mountId, const std::wstring& url){
	CreateMessage(mountId, L"mount", L"update_url");

	WValue urlValue(rapidjson::kStringType);
	urlValue.SetString(url.c_str(), _doc.GetAllocator());
	//_doc[L"body"].AddMember(L"update_url", url, _doc.GetAllocator());
}

AddMemberMessage::AddMemberMessage(int32_t mountId, const std::wstring& name, 
	const std::vector<GKClientUserInfo>& userInfo) {
	CreateMessage(mountId, L"member", L"add");

	WValue countVal(2);
	_doc[L"body"].AddMember(L"count", countVal, _doc.GetAllocator());

	WValue mountnameValue(rapidjson::kStringType);
	mountnameValue.SetString(name.c_str(), _doc.GetAllocator());
	_doc[L"body"].AddMember(L"mount_name", mountnameValue, _doc.GetAllocator());

	WValue listVal;
	listVal.SetArray();

	for (std::size_t i = 0; i < userInfo.size(); ++i) {
		WValue value;
		value.SetObject();

		WValue MemberIdVal(userInfo[i].userId);
		value.AddMember(L"member_id", MemberIdVal, _doc.GetAllocator());

		WValue nameValue(rapidjson::kStringType);
		nameValue.SetString(uc::StringConvert::FromUTF8Str(userInfo[i].userName).c_str(), _doc.GetAllocator());
		value.AddMember(L"member_name", nameValue, _doc.GetAllocator());

		WValue permissionsValue(rapidjson::kStringType);
		permissionsValue.SetString(userInfo[i].role.c_str(), _doc.GetAllocator());
		value.AddMember(L"permissions", permissionsValue, _doc.GetAllocator());

		listVal.PushBack(value, _doc.GetAllocator());
	}

	_doc[L"body"].AddMember(L"list", listVal, _doc.GetAllocator());
}

RemoveMemberMessage::RemoveMemberMessage(int32_t mountId, const std::wstring& name, 
	const std::vector<int32_t>& userIds) {
	CreateMessage(mountId, L"member", L"delete");

	WValue mountnameValue(rapidjson::kStringType);
	mountnameValue.SetString(name.c_str(), _doc.GetAllocator());
	_doc[L"body"].AddMember(L"mount_name", mountnameValue, _doc.GetAllocator());

	WValue listVal;
	listVal.SetArray();

	for (std::size_t i = 0; i < userIds.size(); ++i) {
		WValue value;
		value.SetObject();

		WValue MemberIdVal(userIds[i]);
		value.AddMember(L"member_id", MemberIdVal, _doc.GetAllocator());

		listVal.PushBack(value, _doc.GetAllocator());
	}

	_doc[L"body"].AddMember(L"list", listVal, _doc.GetAllocator());
}

}