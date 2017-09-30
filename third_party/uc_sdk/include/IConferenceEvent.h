//
//  IConferenceEvent.h
//  BeeCoference
//
//  Created by Hualong Zhang on 15/5/20.
//  Copyright (c) 2015年 Hualong Zhang. All rights reserved.
//

#ifndef CONFERNECE_ICONFERENCEEVENT_H
#define CONFERNECE_ICONFERENCEEVENT_H
#include "ITangUser.h"
#include "UCMessage.h"

namespace uc {

	class IConferenceEvent {
	public:

		/*
		* @brief 加入会议结果的返回事件
		* @param statusCode 状态码
		* 0 join succeeded 
		* 1 reconnect succeeded (deprecated)
		* 2 network connect failed
		* 3 network auth failed
		* 4 get conference info failed
		* 5 get user info failed
		* @remarks 好像方法用于加入会议的回调，重连会通过OnConferenceReconnected返回
		* @see OnConferenceReconnected
		*/
		virtual void OnConferenceJoined(int statusCode, int createType, int joinType, int conferenceId, int userId) = 0;

		/*
		* @brief 开启音频状态结果的返回事件
		* @param statusCode 状态码
		* 0 start audio succeeded 
		* 其他代表错误
		* @remarks 好像方法用于加入会议的回调，重连会通过OnConferenceReconnected返回
		* @see OnConferenceReconnected
		*/
		virtual void OnAudioStarted(int statusCode, int callType, int joinType, int conferenceId, UCID ucId) = 0;

		/*
		* @brief 用户入会通知
		* @param tangUser   新加入会议的用户
		* @param conferenceId  会议ID
		*/
		virtual void OnUserAdded(const ITangUser *tangUser, int conferenceId, int callType) = 0;

		/* 
		* @brief 用户退会通知
		* @param tangUser   退出会议的用户
		* @param conferenceId  会议ID
		*/
		virtual void OnUserRemoved(const ITangUser *tangUser, int conferenceId, int callType) = 0;

		/* 
		* @brief 用户属性更新通知
		* @param tangUser   属性发生变更的用户
		* @param properType 属性类型
		* @param conferenceId  会议ID
		* 1 音频类型发生变化
		* 2 静音状态发生变化
		* 3 音频类型和静音状态发生变化
		*/
		virtual void OnUserPropertyChanged(const ITangUser *tangUser, int properType, int conferenceId, int callType) = 0;

		/* 
		* @brief 会议结束通知
		* @remarks 包括主持人结束会议，服务器结束等
		*/
		virtual void OnConferenceEnded(int conferenceId, int callType, int statusCode) = 0;

		/* 
		* @brief 会议断开连接
		* @param conferenceId  会议ID
		*/
		virtual void OnConferenceDisconnected(int conferenceId) = 0;

		/* 
		* @brief 会议重新连接上的通知
		* @param statusCode 状态码
		* 0 join succeeded 
		* 1 reconnect succeeded (deprecated)
		* 2 network connect failed
		* 3 network auth failed
		* 4 get conference info failed
		* 5 get user info failed
		* @remarks 此方法用于重连会议的回调，初次入会通过onConferenceJoined返回
		* @see OnConfReconnected
		*/
		virtual void OnConferenceReconnected(int statusCode, int conferenceId) = 0;

		/* 
		* @brief 拨打电话结果的返回事件
		* @param phoneNumber 拨打的电话号码
		* @param errorCode   错误码
		* 1 被会议助理挂断，即程序挂断的
		* 2 对端挂断
		* 3 无应答
		* 4 电话忙
		* 其他
		*/
		virtual void OnCallPhoneFailed(const std::string &phoneNumber, const std::string &errorCode, int conferenceId) = 0;

		/* 
		* @brief 拨打电话结果的返回事件
		* @param isLoud 是否打开扬声器
		* @remarks 此回调方法仅用于移动端
		*/
		virtual void OnLoudSpeakerStatusChanged(bool isLoud) = 0;

		/*
		* @brief call关闭回调事件
		* @param conferenceId call 会议ID
		* @param callType 0:一对一call 1:群call
		* @param userId 用户ID或群ID
		*/
		virtual void OnCallClose(int conferenceId, int callType, int userId, bool neednotify = true) = 0;

		/*
		* @brief VOIP网络质量回调
		* @param quality 网络质量
		*/
		virtual void OnVoipNetworkQuality(int quality) = 0;

		/*
		* @brief 麦克风状态变更
		* @param type 0：level 1：volume
		* @param value 大小
		*/
		virtual void OnMicrophoneStatusChanged(int type, int value) = 0;

		/*
		* @brief 扬声器状态变更
		* @param type 0：level 1：volume
		* @param value 大小
		*/
		virtual void OnSpeakerStatusChanged(int type, int value) = 0;

		/*
		* @brief 通话状态变更
		* @param conferenceId 会议ID
		* @param callType 0：一对一呼 1：群呼
		* @param userId 用户ID
		* @param status 0:未发言 1，正在发言（Internal描述，包含VOIP和PSTN）
		*/
		virtual void OnSpeakingStatusChanged(int conferenceId, int callType, int userId, int status) = 0;

		/*
		* @brief 音频服务状态
		* @param status 0:音频服务尚未准备好 1：音频服务准备完成
		*/
		virtual void OnAudioServiceReady(int status) = 0;
	};

	//video service event
	class IVideoEvent {
	public:
		virtual void OnVideoPropertyChanged() = 0;

		virtual void OnVideoInstanceAdded(unsigned int userId) = 0;

		virtual void OnVideoInstanceRemoved(unsigned int userId) = 0;

		virtual void OnSessionErrorHandle() = 0;
	};
}

#endif //CONFERNECE_ICONFERENCEEVENT_H
