/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file NativeActionDispatch.h
* @brief defined NativeActionDispatch template class to support automation attribute.
* Each module adapter will be inherited from it to provide API to UI JS.
*
* @author Hongwei Hu (hongwei.hu@gnetis.com)
*
* @date Janu. 26 2010
*
* @version 1.0.0
* 
* @warning 
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug
******************************************************************************/

#pragma once

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <map>
#include <atlbase.h>


typedef std::map< DISPID, void* > ACTION_ADDRESS_MAP;
typedef std::map< CComBSTR, DISPID > ACTION_DISPID_MAP;

#define BEGIN_ACTION_MAP() \
public: \
	void __GetActionMap() { \
		void* fn_add=NULL; 

/** add method, is stand windAPI, DWORD WINAPI method(...) */
#define ADD_ACTION_METHOD(method) \
		_asm { lea eax, method };  \
		_asm { mov fn_add,eax }; \
		m_dispMap[CComBSTR(L#method)] = (DISPID)fn_add; \
		m_addressMap[(DISPID)fn_add] = fn_add; 

#define ADD_ACTION_METHOD_DISPID(id,method) \
		_asm { lea eax, method };  \
		_asm { mov fn_add,eax }; \
		m_dispMap[CComBSTR(L#method)] = id; \
		m_addressMap[id] = fn_add; 

#define END_ACTION_MAP() \
	} /**End of the function __GetActionMap */ \
	DISPID GetActionDISPID(WCHAR* mname) { \
		return m_dispMap[CComBSTR(mname)]; \
	} \
	void *GetActionAddress(DISPID id) { \
		return m_addressMap[id]; \
	}\

/**
* @class NativeActionDispatch
* @brief NativeActionDispatch is a automation template class.the derived class
* provides API which can be invoked by UI JS code.
* 
* @author Hongwei Hu
* @version 1.0.0
*/
template <class T>
class NativeActionDispatch: public IDispatch
{

public:
	NativeActionDispatch() 
	{ 
		((T*)this)->__GetActionMap(); 
	}

    long  WINAPI QueryInterface( REFIID riid,void ** object) 
	{ 
		*object=IsEqualIID(riid, IID_IDispatch)?this:0;
		return *object?S_OK:E_NOINTERFACE;
	}         
    DWORD WINAPI AddRef () { return 0; }
    DWORD WINAPI Release() { return 0; }
    long  WINAPI GetTypeInfoCount( UINT *) { return 0; }
    long  WINAPI GetTypeInfo( UINT, LCID, ITypeInfo **)	{ return 0; }
    
    long  WINAPI GetIDsOfNames( REFIID riid, WCHAR** name, UINT cnt ,LCID lcid, DISPID *id) 
	{ 
		DWORD j;
		bool bIsFound = false;
		T* pT = static_cast<T*>(this);
        for(j=0; j<cnt;j++) 
		{
			id[j]=pT->GetActionDISPID(name[j]);
			if (!id[j])
				id[j]=pT->GetActionDISPID(name[j]+1);
			else
			{
				bIsFound = true;
				goto Back;
			}
            if(!id[j])
				bIsFound = false;
			else
			{
				bIsFound = true;
				goto Back;
			}
        }

Back:	if (!bIsFound)
		{
			return E_FAIL;
		}
        return 0; 
    }
    
    long  WINAPI Invoke( DISPID id, REFIID riid, LCID lcid, WORD flags, DISPPARAMS *arg, VARIANT *ret, EXCEPINFO *excp, UINT *err) { 
        if(id) {
			T* pT = static_cast<T*>(this);
            // we support stdcall conventions for now
			void *fn = pT->GetActionAddress(id);
			if (NULL == fn)
			{
				return 0;
			}

            DWORD i=-1, result=0;
			DWORD param = 0;
            for(i=0; i<arg->cArgs; i++) {
				if ( (arg->rgvarg[i].vt & VT_BYREF ) || (arg->rgvarg[i].vt & VT_ARRAY) ) {
					param = (DWORD)arg->rgvarg[i].byref;
					_asm push param;
					continue;
				} 
				if ( arg->rgvarg[i].vt & VT_VECTOR ) {
					param = (DWORD)arg->rgvarg[i].cyVal.Lo;
					_asm push param;
					param = (DWORD)arg->rgvarg[i].cyVal.Hi;
					_asm push param;
					continue;
				}

				switch (arg->rgvarg[i].vt) {
				case VT_EMPTY:
				case VT_VOID:  /* not support */
				case VT_ILLEGAL: /* not support */
				case VT_USERDEFINED:
				case VT_RECORD:
				case VT_RESERVED:
				case VT_BSTR_BLOB:
				case VT_BLOB:
				case VT_BLOB_OBJECT:
				case VT_NULL:  continue; /* No parameter */
				case VT_I1:
				case VT_UI1:
					param = (DWORD)arg->rgvarg[i].bVal;
					break;
				case VT_I2:
				case VT_UI2:
				case VT_DECIMAL:
				case VT_BOOL:
					param = (DWORD)arg->rgvarg[i].uiVal;
					break;
				case VT_I4:
				case VT_UI4:
				case VT_HRESULT:
				case VT_INT:
				case VT_UINT:
				case VT_R4:
				case VT_ERROR: /* SCODE, long type */
					param = (DWORD)arg->rgvarg[i].lVal;
					break;
				case VT_I8:
				case VT_R8:
				case VT_UI8:
				case VT_CY:
				case VT_DATE:
				case VT_FILETIME:
#ifndef IS_64
					param = (DWORD)arg->rgvarg[i].cyVal.Hi;
					_asm push param;
#endif
					param = (DWORD)arg->rgvarg[i].cyVal.Lo;
					break;
				case VT_UNKNOWN: /* IUnknow* */
				case VT_DISPATCH:/* IDispatch* */
				case VT_VARIANT: /* VARIANT* */
				case VT_PTR: /* void* */
				case VT_BSTR: /* OLE char* */
				case VT_LPSTR:
				case VT_LPWSTR:
				case VT_CARRAY:
				case VT_SAFEARRAY:
				case VT_INT_PTR:
				case VT_UINT_PTR:
				case VT_CF:
				case VT_CLSID:
				case VT_STREAM:
				case VT_STORAGE:
				case VT_STREAMED_OBJECT:
				case VT_STORED_OBJECT:
				case VT_VERSIONED_STREAM:
					param = (DWORD)arg->rgvarg[i].byref;
					break;
				}
                _asm push param; 
            }
			_asm push this;
            _asm call fn;
            _asm mov result, eax;
            //return value to script DWORD
			if(ret) 
			{ 
				ret->vt = VT_INT;
				ret->ulVal = result;
			} 
            return 0; 
        }
        return E_FAIL;
	}

protected:
		ACTION_ADDRESS_MAP  m_addressMap;
		ACTION_DISPID_MAP  m_dispMap;
};