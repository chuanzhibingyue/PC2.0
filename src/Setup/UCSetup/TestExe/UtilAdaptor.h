#pragma once
#include "TestExe.h"
#include "NativeActionDispImpl.h"
#include <comdef.h>
#include <comdefsp.h>

class UtilAdaptor: public NativeActionDispImpl<ITangUtil>
{
public:
	UtilAdaptor(void);
	virtual ~UtilAdaptor(void);

	STDMETHOD(GetModulePath)(BSTR *strModulepath);
	STDMETHOD(GetSpecialFolderPath)(UINT32 iRequest,BSTR *strPath);
    STDMETHOD(GetProcessCount)(
        /*[in]*/ BSTR bstrProcessName,
        /*[out,retval]*/ int* nProcessCount);
};
