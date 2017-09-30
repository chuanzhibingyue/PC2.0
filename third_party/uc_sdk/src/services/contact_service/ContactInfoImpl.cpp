#include "ContactInfoImpl.h"
#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Macro.h"
#include "LogManager.h"
#include "UCErrorCode.h"
#include "common/StringConvert.h"

namespace uc {

ContactInfoImpl::ContactInfoImpl() : _userId(0), _siteId(0), _cardVersion(-1), 
	_status(CS_OFFLINE), _displayName(""), _pingyin(""),
	_sex(CS_MAN), _account(""), _email(""), _mobile(""),
	_workPhone(""), _localAvatar(""), _remoteAvatar(""), _personalSign(""),
	_departmentId(0), _departmentName(""), _position(""), _superiorId(0),
	_superiorName(""),_usertags(""), _colleagueNum(0), _type(CTI_OTHERS), _relation(0) {
}

ContactInfoImpl::ContactInfoImpl(const ContactInfoImpl& info) : _userId(info._userId),
	_siteId(info._siteId), _cardVersion(info._cardVersion), _status(info._status), _displayName(info._displayName), 
	_pingyin(info._pingyin), _sex(info._sex), _account(info._account), _email(info._email), _mobile(info._mobile),
	_workPhone(info._workPhone), _localAvatar(info._localAvatar), _remoteAvatar(info._remoteAvatar),
	_personalSign(info._personalSign), _departmentId(info._departmentId),
	_departmentName(info._departmentName), _position(info._position), _superiorId(info._superiorId),
	_superiorName(info._superiorName), _usertags(info._usertags),_colleagueNum(info._colleagueNum), _type(info._type), _relation(info._relation) {

}

ContactInfoImpl::~ContactInfoImpl() {
}

ContactInfoImpl* ContactInfoImpl::CreateContactInfoImpl() {
	return new ContactInfoImpl();
}

ContactInfoImpl* ContactInfoImpl::CreateContactInfoImpl(const ContactInfoImpl& other) {
	return new ContactInfoImpl(other);
}

ContactInfoImpl* ContactInfoImpl::CreateContactInfoImpl(const boost::property_tree::wptree &ptParse, bool flag) {
	ContactInfoImpl *contact = new ContactInfoImpl();
    try {
        contact->_userId = ptParse.get<int32_t>(L"user_id");
        contact->_siteId = ptParse.get<int32_t>(L"site_id", 0);

        contact->_status = ContactInfo::Status(ptParse.get<int32_t>(L"status", CS_OFFLINE));
        contact->_cardVersion = ptParse.get<int64_t>(L"card_version", -1);
        contact->_displayName = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"display_name", L""));
      //  wcout << L"displayName: " << StringConvert::FromUTF8Str(contact->_displayName) << endl;

        contact->_pingyin = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"name_pinyin", L""));
		contact->_sex = ContactInfo::Sex(ptParse.get<int32_t>(L"sex", CS_MAN));
        contact->_account = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"user_account", L""));
        contact->_email = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"email", L""));

        contact->_mobile = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"mobile", L""));
        contact->_workPhone = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"fix_tel", L""));
        contact->_remoteAvatar = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"avatar", L""));
        contact->_personalSign = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"personal_sign", L""));

        contact->_departmentId = ptParse.get<int32_t>(L"department_id", 0);
        contact->_departmentName = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"department", L""));
        contact->_position = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"position", L""));
        contact->_superiorId = ptParse.get<int32_t>(L"leader_id", 0);
        contact->_superiorName = StringConvert::ToUTF8Str(ptParse.get<std::wstring>(L"leader_name", L""));
        contact->_colleagueNum = ptParse.get<int32_t>(L"colleague_num", 0);

		if (flag){
			boost::property_tree::wptree userTag_Array = ptParse.get_child(L"user_tags");
			std::string strusertags = "";
			BOOST_FOREACH(boost::property_tree::wptree::value_type &vt, userTag_Array){
				std::wstringstream str;
				boost::property_tree::json_parser::write_json(str, vt.second);
				strusertags += (StringConvert::ToUTF8Str(str.str()));
				strusertags += ",";
			}
			contact->_usertags = strusertags;
		}
		else{
			contact->_usertags = "";
		}

        return contact;
    } catch (boost::property_tree::ptree_error & e) {
    	UC_LOG(ERROR) << "encounter an error when constructing ContactInfoImpl object "
    		<< "from an json object, exception: " << e.what();

        delete contact;
        return NULL;
    }
}

int32_t ContactInfoImpl::Update(const boost::property_tree::wptree &ptParse) {
	try {

		boost::optional<int32_t> userId = ptParse.get_optional<int32_t>( L"user_id" );
		if (userId) {
			_userId = userId.get();
		}

		boost::optional<int32_t> siteId = ptParse.get_optional<int32_t>( L"site_id" );
		if (siteId) {
			_siteId = siteId.get();
		}

		boost::optional<int64_t> cardVersion = ptParse.get_optional<int64_t>( L"card_version" );
		if (cardVersion) {
			_cardVersion = cardVersion.get();
		}

		boost::optional<int32_t> status = ptParse.get_optional<int32_t>( L"status" );
		if (status) {
			_status =  ContactInfo::Status(status.get());
		}

		boost::optional<std::wstring> displayName = ptParse.get_optional<std::wstring>( L"display_name" );
		if (displayName) {
			_displayName = StringConvert::ToUTF8Str(displayName.get());
		}

		boost::optional<std::wstring> pingyin = ptParse.get_optional<std::wstring>( L"name_pinyin" );
		if (pingyin) {
			_pingyin = StringConvert::ToUTF8Str(pingyin.get());
		}

		boost::optional<int> sex = ptParse.get_optional<int>( L"sex" );
		if (sex) {
			_sex = ContactInfo::Sex(sex.get());
		}

		boost::optional<std::wstring> account = ptParse.get_optional<std::wstring>( L"user_account" );
		if (account) {
			_account = StringConvert::ToUTF8Str(account.get());
		}

		boost::optional<std::wstring> email = ptParse.get_optional<std::wstring>( L"email" );
		if (email) {
			_email = StringConvert::ToUTF8Str(email.get());
		}

		boost::optional<std::wstring> mobile = ptParse.get_optional<std::wstring>( L"mobile" );
		if (mobile) {
			_mobile = StringConvert::ToUTF8Str(mobile.get());
		}

		boost::optional<std::wstring> workPhone = ptParse.get_optional<std::wstring>( L"fix_tel" );
		if (workPhone) {
			_workPhone = StringConvert::ToUTF8Str(workPhone.get());
		}

		boost::optional<std::wstring> remoteAvatar = ptParse.get_optional<std::wstring>( L"avatar" );
		if (remoteAvatar) {
			_remoteAvatar = StringConvert::ToUTF8Str(remoteAvatar.get());
			_localAvatar = "";
		}

		boost::optional<std::wstring> personalSign = ptParse.get_optional<std::wstring>( L"personal_sign" );
		if (personalSign) {
			_personalSign = StringConvert::ToUTF8Str(personalSign.get());
		}

		boost::optional<int32_t> departmentId = ptParse.get_optional<int32_t>( L"department_id" );
		if (departmentId) {
			_departmentId = departmentId.get();
		}

		boost::optional<std::wstring> departmentName = ptParse.get_optional<std::wstring>( L"department" );
		if (departmentName) {
			_departmentName = StringConvert::ToUTF8Str(departmentName.get());
		}

		boost::optional<std::wstring> position = ptParse.get_optional<std::wstring>( L"position" );
		if (position) {
			_position = StringConvert::ToUTF8Str(position.get());
		}

		boost::optional<std::wstring> superiorName = ptParse.get_optional<std::wstring>( L"leader_name" );
		if (superiorName) {
			_superiorName = StringConvert::ToUTF8Str(superiorName.get());
		}

		boost::optional<int32_t> superiorId = ptParse.get_optional<int32_t>( L"leader_id" );
		if (superiorId) {
			_superiorId = superiorId.get();
		}

		boost::optional<int32_t> colleagueNum = ptParse.get_optional<int32_t>( L"colleague_num" );
		if (colleagueNum) {
			_colleagueNum = colleagueNum.get();
		}

		boost::optional<std::wstring> usertags = ptParse.get_optional<std::wstring>(L"user_tags");
		if (usertags){
			_usertags = StringConvert::ToUTF8Str(usertags.get());
		}

        return UC_SUCCEEDED;
    } catch (boost::property_tree::ptree_error & e) {
    	UC_LOG(ERROR) << "encounter an error when updating ContactInfoImpl object "
    		<< "from an json object, exception: " << e.what();
        return JSON_PARSE_ERROR;
    }
}

void ContactInfoImpl::Update(const ContactInfoImpl& other) {
	_userId = other._userId;
    _siteId = other._siteId;

    _cardVersion = other._cardVersion;
    _status = other._status;

	_displayName = other._displayName;
	_pingyin = other._pingyin;
	_sex = other._sex;
	_account = other._account;
	_email = other._email;
	_mobile = other._mobile;
	_workPhone = other._workPhone;
	_localAvatar = other._localAvatar;
	_remoteAvatar = other._remoteAvatar;
	_personalSign = other._personalSign;
	_usertags = other._usertags;

	_departmentId = other._departmentId;
	_departmentName = other._departmentName;
	_position = other._position;
	_superiorId = other._superiorId;
	_superiorName = other._superiorName;
	_colleagueNum = other._colleagueNum;

	_type = other._type;	
}

ContactInfoImpl& ContactInfoImpl::operator=(const ContactInfoImpl& other) {
    if (this == &other) {
        return *this;
    }
    
    _userId = other._userId;
    _siteId = other._siteId;
    _displayName = other._displayName;
    _cardVersion = other._cardVersion;
    _pingyin = other._pingyin;
	_sex = other._sex;
    _account = other._account;
	_mobile = other._mobile;
    _email = other._email;
    _workPhone = other._workPhone;
    _localAvatar = other._localAvatar;
    _remoteAvatar = other._remoteAvatar;
    _personalSign = other._personalSign;
	_usertags = other._usertags;
    _departmentId = other._departmentId;
    _departmentName = other._departmentName;
    _position = other._position;
    _superiorId = other._superiorId;
	_superiorName = other._superiorName;
    _colleagueNum = other._colleagueNum;
    _status = other._status;
	_type = other._type;
    
    return *this;
}
    
bool ContactInfoImpl::Compare(const ContactInfoImpl& other) {
	int res = this->_pingyin.compare(other._pingyin);
	if (res < 0) {
		return true;
	}
	else {
		// res为0时，返回false，表示小于。
		return false;
	}
}

int32_t ContactInfoImpl::GetRelation() const {
	return _relation;
}

void ContactInfoImpl::SetRelation(int32_t relation) {
	_relation = relation;
}

}
