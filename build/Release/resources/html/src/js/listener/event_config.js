/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function(uc){
    uc.listener.event.config = [
        {
            name: "Login Event Listener",
            type: ["IClientEvent", "IUpdateEvent","IPbxEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.login.LoginEventListener"
        },
        {
            name: "Forget Passowrd Event Listener",
            type: ["IClientEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.password.ForgetPasswordEventListener"
        },
        {
            name: "Main Listener",
            type: ["IClientEvent", "IUIEvent", "IIMEvent", "IGroupEvent", "IMeetingEvent", "IOpenApiEvent", "IUpdateEvent", "IContactEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.main.MainEventListener"
        },
        {
            name: "Conversation Listener",
            type: ["IClientEvent", "IUIEvent", "IContactEvent", "IIMEvent", "IGroupEvent", "IMeetingEvent", "IOpenApiEvent", "IRemoteAssistanceEvent"],
            priority: uc.constants.UI.ListenerPriority.Eight,
            clazz: "uc.modules.conversation.ConversationEventListener"
        },
        {
            name: "Meeting Listener",
            type: ["IMeetingEvent","IUIEvent",'IContactEvent','IGroupEvent','IFSEvent','IMeetingRoomEvent'],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.meeting.MeetingEventListener"
        },
        {
            name: "Meeting Event Listener",
            type: ["IUIEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.meeting.MeetingInterface"
        },
        {
            name: "Meeting Create Or Update Listener",
            type: ["IMeetingEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.meeting.MeetingCreatedOrUpdated"
        },
        {
            name: "Meeting Alarm Listener",
            type: ["IMeetingEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.meeting.MeetingReminderEventListener"
        },
        {
            name: "Contact Listener",
            type: ["IUIEvent", "IContactEvent", "IFSEvent", "IMicroblogEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.ContactMainEventListener"
        },
        {
            name: "Group Listener",
            type: ["IUIEvent", "IGroupEvent", "IFSEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.group.GroupEventListener"
        },
        {
            name: "Chat Listener",
            type: ["IClientEvent", "IUIEvent", "IIMEvent", "IGroupEvent", "IContactEvent", "IFSEvent", "IMeetingEvent", "IOpenApiEvent", "IRemoteAssistanceEvent", "ICallEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.chat.ChatEventListener"
        },
        {
            name: "External Message Listener",
            type: ["IExternalMessageEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.chat.ExternalMessageEventListener"
        },
        {
            name: "Profile Listener",
            type: ["IUIEvent", "IContactEvent", "IGroupEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.profile.ProfileEventListener"
        },
        {
            name: "ChooseContacts Listener",
            type: ["IUIEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.chooseContacts.ChooseContactsEventListener"
        },
        {
            name: "Search Listener",
            type: ["IUIEvent", "IContactEvent", "IGroupEvent",  "ISearchEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.search.SearchEventListener"
        },
        {
            name: "Contacts Search Listener",
            type: ["IUIEvent", "ISearchEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.chooseContacts.SearchContactsEventListener"
        },
        {
            name: "Image Listener",
            type: ["IUIEvent", "IFSEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.image.ImageEventListener"
        },
        {
            name: "Code Listener",
            type: ["IIMEvent", "IGroupEvent", "IFSEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.code.CodeMsgHandler"
        },
        {
            name: "CodePreview Listener",
            type: ["IIMEvent", "IGroupEvent", "IFSEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.code.CodePreviewEventListener"
        },
        {
            name: "Client Event Listener",
            type: ["IClientEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.password.ResetPasswordEventListener"
        },
        {
            name: "OpenApi Event Listener",
            type: ["IOpenApiEvent", "IUIEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.openApi.OpenApiEventListener"
        },
        {
            name: "Setting Event Listener",
            type: ["IUIEvent", "IFSEvent", "IClientEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.setting.SettingEventListener"
        },
        {
            name: "Microblog Event Listener",
            type: ["IMicroblogEvent", "IUIEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.microblog.MicroblogEventListener"
        },
        {
            name: "Client Event Listener",
            type: ["IClientEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.settingUpdate.SettingUpdateEventListener"

        },
        {
            name: "SearchBox Event Listener",
            type: ["ISearchEvent", "IUIEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.component.SearchBox.SearchBoxEventListener"

        },
        {
            name: "UploadLog Event Listener",
            type: ["IFSEvent","IClientEvent"],
            priority: uc.constants.UI.ListenerPriority.Maximum,
            clazz: "uc.modules.setting.SettingPageEventListener"
        },
        {
            name: "Update Event Listener",
            type: ["IUpdateEvent", "IUIEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.update.UpdateEventListener"
        },
        {
            name: "PreviewVideo Event Listener",
            type: ["IVideoEvent", "IUIEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.video.VideoEventListener"
        },
        {
            name: "Bubble Event Listener",
            type: ["IClientEvent", "IContactEvent", "IGroupEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.bubble.BubbleEventListener"
        },
        {
            name: "RemoteAssistance Event Listener",
            type: ["IRemoteAssistanceEvent", "IMeetingEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.remoteAssistance.RemoteAssistanceEventListener"
        },
        {
            name: "Notice Event Listener",
            type: ["IClientEvent", "IContactEvent","IUIEvent", "IGroupEvent","IFSEvent","IBBSEvent","IOpenApiEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.notice.NoticeEventListener"
        },
	    {
            name: "Cloud Event Listener",
            type: [ "IUIEvent","IContactEvent", "IGroupEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.cloud.CloudEventListener"
        },
        {
            name: "Comment Event Listener",
            type: ["IClientEvent", "IUIEvent", "IIMEvent", "IGroupEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.comment.CommentEventListener"
        },
        {
            name: "Popup Listener",
            type: ["IContactEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.profile.PopupEventListener"

        },
        {
            name: "Cache Event Listener",
            type: ["IContactEvent", "IUIEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.cache.CacheEventListener"
        },
        {
            name: "Call Event Listener",
            type: ["IUIEvent", "IContactEvent", "IGroupEvent","ICallEvent","IIMEvent","ISharingEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.call.CallEventListener"
        },
        {
            name: "Call Event Listener",
            type: ["IContactEvent", "IGroupEvent","ICallEvent","IIMEvent","ISharingEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.call.FloatingCallWindowEventListener"
        },
        {
            name: "Call Choose Contacts Event Listener",
            type: ["IUIEvent", "IContactEvent", "IGroupEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.call.ChooseCallContactsEventListener"
        },
        {
            name: "Call Search Contacts Event Listener",
            type: ["ISearchEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.call.SearchCallContactsEventListener"
        },
	    {
		    name: "AVdetection Event Listener",
		    type: ["IUIEvent","ICallEvent"],
		    priority: uc.constants.UI.ListenerPriority.Minimum,
		    clazz: "uc.modules.AVdetection.AVdetectionEventListener"
	    },
	     {
            name: "historymsg Listener",
            type: ["IClientEvent", "IUIEvent", "IIMEvent", "IGroupEvent", "IMeetingEvent","ISearchEvent","IFSEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.historymsg.HistoryEventListener"
       },
       {
            name: "PBX Event Listener",
            type: ["IContactEvent","IUIEvent","IIMEvent","IPbxEvent","ISearchEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.pbx.PbxEventListener"
        },
        {
		    name: "ScreenSharing EventListener",
			type: ["IClientEvent", "IUIEvent","ISharingEvent"],
			priority: uc.constants.UI.ListenerPriority.Minimum,
			clazz: "uc.modules.screenSharing.ScreenSharingEventListener"
	    },
        {
            name: "PBX Event Listener",
            type: ["IContactEvent","IUIEvent","IIMEvent","IPbxEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.pbx.FloatingPbxCallWindowEventListener"
        },
        {
            name: "PBX CallWindow Event Listener",
            type: ["IContactEvent","IUIEvent","IIMEvent","IPbxEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.pbx.PbxCallWindowEventListener"
        },
        {
            name: "Meeting Wait Listener",
            type: ["IMeetingEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.meeting.MeetingJoinEventListener"
        },
        {
            name: "Meeting Authorize Listener",
            type: ["IMeetingEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.meeting.MeetingAuthorizeEventListener"
        },
	    {
		    name: "MeetingRoom Event Listener",
		    type: ['IContactEvent',"IUIEvent",'IMeetingEvent','IMeetingRoomEvent'],
		    priority: uc.constants.UI.ListenerPriority.Minimum,
		    clazz: "uc.modules.meeting.room.MeetingRoomEventListener"
	    },
        {
            name: "MeetingAttendeeBusyStatus Event Listener",
            type: ['IContactEvent','IMeetingEvent'],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.meeting.MeetingAttendeeBusyStatusEventListener"
        },
        {
            name: "Meeting Create Or Edit Listener",
            type: ["IContactEvent", "IMeetingEvent","ISearchEvent","IMeetingRoomEvent"],
            priority: uc.constants.UI.ListenerPriority.Minimum,
            clazz: "uc.modules.meeting.MeetingSchedulerEventListener"
        },
	    {
		    name: "Create MeetingRoom EventListener",
		    type: ['IContactEvent',"IUIEvent",'IMeetingRoomEvent'],
		    priority: uc.constants.UI.ListenerPriority.Minimum,
		    clazz: "uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoomEventListener"
	    },
	    {
		    name: "Map EventListener",
		    type: ["IUIEvent"],
		    priority: uc.constants.UI.ListenerPriority.Minimum,
		    clazz: "uc.modules.map.MapEventListener"
	    },
	    {
		    name: "JoinMeetingEventListener",
		    type: ["IUIEvent","IMeetingRoomEvent"],
		    priority: uc.constants.UI.ListenerPriority.Minimum,
		    clazz: "uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeetingEventListener"
	    }
    ]
})(uc);