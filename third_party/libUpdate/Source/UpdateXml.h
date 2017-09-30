/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file UpdateXml.h
* @brief This class supply some methods for operating xml data.
* 
* @author Wei Chen (wei.chen@gnetis.com)
*
* @date Jun 18 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/
#pragma once
#include <windows.h>
#include "UpdateInfoDef.h"
#include <vector>
#include <comutil.h>
#include <XmlWrapper.h>


#define UPDATE_SERVICE_XML_STRING L"<?xml version='1.0' encoding='utf-8'?> \
<tangupdate>\
	<Operation></Operation> \
	<ErrorInfo></ErrorInfo> \
	<version></version>\
	<needtoupdate></needtoupdate>\
	<needtorestart></needtorestart>\
	<queryprogress></queryprogress>\
	<downloadprogress></downloadprogress>\
	<installprogress></installprogress>\
</tangupdate>"

#define NODE_NAME_UPDATE_TANG_UPDATE		L"tangupdate"
#define NODE_NAME_UPDATE_VERSION			L"version"
#define NODE_NAME_PHONE_ERROR_OPERATION		L"Operation"
#define NODE_NAME_PHONE_ERROR_INFO			L"ErrorInfo"
#define NODE_NAME_UPDATE_NEED_TO_UPDATE     L"needtoupdate"
#define NODE_NAME_UPDATE_NEED_TO_RESTART    L"needtorestart"
#define NODE_NAME_UPDATE_QUERY_PROGRESS     L"queryprogress"
#define NODE_NAME_UPDATE_DOWNLOAD_PROGRESS  L"downloadprogress"
#define NODE_NAME_UPDATE_INSTALL_PROGRESS   L"installprogress"

// definition for error id in Update module
#define UPDATE_ERROR_ID_TASK_WORKING			0x10		// 任务正在执行
#define UPDATE_ERROR_ID_NEED_CHECK_UPDATE		0x11
#define UPDATE_ERROR_ID_NEED_INSTALL			0x12
#define UPDATE_DOWNLOAD_ERROR					0x20

// definition for error detail in Update module.
#define UPDATE_ERROR_DETAIL_NEED_CHECK_UPDATE  L"请先调用CheckUpdate操作！"

class XmlWrapper;

/*
* @class UpdateXml
* @brief This class supply some methods for operating xml data.
* 
* @author wei.chen
* @version 1.0.0
*/
class UpdateXml
{
public:
	UpdateXml();
	virtual ~UpdateXml();
    HRESULT AddXmlDocument( IUnknown * pXmlDoc );
    HRESULT RemoveXmlDocument( IUnknown * pXmlDoc );
	HRESULT SetErrorStatus(	UINT32 uErrorId, BSTR szErrorDetail);
	HRESULT SetVersion ( BSTR szVersion);
	HRESULT SetNeedUpdate( BOOL bNeedUpdate);
	HRESULT SetNeedRestart( BOOL bNeedRestart);
	HRESULT SetQueryStatus( UINT uProgress);
	HRESULT SetUpdateFiles ( UpdateConfig* pUpdateFiles);
	HRESULT SetDownloadStatus( UINT uProgress);
	HRESULT SetInstallStatus( UINT uProgress);

	HRESULT InitXmlData(UINT uError, BSTR detail, BOOL bNeedToUpdate, BOOL bNeedToRestart,
		UINT uQueryProgress, UINT uDownloadProgress, UINT uInstallProgress);
private:
	BSTR ConvertIntToBSTR(UINT32 value)
	{
		VARIANT var;
		var.vt = VT_I4;
		var.lVal = value;
		_bstr_t bstr_xh(var);
		return bstr_xh.copy();
	}
private:
	XmlWrapper*     m_pXmlWrapper;  /**< the pointer of xml wrapper. */

};
