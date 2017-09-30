#if !defined(__IBEE2MEETINGPLUGIN_H__)
#define __IBEE2MEETINGPLUGIN_H__

#if _MSC_VER > 1000
#pragma once
#endif

class IBee2MeetingPluginSink
{
public:
	virtual void NotifyUpdateResult(int Result) = 0;/*开始更新（1），更新成功（2），更新失败（3），超时（4）不需要更新（0）*/
};


class IBee2MeetingPlugin
{
	public:
		virtual void setBeePluginSink(IBee2MeetingPluginSink *pSink) = 0;

		/*
			action：标识蜜蜂吊起云会议客户端，固定值为11。

			env：标识运行环境，值为a b c d之一。

			timeout：为超时时间,单位毫秒，建议为10000，即十秒。

			返回值： 0表示已经吊起云会议客户准备升级；100表示云会议客户已经启动，停止升级；101表示云会议客户正在更新中 102表示不能静默升级
		*/
		virtual int updateMeetingClient(int appid, int siteid, int skinid, int action, const char *env, int timeout) = 0;
};


extern "C"
{
	__declspec(dllexport)	void*	sgCreateInstance(const char * pszInterfaceName);
	__declspec(dllexport)	void	sgDestroyInstance(void* lpInterface);
}

#endif // __IBEE2MEETINGPLUGIN_H__  