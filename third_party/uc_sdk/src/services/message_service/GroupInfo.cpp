#include "GroupInfo.h"
#include "rapidjson/document.h"
#include "LogManager.h"
#include "jsonhelper.h"

namespace uc{

GroupInfo::GroupInfo(){

	_id = 0;
	_siteId = 0;
	_name = "";
	_pinYinName = "";
	_createTime = 0;
	_avatarIndex = "";
	_localAvatar = "";
	_joinState = 0;
	_memberCount = 0;
	_adminId = 0;
	_alertState = 0;
	_alertRule = 0;
	_alertTime = 0;
	_topState = 0;
	_status = 0;
	_disturbState = 0;
	_nameFlag = 0;
	_logoLevel = 0;
	_groupType = 0;
	_memberType = 0;
	_modifyTime = 0;
	_statusInfo = "";
	_memberList.clear();
	_mountId = 0;
}

GroupInfo::~GroupInfo(){


}

int GroupInfo::ParseMountId(const std::string &jsonInfo) {
	rapidjson::Document document;
	if (document.Parse<0>(jsonInfo.c_str()).HasParseError()) {
		UC_LOG(ERROR) << "jsonInfo string format error";
		return 0;
	}
	
	return jsonhelper::getIntValue(document, "mount_id", 0);
}

// GroupInfo::GroupInfo(const GroupInfo& info){
// 
// 	_id = info.GetId();
// 	_siteId = info.GetSiteId();
// 	_name = info.GetName();
// 	_pinYinName = info.GetPinYinName();
// 	_createTime = info.GetCreateTime();
// 	_avatarIndex = info.GetAvatarIndex();
// 	_memberCount = info.GetMemberCount();
// 	_adminId = info.GetAdminId();
// 	_alertState = info.GetGroupAlert();
// 	_alertRule = info.GetAlertRule();
// 	_alertTime = info.GetAlertTime();
// 	_topState = info.GetGroupTop();
// 	_status = info.GetGroupStatus();
// 	_disturbState = info.GetGroupDisturb();
// 	_nameFlag = info.GetNameFlag();
// 	_logoLevel = info.GetLogoLevel();
// 	_groupType = info.GetGroupType();
// 
// }

}
