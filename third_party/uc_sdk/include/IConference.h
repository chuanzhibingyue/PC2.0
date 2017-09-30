//
//  IConference.h
//  BeeCoference
//
//  Created by Hualong Zhang on 15/5/20.
//  Copyright (c) 2015年 Hualong Zhang. All rights reserved.
//

#ifndef CONFERENCE_ICONFERENCE_H_
#define CONFERENCE_ICONFERENCE_H_
#include "ITangUser.h"

namespace uc {

	class IConference {
	public:
		/**
		* @brief 加入呼会议
		* @param joinKey	会议调起串
		*/
		//virtual int JoinConference(const std::string &joinKey) = 0;

		/**
		* @brief 退出呼会议
		* @remarks 在呼的会议中每个人都是主持人，所以调用者不用关心是退出还是结束。
		*		   会由底层业务代码根据当前会议中的人数做智能处理:
		*		       如果会议中只剩自己一个人，则调用SDK的结束会议接口;
		*              如果会议中超过一个人，则调用SDK的退出会议接口。
		*/
		//virtual int LeaveConference() = 0;

		/**
		* @brief 主动结束会议
		*/
		virtual int EndConference() = 0;

		/**
		* @brief 重连接口
		* @param joinKey	会议调起串
		* @remarks 当会议进行过程中，发生断网，会通过回调接口抛给应用，应用在得到通知后，根据相应的场景调用该接口
		*/
		virtual int ReconnectConference() = 0;

		/**
		* @brief 切换到电话
		* @param phoneNumber	电话号码，手机号码：151********；固定电话：0296568****
		*/
		virtual int SwitchToPhone(const std::string &phoneNumber) = 0;

		/**
		* @brief 切换到VOIP
		*/
		virtual int SwitchToVOIP() = 0;

		/**
		* @brief 自我静音
		*/
		virtual int MuteSelf() = 0;

		/**
		* @brief 解除自我静音
		*/
		virtual int UnMuteSelf() = 0;

		/**
		* @brief 自我挂断电话
		*/
		virtual int HangupPhone() = 0;

		/**
		* @brief 开启扬声器模式
		*/
		virtual int EnableLoudSpeaker() = 0;

		/**
		* @brief 关闭扬声器模式
		*/
		virtual int DisableLoudSpeaker() = 0;
	};

}

#endif //CONFERENCE_ICONFERENCE_H_
