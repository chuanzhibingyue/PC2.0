#ifndef IGNETTANGDOCSHARINGSESSION_H
#define IGNETTANGDOCSHARINGSESSION_H

#include "ignettangbase.h"


/**
 *
 * @brief 文档service回调
 *
 */
class IGNetTangDocShareSessionSink : public IGNetTangBaseSessionSink
{
public:
    /**
      * @brief  文档属性变化回调
      * @param [out] propName:变化的属性名
      * @param [out] oldPropValue:属性旧值
      * @param [out] newPropValue:属性新值
      * @param [out] pItemInstance:属性对应的具体对象
      * @return void
      */
    virtual void onDocumentPropertyChanged(const char*propName,
                                          const CGNetTangVariant &oldPropValue,
                                          const CGNetTangVariant &newPropValue,
                                          IGNetTangProperty* pItemInstance) = 0;

    /**
      * @brief  文档服务属性状态变化
      * @param [out] propName:变化的属性名
      * @param [out] oldPropValue:属性旧值
      * @param [out] newPropValue:属性新值
      * @return void
      */
    virtual void onDocServicePropertyChanged(const char*propName,
                                            const CGNetTangVariant &oldPropValue,
                                            const CGNetTangVariant &newPropValue) = 0;

    /**
      * @brief  共享进度
      * @param [in] uiPercent:文档共享的进度[0-100]
      * @return void
      */
    virtual void onSharedDocStatus(unsigned int uiPercent) = 0;

    /**
      * @brief  文档共享成功的回调
      * @return void
      */
    virtual void onDocShared() = 0;

    /**
      * @brief  文档关闭成功的回调
      * @return void
      */
    virtual void onDocClosed() = 0;

    /**
      * @brief  文档页已准备好回调
      * @param [in] uiPageNum:当前准备好的页码
      * @param [in] uiReadyPageCount:已经准备好的页数
      * @param [in] uiPageCount:当前文档总共页数
      * @return void
      */
    virtual void onPageDataReady(const unsigned int uiPageNum,
                                 const unsigned int uiReadyPageCount,
                                 const unsigned int uiPageCount) = 0;

    /**
      * @brief  文档获得焦点
      * @return void
      */
    virtual void focusIn() = 0;


	#ifdef WIN32
	  /**
      * @brief  文档DC 大小变化时通知，主要用具通知windows 来改变滚动条长度用
      * @param [in] uWidth 文档展示dc 宽度
	  * @param [in] uHeight 文档展示dc 高度
      * @return void
      */
	virtual void OnViewSizeChanged(
		const unsigned int  uWidth,
		const unsigned int  uHeight ) = 0;
	#endif

};

class IGNetTangDocShareSession: public IGNetTangBaseSession
{

public:
	/**
      * @brief  获取文档服务状态
      * @return int: 0 未打开 3 准备好
      */
	virtual  int getServiceState()=0;
	
	/**
      * @brief  获取录制状态
      * @return int: 0 未打开 1 打开
      */
	virtual  int getRecordState()=0;
	/**
      * @brief  获取文档名称
      * @return char*:文档名称
      */
	virtual  char* getDocName() = 0;
	/**
      * @brief  获取文档页数
      * @return int:文档页数
      */
	virtual  int getPageCount() = 0;
	/**
      * @brief  获取当前收到文档页数
      * @return int:收到文档页数
      */
	virtual  int getRecivedCount() = 0;
	 /**
      * @brief  获取文档页数
      * @return int:文档页数
      */
	virtual  int getCurentPage() = 0;

#ifdef WIN32
	 /**
      * @brief  获取当前会话是否是主动创建的
      * @return bool:true 表示是用户主动创建的,false 表示是别人共享的
      */
	virtual  bool IsInitiativeCreation()=0;
#endif
	//激活当前文档
	/*virtual  void Active()=0;
	virtual  bool IsActive()=0;*/
    /**
      * @brief  共享文档
      * @param [in] pDocPath:文档路径
      * @return int:0表成功,其它则失败
      */
    virtual int shareDoc(const char* pDocPath) = 0;

    /**
      * @brief  停止共享文档
      * @return int:0表成功,其它则失败
      */
    virtual int closeDoc() = 0;

    /**
      * @brief  显示文档
      * @param [in] pViewObj:显示文档的View对象
      * @return int:0表成功,其它则失败
      */
    virtual int showView(TangImageViewHandle pViewObj) = 0;

    /**
      * @brief  停止显示文档
      * @return int:0表成功,其它则失败
      */
    virtual int hideView() = 0;

    /**
      * @brief  设置协同浏览
      * @param [in] bIsCoBrowsing:当为主持人且为true时，主持人的所有滚动文档、切换文档等动作都会影响所有参会人
      * @return int:0表成功,其它则失败
      */
    virtual int setCoBrowsing(const bool bIsCoBrowsing) = 0;

    /**
      * @brief  设置同步浏览
      * @param [in] bIsSynchronization:当为参会人且为false时，主持人的文档的浏览动作操作不会影响本地浏览
      * @return int:0表成功,其它则失败
      */
    virtual int setSynchronization(const bool bIsSynchronization) = 0;

    /**
      * @brief  设置同步参数
      * @param [in] bSyncScale:同步时是否同步缩放比例
      * @param [in] bSyncFocus:同步时是否同步焦点
      * @param [in] bSyncChangePage:同步时是否同步页码
      * @param [in] bSyncScroll:同步时是否同步滚动条位置
      * @param [in] bSyncAngle:同步时是否同步文档旋转角度
      * @return int:0表成功,其它则失败
      */
    virtual int setSyncConfig(const bool bSyncScale,
                              const bool bSyncFocus,
                              const bool bSyncChangePage,
                              const bool bSyncScroll,
                              const bool bSyncAngle) = 0;

    /**
      * @brief  缩放文档
      * @param [in] uScaling:缩放百分比
      * @return int:0表成功,其它则失败
      */
    virtual int zoomDoc(const unsigned short uScaling) = 0;

    /**
      * @brief  切换文档页
      * @param [in] uPageID:页码
      * @return int:0表成功,其它则失败
      */
    virtual int turnToPage(const unsigned short uPageID) = 0;

    /**
      * @brief  滚动文档
      * @param [in] uiCurrentX:X坐标
      * @param [in] uiCurrentY:Y坐标
      * @return int:0表成功,其它则失败
      */
    virtual int scrollDoc(const unsigned int uiCurrentX,
                          const unsigned int uiCurrentY) = 0;

    /**
      * @brief  判断文档页数据是否准备好
      * @param [in] uiPageID:页码
      * @return bool:true表已准备好，false则还没准备好
      */
    virtual bool pageIsReady(const unsigned int &uiPageID) = 0;

    /**
      * @brief  旋转文档页接口
      * @param [in] dAngle:旋转角度
      * @return int:0表成功,其它则失败
      */
    virtual int rotatePage(double dAngle) = 0;

    /**
      * @brief  判断当前文档是否有焦点
      * @return bool:true表有焦点，false则无焦点
      */
    virtual bool isFocusIn() = 0;

    /**
      * @brief  设置文档为当前文档
      * @return int:0表成功,其它则失败
      */
    virtual int switchFocusIn() = 0;
};

#endif // IGNETTANGDOCSHARINGSESSION_H

