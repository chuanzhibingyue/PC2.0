/**
 * Created by senlin.ouyang on 2015/7/15.
 */
[
    'src/js/lib/jquery-2.1.4.js',
    'src/js/lib/jquery-qtips.js',
    'src/js/lib/jquery.mousewheel.js',
    'src/js/lib/jquery.intlTelInput.js',
    'src/js/lib/jquery-caret.js',
    'src/js/uc.js',
    /* Constants **/
    'src/js/constants/Constants.js',
    'src/js/constants/ErrorCode.js',
    /* Util **/
    'src/js/util/EventUtil.js',
    'src/js/util/HashMap.js',
    'src/js/util/WindowUtil.js',
    'src/js/util/TimeUtil.js',
    'src/js/util/EmotionUtil.js',
    'src/js/util/CollectionUtil.js',
    'src/js/util/StringUtil.js',
    'src/js/util/HtmlUtil.js',
    'src/js/util/FileDialogUtil.js',
    'src/js/util/StorageUtil.js',
    'src/js/util/PageUtil.js',
    'src/js/util/FileUtil.js',
    'src/js/util/ImageUtil.js',
    'src/js/util/AvatarUtil.js',
    'src/js/util/AudioUtil.js',
    'src/js/util/VideoUtil.js',
    'src/js/util/SystemUtil.js',
    'src/js/util/LogUtil.js',
    'src/js/util/MenuUtil.js',
    'src/js/util/CaretUtil.js',
    /* Third Party **/
    'src/js/third_party/emoji/EmojiConfig.js',
    'src/js/third_party/emoji/EmojiConverter.js',
    'src/js/third_party/emoji/jquery.emojiarea.js',
    'src/js/third_party/jquery.searchInput.js',
    'src/js/third_party/jquery.popMenu.js',
    'src/js/third_party/jquery.caret.js',
    /* Interfaces **/
    'src/js/interfaces/IClientService.js',
    'src/js/interfaces/IClientEvent.js',
    'src/js/interfaces/IContactService.js',
    'src/js/interfaces/IContactEvent.js',
    'src/js/interfaces/IMeetingService.js',
    'src/js/interfaces/IMeetingEvent.js',
    'src/js/interfaces/IGroupService.js',
    'src/js/interfaces/IGroupEvent.js',    
    'src/js/interfaces/IIMService.js',
    'src/js/interfaces/IIMEvent.js',
    'src/js/interfaces/IUIEvent.js',
    'src/js/interfaces/IUIService.js',
    'src/js/interfaces/IFSService.js',
    'src/js/interfaces/IFSEvent.js',
    'src/js/interfaces/ISearchService.js',
    'src/js/interfaces/ISearchEvent.js',
	'src/js/interfaces/ISharingService.js',
	'src/js/interfaces/ISharingEvent.js',
    'src/js/interfaces/IOpenApiService.js',
    'src/js/interfaces/IOpenApiEvent.js',
    'src/js/interfaces/IMicroblogEvent.js',
    'src/js/interfaces/IMicroblogService.js',
    'src/js/interfaces/IUpdateService.js',
    'src/js/interfaces/IUpdateEvent.js',
    'src/js/interfaces/IRemoteAssistanceService.js',
    'src/js/interfaces/IRemoteAssistanceEvent.js',
    'src/js/interfaces/ICallService.js',
    'src/js/interfaces/ICallEvent.js',
    'src/js/interfaces/IBBSService.js',
    'src/js/interfaces/IBBSEvent.js',
    'src/js/interfaces/IPbxService.js',
    'src/js/interfaces/IPbxEvent.js',
	'src/js/interfaces/IMeetingRoomService.js',
	'src/js/interfaces/IMeetingRoomEvent.js',
    'src/js/listener/event_config.js',
    'src/js/listener/service_config.js',
    'src/js/listener/native_event_config.js',
    'src/js/listener/native_service_config.js',
    'src/js/listener/manager.js',
    /* Components **/
    'src/js/service/UIService.js',
    /* setting */
    'src/js/modules/setting/SettingData.js',
    'src/js/modules/setting/SettingManager.js',
    'src/js/modules/setting/SettingEventListener.js',
    /* Components **/
    'src/js/modules/component/MainMenuBar.js',
    'src/js/modules/component/InputBox.js',
    //'src/js/modules/component/SearchBox.js',
    'src/js/modules/component/TitleBarButton.js',
    'src/js/modules/component/ExtraMenuBar.js',
    'src/js/modules/component/MessageBox.js',
    'src/js/modules/component/RtxBox.js',
    'src/js/modules/component/EmotionPanel.js',
    'src/js/modules/component/ChatToolBar.js',
    'src/js/modules/component/HtmlEditor.js',
    'src/js/modules/component/MessageInputBox.js',
    'src/js/modules/component/PromptMessageBox.js',
    'src/js/modules/component/Menu.js',
    'src/js/modules/component/SwitchButton.js',
    'src/js/modules/component/DropdownList.js',
    'src/js/modules/component/EmbeddedCallWindowPanel.js',
    'src/js/modules/component/EmbeddedCallWindowToolBar.js',
    'src/js/modules/component/DialPadComponent.js',
//    'src/js/modules/component/PbxDialPadComponent.js',
//    'src/js/modules/component/PbxDmzDialPadComponent.js',
    'src/js/modules/component/PbxSearchComponent.js',
    'src/js/modules/component/TimeBox.js',
    'src/js/modules/component/TimeChooseBox.js',
    'src/js/modules/component/DatePicker.js',
    /* ============ Modules ===============**/
    /* Conversation **/
    'src/js/modules/openApi/OpenApiDataSet.js',
    'src/js/modules/message/conversation/ConversationInfo.js',
    'src/js/modules/message/conversation/ConversationView.js',
    'src/js/modules/message/conversation/ConversationDataSet.js',
    'src/js/modules/message/conversation/ConversationList.js',
    'src/js/modules/message/conversation/ConversationListAdapter.js',
    'src/js/modules/message/conversation/ConversationManager.js',
    'src/js/modules/message/conversation/ConversationEventListener.js',
    'src/js/modules/notice/NoticeEventListener.js',
    'src/js/modules/notice/Notice.js',
    'src/js/modules/notice/NoticeManager.js',
    'src/js/modules/message/bubble/BubbleManager.js',
        /* cloud */
    'src/js/modules/cloud/CloudEventListener.js',
    'src/js/modules/cloud/Cloud.js',
    'src/js/modules/cloud/CloudManager.js',
	/*meetingRoomdata*/
	'src/js/modules/meeting/room/MeetingRoomDataMap.js',
    /*Meeting**/
    'src/js/modules/meeting/minutes/MeetingMinutes.js',
    'src/js/modules/component/CalendarDatePicker.js',
	'src/js/modules/component/Circle_loader.js',

    /*Meeting New*/
    'src/js/modules/meeting/main/MeetingEventListener.js',
    'src/js/modules/meeting/main/MeetingMainManager.js',
    /*Meeting List*/
    'src/js/modules/meeting/main/list/MeetingDataSet.js',
    'src/js/modules/meeting/main/list/MeetingList.js',
    'src/js/modules/meeting/main/list/MeetingListAdapter.js',
    'src/js/modules/meeting/main/list/MeetingListManager.js',
    'src/js/modules/meeting/main/list/MeetingListItemView.js',
    /*Meeting Attendee*/
    'src/js/modules/meeting/main/attendee/MeetingAttendee.js',
    'src/js/modules/meeting/main/attendee/MeetingAttendeeManager.js',
    /*Meeting Chat*/
    'src/js/modules/meeting/main/chat/MeetingChat.js',
    'src/js/modules/meeting/main/chat/MeetingChatManager.js',
    /*Meeting Detail*/
    'src/js/modules/meeting/main/detail/MeetingDetail.js',
    'src/js/modules/meeting/main/detail/MeetingDetailManager.js',
    /*Meeting File*/
    'src/js/modules/meeting/main/file/MeetingFile.js',
    'src/js/modules/meeting/main/file/MeetingFileManager.js',
    /*Meeting Header*/
    'src/js/modules/meeting/main/header/MeetingHeader.js',
    'src/js/modules/meeting/main/header/MeetingHeaderManager.js',

    /*Meeting Join Animation*/
    'src/js/modules/meeting/main/joinAnimation/JoinAnimationManager.js',
    'src/js/modules/meeting/main/joinAnimation/JoinAnimation.js',

    /* Contacts **/
    'src/js/modules/contacts/main/ContactMain.js',
    'src/js/modules/contacts/main/ContactMainEventListener.js',
    'src/js/modules/contacts/main/contact/ContactDataSet.js',
    'src/js/modules/contacts/main/contact/ContactStatusDataSet.js',
    'src/js/modules/contacts/main/contact/ContactManager.js',
    'src/js/modules/contacts/main/group/GroupDataSet.js',
    'src/js/modules/contacts/main/group/GroupEventListener.js',
    'src/js/modules/contacts/main/group/GroupManager.js',
    'src/js/modules/contacts/main/org/OrgManager.js',
    'src/js/modules/contacts/main/contact/ContactFiguresView.js',
    'src/js/modules/contacts/main/contact/ContactFigureView.js',
    /* Chat **/
    'src/js/modules/message/chat/ChatEventListener.js',
    'src/js/modules/message/chat/ChatManager.js',
    'src/js/modules/message/chat/MessageManager.js',
    'src/js/modules/message/chat/SingleChat.js',
    'src/js/modules/message/chat/GroupChat.js',
    'src/js/modules/message/chat/ProjectChat.js',
    'src/js/modules/message/chat/CloudChat.js',
    'src/js/modules/message/chat/MeetingChat.js',
    'src/js/modules/message/chat/MeetingDetailChat.js',
    'src/js/modules/message/chat/OpenApiChat.js',
    'src/js/modules/message/chat/FileList.js',

    /* Comment */
    'src/js/modules/comment/Comment.js',
    'src/js/modules/comment/CommentEventListener.js',
    'src/js/modules/comment/CommentManager.js',
    
    /* Profile **/
    'src/js/modules/contacts/profile/ProfileEventListener.js',
    'src/js/modules/contacts/profile/ProfileManager.js',
    'src/js/modules/contacts/profile/Me.js',
    //'src/js/modules/profile/PopupContactProfile.js',
    
    /* Search **/
    'src/js/modules/search/Search.js',
    'src/js/modules/search/SearchEventListener.js',
    'src/js/modules/search/SearchManager.js',

    /* OpenApi */
    'src/js/modules/openApi/OpenApi.js',
    'src/js/modules/openApi/OpenApiEventListener.js',
    'src/js/modules/openApi/OpenApiManager.js',
    'src/js/modules/openApi/OpenApiView.js',

    /* Image **/
    'src/js/modules/image/ImageEventListener.js',
    'src/js/modules/image/ImageManager.js',

    /* chooseContacts */

     'src/js/modules/contacts/chooseContacts/ChooseContactsEventListener.js',
     'src/js/modules/contacts/chooseContacts/ChooseContactsManager.js',
     'src/js/modules/contacts/chooseContacts/SearchContactsManager.js',

    /* Microblog */
    'src/js/modules/microblog/Microblog.js',
    'src/js/modules/microblog/MicroblogEventListener.js',
    'src/js/modules/microblog/MicroblogManager.js',

    /* Draft */
    'src/js/modules/message/draft/DraftMessageDataSet.js',
    'src/js/modules/message/draft/DraftMessageManager.js',

    /* video Preview */
    'src/js/modules/video/VideoEventListener.js',
    'src/js/modules/video/VideoManager.js',

    /* Remote Assistance **/
    'src/js/modules/remoteAssistance/RemoteAssistanceEventListener.js',
    'src/js/modules/remoteAssistance/RemoteAssistanceManager.js',
    'src/js/modules/remoteAssistance/RemoteAssistanceBanner.js',

    /* Cache Data **/
    'src/js/modules/cache/CacheEventListener.js',
    'src/js/modules/cache/BasicContactInfoCache.js',

    /* Call Module*/
    'src/js/modules/call/CallInfo.js',
    'src/js/modules/call/CallEventListener.js',
    'src/js/modules/call/CallManager.js',
    'src/js/modules/call/EmbeddedCallWindow.js',
    'src/js/modules/call/EmbeddedCallBanner.js',

    /* PBX **/
    'src/js/modules/pbx/PbxInfo.js',
    'src/js/modules/pbx/PbxView.js',
    'src/js/modules/pbx/PbxDataSet.js',
    'src/js/modules/pbx/PbxList.js',
    'src/js/modules/pbx/PbxDetail.js',
    'src/js/modules/pbx/PbxListAdapter.js',
    'src/js/modules/pbx/PbxManager.js',
    'src/js/modules/pbx/PbxEventListener.js',
    'src/js/modules/pbx/PbxCallManager.js',

	/*meetingRoom*/

	'src/js/modules/meeting/room/info/MeetingAttendeeInfo.js',
	'src/js/modules/meeting/room/info/MeetingRoomInfo.js',
	'src/js/modules/meeting/room/MeetingRoomEventListener.js',
	'src/js/modules/meeting/room/MeetingRoomManager.js',
	'src/js/modules/meeting/room/MeetingRoom.js',
	'src/js/modules/meeting/room/roomAttendee/MeetingRoomAttendee.js',
	'src/js/modules/meeting/room/roomAttendee/MeetingRoomAttendeeManager.js',
	'src/js/modules/meeting/room/MeetingRoomOrgList.js',
	'src/js/modules/meeting/room/MeetingRoomDetails.js',

    /* Map*/
    'src/js/modules/map/MapEventListener.js',

    /* Main **/
    'src/js/modules/main/CrossWindowEventListener.js',
    'src/js/modules/main/NativeEventListener.js',
    'src/js/modules/main/MainEventListener.js',
    'src/js/modules/main/Main.js',
    /* history message */
    'src/js/modules/message/history/HistoryEventListener.js',
    'src/js/modules/message/history/HistoryManager.js',
    'src/js/modules/message/history/History.js'
]