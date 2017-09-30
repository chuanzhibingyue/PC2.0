(function(uc){
    uc.modules.pbx.PbxInfo = function(){
        this.userName = '';
        this.userID=0;
        this.phoneNumber = 0;
        this.incoming = 0;
        this.type = 0;
        this.time = 0;
        this.state = 0;
        this.druation = 0;
        this.selected = false;
        this.isInCall = false;
        this.avatar =0;
        this.count = 0;
    };
    jQuery.extend(uc.modules.pbx.PbxInfo.prototype,{
        init:function(options){
            this.userName = options.name ? options.name : this.userName;
            this.userID = options.userID ? options.userID : this.userID;
            this.avatar = options.avatar ? options.avatar : this.avatar;
            this.phoneNumber = options.number	? options.number : this.phoneNumber;
            this.incoming = options.incoming ? options.incoming : this.incoming;
            this.type = options.type ? options.type : this.type;
            this.time = options.time ? options.time : this.time;
            this.state = options.state ? options.state : this.state;
            this.druation = options.druation ? options.druation : this.druation;
            this.isInCall = options.isInCall ? options.isInCall : this.isInCall;
            this.count = options.count ? options.count : this.count;
        },
        setSelected: function(selected){
            this.selected = selected;
        },
        update: function(options){
            this.userName = options.name ? options.name : this.userName;
            this.userID = options.userID ? options.userID : this.userID;
            this.incoming = options.incoming ? options.incoming : this.incoming;
            this.state = options.status ? options.state : this.state;
            this.time = options.time > this.time ? options.time : this.time;
            this.druation = options.druation ? options.druation : this.druation;
            this.count = options.count ? options.count : this.count;
        },
        updateData: function(updateData){
            this.userName = updateData.userName;
            this.phoneNumber = updateData.phoneNumber ? updateData.phoneNumber : this.phoneNumber;
            this.incoming = updateData.incoming ? updateData.incoming : this.incoming;
            this.type = updateData.type ? updateData.type : this.type;
            this.time = updateData.time ? updateData.time : this.time;
            this.state = updateData.state ? updateData.state : this.state;
            this.druation = updateData.druation ? updateData.druation : this.druation;
            this.count = updateData.count ? updateData.count : this.count;
        }
    })
})(uc);