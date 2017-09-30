/*
 * UccRequestFactory.h
 *
 *  Created on: May 28, 2015
 *      Author: gang wang
 */

#ifndef CONTACT_MSG_PROCESSOR_H
#define CONTACT_MSG_PROCESSOR_H

#include "components/parser/msg_parser/include/UCMessageType.h"
#include "ContactEventDispatcher.h"
#include <boost/thread/recursive_mutex.hpp>

namespace uc {

class ContactService;
    
class ContactMsgProcessor {
public:
    ContactMsgProcessor(ContactService *contactService, ContactEventDispatcher *dispatcher);
    ~ContactMsgProcessor();

    /**
     * 处理ucas联系人状态变更消息
     */
    void ProcessPresenceMsg(MessagePresenceMsg &msg);
    
    /**
     * 处理ucas联系人增加消息
     */
    void ProcessContactAddMsg(ContacterAddContentMsg &msg);
    
    /**
     * 处理ucas联系人删除消息
     */
    void ProcessContactDelMsg(ContacterDelContentMsg &msg);
    
    /**
     * 处理ucas联系人更新消息
     */
    void ProcessContactUpdateMsg(CardUpdateContentMsg &msg);
    
    /**
     * 处理ucas标签添加、标签更新消息
     * @param msg 标签消息
     * @param modification 1-新增，2-修改
     */
    void ProcessCompleteInfoTypeMsg(CompleteInfoContentMsg& msg, int modification);
    
    /**
     * 处理ucas标签删除消息
     * @param msg 标签消息
     */
    void ProcessTagDelContentMsg(TagDelContentMsg &msg);
    
    /**
     * 处理ucas部门名称变更
     * @param msg 消息
     */
    void ProcessDeptUpdated(DeptUpdateContentMsg &msg);
    
    /**
     * 处理ucas员工部门调动
     * @param msg 消息
     */
    void ProcessDeptTransfered(DeptTransferContentMsg &msg);
    
    /**
     * 处理ucas员工职位调整
     * @param msg 消息
     */
    void ProcessPositionUpdated(PositionUpdateContentMsg &msg);
    
    /**
     * 处理ucas员工入职
     * @param msg 消息
     */
    void ProcessEmployeeEntry(EmployeeEntryContentMsg &msg);
    
    /**
     * 处理ucas员工离职
     * @param msg 消息
     */
    void ProcessEmployeeLeaved(EmployeeLeaveContentMsg &msg);
    
    /**
     * 处理ucas员工权限变更
     * @param msg 消息
     */
    void ProcessEmployeeRightsUpdated(EmployeeRightsChangeContentMsg &msg);
    
    /**
     * 处理ucas部门移动
     * @param msg 消息
     */
    void ProcessDeptParentChanged(DeptParentChangeContentMsg &msg);
    
    /**
     * 处理ucas员工加入生态企业
     * @param msg 消息
     */
    void ProcessCompanyJoined(CompanyJoinContentMsg &msg);
    
    /**
     * 处理ucas部门删除
     * @param msg 消息
     */
    void ProcessDeptDeleted(DeptDeleteContentMsg &msg);
    
    /**
     * 处理ucas员工入职确认
     * @param msg 消息
     */
    void ProcessEmployeeEntryConfirmed(EmployeeEntryConfirmContentMsg &msg);
    
    /**
     * 处理ucas员工离职确认
     * @param msg 消息
     */
    void ProcessEmployeeLeaveConfirmed(EmployeeLeaveConfirmContentMsg &msg);
    
    /**
     * 处理ucas员工部门调动确认
     * @param msg 消息
     */
    void ProcessDeptTransferConfirmed(DeptTransferConfirmContentMsg &msg);
    
private:
	void ProcessCompleteInfoTypeMsg(std::vector<ContactTag*>* tags, int modification);

private:
    ContactService *_contactService;
    ContactEventDispatcher *_dispatcher;
    
    boost::recursive_mutex _mtx;
};
    
}

#endif
