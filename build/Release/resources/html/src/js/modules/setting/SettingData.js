/**
 * Created by pan.li on 2015/9/10.
 */
(function(uc){
    uc.settings = {
        autoRun: true,              // 开机自启动
        topEst: false,              // 最顶层显示
        bubbleSet: true,            // 新消息弹出气泡提示
        msgPrompt: {                // 消息提示音
            playPrompt: false,      // 是否播放
            name: "01.amr"          // 文件名
        },
        skMsg: 0,                   // 发送消息快捷键， 0: enter, 1: ctrl + enter
        skScreenshot: {             // 截图快捷键
            ctrlKey: true,
            shiftKey: false,
            altKey: true,
            keyCode: 65,
            hideWindow: false       // 截图时候隐藏当前窗口
        },
        skOpenWindow: {             // 打开主界面快捷键
            ctrlKey: true,
            shiftKey: false,
            altKey: true,
            keyCode: 87
        },
        textMsgStyle: {             // 文字消息样式
            fontSize: 14,
            fontColor: "",
            bold: false,
            italic: false,
            underline: false
        },
        meeting: {                  // 日程
            timeZone: {             // 时区
                offset: parseInt(new Date().getTimezoneOffset()/60)*(-1),  // 小时
                name: function(){
                    var i = 0;

                    while(i < uc_resource.Meeting.CreateMeetingTimeZone.length){
                        if(uc_resource.Meeting.CreateMeetingTimeZone[i].value == (-1) * parseInt(new Date().getTimezoneOffset()/60)){
                            return uc_resource.Meeting.CreateMeetingTimeZone[i].text;
                        }else{
                            i++;
                        }
                    }
                }()                 // "(UTC+08:00)北京，重庆，香港特别行政区，乌鲁木齐"
            },
            dateFormat: "2015/09/10",
            timeFormat: 0,          // 时间显示格式: 0, 24小时制； 1，12小时制
            timeInterval: 30,       // 时间间隔(分钟)
            meetingDuration: 60     // 默认会议时长（分钟）
        }
    };
})(uc);