#pragma once

class ComputerExamineUI : public DuiLib::CContainerUI
{
public:
	ComputerExamineUI()
	{
		DuiLib::CDialogBuilder builder;
		DuiLib::CContainerUI* pComputerExamine = static_cast<DuiLib::CContainerUI*>(builder.Create(_T("ComputerExamine.xml"), (UINT)0));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};

class CDialogBuilderCallbackEx : public DuiLib::IDialogBuilderCallback
{
public:
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, _T("ComputerExamine")) == 0 ) return new ComputerExamineUI;
		return NULL;
	}
};