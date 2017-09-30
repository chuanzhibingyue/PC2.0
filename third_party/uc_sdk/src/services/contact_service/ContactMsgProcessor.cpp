#include "ContactMsgProcessor.h"
#include "ContactService.h"

#include "common/StringConvert.h"
#include "Macro.h"
#include "DBManager.h"
#include "LogManager.h"
#include "UCErrorCode.h"

namespace uc {

ContactMsgProcessor::ContactMsgProcessor(ContactService *contactService, 
    ContactEventDispatcher *dispatcher) 
    : _contactService(contactService), _dispatcher(dispatcher) {
}

ContactMsgProcessor::~ContactMsgProcessor(){
}

void ContactMsgProcessor::ProcessPresenceMsg(MessagePresenceMsg &msg) {
 
	UCHead head;
    msg.GetHead(head);
	if (_contactService->IsCurUser(head.GetFrom())){
		return;
	}

	UserStatusList infos;
	UserStatus info;
	info.SetUserId(head.GetFrom().GetUserId());
	info.SetUserStatus(msg.get_status());
	info.SetStatusShow(msg.get_show());
	infos.push_back(info);
	_contactService->UpdateUserStatus(infos);
	
	UC_LOG(WARNING) << "receive presence message, from: " << head.GetFrom().GetUserId()
		<< ", presense: " << msg.get_status();
	
	_dispatcher->UserStatusReceived(UC_SUCCEEDED, infos);
	
}

void ContactMsgProcessor::ProcessContactAddMsg(ContacterAddContentMsg& msg) {
    UCHead head;
    msg.GetHead(head);
    int32_t from = head.GetFrom().GetUserId();
    int32_t to = head.GetTo().GetUserId();
    int32_t curUserId= _contactService->GetCurUserId();
    
    UC_LOG(WARNING) << "receive contact add message, from: " << from
        << ", to: " << to << ", curUserId:" << curUserId;

    // 如果我是发起人,说明是其他终端发起该操作,则在本终端上添加联系人
    if (curUserId == from) {
        // 先判断是否已经是常用联系人
		if (!_contactService->GetContactCache()->IsCommonContact(to)) {
			std::list<int32_t> userIds;
            userIds.push_back(to);

            //_dispatcher->ContactAdded(UC_SUCCESS, to);

            // 拿到最新联系人信息后，则触发contact add事件
            _contactService->GetContactDelayLoader()->GetContact(userIds, DISPATCH_CONTACT_ADDED);
		}
    }
}

void ContactMsgProcessor::ProcessContactDelMsg(ContacterDelContentMsg& msg) {
    UCHead head;
    msg.GetHead(head);
    int32_t from = head.GetFrom().GetUserId();
    int32_t to = head.GetTo().GetUserId();
    int32_t curUserId= _contactService->GetCurUserId();
    
    UC_LOG(WARNING) << "receive contact delete message, from: " << from
        << ", to: " << to << ", curUserId:" << curUserId;

    if (curUserId == from) {
		//该用户是常用联系人，触发OnContactDeleted回调
		if (_contactService->GetContactCache()->IsCommonContact(to)) {
			_contactService->GetContactCache()->RemoveCommonContact(to);
			_dispatcher->ContactDeleted(UC_SUCCEEDED, to);
		}
    }
}

void ContactMsgProcessor::ProcessContactUpdateMsg(CardUpdateContentMsg& msg) {
    int32_t userId = msg.get_userId();
    int32_t cardVersion = msg.get_cardVersion();

    UC_LOG(WARNING) << "receive contact update message, userId: " << userId
        << ", cardVersion: " << cardVersion;

	ContactInfoImpl *contact = _contactService->GetContactCache()->GetContact(userId);
	if (contact != NULL  && contact->IsCommon() && cardVersion != 0 
		&&contact->GetCardVersion() > cardVersion) {
		delete contact;
		return;
	}

	_contactService->GetContactInfoFromServer(userId, DISPATCH_CONTACT_UPDATED);

	if (contact != NULL) {
		delete contact;
	}
}

void ContactMsgProcessor::ProcessTagDelContentMsg(TagDelContentMsg &msg) {
    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
    
    UC_LOG(WARNING) << "receive tag delete message";

    std::vector<int32_t> *ids = new std::vector<int32_t>(msg.get_delTagIds());
	for (std::size_t i = 0; i < ids->size(); i++) {
//		DBManager::GetInstance().DeleteTag((*ids)[i]);
	}

	_dispatcher->SystemTagsDeleted(ids);
}

void ContactMsgProcessor::ProcessCompleteInfoTypeMsg(CompleteInfoContentMsg& msg, int modification) {

    UC_LOG(WARNING) << "receive complete info message, modification: " << modification;

    boost::unique_lock<boost::recursive_mutex> lock(_mtx);
    
    std::vector<InfoCell> infos = msg.get_info_cell_list();

    int32_t curUserId= _contactService->GetCurUserId();
    std::vector<ContactTag*>* tags = new std::vector<ContactTag*>();
    for (std::vector<InfoCell>::iterator iter = infos.begin();
         iter != infos.end(); ++iter) {
        if (iter->tagId > 0) {
            ContactTag *tag = new ContactTag();
            
			tag->SetId(iter->tagId);
			tag->SetUserId(curUserId);
            if (modification == 1 || modification == 2) {
				tag->SetName(iter->label);
				tag->SetState(iter->state);
				tag->SetInputType(ContactTag::InputType(iter->inputType));
				tag->SetType(ContactTag::Type(iter->type));
				tag->SetScope(ContactTag::Scope(iter->scope));
            }
            
            tags->push_back(tag);
        }
    }
    
	ProcessCompleteInfoTypeMsg(tags, modification);
}

void ContactMsgProcessor::ProcessDeptUpdated(DeptUpdateContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t deptId = msg.get_dept_id();
    std::string *oldDeptName = new std::string(msg.get_old_dept_name());
    std::string *newDeptName = new std::string(msg.get_new_dept_name());
    std::string *desc = new std::string(msg.get_desc());

	UCHead ucHead;
	msg.GetHead(ucHead);

    UC_LOG(WARNING) << "receive deparment updated message, deptId: " << deptId;

	_dispatcher->DeptUpdated(operatorId, deptId, oldDeptName, newDeptName, desc, ucHead.GetTimestamp());
}
    
void ContactMsgProcessor::ProcessDeptTransfered(DeptTransferContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t userId = msg.get_user_id();
    std::string *userName = new std::string(msg.get_user_name());
    int32_t deptId = msg.get_dept_id();
    std::string *oldDeptName = new std::string(msg.get_old_dept_name());
    std::string *deptName = new std::string(msg.get_dept_name());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive deparment transfered message, deptId: " << deptId
        << "userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->DeptTransfered(operatorId, userId, userName, deptId, oldDeptName, deptName, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessPositionUpdated(PositionUpdateContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t userId = msg.get_user_id();
    std::string *userName = new std::string(msg.get_user_name());
    std::string *newPosition = new std::string(msg.get_new_position());
    std::string *oldPosition = new std::string(msg.get_old_position());
    std::string *deptName = new std::string(msg.get_dept_name());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive position updated message, userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
     _dispatcher->PositionUpdated(operatorId, userId, userName, newPosition, oldPosition, 
        deptName, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessEmployeeEntry(EmployeeEntryContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t userId = msg.get_user_id();
    int32_t deptId = msg.get_dept_id();
    std::string *userName = new std::string(msg.get_user_name());
    std::string *deptName = new std::string(msg.get_dept_name());
    std::string *position = new std::string(msg.get_position());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive employee entry message, deptId: " << deptId
        << "userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->EmployeeEntry(operatorId, userId, deptId, userName, deptName, position, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessEmployeeLeaved(EmployeeLeaveContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t userId = msg.get_user_id();
    std::string *userName = new std::string(msg.get_user_name());
    std::string *deptName = new std::string(msg.get_dept_name());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive employee leaved message, userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->EmployeeLeaved(operatorId, userId, userName, deptName, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessEmployeeRightsUpdated(EmployeeRightsChangeContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t userId = msg.get_user_id();
    std::string *userName = new std::string(msg.get_user_name());
    std::string *deptName = new std::string(msg.get_dept_name());
    int32_t deptId = msg.get_dept_id();
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive employee rights updated message, deptId: " << deptId
        << "userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->EmployeeRightsUpdated(operatorId, userId, userName, deptName, deptId, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessDeptParentChanged(DeptParentChangeContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t deptId = msg.get_dept_id();
    std::string *deptName = new std::string(msg.get_dept_name());
    std::string *oldDeptName = new std::string(msg.get_old_dept_name());
    std::string *newDeptName = new std::string(msg.get_new_dept_name());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive parent department changed message, deptId: " << deptId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->DeptParentChanged(operatorId, deptId, deptName, oldDeptName, newDeptName, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessCompanyJoined(CompanyJoinContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t companyId = msg.get_company_id();
    int32_t userId = msg.get_user_id();
    std::string *companyName = new std::string(msg.get_company_name());
    std::string *userName = new std::string(msg.get_user_name());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive company joined message, companyId: " << companyId
        << "userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->CompanyJoined(operatorId, companyId, userId, companyName, userName, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessDeptDeleted(DeptDeleteContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t deptId = msg.get_dept_id();
    std::string *deptName = new std::string(msg.get_dept_name());
    std::string *parentDeptName = new std::string(msg.get_parent_dept_name());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive parent department deleted message, deptId: " << deptId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->DeptDeleted(operatorId, deptId, deptName, parentDeptName, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessEmployeeEntryConfirmed(EmployeeEntryConfirmContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t deptId = msg.get_dept_id();
    std::string *userName = new std::string(msg.get_user_name());
    std::string *deptName = new std::string(msg.get_dept_name());
    int32_t userId = msg.get_user_id();
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive employee entry confirmed message, deptId: " << deptId
        << "userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->EmployeeEntryConfirmed(operatorId, deptId, userName, deptName,
        userId, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessEmployeeLeaveConfirmed(EmployeeLeaveConfirmContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t userId = msg.get_user_id();
    int32_t deptId = msg.get_dept_id();
    std::string *userName = new std::string(msg.get_user_name());
    std::string *deptName = new std::string(msg.get_dept_name());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive employee entry leaved message, deptId: " << deptId
        << "userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
    _dispatcher->EmployeeLeaveConfirmed(operatorId, userId, deptId, userName, deptName, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessDeptTransferConfirmed(DeptTransferConfirmContentMsg &msg) {
    int32_t operatorId = msg.get_operator_id();
    int32_t userId = msg.get_user_id();
    std::string *userName = new std::string(msg.get_user_name());
    std::string *deptName = new std::string(msg.get_dept_name());
    std::string *desc = new std::string(msg.get_desc());
    
    UC_LOG(WARNING) << "receive department transfered confirmed message, userId: " << userId;

	UCHead ucHead;
	msg.GetHead(ucHead);
	_dispatcher->DeptTransferConfirmed(operatorId, userId, userName, deptName, desc, ucHead.GetTimestamp());
}

void ContactMsgProcessor::ProcessCompleteInfoTypeMsg(std::vector<ContactTag*>* tags, int modification) {
	int32_t curUserId= _contactService->GetCurUserId();
	ContactInfoImpl* contact = _contactService->GetContactCache()->GetContact(curUserId);
    //if (tags->size() > 0 && contact != NULL) {
    //    
    //    // 添加标签。先查看标签id是否已存在，存在则错误。给登陆用户添加标签
    //    if (modification == 1) {
    //        bool hasTag = false;
    //        for (std::vector<ContactTag*>::iterator iter = tags->begin();
    //             iter != tags->end(); ++iter) {
    //                 if (contact->HasTag((*iter)->GetId())) 
				//	 {
    //                hasTag = true;
    //                break;
    //            }
    //        }
    //        
    //         if (!hasTag) {
    //             _dispatcher->SystemTagsAdded(tags);
    //        }
    //        else {
    //             SAFE_DELETE_CONTAINER(std::vector<ContactTag*>, tags);
    //        }
    //    }
    //    else if (modification == 2) {
    //        // 修改标签。
    //        for (std::vector<ContactTag*>::iterator iter = tags->begin();
    //             iter != tags->end();) {
				//DBManager::GetInstance().UpdateTag(curUserId,
				//	(*iter)->GetId(), (*iter)->GetName(), (*iter)->GetInputType(),
				//	(*iter)->GetState(), (*iter)->GetScope(), (*iter)->GetType());
    //        }

    //        if (!tags->empty()) {
    //            _dispatcher->SystemTagsUpdated(tags);
    //        }
    //        else {
    //             SAFE_DELETE_CONTAINER(std::vector<ContactTag*>, tags);
    //        }
    //    }
    //}
    //else {
    //    SAFE_DELETE_CONTAINER(std::vector<ContactTag*>, tags);
    //}
}

}
