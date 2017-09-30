/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file UpdateInfoDef.h 
* @brief Update结构定义
* 
* @author Wei Chen (wei.chen@gnetis.com)
*
* @date Jun 17 2010
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
#include <atlstr.h>
#include <vector>
struct UpdateFile 
{
	CString	m_strFileFullName;
	CString	m_strVersion;
	UINT	m_uSize;
	CString	m_strChecksum;
	BOOL	m_bShouldDelete;//added by jianhua.chen

	UpdateFile()
	{
		m_strFileFullName = _T("");
		m_strVersion = _T("");
		m_strChecksum = _T("");
		m_uSize = 0;
		m_bShouldDelete = FALSE;//set should delete by default. 
	}
};

typedef std::vector<UpdateFile>	UPDATEFILEVECTOR;

class UpdateConfig 
{
public:
	CString	m_strSiteId;
	CString	m_strVersion;
	CString	m_strServer;
	CString m_releaseId;
	UPDATEFILEVECTOR	m_components;

	UpdateConfig()
	{
		m_strVersion = _T("");
		m_strServer = _T("");
		m_releaseId = _T("");
	}
};

class NotesCfg
{
public:
	std::wstring m_strVersion;
	std::wstring m_notes;

	NotesCfg()
	{
		m_strVersion = _T("");
		m_notes = _T("");
	}
};