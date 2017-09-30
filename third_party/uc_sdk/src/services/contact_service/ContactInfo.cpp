#include "ContactInfo.h"
#include "ContactInfoImpl.h"

#include <cassert>

namespace uc {

ContactInfo::ContactInfo() {
	_contactInfoImpl = ContactInfoImpl::CreateContactInfoImpl();
}

ContactInfo::ContactInfo(const ContactInfo& other) {
	_contactInfoImpl = ContactInfoImpl::CreateContactInfoImpl();
	*_contactInfoImpl = *(other._contactInfoImpl);
}

ContactInfo::ContactInfo(ContactInfoImpl* impl) : _contactInfoImpl(impl) {
	assert(_contactInfoImpl != NULL);
}

ContactInfo::~ContactInfo() {
	delete _contactInfoImpl;
}

ContactInfo& ContactInfo::operator=(const ContactInfo& other) {
    if (this == &other) {
        return *this;
    }
    
    *_contactInfoImpl = *(other._contactInfoImpl);

    return *this;
}

ContactInfo& ContactInfo::operator=(const ContactInfoImpl& other) {
    *_contactInfoImpl = other;

    return *this;
}

void ContactInfo::Reset(ContactInfoImpl* impl) {
	if (_contactInfoImpl != NULL) {
		delete _contactInfoImpl;
	}

	_contactInfoImpl = impl;
}

int32_t ContactInfo::GetUserId() const {
	return _contactInfoImpl->GetUserId();
}

void ContactInfo::SetUserId(int32_t userId) {
	_contactInfoImpl->SetUserId(userId);
}

int32_t ContactInfo::GetSiteId() const {
	return _contactInfoImpl->GetSiteId();
}

void ContactInfo::SetSiteId(int32_t siteId) {
	_contactInfoImpl->SetSiteId(siteId);
}

int64_t ContactInfo::GetCardVersion() const {
	return _contactInfoImpl->GetCardVersion();
}

void ContactInfo::SetCardVersion(int64_t cardVersion) {
	_contactInfoImpl->SetCardVersion(cardVersion);
}

ContactInfo::Status ContactInfo::GetStatus() const {
	return _contactInfoImpl->GetStatus();
}

void ContactInfo::SetStatus(Status status) {
	_contactInfoImpl->SetStatus(status);
}

const std::string& ContactInfo::GetDisplayName() const {
	return _contactInfoImpl->GetDisplayName();
}

void ContactInfo::SetDisplayName(const std::string& displayName) {
	_contactInfoImpl->SetDisplayName(displayName);
}

const std::string& ContactInfo::GetPingyin() const {
	return _contactInfoImpl->GetPingyin();
}

void ContactInfo::SetPingyin(const std::string& pingyin) {
	_contactInfoImpl->SetPingyin(pingyin);
}

ContactInfo::Sex ContactInfo::GetSex() const {
	return _contactInfoImpl->GetSex();
}

void ContactInfo::SetSex(Sex sex) {
	_contactInfoImpl->SetSex(sex);
}

const std::string& ContactInfo::GetAccount() const {
	return _contactInfoImpl->GetAccount();
}

void ContactInfo::SetAccount(const std::string& account) {
	_contactInfoImpl->SetAccount(account);
}

const std::string& ContactInfo::GetEmail() const {
	return _contactInfoImpl->GetEmail();
}

void ContactInfo::SetEmail(const std::string& email) {
	_contactInfoImpl->SetEmail(email);
}

const std::string& ContactInfo::GetMobile() const{
	return _contactInfoImpl->GetMobile();
}

void ContactInfo::SetMobile(const std::string& mobile) {
	_contactInfoImpl->SetMobile(mobile);
}

const std::string& ContactInfo::GetWorkPhone() const {
	return _contactInfoImpl->GetWorkPhone();
}

void ContactInfo::SetWorkPhone(const std::string& workPhone) {
	_contactInfoImpl->SetWorkPhone(workPhone);
}

const std::string& ContactInfo::GetLocalAvatar() const {
	return _contactInfoImpl->GetLocalAvatar();
}

void ContactInfo::SetLocalAvatar(const std::string& localAvatar) {
	_contactInfoImpl->SetLocalAvatar(localAvatar);
}

const std::string& ContactInfo::GetRemoteAvatarUrl() const {
	return _contactInfoImpl->GetRemoteAvatarUrl();
}

void ContactInfo::SetRemoteAvatarUrl(const std::string& remoteAvatarUrl) {
	_contactInfoImpl->SetRemoteAvatarUrl(remoteAvatarUrl);
}

const std::string& ContactInfo::GetPersonalSign() const {
	return _contactInfoImpl->GetPersonalSign();
}

void ContactInfo::SetPersonalSign(const std::string& personalSign) {
	_contactInfoImpl->SetPersonalSign(personalSign);
}

const std::string& ContactInfo::GetUserTags() const{
	return _contactInfoImpl->GetUserTags();
}

void ContactInfo::SetUserTags(const std::string& userTags){
	_contactInfoImpl->SetUserTags(userTags);
}

int32_t ContactInfo::GetDepartmentId() const {
	return _contactInfoImpl->GetDepartmentId();
}

void ContactInfo::SetDepartmentId(int32_t departmentId) {
	_contactInfoImpl->SetDepartmentId(departmentId);
}

const std::string& ContactInfo::GetDepartmentName() const {
	return _contactInfoImpl->GetDepartmentName();
}

void ContactInfo::SetDepartmentName(const std::string& departmentName) {
	_contactInfoImpl->SetDepartmentName(departmentName);
}

const std::string& ContactInfo::GetPosition() const {
	return _contactInfoImpl->GetPosition();
}

void ContactInfo::SetPosition(const std::string& position) {
	_contactInfoImpl->SetPosition(position);
}

int32_t ContactInfo::GetSuperiorId() const {
	return _contactInfoImpl->GetSuperiorId();
}

void ContactInfo::SetSuperiorId(int32_t superiorId) {
	_contactInfoImpl->SetSuperiorId(superiorId);
}

const std::string& ContactInfo::GetSuperiorName() const {
	return _contactInfoImpl->GetSuperiorName();
}

void ContactInfo::SetSuperiorName(const std::string& superiorName) {
	_contactInfoImpl->SetSuperiorName(superiorName);
}

int32_t ContactInfo::GetColleagueNum() const {
	return _contactInfoImpl->GetColleagueNum();
}

void ContactInfo::SetColleagueNnum(int32_t colleagueNum) {
	_contactInfoImpl->SetColleagueNnum(colleagueNum);
}
    
ContactInfo::Type ContactInfo::GetType() const {
	return _contactInfoImpl->GetType();
}

void ContactInfo::SetType(Type type) {
	_contactInfoImpl->SetType(type);
}

int32_t ContactInfo::GetRelation() const {
	return _contactInfoImpl->GetRelation();
}

void ContactInfo::SetRelation(int32_t relation) {
	_contactInfoImpl->SetRelation(relation);
}

BasicContactInfo::BasicContactInfo(const ContactInfoImpl* impl) : _userId(impl->GetUserId()), 
	_account(impl->GetAccount()), _displayName(impl->GetDisplayName()), 
	_localAvatar(impl->GetLocalAvatar()), _remoteAvatar(impl->GetRemoteAvatarUrl()), 
	_personalSign(impl->GetPersonalSign()), _pinyin(impl->GetPingyin()), 
	_position(impl->GetPosition()), _departmentId(impl->GetDepartmentId()), 
	_departmentName(impl->GetDepartmentName()), _relation(impl->GetRelation()), _siteId(impl->GetSiteId()) {
}

}
