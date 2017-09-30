#pragma once

//PBX error code

#define EC_FAIL			30000 // 其他
#define EC_CALLING		30001 // 通话中
#define EC_PARAMETER2	30002 // 参数错误
#define EC_MAKECALL		30003 // 呼叫失败
#define EC_CALLID_EXIST 30004 // callID 已经存在
#define EC_OPERATE		30005 // 不合理的操作
#define EC_STATE		30006 // 当前状态不允许此操作
#define EC_MIC_IN		30007 // 没有麦克风输入
#define EC_MIC_OUT		30008 // 没有麦克风输出
#define EC_MIC_INOUT	30009 // 没有麦克风输入输出
#define EC_MAKING		30010 // 正在创建