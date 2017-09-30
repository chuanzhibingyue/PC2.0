#ifndef GK_CLIENT_MESSAGE_H_
#define GK_CLIENT_MESSAGE_H_

#include "rapidjson/document.h"
#include "common/platform.h"
#include "IGroupService.h"
#include "common/StringConvert.h"

#include <vector>

namespace ucclient {

typedef rapidjson::GenericDocument<rapidjson::UTF16<> >			 WDocument;
typedef rapidjson::GenericValue<rapidjson::UTF16<> >			 WValue;

struct GKClientUserInfo{
	int32_t      userId;
	std::string userName;
	std::wstring role;
};

class GkClientMessage {
public:
	GkClientMessage(){}
	GkClientMessage(GkClientMessage&& other);

	GkClientMessage(WDocument& doc) : _doc(std::move(doc)){}
	WDocument& MessageDoc() { return _doc; }

	GkClientMessage& operator=(GkClientMessage&& other);
protected:
	void CreateMessage(int32_t mountId, const std::wstring& type, const std::wstring& subType);

protected:
	WDocument _doc;
};

class CreateGroupMessage : public GkClientMessage {
public:
	CreateGroupMessage(int32_t mountId, const std::wstring& name, const std::wstring& url, 
		const std::vector<GKClientUserInfo>& userInfo);

	CreateGroupMessage(WDocument& document);
};

class UpdateNameMessage : public GkClientMessage {
public:
	UpdateNameMessage(int32_t mountId, const std::wstring& name);
};

class UpdateUrlMessage : public GkClientMessage {
public:
	UpdateUrlMessage(int32_t mountId, const std::wstring& url);
};

class AddMemberMessage : public GkClientMessage {
public:
	AddMemberMessage(int32_t mountId, const std::wstring& name, const std::vector<GKClientUserInfo>& userInfo);
};

class RemoveMemberMessage : public GkClientMessage {
public:
	RemoveMemberMessage(int32_t mountId, const std::wstring& name, const std::vector<int32_t>& userIds);
};

}

#endif