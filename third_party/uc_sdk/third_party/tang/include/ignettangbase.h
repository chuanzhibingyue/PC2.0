#ifndef IGNETTANGBASE_H
#define IGNETTANGBASE_H

#include <map>
#include "GNetTangCrossPlatform.h"

#if defined(WIN32) || defined(WIN64)
#include <comutil.h>
#include "CGNetTangCommonDef.h"
#include "GNetTangPropertiesDefine.h"
#define CGNetTangVariant VARIANT
#else
#include "GNetTangPropertiesDefine.h"
#include "CGNetTangCommonDef.h"

/**
 * @brief 变量类
 *
 */
class CGNetTangVariant
{
public:
    CGNetTangVariant(GNETTANG_VARTYPE type = TANG_VT_EMPTY);
    CGNetTangVariant(char* szValue);
    CGNetTangVariant(const char* szValue);
    CGNetTangVariant(void* pVoidValue);
    CGNetTangVariant(long lVal);
    CGNetTangVariant(int intVal);
    CGNetTangVariant(unsigned int uintVal);
    CGNetTangVariant(bool boolVal);

    CGNetTangVariant(const CGNetTangVariant& tangVar);

    void operator=(const CGNetTangVariant& tangVar);

    ~CGNetTangVariant();

public:
    GNETTANG_VARTYPE    vt;
    char*               pUtf8Val;
    void*               pdispVal;
    long                lVal;
    int                 intVal;
    unsigned int 		uintVal;
    bool                boolVal;
};

#endif



/**
 * @brief 属性管理基类
 *
 */
class IGNetTangProperty
{
public:
    virtual int  getPropertyValue(const char* propName, CGNetTangVariant& value)     =0;
};

class CGNetTangSessionErrorInfo;
class IGNetTangBaseSessionSink;

/**
 * @brief 服务管理基类
 *
 */
class IGNetTangBaseSession: public IGNetTangProperty
{
public:

    /**
        *
        * @brief 设置服务类的回调
        *
        * @param pSink IGNetTangBaseSessionSink类的指针，在pSink类的成员函数中会收到响应服务的通知
        *
      * @return 返回HRESULT; S_OK  成功; 其他值 失败
      */
    virtual void setSessionCallback(IGNetTangBaseSessionSink *psink)                    =0;
    virtual GNetTangSessionType getSessionType()                                        =0;
    virtual int                 getSessionID()                                          =0;
};

/**
 * @brief 服务管理基类回调
 *
 */
class IGNetTangBaseSessionSink
{
public:
    virtual void onSessionErrorHandle(CGNetTangSessionErrorInfo *pErrorInfo)        = 0;
};

/**
 * @brief 服务错误信息类
 *
 *
 */
class CGNetTangSessionErrorInfo
{
public:
    CGNetTangSessionErrorInfo();
    virtual ~CGNetTangSessionErrorInfo();

    bool setErrorInfo(const char* pErrorInfo, const unsigned int uiLen);
    bool setDescriptionInfo(const char* pDescInfo, const unsigned int uiLen);
    bool setMessageInfo(const GNetTangSessionType curSessionType, const unsigned int curMessageID);

    GNetTangSessionType sessionType;
    unsigned  int       messageID;
    char*               statusCode;
    char*               description;
    char*               reserved;
};



#endif // IGNETTANGBASE_H

