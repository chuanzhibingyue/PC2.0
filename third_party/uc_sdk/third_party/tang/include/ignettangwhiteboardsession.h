#ifndef IGNETTANGWHITEBOARDSERVICE_H
#define IGNETTANGWHITEBOARDSERVICE_H

#include "ignettangbase.h"


/**
 *
 * @brief 白板service回调
 *
 */
class IGNetTangWhiteboardSessionSink : public IGNetTangBaseSessionSink
{
public:
    /**
      * @brief  白板属性变化回调
      * @param [out] propName:变化的属性名
      * @param [out] oldPropValue:属性旧值
      * @param [out] newPropValue:属性新值
      * @param [out] pItemInstance:属性对应的具体对象
      * @return void
      */
    virtual void onWhiteboardPropertyChanged(const char*propName,
                                          const CGNetTangVariant &oldPropValue,
                                          const CGNetTangVariant &newPropValue,
                                          IGNetTangProperty* pItemInstance) = 0;

    /**
      * @brief  白板服务属性状态变化
      * @param [out] propName:变化的属性名
      * @param [out] oldPropValue:属性旧值
      * @param [out] newPropValue:属性新值
      * @return void
      */
    virtual void onWhiteboardPropertyChanged(const char*propName,
                                            const CGNetTangVariant &oldPropValue,
                                            const CGNetTangVariant &newPropValue) = 0;

    /**
      * @brief  白板开启事件回调
      * @return void
      */
    virtual void onWhiteboardStarted() = 0;
    /**
      * @brief  白板停止事件回调
      * @return void
      */
    virtual void onWhiteboardStoped() = 0;

};

class IGNetTangWhiteboardSession: public IGNetTangBaseSession
{
public:
    /**
      * @brief  白板是否已经创建
      * @return bool 返回true白板已经创建
      */
    virtual bool isCreated()=0;
    /**
      * @brief  创建白板
      * @param [in] uPageCount:页面数
      * @param [in] uPageWidth:宽度
      * @param [in] uPageHeight:高度
      * @return int 返回0成功否则失败
      */
    virtual int create(unsigned short uPageCount, unsigned short uPageWidth, unsigned short uPageHeight)=0;
    /**
      * @brief  设置协同浏览
      * @param [in] bIsCoBrowsing:协同浏览
      * @return int 返回0成功否则失败
      */
    virtual int setCoBrowsing(bool bIsCoBrowsing)=0;
    /**
      * @brief  设置同步浏览
      * @param [in] bIsSynchronization:同步浏览
      * @return int 返回0成功否则失败
      */
    virtual int setSynchronization(bool bIsSynchronization)=0;
    /**
      * @brief  设置工具类型
      * @param [in] uType: 5-徒手画 6－激光笔 9-橡皮擦
      * @return int 返回0成功否则失败
      */
    virtual int setToolType(unsigned int uType)=0;
    /**
      * @brief  设置颜色
      * @param [in] uType: 1-线
      * @param [in] uColor: 颜色
      * @return int 返回0成功否则失败
      */
    virtual int setColor(unsigned int uType, unsigned int uColor)=0;
    /**
      * @brief  设置线的宽度
      * @param [in] uType: 1－实线 2-虚线
      * @param [in] uWidth: 宽度
      * @return int 返回0成功否则失败
      */
    virtual int setLineStyle(unsigned int uType, unsigned int uWidth)=0;
    /**
      * @brief  设置LineCap
      * @param [in] uStartCapType:
      * @param [in] uEndCapType:
      * @return int 返回0成功否则失败
      */
    virtual int setLineCap(unsigned int uStartCapType, unsigned int uEndCapType)=0;
    /**
      * @brief  设置图片资源路径，目前用的主要是激光笔资源
      * @param [in] szPath: 资源目录的绝对路径，
      * @return int 返回0成功否则失败
      */
    virtual int setImageResFolder(const char* szPath)=0;
    /**
      * @brief  显示视图
      * @param [in] pViewObj: 显示控件句柄
      * @return int 返回0成功否则失败
      */
    virtual int showView(TangImageViewHandle pViewObj)=0;
    /**
      * @brief  停止观看
      * @return int 返回0成功否则失败
      */
    virtual int stopView()=0;
    /**
      * @brief  获取页面数
      * @return int 页面数
      */
    virtual unsigned int getPageCount()=0;
    /**
      * @brief  根据索引获取PageId
      * @param [in] nIndex: 页面索引
      * @return int 页面PageId
      */
    virtual unsigned short getPageIdByIndex(unsigned int nIndex)=0;
    /**
      * @brief  获取当前页
      * @param [out] uPageId: 返回当前页的PageId
      * @return int 返回当前页的索引
      */
    virtual unsigned int getCurrentPage(CGNetTangVariant& uPageId)=0;
    /**
      * @brief  判断页是否有图形
      * @param [in] uPageId: PageId
      * @return bool true为有
      */
    virtual bool pageHaveFigure(unsigned short uPageId)=0;
    /**
      * @brief  设置焦点
      * @param [in] bFocus: 焦点
      * @return int 返回0成功否则失败
      */
    virtual int setFocus(bool bFocus)=0;
    /**
      * @brief  增加页
      * @param [in] uPageWidth:宽度
      * @param [in] uPageHeight:高度
      * @return int 返回0成功否则失败
      */
    virtual int addPage(unsigned short uPageWidth = 0, unsigned short uPageHeight = 0)=0;
    /**
      * @brief  删除
      * @param [in] uPageId:PageId
      * @return int 返回0成功否则失败
      */
    virtual int deletePage(unsigned short uPageId)=0;
    /**
      * @brief  切换页
      * @param [in] uPageId:PageId
      * @return int 返回0成功否则失败
      */
    virtual int switchPage(unsigned short uPageId)=0;
    /**
      * @brief  缩放
      * @param [in] uZoom: 缩放比例＊100
      * @return int 返回0成功否则失败
      */
    virtual int zoom(unsigned short uZoom)=0;
    /**
      * @brief  滚动视图
      * @param [in] uScrollX: x轴偏移
      * @param [in] uScrollY: y轴偏移
      * @return int 返回0成功否则失败
      */
    virtual void scroll(unsigned int uScrollX, unsigned int uScrollY)=0;
    /**
      * @brief  清除当前页所有图形
      * @param [in] uPageId:  PageId
      * @param [in] uType:1-清除自己画的 2－清除别人画的 3-清除所有
      * @return int 返回0成功否则失败
      */
    virtual int clearScreen(unsigned short uPageId, unsigned int uType)=0;
    /**
      * @brief  Undo
      * @return int 返回0成功否则失败
      */
    virtual int undo()=0;
    /**
      * @brief  Redo
      * @return int 返回0成功否则失败
      */
    virtual int redo()=0;
    /**
      * @brief  刷新
      * @return int 返回0成功否则失败
      */
    virtual int refresh()=0;
    /**
      * @brief  保存
      * @param [in] uPageId: PageId
      * @param [in] szPath: 保存文件路径
      * @return int 返回0成功否则失败
      */
    virtual int save(unsigned short uPageId, const char*  szPath)=0;
    /**
      * @brief  装载
      * @param [in] szPath: 装载文件路径
      * @return int 返回0成功否则失败
      */
    virtual int load(const char*  szPath)=0;
    virtual unsigned int getType() = 0;
};

#endif // IGNETTANGWHITEBOARDSERVICE_H

