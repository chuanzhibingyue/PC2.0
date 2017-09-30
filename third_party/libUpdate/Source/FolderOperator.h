#pragma once
#include <atlstr.h>

class CFolderOperator
{
public:
	CFolderOperator(void);
	~CFolderOperator(void);
	BOOL IsFileExists(CString strFileName);
	BOOL DeleteDirectoryEx(CString strDir);
	BOOL FolderExist(CString strPath);
	HRESULT CopyDirectory(CString strSrcPath, CString strDesPath, BOOL bFailIfExists = FALSE );
	HRESULT CycleCreateDirectory(CString strDesPath);
	CString GetFilePath(const CString &strFilePath);
	BOOL CopyFile( CString strSrcFileName, CString strDesFileName, BOOL bFailIfExists );
};
