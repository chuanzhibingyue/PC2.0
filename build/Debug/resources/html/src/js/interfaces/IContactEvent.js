/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.interfaces.IContactEvent = {
        OnBasicContactInfoReceived: function(errorCode, userIds, contacts){},
        OnContactListLoaded: function(errorCode){},
        OnContactPresenceChanged: function(errorCode, status){},
        OnContactInfoReceived: function(errorCode, userIds, contacts){},
        OnContactAdded: function(errorCode, userId){},
        OnContactDeleted: function(errorCode, userId){},
        OnContactUpdated: function(contacts){},
        OnCurrentUserInfoUpdated: function(errorCode, userInfo){},
        OnContactTagsUpdated: function(errorCode, addedTags, updatedTags, deletedTags){},
        OnCurrentUserTagsUpdated: function(errorCode, tags){},
        OnAdminAddTags: function(tags){},
        OnAdminUpdateTags: function(tags){},
        OnAdminDeleteTags: function(tags){},
        OnColleagueInfoReceived: function(errorCode, userId, page, colleagueNumber, colleagues, leaders){},
        OnOrgInfoReceived: function(errorCode, department, myDepartment,copyBack){},
        OnOrgAllMembersReceived: function(errorCode, orgId, userCount, memberIds){},
        OnDepartmentNameChanged: function(operatorId, departmentId, oldDepartmentName, newDepartmentName, description, msgTime){},
        OnDepartmentTransferred: function(operatorId, userId, userName, departmentId, oldDepartmentName, departmentName, description, msgTime){},
        OnPositionChanged: function(operatorId, userId, userName, newPosition, oldPosition, departmentName, description, msgTime){},
        OnEmployeeJoined: function(operatorId, userId, departmentId, userName, position, description, msgTime){},
        OnEmployeeLeft: function(operatorId, userId, userName, departmentName, description, msgTime){},
        OnEmployeeRightsChanged: function(operatorId, userId, userName, departmentId, departmentName, description, msgTime){},
        OnJoinCompany: function(operatorId, companyId,companyName, userId, userName, description, msgTime){},
        OnDepartmentParentChanged: function(operatorId, departmentId, departmentName, oldDepartmentName, newDepartmentName, description, msgTime){},
        OnDepartmentDeleted: function(operatorId, departmentId, departmentName, parentDepartmentName, description, msgTime){},
        OnEmployeeJoinConfirmed: function(operatorId, departmentId, departmentName, userName, userId, description, msgTime){},
        OnEmployeeLeaveConfirmed: function(operatorId, departmentId,departmentName, userName, userId, description, msgTime){},
        OnDepartmentTransferConfirmed: function(operatorId, userId,userName, departmentName, description, msgTime){},
	    OnUserTagsUpdated:function(errorCode,userTags){},
	    OnUserStatusSetted:function(errorCode, userId, status){},
	    OnUserStatusReceived:function(errorCode, status){},
	    OnGetUserStatusResponse:function(errorCode,status,copyBack){}
    }
})(uc);