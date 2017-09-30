#ifndef IMICROBLOGSERVICE_H_
#define IMICROBLOGSERVICE_H_

#include "UCSDK.h"
#include "IUCEngine.h"

namespace uc {

	interface UC_API IMicroblogServiceEvent {
		virtual ~IMicroblogServiceEvent() {};

		virtual void OnNewMsgCountGet(int32_t errorCode, int32_t countAll, int32_t relationCount, int32_t timenewsCount, int32_t pollCount, int32_t caseCount) = 0;
		/**
		* 添加关注的回调事件
		* @param errorCode 错误码
		* @param userId 关注用户的id
		* @param relation 关系 1:关注, 2:粉丝, 3:相互关注, 4:没有关联
		*/
		virtual void OnAttentionAdd(int32_t errorCode, int32_t userId, int32_t relation) = 0;

		/**
		* 取消关注的回调事件
		* @param errorCode 错误码
		* @param userId 取消关注用户的id
		* @param relation 关系 1:关注, 2:粉丝, 3:相互关注, 4:没有关联
		*/
		virtual void OnAttentionCancelled(int32_t errorCode, int32_t userId, int32_t relation) = 0;

		/**
		* 会议录制分享回调事件
		* @param errorCode 错误码
		* @param resourceID 新贴ID
		*/
		virtual void OnMeetingRecordingShared(int32_t errorCode, int64_t resourceID) = 0;
	};

	interface UC_API IMicroblogService : public IUCService, Event<IMicroblogServiceEvent> {
		virtual ~IMicroblogService() {};

		/**
		 * 获取自己的工作圈主页
		 */
		virtual std::string GetHomePageURL(int32_t userId) = 0;

		/**
		* 获取别人的工作圈主页
		* @param SelfId 自己的Id
		* @param userId 目标的Id
		*/
		virtual std::string GetUserHomePageURL(int32_t selfId, int32_t userId) = 0;

		/**
		 * 获取未读消息数量的URL
		 */
		virtual void GetNewMsgCount() = 0;

		/**
		 * 添加关注
		 * @param userId 待关注用户id
		 */
		virtual void AddAttention(int32_t userId) = 0;

		/**
		 * 取消关注
		 * @param userId 待取消关注的用户id
		 */
		virtual void CancelAttention(int32_t userId) = 0;

		/**
		 * 获取关注状态
		 * @param userId 对方用户id
		 * @return 返回值 1:关注, 2:粉丝, 3:相互关注, 4:没有关联
		 */
		virtual int32_t GetRelation(int32_t userId) = 0;

		/**
		 * 分享会议recording
		 * @param url recording地址
		 * @param comments 注释
		 */

		virtual void ShareMeetingRecording(std::string urlVideo,std::string urlThumb, std::string fileName,int64_t fileSize,int64_t voiceTime, std::string comments) = 0;
	};
}


#endif /* IMICROBLOGSERVICE_H_*/