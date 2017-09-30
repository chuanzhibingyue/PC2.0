(function (uc) {
    var _this = null;
    uc.modules.pbx.PbxManager = {
        DialIsClick:true,
        callWindowIsExist:false,
        getHistoryTime:0,
        sortedPbxList: [],
        notifyDataChangedTimer: -1,
        notifyDataChangedDelay: 300,
        currentCallUserID:0,
        selectedPbxInfo: null,
        historyCallLoadEnd:false,
        moduleId:0,
        synUserId:{},
        OnModuleChanged: function(moduleId){
            uc.modules.pbx.PbxManager.moduleId=moduleId;
            if(moduleId == uc.constants.UI.ModuleId.Pbx){
                uc.modules.pbx.PbxManager.init();
            }
        },
        init:function(){
            _this = this;
            if(this.initilized){
                return;
            }
            this.initilized = true;
            this.pbxDataSet = new uc.modules.pbx.PbxDataSet();
            this.bindEvent();
            this.getPbxList();
            this.initPbxList();
        },
        bindEvent:function(){
            var self = this;
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxFloatingGetCallData, function(data){
                uc.modules.pbx.PbxCallManager.getMainCallDataToPbxFloating(data);
            });
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxFloatingWindowSwitchMainWindow, function(data){
                if(uc.modules.pbx.PbxManager.moduleId != uc.constants.UI.ModuleId.Pbx){
                    uc.IUIService.changeModule(uc.constants.UI.ModuleId.Pbx);
                }
                uc.util.WindowUtil.show();
                uc.modules.pbx.PbxCallManager.PbxFloatingWindowSwitchMainWindow(data);
            });
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxDialWindowSwitchMainWindow, function(data){
                self.dialPadCallback(data.phoneNumber);
            });
        },
        getPbxList:function(){
            uc.IPbxService.getCallRecordLst(uc.constants.Pbx.GetCallCount,this.getHistoryTime,function(args){
                uc.util.LogUtil.info('PbxManager','getPbxList', 'errorCode = ' + args[0], null);
            });
        },
        initPbxList:function(){
//            this.PbxDialPad=new uc.modules.component.PbxDialPad(
//                {"callBack":this.dialPadCallback,"selectedDialCode":"+86"});
            this.PbxListAdapter = new uc.modules.pbx.PbxListAdapter(this);
            this.PbxListAdapter.init();
            this.PbxList = new uc.modules.pbx.PbxList(this.PbxListAdapter);
            this.PbxList.init();
            this.PbxDetail = new uc.modules.pbx.PbxDetail();
            this.PbxDetail.init();
        },
        OnWindowResized: function(){
            if(this.PbxList){
                this.PbxList.notifyDataSetChanged();
            }
        },
        OnPbxListReceived:function(errorCode,data){
            uc.util.LogUtil.info('PbxManager','OnPbxListReceived', 'errorCode = ' + errorCode, null);
            if(errorCode == 0 || errorCode == 1004){
                var len = data.length;
                var callRecordListData, pbxInfo;
                for(var i = 0; i < len; i++){
                    callRecordListData = data[i];
                    pbxInfo = _this.pbxDataToPbxInfo(callRecordListData);
                    _this.addPbx(pbxInfo);
                }
                if(len){
                    this.getHistoryTime=data[len-1].time;
                }
                if(len<uc.constants.Pbx.GetCallCount){
                    uc.modules.pbx.PbxManager.historyCallLoadEnd=true;
                }
            }
        },
        pbxDataToPbxInfo:function(data){
            var pbxNum=data.number;
            if(data.userID && data.userID != 0){
                pbxNum=data.userID;
            }
            var pbxInfo = this.getPbx(pbxNum);
            var options = {
                name: data.name,
                number: data.number,
                userID: data.userID,
                incoming : data.incoming,
                time: data.time,
                state: data.state,
                count: data.count,
                druation : data.druation
            };
            if(options.userID){
                var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(Number(options.userID));
                if(contactInfo){
                    options.userName=contactInfo.displayName;
                    options.name=contactInfo.displayName;
                    options.avatar=contactInfo.localAvatar;
                }
            }
            if(pbxInfo){
                pbxInfo.update(options);
            }else{
                pbxInfo = new uc.modules.pbx.PbxInfo();
                pbxInfo.init(options);
            }
            return pbxInfo;
        },
        addPbx: function (pbxInfo) {
            if (!pbxInfo) {
                return;
            }
            var pbxNum=pbxInfo.phoneNumber;
            if(pbxInfo.userID && pbxInfo.userID!=0){
               // pbxNum=pbxInfo.phoneNumber+pbxInfo.userID;
                pbxNum=pbxInfo.userID;
            }
            if (this.pbxDataSet.isPbxExist(pbxNum)) {
                this.updatePbx(pbxNum,pbxInfo);
            } else {
                this.pbxDataSet.addPbx(pbxNum,pbxInfo);
                this.notifyDataSetChanged();
            }
        },
        updatePbx: function (pbxNum,pbxInfo) {
            if (!pbxInfo) {
                return;
            }
            if (this.pbxDataSet.isPbxExist(pbxNum)) {
                this.pbxDataSet.updatePbx(pbxNum,pbxInfo);
                this.notifyDataSetChanged();
            } else {
                this.addPbx(pbxInfo);
            }
        },
        removePbx: function (pbxNum) {
            if (this.pbxDataSet.isPbxExist(pbxNum)) {
                var pbxInfo = this.getPbx(pbxNum);
                this.pbxDataSet.removePbx(pbxNum);
                this.updateSelectedPbx();
                this.notifyDataSetChanged();
            }
        },
        updateSelectedPbx:function(){
            var index, pbxInfo;
            index = this.PbxList.getNextSelectedPbxIndex();
            if (index != -1) {
                pbxInfo = this.sortedPbxList[index];
                if (pbxInfo) {
                    uc.IUIEvent.OnOpenPbxCallChat(pbxInfo);
                }
            }
        },
        notifyDataSetChanged: function () {
            if (!this.PbxListAdapter) {
                return;
            }
            clearTimeout(this.notifyDataChangedTimer);
            this.notifyDataChangedTimer = setTimeout(function () {
                _this.sortPbx();
                _this.PbxListAdapter.notifyDataSetChanged();
            }, this.notifyDataChangedDelay);
        },
        sortPbx: function () {
            this.sortedPbxList = this.getAllPbx();
            this.sortedPbxList.sort(this.compareFunction);
            return this.sortedPbxList;
        },
        getAllPbx: function () {
            if(this.pbxDataSet){
                return this.pbxDataSet.getAllPbx();
            }
        },
        getPbxCount: function () {
            return this.pbxDataSet.getPbxCount();
        },
        compareFunction: function (a, b) {
    //        return Math.max(b.time, b.time) - Math.max(a.time, a.time);
            if(a.isInCall && !b.isInCall){
                return -1;
            }else if(!a.isInCall && b.isInCall){
                return 1;
            }else{
                return Math.max(b.time, b.time) - Math.max(a.time, a.time);
            }
        },
        getPbx: function (PbxNum) {
            return this.pbxDataSet.getPbx(PbxNum);
        },
        isPbxExist: function (key) {
            return this.pbxDataSet.isPbxExist(key);
        },
        getSortedPbxCallList: function (start, length) {
            var result = [];
            if (!this.sortedPbxList || this.sortedPbxList.length == 0) {
                return result;
            }
            if (start < 0 || start > this.sortedPbxList.length - 1) {
                return result;
            }
            if (length == -1) {
                length = this.sortedPbxList.length;
            }
            if (this.sortedPbxList.length - 1 >= start && start >= 0) {
                result = this.sortedPbxList.slice(start, start + length);
            }
            return result;
        },
        OnUserAvatarDownloaded: function(userId, localAvatarUrl){
            if(!this.pbxDataSet){
                return;
            }
            this.PbxDetail.updateDetailAvatar(userId,localAvatarUrl);
            var allPbxInfo = this.getAllPbx();
            var pbxInfo, needRefreshPbxList = false;
            for(var i = 0, len = allPbxInfo.length; i < len; i++){
                pbxInfo = allPbxInfo[i];
                if(pbxInfo.userID && pbxInfo.userID == userId){
                    needRefreshPbxList = true;
                    pbxInfo.avatar=localAvatarUrl;
                }
            }
            if(needRefreshPbxList){
                this.notifyDataSetChanged();
            }
        },
        dialPadCallback:function(phoneNumber){
                var callInfo={"userName":"",
                    "userID":0,
                    "phoneNumber":phoneNumber,
                    "callType":0,
                    "time":new Date().getTime(),
                    "Type":"ADEXEO2103"
                };
                uc.modules.pbx.PbxCallManager.startCallPbx(callInfo);
        },
        scrollToTop:function(){
           this.PbxList.scrollToTop();
        },
        prepareCallData:function(data){
            if(this.initilized){
                uc.modules.pbx.PbxCallManager.prepareCallData(data);
            }
        },
        restoreDialStatus:function(flag){
            uc.modules.pbx.PbxManager.DialIsClick = flag;
        },
        restoreCallWindowStatus:function(){
            uc.modules.pbx.PbxManager.callWindowIsExist = false;
        },
        OnOpenPbxCallChat:function(data){
            this.PbxDetail.loadPbxDetail(data);
            if(uc.modules.pbx.PbxManager.DialIsClick){
                var pbxNum= data.phoneNumber;
                if(data.userID && data.userID != 0 ){
                    pbxNum= data.userID;
                }
                var info= this.getPbx(pbxNum);
                if(info){
                    this.setSelectedPbxInfo(info);
                    this.notifyDataSetChanged();
                }
            }
        },
        setSelectedPbxInfo: function (data) {
            if (!this.selectedPbxInfo) {
                data.setSelected(true);
                this.selectedPbxInfo = data;
            } else {
                this.selectedPbxInfo.setSelected(false);
                data.setSelected(true);
                this.selectedPbxInfo = data;
            }
        },
        OnPbxHangUpCallStatus:function(errorCode,data){
            if(uc.modules.pbx.PbxCallManager.currentCallIngId == data.time){
                uc.modules.pbx.PbxCallManager.OnPbxHangUpCallStatus(data);
                if((errorCode == uc.ErrorCode.Success) && (this.initilized)){
                        var pbxNum=data.number;
                        if(this.currentCallUserID && this.currentCallUserID != 0){
                            pbxNum=this.currentCallUserID;
                        }
                        var Info = this.getPbx(pbxNum);
                        if(Info){
                            Info.incoming= data.incoming;
                            Info.state= data.state;
                            Info.time= data.time;
                            Info.druation= data.druation;
                            Info.count= (Info.count*1) + 1;
                        }else {
                            var InfoData={
                                "name":"",
                                "userID":"",
                                "incoming" :data.incoming,
                                "time":data.time ? data.time : new Date().getTime(),
                                "state":data.state,
                                "druation": data.druation ? data.druation : 0,
                                "number":data.number,
                                "count":1
                            };
                            Info = new uc.modules.pbx.PbxInfo();
                            Info.init(InfoData);
                        }
                        var callInfo = uc.modules.pbx.PbxCallManager.onMakeCallInfo[Info.time];
                        if(callInfo && callInfo.userID && callInfo.userID!=""){
                            Info.userID = callInfo.userID;
                            var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(Number(callInfo.userID));
                            if(contactInfo){
                                Info.userName=contactInfo.displayName;
                                Info.name=contactInfo.displayName;
                                Info.avatar=contactInfo.localAvatar;
                            }
                        }
                        this.updatePbx(pbxNum,Info);
                        this.scrollToTop();
                        uc.IUIEvent.OnOpenPbxCallChat(Info);
                }
            }
        },
        SaveBeeCallDetailsToPbx:function(callData){
            var contactInfo = uc.IContactService.getContactInfo(Number(callData.userID));
            if(contactInfo){
                if(contactInfo.mobile){
                    callData.number = contactInfo.mobile;
                }
                this.saveCallDetails(callData);
            }else{
                uc.modules.pbx.PbxManager.synUserId = callData;
            }
        },
        OnPbxListUpdateCallList :function(errorCode, userIds, contacts){
            if($.isEmptyObject(uc.modules.pbx.PbxManager.synUserId)){
                return;
            }
            if(errorCode == 0){
                for (var t = 0; t < contacts.length; t++) {
                    var contact = contacts[t];
                    if(contact.userId == uc.modules.pbx.PbxManager.synUserId.userID){
                        uc.modules.pbx.PbxManager.synUserId.number = contact.mobile;
                        this.saveCallDetails(uc.modules.pbx.PbxManager.synUserId);
                        uc.modules.pbx.PbxManager.synUserId = {};
                    }
                }
            }
        },
        saveCallDetails:function(data){
            var self = this;
            uc.IPbxService.saveCallDetails(
                data.time ? data.time : new Date().getTime(),
                2, //bee call id
                data.number+"",
                data.userID,
                data.incoming,
                5, //bee call type
                data.state,
                data.druation ? data.druation : 0,
                function(arg){
                    if(arg[0] == uc.ErrorCode.Success){
                        self.updateBeeCallList(data);
                    }
                });
        },
        updateBeeCallList:function(data){
            if(data.userID){
                var InfoData;
                var pbxNum = data.userID;
                var Info = this.getPbx(pbxNum);
                if(Info){
                    Info.incoming= data.incoming;
                    Info.state= data.state;
                    Info.time= new Date().getTime();
                    Info.druation= data.druation;
                    Info.count= (Info.count*1) + 1;
                }else{
                    var InfoData={
                        "userID":data.userID,
                        "incoming":data.incoming,
                        "time": new Date().getTime(),
                        "state":data.state,
                        "number":data.number,
                        "druation":data.druation,
                        "count":1
                    };
                    Info = new uc.modules.pbx.PbxInfo();
                    Info.init(InfoData);
                }
                var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(Number(data.userID));
                if(contactInfo){
                    Info.avatar=contactInfo.localAvatar;
                    Info.userName=contactInfo.displayName;
                }
                this.updatePbx(pbxNum,Info);
                this.scrollToTop();
                uc.IUIEvent.OnOpenPbxCallChat(Info);
            }
        },
        //呼出失败，需要同步通话列表状态
        OnPbxUNCallStatus:function(data){
            uc.modules.pbx.PbxCallManager.OnPbxHangUpCallStatus(data);
                if(this.initilized){
                    var pbxNum=data.number;
                    if(this.currentCallUserID && this.currentCallUserID != 0){
                        pbxNum=this.currentCallUserID;
                    }
                    var Info = this.getPbx(pbxNum);
                    if(Info){
                        Info.isInCall = false;
                        Info.incoming = Info.incoming;
                        Info.state = 1;
                        Info.time = data.time;
                        Info.druation = Info.druation;
                        this.updatePbx(pbxNum,Info);
                    }
            }
        },
        OnPbxCallListDetail:function(errorCode,data){
            if(errorCode == 0){
                this.PbxDetail.OnPbxCallListDetail(data);
            }
        },
        OnPbxCallStarted: function(info){
            this.currentCallUserID=info.userID;
            return;
            var pbxNum=info.phoneNumber;
            if(this.currentCallUserID && this.currentCallUserID != 0){
                 pbxNum = info.userID;
            }
            var contactInfo="";
            var data={
                "name":info.userName,
                "userID":info.userID,
                "incoming" :uc.constants.Pbx.CallIncomingType.CallOut,
                "time":info.time ? info.time :new Date().getTime(),
                "number":info.phoneNumber
            };
            if(info.userID){
                 contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(Number(info.userID));
                if(contactInfo){
                    data.avatar=contactInfo.localAvatar;
                    data.name=contactInfo.displayName;
                }
            }
            var pbxInfo = this.getPbx(pbxNum);
            if(!pbxInfo){
                pbxInfo = new uc.modules.pbx.PbxInfo();
                pbxInfo.init(data);
            }
          //  pbxInfo.isInCall = true;
            if(contactInfo){
                pbxInfo.avatar=contactInfo.localAvatar;
                pbxInfo.userName=contactInfo.displayName;
            }
            pbxInfo.phoneNumber = info.phoneNumber;
            this.updatePbx(pbxNum, pbxInfo);
            this.PbxDetail.loadPbxDetail(pbxInfo);
        },
        OnPbxIncomingNotification:function(data){
            if(!this.initilized)return;
            var pbxNum = data.fromNumber;
            var pbxInfo = this.getPbx(pbxNum);
            var option={
                "number":pbxNum,
                "state":0,
                "incoming":uc.constants.Pbx.CallIncomingType.CallIn,
                "time":new Date().getTime()
            };
            if(!pbxInfo){
                pbxInfo = new uc.modules.pbx.PbxInfo();
                pbxInfo.init(option);
            }
            this.updatePbx(pbxNum,pbxInfo);
        },
        OnServerSearchResultReceived:function(errorCode,searchId, contacts, projects, groupChats, cloudGroups, meetings, messages){
            if( uc.modules.pbx.PbxManager.moduleId == uc.constants.UI.ModuleId.Pbx){
                uc.modules.component.PbxSearch.OnServerSearchResultReceived(errorCode,searchId, contacts, projects, groupChats, cloudGroups, meetings, messages)
            }
        }
    }
})(uc);