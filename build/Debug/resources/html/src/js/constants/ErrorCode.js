/**
 * Created by senlin.ouyang on 2015/7/18.
 */
(function(uc){
    uc.ErrorCode = {
        Success: 0,
        NeedUpdate: 1,
        OnlyOneApp: 1,
        NoResult: 0,
        /*** Error Code from SDK **/
        SDK_Call_Failed: 100, //调用SDK接口错误
        SDK_UCC_Status_Code_Error: 101, //请求UCC Server未成功，如404，500等，非200
        SDK_JSON_Parse_Error: 102,      //解析JSON数据失败
        SDK_UCAS_Close_Abnormally: 103, //UCAS连接非正常关闭
        SDK_Server_Error: 104,          // htpp请求成功，但服务器出现了内部错误
        SDK_Call_Pending: 105,          // 表示异步操作正在执行
        SDK_User_Cancel: 106,           // 用户取消了操作
        SDK_UCAS_Close_NoData: 108,     // 长时间没有收到UCAS的任何消息

        /*** Error Code for session expired **/
        Client_Session_Expired: 10101,

        /*** Error Code for Login **/
        Login_User_Or_Pass_Empty: 10137,        //用户名密码为空
        Login_Client_Info_Error: 10138,         //客户端信息错误
        Login_Client_Type_Error: 10139,         //客户端类型错误
        Login_Client_Need_Update: 10152,        //客户端需要强制更新
        Login_Client_Id_Or_Mac_Empty: 10411,    //客户端id和mac地址不能为空
        Login_UCAS_Channel_Error: 10140,		//调用ucas接口初始化ucas数据,输入的站点id和客户编码不存在
        Login_User_Not_Exist: 10128,            //用户不存在，或者UMS内部错误
        Login_UMS_Error: 10434,                 //批量获取用户联系人版本号失败,unexcept ums error
        Login_User_Account_Closed: 10433,       //用户账号被停用

        /*** Error Code for FS **/
        FS_File_Downloading: 60032,       //文件正在下载
        FS_Task_Cancelled: 60021                 //任务已经取消
    }
})(uc);