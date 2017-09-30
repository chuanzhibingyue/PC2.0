/**
 * Created by senlin.ouyang on 2015/7/15.
 */
(function (uc) {
    uc.constants = {
        Conversation: {
            ConversationType: {
                None: -1,
                MeetingMsg: 0,
                ProjectChat: 1,
                GroupChat: 2,
                SingleChat: 3,
                OpenApi: 4,
                OrgMsg: 5,
                CloudChat: 6,
                WelcomeMsg: 10
            },
            type2Name: {
                '-1': 'None',
                '0': 'MeetingMsg',
                '1': 'Project',
                '2': 'Group',
                '3': 'SingleChat',
                '4': 'OpenApi',
                '5': 'OrgMsg',
                '6': 'CloudFile',
                '10': 'WelcomeMsg'
            },
            type2ChartName: {
                '1': 'Project',
                '2': 'GroupChat',
                '6': 'CloudChat'
            },
            ConversationCountForSync: 20,
            sessionType: { // added by xue.bai_2 on 2016/04/06
                p2p: 0,
                group: 1,
                org: 2,
                appId: 3
            }
        },
        Contact: {
            SearchType: {
                Project: 1,
                Group: 2,
                ProjectGroup: 4
            }
        },
        Message: {
            SendState: {
                None: -1,
                SendOK: 0,
                Sending: 1,
                SendFail: 2
            },
            MessageChannel: {
                UCAS: 1,
                MessageList: 2,
                ChatHistory: 3,
                History: 4,
                Search: 5,
                Notice:6
            },
            ContentType: {
                None: 0,
                Text: 1,
                File: 2,
                Image: 3,
                Audio: 4,
                Video: 5,
                Code: 6,
                CloudFile: 7,
                Comment: 8,
                MeetingReport: 9
            },
            TextMessageType: {
                Text: 1,
                MIME: 2
            },
            AckReadType: {
                Conversation: 1,
                SingleMessage: 0
            },
            CloudType: {
                FS: 0,     //fs
                GoKuai: 1, //够快云盘
                AnZhen: 2  //安贞母婴
            },
            FileListType: {
                Minutes: 0,//会议纪要
                Record: 1,//会议录制
                Doc: 2//文件
            },
            OnceGetHistoryCount: 10
        },
        Meeting: {
            Week: [uc_resource.Meeting.Sunday, uc_resource.Meeting.Monday, uc_resource.Meeting.Tuesday, uc_resource.Meeting.Wednesday, uc_resource.Meeting.Thursday, uc_resource.Meeting.Friday, uc_resource.Meeting.Saturday, uc_resource.Meeting.WorkingDay, uc_resource.Meeting.Weekday],
            MsgType: {
                chat: 'chat',
                detail: 'detail',
                Text: 'chat',
                Image: 'chat',
                Audio: 'chat',
                Video: 'chat',
                Invitation: 'detail',
                InvitationAccepted: 'detail',
                InvitationRejected: 'detail',
                Updated: 'detail',
                Forwarded: 'detail',
                Cancelled: 'detail',
                SummaryCreated: 'minutes',
                SummaryUpdated: 'minutes',
                Accredit: 'detail',
                AccreditCancelled: 'detail',
                PartJoin: 'detail',
                PartCancel: 'detail'
            },
            AttendeeType: {
                User: 1,
                Mail: 2,
                TelUser: 3
            },
            AttendeeStatus: {
                Pending: 0,
                Accepted: 1,
                Reject: 2,
                Cancel: 3
            },
            Operations: {
                Accepted: 0,
                Cancel: 1,
                Reject: 2
            },
            CalendarSource: {
                Default: 0,
                MeetingOwner: 1,
                Attendee: 2,
                Forward: 3
            },
            EditMeetingType: {
                MainMenu: 0,
                Edit: 1,
                RestTime: 2
            },
            UpdateFlag: {
                None: 0,
                AttendeeChanged: 1,
                MeetingInfoChanged: 2,
                AttendeeAndMeetingInfoChanged: 3
            },
            AuthorizeType: {
                AuthorizeFromMe: 0,
                AuthorizeToMe: 1
            },
            OnceGetMeetingListCount: 30,
            CreateMeetingName: 'MeetingCreator',
            DefaultTimeZone: 8,
            useAnimation: true,
	        MeetingRoom:{
		        RoomNameInputMaxLength:30,
		        RoomRemarkInputMaxLength:500
	        },
            MeetingCreateType: {
                Default: 0,
                RoomMeeting: 1
            },
            AuthorizeMySelf: 0,
            DefaultMeetingDuration: 60,
            AddressType: {
                Out: 1,       //1-外出
                Company: 2    //2-公司内
            },
	        Room:{
		        CreateOrEditType:{
			        Create:0,
			        Edit:1
		        }
	        },
            FileTransfer: {
                Downloading: 1,
                Uploading: 2
            }
        },
        Group: {
            JoinStatus: {
                InGroup: 0,
                Exited: 1
            },
            GroupStatus: {
                Working: 0,
                Ended: 1
            },
            GroupCloudType: {
                CloudFileCreate: 0,
                CloudFileUpdate: 1,
                CloudFileDel: 2,
                CloudFileMove: 3,
                CloudFileRename: 4
            }
        },
        Chat: {
            ChatType: {
                None: -1,
                MeetingChat: 0,
                ProjectChat: 1,
                GroupChat: 2,
                SingleChat: 3,
                OpenApiChat: 4,
                Org: 5,
                CloudChat: 6,
                Welcome: 10,
                notice: 16
            }
        },
        GroupType: {
            MeetingGroup: 0,
            Project: 1,
            Group: 2,
            Cloud: 3
        },
        AvatarType: {
            Project: "Project",
            Contact: "Contact"
        },
        Search: {
            SearchScope: {
                Contact: 1,
                Project: 2,
                Message: 3,
                Meeting: 4,
                Group: 22,
                CloudGroup: 23
            },
            SearchCategory: {
                All: 0,
                Contact: 1,
                Meeting: 2,
                Document: 3,
                Link: 4,
                Message: 5
            },
            SearchResultType: {
                Contact: 0,
                Project: 1,
                Group: 2,
                Meeting: 3,
                Document: 4,
                Message: 5,
                Link: 6,
                CloudGroup: 7
            },
            SearchPage: 1,
            SearchCountPerPage: 20,
            SearchTimerDelay: 500,
            SearchResultCountPerCategoryInAll: 3
        },
        NetworkStatus: {
            OK: 0,
            VPNConnected: 1,
            Error: 2
        },
        UCASStatus: {
            Connected: 0,
            Disconnected: 1
        },
        SessionStatus: {
            OK: 0,
            Expired: 1
        },
        ClientStatus: {
            Disconnected: 0,
            Connecting: 1,
            Connected: 2,
            ReConnectFailed: 3
        },
        RemoteAssistanceStatus: {
            NONE: 0,
            CREATED: 1,
            IN_ASSISTANCE: 2,
            INVITED: 3,
            ACCEPTED: 4
        },
        Pbx: {
            GetCallCount: 20,
            GetCallDetailCount: 30,
            GetSearchCallCount: 15,
            CallStatus: {
                CALL_STATE_INCOMING: 0,
                CALL_STATE_INIT: 1,
                CALL_STATE_EARLY: 2,
                CALL_STATE_CONNECTING: 3,
                CALL_STATE_CONFIRMED: 4,
                CALL_STATE_DISCONNECTED: 5
            },
            CallStatusType: {
                CALL_STATE_NONE: 0,
                CALL_STATE_FAIL: 1,
                CALL_STATE_EARLY: 2,
                CALL_STATE_CONFIRMED: 4,
                CALL_STATE_CANCEL: 6,
                CALL_STATE_HANGUP: 7,
                CALL_STATE_Rejected: 8
            },
            CallIncomingType: {
                CallOut: 0, //呼出类型
                CallIn: 1   //呼入类型
            },
            MicStatus: {
                MicStatusNotIn: 30007, // 没有麦克风输入
                MicStatusNotOut: 30008, // 没有麦克风输出
                MicStatusNotOutOrIn: 3009// 没有麦克风输入输出
            },
            LoadPbxType: {
                PbxManagePbxHangUp : "PbxManagePbxHangUp",
                PbxManageCallHangUp : "PbxManageCallHangUp",
                PbxIsMute : "PbxIsMute"
            },
            PbxCallBackList:[10010,10086,110,112,119,120,121,122,160,168,17951,17968,201,303,95500,95588,197200,96168],
            PbxKeyCode:{
                "48":"0",
                "49":"1",
                "50":"2",
                "51":"3",
                "52":"4",
                "53":"5",
                "54":"6",
                "55":"7",
                "56":"8",
                "57":"9",
                "42":"*",
                "35":"#"
            }
        },
        Call: {
            CallType: {
                P2P: 0,
                GROUP: 1
            },
            InCallStatus: {
                NotInCall: 0,
                InCall: 1
            },
            CallPropertyChangedType: {
                AUDIO_TYPE_CHANGED: 1,
                MUTE_STATUS_CHANGED: 2,
                BOTH_AUDIO_TYPE_AND_MUTE_STATUS: 3
            },
            CallStatus: {
                DISCONNECTED: 0,
                DISCONNECTING: 1,
                REJECTED: 2,
                INVITING: 3,
                INVITED: 4,
                CONNECTING: 5,
                CONNECTED: 6
            },
            MuteStatus: {
                UN_MUTE: 0,
                MUTE: 1
            },
            VideoStatus: {
                CLOSE: 0,
                OPEN: 1
            },
            DesktopShareStatus: {
                CLOSE: 0,
                OPEN: 1
            },
            AudioType: {
                VOIP: 1,
                PHONE: 2,
                NONE: 3
            },
            FullScreenStatus: {
                NORMAL: 0,
                FULL_SCREEN: 1
            },
            CallWindowType: {
                NO_CALL_WINDOW: 0,
                EMBEDDED_CALL_WINDOW: 1,
                FLOATING_CALL_WINDOW: 2,
                EMBEDDED_CALL_BANNER: 3
            },
            PropertyChanged: {
                AudioType: 1,
                MuteStatus: 2,
                ALL: 3
            },
            AcceptorVideoStatus: {
                Mute: 1,
                Video: 2,
                Speaking: 3,
                Rejected: 4,
                None: 5
            },
            CallFailedType: {
                CreateFailed: 1,
                InvitedCall: 2,
                BusyUserStatus: 3,
                GetUserStatus: 4,
                SelectedUser: 5
            },
            callMediaType: {
                closeVideo: 'closeVideo',
                ringVideo: 'ring',
                ringInVideo: 'videoRingIn'
            },
            callAvDetectionType: {
                speaker: "speaker",
                microphone: "microphone"
            },
            MaxJoinCount: 9
        },
        UI: {
            ListenerPriority: {
                Minimum: 0,
                One: 1,
                Two: 2,
                Three: 3,
                Four: 4,
                Five: 5,
                Six: 6,
                Seven: 7,
                Eight: 8,
                Maximum: 9
            },
            ModuleId: {
                Message: 0,
                Meeting: 1,
                Contacts: 2,
                App: 3,
                Me: 4,
                Cloud: 5,
                Pbx: 6,
				MeetingRoom:7,
                Notice:10
            },
            DragCaption: {
                Login: [{left: 0, top: 0, width: -50,height: 40}],
                Message: [{ top: 10, left: 360, width: -80, height: 30}, {top: 110, left: 20, width: 60, height: 30}, {top: 75, left: 20, width: 20, height: 30}, {top: 480, left: 20, width: 60, height: -100}],
                Meeting: [{ top: 10, left: 310, width: -300, height: 50}, {top: 110, left: 20, width: 60, height: 30}, {top: 75, left: 20, width: 20, height: 30}, {top: 480, left: 20, width: 60, height: -100}],
                Contacts: [{ top: 10, left: 360, width: -80, height: 50}, {top: 110, left: 20, width: 60, height: 30}, {top: 75, left: 20, width: 20, height: 30}, {top: 480, left: 20, width: 60, height: -100}],
                App: [{ top: 10, left: 60, width: -80, height: 30}, {top: 110, left: 20, width: 60, height: 30}, {top: 75, left: 20, width: 20, height: 30} ,{top: 480, left: 20, width: 60, height: -100}],
                Notice: [{ top: 10, left: 60, width: -150, height: 30}, {top: 110, left: 20, width: 60, height: 30}, {top: 75, left: 20, width: 60, height: 30} ,{top: 480, left: 20, width: 60, height: -100}],
                Me: [{ top: 10, left: 100, width: -100, height: 40}, {top: 110, left: 20, width: 60, height: 30}, {top: 75, left: 20, width: 20, height: 30},{top: 480, left: 20, width: 60, height: -100}],
                Cloud: [{ top: 10, left: 360, width: -80, height: 30}, {top: 110, left: 20, width: 60, height: 30}, {top: 75, left: 20, width: 20, height: 30},{top: 480, left: 20, width: 60, height: -100}],
                Pbx: [{ top: 10, left: 360, width: -80, height: 30}, {top: 110, left: 20, width: 60, height: 30}, {top: 75, left: 20, width: 20, height: 30},{top: 480, left: 20, width: 60, height: -100}],
                ChooseContacts: [{left: 340, top: 5, width: -120, height: 40}],
                ImagePreview: [{left: 20, top: 5, width: -90, height: 40}],
                ChooseCallContacts: [{left: 0, top: 5, width: 601, height: 12}],
                ImageCropper: [{left: 20, top: 5, width: -90, height: 40}],
                ForgetPassword: [{left: 20, top: 5, width: 520, height: 40}],
                ConfirmSetPassword: [{left: 20, top: 5, width: 370, height: 40}],
                PasswordModified: [{left: 20, top: 5, width: 370, height: 40}],
                UpdateClient: [{left: 20, top: 5, width: 440, height: 40}],
                ProxyAuthenticate: [{left: 20, top: 5, width: 300, height: 40}],
                AccreditMeeting: [{left: 20, top: 5, width: -50, height: 40}],
                CreateMeeting: [{left: 20, top: 5, width: -50, height: 40}],
                MeetingAttendeeList: [{left: 20, top: 5, width: -50, height: 40}],
                EditMeetingMinutes: [{left: 20, top: 5, width: -50, height: 40}],
                ShowMeetingMinutes: [{left: 20, top: 5, width: -50, height: 40}],
                CreateProject: [{left: 20, top: 5, width: -120, height: 40}],
                CodePreview: [{left: 20, top: 5, width: 620, height: 60}],
                CodeEditor: [{left: 20, top: 5, width: 620, height: 60}],
                Profile: [{left: 20, top: 5, width: -120, height: 40}],
                GroupProfile: [{left: 20, top: 5, width: -120, height: 40}],
                ResetPassword: [{left: 20, top: 5, width: 400, height: 40}],
                CheckForUpdate: [{left: 20, top: 5, width: 500, height: 40}],
                Setting: [{left: 20, top: 5, width: -100, height: 40}],
                TermsOfService: [{left: 20, top: 5, width: 500, height: 40}],
                //Feedback: [{ left: 20, top: 5, width: 430, height: 40 }],
                AlarmedMeeting: [{left: 20, top: 5, width: -50, height: 40}],
                ApiMsgPreview: [{left: 20, top: 5, width: 510, height: 60}],
                selectCloudFile: [{left: 20,top:0, width:429, height: 40},{left: 434,top:0, width:210, height:10},{left: 655,top:0, width:25, height:40}],
                saveCloudFile: [{left: 20,top:0, width:429, height: 40},{left: 434,top:0, width:210, height:10},{left: 655,top:0, width:25, height:40}],
                CloudSetting: [{left: 20, top: 5, width: -50, height: 60}],
                CloudLibCreate: [{left: 20, top: 5, width: -50, height: 60}],
                CloudFilePreviewer: [{left: 20, top: 5, width: -80, height: 60}],
                CloudChatSet: [{left: 20, top: 5, width: -50, height: 60}],
                ApiNoticePreview: [{left: 20,top: 5,width: -80, height: 60}],
                ApiOAApprove: [{left: 20, top: 5, width: 350, height: 60}],
                FloatingCallWindow: [{left: 20, top: 5, width: 250, height: 20}],
                Map: [{left: 20, top: 5, width: -80, height: 30}],
	            JoinMeetingRoomMeeting: [{left: 0, top: 5, width: -50, height: 50}],
	            JoinMeeting: [{left: 0, top: 5, width: -80, height: 80}],
                PbxDialWindow: [{left: 0, top: 0, width: 192, height: 29}],
                PbxCallWindow: [{left: 0, top: 0, width: 280, height: 29}]
            },
            TimeFormat: {
                Message: {
                    None: 0,
                    HourMinutes: 1,
                    HourMinutesSecond: 2,
                    MonthDay: 3,
                    YearMonthDayHourMinuteSecond: 4,
                    YearMonthDay: 5,
                    Format: {
                        1: '{1}:{2}',
                        2: '{1}:{2}:{3}',
                        3: '{1}/{2}',
                        4: '{1}-{2}-{3} {4}:{5}:{6}',
                        5: '{1}-{2}-{3}',
                        6: '{1}月{2}日'
                    }
                },
                Meeting: {
                    None: 0,
                    HourMinutes: 1,
                    MonthDay: 2,
                    YearMonthDayHourMinuteSecond: 3,
                    YearMonthDayHourMinutes: 4,
                    YearMonthDay: 5,
                    DayMonthYear: 6,
                    MonthDayYear: 7,
                    MonthDayChs: 8,
                    YearMonthDayChs: 9,
                    HourMinutesChs: 10,
                    Format: {
                        1: '{1}:{2}',
                        2: '{1}.{2}',
                        3: '{1}-{2}-{3} {4}:{5}:{6}',
                        4: '{1}/{2}/{3} {4}:{5}',
                        5: '{1}/{2}/{3}',
                        6: '{1}/{2}/{3}',
                        7: '{1}/{2}/{3}',
                        8: '{1}月{2}日',
                        9: '{1}年{2}月{3}日',
                        10: '{1}/{2}'
                    }
                }
            },
            WindowCloseBehavior: {
                Close: 0,
                Hide: 1,
                Quit: 2,
                Minimize: 3,
                UnClose: 4
            },
            DirectoryType: {
                Audio: 1,
                Video: 2,
                Image: 3,
                File: 4,
                Avatar: 5
            },
            OpenChatWindowSourceModule: {
                Conversation: 0,
                Other: 1
            },
            ClipboardDataType: {
                Text: 0,
                Image: 1,
                File: 2
            },
            AvatarType: {
                User: 100,
                Project: 101
            },
            DefaultLocalUrl: {
                Conversation: {
                    System: '../images/conversation/icon_system_default.png',
                    Avatar: '../images/icon_avatar_default_40.png',
                    Project: '../images/icon_project_default_40.png',
                    CloudFile: '../images/icon_cloud_default_80.jpg',
                    Token: '../images/conversation/icon_token_default.png',
                    Bulletin: '../images/conversation/icon_bulletin_default.png',
                    Org: '../images/conversation/icon_org_default.png',
                    OpenApi: '../images/conversation/icon_system_default.png'//TODO user open Api icon
                },
                Avatar: '../images/icon_avatar_default_100.png',
                Avatar_32: '../images/icon_avatar_default_32.png',
                Avatar_40: '../images/icon_avatar_default_40.png',
                Avatar_60: '../images/icon_avatar_default_60.png',
                Avatar_80: '../images/icon_avatar_default_80.png',
                Avatar_100: '../images/icon_avatar_default_100.png',
                Project: '../images/icon_project_default_80.png',
                Project_32: '../images/icon_project_default_32.png',
                Project_40: '../images/icon_project_default_40.png',
                Project_80: '../images/icon_project_default_80.png',
                Cloud_Avatar_40: '../images/icon_cloud_default_40.png',
                CloudGroup: '../images/icon_cloud_default_80.jpg',
                PbxAvatar_100: '../images/call/icon_pbx_mobile_100.png',
                PbxAvatar_40: '../images/call/icon_pbx_mobile_40.png'
            },
            LogLevel: {
                Info: 0,
                Warning: 1,
                Error: 2
            },
            MenuItemType: {
                MenuItem: 0,
                Split: 1,
                MenuParent: 2
            },
            GroupAvatarUserCount: 2,
            KeyCode: {
                Enter: 13,
                ESC: 27
            },
            WindowShadowWidth: {
                Top: 5,
                Left: 15,
                Right: 15,
                Bottom: 15
            },
            CrossWindowEventName: {
                OpenCalendarWindow: 'OpenCalendarWindow',
                OnSettingChanged: 'OnSettingChanged',
                ModifyingPassword: 'ModifyingPassword',
                OnRestartClient: 'OnRestartClient',
                OnPasswordReset: 'OnPasswordReset',
                OpenChatWindow: 'OpenChatWindow',
                OnProxyAuthenticateSet: 'OnProxyAuthenticateSet',
                OnForwardMeetingVideo: 'OnForwardMeetingVideo',
                OpenSettingWindow: 'OpenSettingWindow',
                OnMeetingVideoMake: 'OnMeetingVideoMake',
                OnShareMeetingVideo: 'OnShareMeetingVideo',
                OpenChatWindowFromBubbleWindow: 'OpenChatWindowFromBubbleWindow',
                ShowPopupBubbleMessage: 'ShowPopupBubbleMessage',
                UpdateBubbleMessageList: 'UpdateBubbleMessageList',
                OnMainWindowOnTop: 'OnMainWindowOnTop',
                SwitchFloatingCallWindowToEmbedded: 'SwitchFloatingCallWindowToEmbedded',
                RejectCallInvitation: 'RejectCallInvitation',
                UpdateInProgressCallInfo: 'UpdateInProgressCallInfo',
                CancelCall: 'CancelCall',
                LeaveCall: 'LeaveCall',
                JoinCall: 'JoinCall',
                CloseFloatingCallWindow: 'CloseFloatingCallWindow',
                chooseContactsCreateCall: 'chooseContactsCreateCall',
                AddGroupMemberToCall: 'AddGroupMemberToCall',
                IsMuteCall: 'IsMuteCall',
                CloseGroupProfile: 'CloseGroupProfile',
                MinimizeWindow: 'MinimizeWindow',
                ReOpenCallWindowFormMessage: 'ReOpenCallWindowFormMessage',
                ReOpenPbxChatDetail: 'ReOpenPbxChatDetail',
                PbxFloatingGetCallData: 'PbxFloatingGetCallData',
                PbxMainWindowGetCallDataToFloating: 'PbxMainWindowGetCallDataToFloating',
                PbxFloatingWindowSwitchMainWindow: 'PbxFloatingWindowSwitchMainWindow',
                PbxDialWindowSwitchMainWindow:'PbxDialWindowSwitchMainWindow',
                LeaveCallSaveCallLogToPbx:'LeaveCallSaveCallLogToPbx',
                startScreenShare: 'startScreenShare',
                stopScreenShare: 'stopScreenShare',
                closeScreenShareWindow: 'closeScreenShareWindow'
            },

            HotKeyType: {
                CaptureScreen: 0,
                ShowClient: 1
            },
            HotKey: {
                Alt: 1,
                Control: 2,
                Shift: 4,
                Win: 8
            },
            WindowState: {
                Normal: 0,
                Maximized: 1
            },
            WindowActiveState: {
                Normal: 0,
                Hidden: 1,
                Minimized: 2,
                Maximized: 3,
                Topmost: 4
            },
            DraftSource: {
                Event: 0,
                DraftDataSet: 1
            },
            GroupInviteMsgUserMaxCount: 10,
            chooseCallContacts: {
                createCall_Ok: 1,
                createcall_fail: 0
            },
            DatePicker: {
                DateMode: {
                    OneWeek: 1,
                    OneMonth: 2,
                    TwoWeek: 3
                }
            },
            TimeStampUnit: {
                Second: 0,
                MilliSecond: 1
            }
        },
        Switch: {
            OpenMultiVersion: 1,
            OpenUserStatus: 1
        },
        UserStatusType: {
            Online: 1, // 在线
            Busy: 2, // 请勿打扰
            Away: 3, // 离开
            InMeeting: 4, // 会议中
            Offline: 5, // 离线
            InCall: 6 // 呼中（新增）
        },
        Api: {
            filter: {
                All: 0,
                Untreated: 1,
                Treated: 2,
                Other: 3
            },
            AppChange: {
                Add: 1,
                Disabled: 2,
                Modify: 3
            },
            OAApproveMaxLen: 128,
            titleStyleMode: {
                Default: 'default',
                simple: 'simple'
            }
        },
        Http: {
            Get: 1,
            Post: 2
        },
        chooseContacts: {
            allowMultipleSelectConversations: 9
        },
        Timer: {
            OneSecond: 1000,
            OneAndAHalfSeconds: 1500,
            HalfMinutes: 30000,
            OneMinutes: 60000
        },
        Profile: {
            InputBox: {
                EmailTagId: 184674,
                PhoneTagId: 184671,
                SetMaxLength: 50,
                EnterKeyCode: 13
            },
            Me: {
                DisplayNameIndex: 0,
                PhoneIndex: 1,
                EmailIndex: 2
            }
        }
    }
})(uc);