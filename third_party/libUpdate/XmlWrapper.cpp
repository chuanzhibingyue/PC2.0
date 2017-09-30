/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file XmlWrapper.h
* @brief Header files for XmlWrapper class.Definition operations for xml,including add node,edit node,query node,delete node
* 
* @author Jianhua Chen (jianhua.chen@gnetis.com)
*
* @date March. 8 2010
*
* @version 1.0.0
* 
* @warning 
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug
******************************************************************************/

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <string>
#include "XmlWrapper.h"
#include "Lock.h"

/**
* @brief constructor of XmlWrapper class.
* @param [in] void.
* @return 
*/
XmlWrapper::XmlWrapper( BSTR  bstrDom)
{
    IXMLDOMDocument * pRecorderXML = NULL;
    HRESULT hr =::CoCreateInstance(
        CLSID_DOMDocument, 
        NULL, 
        CLSCTX_INPROC_SERVER,
        IID_IXMLDOMDocument,
        (LPVOID*)(&pRecorderXML) );
    if(SUCCEEDED(hr) && NULL != pRecorderXML)
    {
        //pRecorderXML->AddRef();
		pRecorderXML->put_async(VARIANT_FALSE);
        VARIANT_BOOL bSuccess = VARIANT_TRUE;
        hr = pRecorderXML->loadXML(bstrDom,&bSuccess);
        if (FAILED(hr) || bSuccess == VARIANT_FALSE)
        {
            hr = HRESULT_FROM_WIN32(::GetLastError());
            //LOG_ERR(L"XmlWrapper::constructor error with error 0x%08X", hr);
        }
        m_xmlDocLst.clear();
        IUnknown * pIUN = NULL;
        hr = pRecorderXML->QueryInterface(IID_IUnknown,(LPVOID*)&pIUN);
        pRecorderXML->Release();
        if (FAILED(hr) || NULL == pIUN)
        {
            //LOG_ERR(L"XmlWrapper::constructor error with error 0x%08X", hr);
        }
        else
        {
            m_xmlDocLst.push_front(pIUN);
        }
    }
    ::InitializeCriticalSection(&m_CriticalSection);
    
}

/**
* @brief destructor of XmlWrapper class.
* @param [in] void.
* @return 
*/
XmlWrapper::~XmlWrapper(void)
{
	std::map<IUnknown*,BSTR>::iterator iter;
	std::map<IUnknown*,IHTMLElement*>::iterator iter1;

	//clean event handler bstr memory.
	if (m_eventHandlerMap.size() != 0)
	{
		iter = m_eventHandlerMap.begin();
		while (iter != m_eventHandlerMap.end())
		{
			if (iter->second != NULL)
			{
				::SysFreeString(iter->second);
			}

			iter = m_eventHandlerMap.erase(iter);
		}
	}

	//release html element instance.
	if (m_elementMap.size() != 0)
	{
		iter1 = m_elementMap.begin();
		while (iter1 != m_elementMap.end())
		{
			if (iter1->second != NULL)
			{
				iter1->second->Release();
			}

			iter1 = m_elementMap.erase(iter1);
		}
	}

	ReleaseAllXmlDocument();
	::DeleteCriticalSection(&m_CriticalSection);
	//UninitLog();
}

/**
* @brief Add type IXMLDOMDocument * to the date member. 
* @param [in] pXmlDomDocument specify IXMLDOMDocument * which to be added.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT XmlWrapper::AddXmlDocument(IUnknown *pElement)
{
    HRESULT hr = S_OK;
	IHTMLElement* pElement1 = NULL;
	IXMLDOMNode* pNode = NULL;
	IUnknown* pIUnknownXml = NULL;
	BSTR strNodeName = NULL;
	_bstr_t xpath;
	VARIANT var;

	ARG_CHECK(pElement);
	{
		//get the IUnknown instance of xml document.
		hr = GetXmlDocument(pElement,&pIUnknownXml);
		HR_CHECK(hr);
		CHECK_IF_NULL(pIUnknownXml);

		CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(pIUnknownXml);
		CHECK_IF_NULL(pXML);
		pXML->put_async(VARIANT_FALSE);
		AutoLock lock(&m_CriticalSection);
		CHECK_IF(m_xmlDocLst.empty(),E_FAIL);
		std::list<IUnknown *>::iterator it = m_xmlDocLst.begin();
		CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pRecorder(*it);
		CHECK_IF_NULL(pRecorder);
		// (*it).get_childNodes(IXMLDDOMNode))
		BSTR bstrXMLStr = NULL;
		hr = pRecorder->get_xml(&bstrXMLStr);
		HR_CHECK(hr);
		VARIANT_BOOL bSuccess = VARIANT_TRUE;
		hr = pXML->loadXML(bstrXMLStr,&bSuccess);
		::SysFreeString(bstrXMLStr);
		HR_CHECK(hr);
		BOOL_CHECK(bSuccess);
		m_xmlDocLst.push_back(pIUnknownXml);

		//get the event handler for xml document.
		pElement->QueryInterface(IID_IHTMLElement,(void **)&pElement1);
		CHECK_IF_NULL(pElement1);
		::VariantInit(&var);
        BSTR strAttributeName = ::SysAllocString(L"OnTangXmlEvent");
        CHECK_IF_NULL(strAttributeName);
		hr = pElement1->getAttribute(strAttributeName,0,&var);
        ::SysFreeString(strAttributeName);
        strAttributeName = NULL;
		HR_CHECK(hr);
		if (var.vt == VT_BSTR && var.bstrVal != NULL)
		{
			m_eventHandlerMap.insert(std::make_pair(pIUnknownXml,var.bstrVal));

			//insert element into map.
			m_elementMap.insert(std::make_pair(pIUnknownXml,pElement1));

			//fire init event.
			hr = pXML->get_lastChild(&pNode);
			HR_CHECK(hr);
			CHECK_IF_NULL(pNode);
			hr = pNode->get_nodeName(&strNodeName);
            HR_CHECK(hr);
			CHECK_IF_NULL(strNodeName);
			xpath = strNodeName;
			xpath = L"/" + xpath;
			::SysFreeString(strNodeName);
			XmlEvent insertEvent(pElement1,(IXMLDOMNode*)pIUnknownXml,var.bstrVal,xpath.GetBSTR(),XML_ET_INITED);
			insertEvent.Fire();
		}
	}
    
exit:
    //::LeaveCriticalSection(&m_CriticalSection);
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::AddXmlDocument exit with error 0x%08X", hr);
    }
    return hr;
}

//HRESULT XmlWrapper::InitXmlDom(__in BSTR bstrDom)
//{
//    HRESULT hr = S_OK;
//    std::list<IXMLDOMDocument *>::iterator it;
//
//    ::EnterCriticalSection(&m_CriticalSection);
//    ARG_CHECK(bstrDom);
//    VARIANT_BOOL bSuccess = VARIANT_TRUE;
//    it = m_xmlDocLst.begin();
//    for (;it!= m_xmlDocLst.end();it++)
//    {
//        hr = (*it)->loadXML(bstrDom,&bSuccess);
//        if (FAILED(hr) && bSuccess == VARIANT_FALSE)
//        {
//            hr = HRESULT_FROM_WIN32(::GetLastError());
//            //LOG_ERR(L"XmlWrapper::InitXmlDom error with error 0x%08X", hr);
//        }
//    }
//
//exit:
//    ::LeaveCriticalSection(&m_CriticalSection);
//    if(FAILED(hr) )
//    {
//        //LOG_ERR(L"XmlWrapper::InitXmlDom exit with error 0x%08X", hr);
//    }
//    return hr;
//}
/**
* @brief Query xml node text.
* @param [in] pXmlDomDocument specify IXMLDOMDocument * which to be deleted.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT XmlWrapper::DeleteXmlDocument(IUnknown *pElement)
{
    HRESULT hr = S_OK;
	IUnknown* pIUnknownXml = NULL;
	std::map<IUnknown*,BSTR>::iterator iter;
	std::map<IUnknown*,IHTMLElement*>::iterator iter1;
    //::EnterCriticalSection(&m_CriticalSection);
    ARG_CHECK(pElement);
    {
		hr = GetXmlDocument(pElement,&pIUnknownXml);
		HR_CHECK(hr);
		CHECK_IF_NULL(pIUnknownXml);
		{
			AutoLock lock(&m_CriticalSection);
			//remove the event handler for the xml document.
			iter = m_eventHandlerMap.find(pIUnknownXml);
			if (iter != m_eventHandlerMap.end())
			{
				// ToDo need to free bstr
				if (iter->second != NULL)
				{
					::SysFreeString(iter->second);
				}
				m_eventHandlerMap.erase(iter);
			}

			//remove the element for the xml document.
			iter1 = m_elementMap.find(pIUnknownXml);
			if (iter1 != m_elementMap.end())
			{
				m_elementMap.erase(iter1);
			}

			m_xmlDocLst.remove(pIUnknownXml);
		}
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(pIUnknownXml);
        if (pXML)
        {
            long state = 0;
            if (SUCCEEDED(pXML->get_readyState(&state)))
            {
                if( 1 == state )
                {
                    pXML->abort();
                }
            }
        }
        pIUnknownXml->Release();
    }
exit:
    //::LeaveCriticalSection(&m_CriticalSection);
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::DeleteXmlDocument exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief Add xml node.call like this:AddRootNode(L"TangClient");
* @param [in] bstrRootNodeName the root node name.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT XmlWrapper::AddRootNode(
                                __in BSTR bstrRootNodeName)
{
    HRESULT hr = S_OK;
    _bstr_t bstrXmlHeader;
    VARIANT_BOOL isSuccessful = VARIANT_FALSE;
    std::list<IUnknown *>::iterator it;
   // ::EnterCriticalSection(&m_CriticalSection);
    ARG_CHECK(bstrRootNodeName);
    RegulNodePath(bstrRootNodeName);
    bstrXmlHeader = XMLHEADER;
    bstrXmlHeader += L"<";
    bstrXmlHeader += bstrRootNodeName;
    bstrXmlHeader += L"></";
    bstrXmlHeader += bstrRootNodeName;
    bstrXmlHeader += L">";
    {
        AutoLock lock(&m_CriticalSection);
        it = m_xmlDocLst.begin();
        for (;it!= m_xmlDocLst.end();it++)
        {
            CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
            CHECK_IF_NULL(pXML);
            hr = pXML->loadXML(bstrXmlHeader ,&isSuccessful);
            if (FAILED(hr) && isSuccessful == VARIANT_FALSE)
            {
                hr = HRESULT_FROM_WIN32(::GetLastError());
                //LOG_ERR(L"XmlWrapper::AddRootNode error with error 0x%08X", hr);
            }
        }
    }
   
exit:
    //::LeaveCriticalSection(&m_CriticalSection);
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::AddRootNode exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief Add xml node.call like this: AddNode(L"TangClient/Service[@ID="1"]",pNode *);
* @param [in] bstrParentNodePath the parent node path.
* @param [in] pNode the node name to be added.
* @return   S_OK:successful;
*           S_FALSE: the parent node do not exit;
*           otherwise return error code.
*/
HRESULT XmlWrapper::AddNode(
                            __in BSTR bstrParentNodePath,
                            __in IXMLDOMNode * pNode,
                            __in BSTR position)
{
    HRESULT hr = S_OK;
    std::list<IUnknown *>::iterator it;
	BSTR bstrNodeName = NULL;
	std::map<IUnknown*,BSTR>::iterator iter;
	std::map<IUnknown*,IHTMLElement*>::iterator iter1;

    ARG_CHECK(bstrParentNodePath);
    ARG_CHECK(pNode);
    RegulNodePath(bstrParentNodePath);
    {
        CComPtr<IXMLDOMNode> pParentNode = NULL;

        //append the adding node to first xml document instance.
        AutoLock lock(&m_CriticalSection);
        CHECK_IF (m_xmlDocLst.empty(),E_FAIL);
        it = m_xmlDocLst.begin();
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pRecorder(*it);
        CHECK_IF_NULL(pRecorder);
        hr = pRecorder->selectSingleNode(bstrParentNodePath,&pParentNode);
        HR_CHECK(hr);
        CHECK_IF_NULL(pParentNode);
        if (NULL == position)
        {
            hr = pParentNode->appendChild(pNode,NULL);      //never use the out new node in recorder xml document.
            HR_CHECK(hr);
        }
        else
        {
            CComVariant varRef;
            CComPtr<IXMLDOMNode> pXMLNodeRef = NULL;
            hr = pRecorder->selectSingleNode(position,&pXMLNodeRef);
            HR_CHECK(hr);
            CHECK_IF_NULL(pXMLNodeRef);
            varRef = pXMLNodeRef;
            hr = pParentNode->insertBefore(pNode, varRef,NULL);     //never use the out new node in recorder xml document.
            varRef.Clear();
            HR_CHECK(hr);
        }

        //append the adding node to other xml document instance.
        it++;
        for (;it!= m_xmlDocLst.end();it++)
        {
            CComPtr<IXMLDOMNode> pParentNode = NULL;
            CComPtr<IXMLDOMNode> pOutNewNode = NULL;        //use this node to xml wrapper event.
            CComPtr<IXMLDOMNode> pItemNode = NULL;  //node used to copy.

            CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
            CHECK_IF_NULL(pXML);
            hr = pXML->selectSingleNode(bstrParentNodePath,&pParentNode);
            HR_CHECK(hr);
            CHECK_IF_NULL(pParentNode);
            
            hr = CopyNode(pNode,&pItemNode);    //copy the adding node.
            HR_CHECK(hr);
            CHECK_IF_NULL(pItemNode);
            if (NULL == position)
            {
                hr = pParentNode->appendChild(pItemNode,&pOutNewNode);
                HR_CHECK(hr);
            }
            else
            {
                CComVariant varRef;
                CComPtr<IXMLDOMNode> pXMLNodeRef = NULL;
                hr = pXML->selectSingleNode(position,&pXMLNodeRef);
                HR_CHECK(hr);
                CHECK_IF_NULL(pXMLNodeRef);
                varRef = pXMLNodeRef;
                hr = pParentNode->insertBefore(pItemNode, varRef, &pOutNewNode);
                varRef.Clear();
                HR_CHECK(hr);
            }

            //fire insert event.
            CHECK_IF_NULL(pOutNewNode);
			hr = pOutNewNode->get_nodeName(&bstrNodeName);
			HR_CHECK(hr);
			CHECK_IF_NULL(bstrNodeName);
			iter = m_eventHandlerMap.find((*it));
			if (iter != m_eventHandlerMap.end())
			{
				CHECK_IF_NULL(iter->second);
				iter1 = m_elementMap.find((*it));
				if (iter1 != m_elementMap.end())
				{
					_bstr_t xpath(bstrParentNodePath);
					xpath = L"/" + xpath + L"/" + _bstr_t(bstrNodeName);
					XmlEvent insertEvent(iter1->second,pOutNewNode,iter->second,xpath.GetBSTR(),XML_ET_INSERT);
					insertEvent.Fire();
				}
			}

            SYSFREESTRING(bstrNodeName);
        }
    }

exit:
	if ( bstrNodeName != NULL )
	{
		::SysFreeString(bstrNodeName);
		bstrNodeName = NULL;
	}

    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::AddNode exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief Add xml node.call like this:AddNode(L"TangClient/Service[@ID="2"]",L"privileges",L"10000003",NULL,NULL,FALSE) to add node and node value
* AddNode(L"TangClient",L"Service",NULL,L"ID",L"2",FALSE) to add node and set attribute value
* @param [in] bstrParentNodePath the parent node path.
* @param [in] bstrNodeName the node name.
* @param [in] bstrNodeValue the node value.Note:if the node have no value,this parameter should set to NULL
* @param [in] bstrAttributeName the node's attribute name.
* @param [in] bstrAttributeValue the node's attribute value.
* @return   S_OK:successful;
*           S_FALSE: the parent node do not exit;
*           otherwise return error code.
*/
HRESULT XmlWrapper::AddNode(
                            __in BSTR bstrParentNodePath,
                            __in BSTR bstrNodeName,
                            __in BSTR bstrNodeValue,
                            __in BSTR bstrAttributeName,
                            __in BSTR bstrAttributeValue,
                            __in BSTR position)
{
    HRESULT hr = S_OK;
    _bstr_t bstrNodePath;
    std::list<IUnknown *>::iterator it;
	std::map<IUnknown*,BSTR>::iterator iter;
	std::map<IUnknown*,IHTMLElement*>::iterator iter1;

    ARG_CHECK(bstrParentNodePath);
    ARG_CHECK(bstrNodeName);
    ARG_CHECK(bstrNodeValue);
    RegulNodePath(bstrParentNodePath);
    RegulNodePath(bstrNodeName);
    bstrNodePath = _bstr_t(bstrParentNodePath) + _bstr_t( L"/") + _bstr_t(bstrNodeName);
    {
        AutoLock lock(&m_CriticalSection);
        for (it = m_xmlDocLst.begin(); it!= m_xmlDocLst.end(); it++)
        {
            CComPtr<IXMLDOMNode> pParentNode = NULL;
            CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
            CHECK_IF_NULL(pXML);
            hr = pXML->selectSingleNode(bstrParentNodePath,&pParentNode);
            HR_CHECK(hr);
            CHECK_IF_NULL(pParentNode);
            {
                CComPtr<IXMLDOMNode> pOutAddNode = NULL;

                if(bstrAttributeName != NULL && bstrAttributeValue != NULL)
                {
                    CComPtr<IXMLDOMElement> pNewElement = NULL;
                    //the node have attribute
                    hr = pXML->createElement(bstrNodeName,&pNewElement);
                    HR_CHECK(hr);
                    CHECK_IF_NULL( pNewElement )
                    {
						CComQIPtr<IXMLDOMNode,&IID_IXMLDOMNode> pNewNode(pNewElement);
                        CComVariant varAttributeValue(bstrAttributeValue);
                        hr = pNewElement->setAttribute(bstrAttributeName,varAttributeValue);
                        HR_CHECK(hr);
                        if(bstrNodeValue != NULL)
                        {
                            hr = pNewNode->put_text(bstrNodeValue);
                            HR_CHECK(hr);
                        }
                        if (NULL == position)
                        {
                            hr = pParentNode->appendChild(pNewNode,&pOutAddNode);
                            HR_CHECK(hr);
                        }
                        else
                        {
                            CHECK_IF_NULL(pNewNode);
                            CComVariant varRef;
                            CComPtr<IXMLDOMNode> pXMLNodeRef = NULL;
                            hr = pXML->selectSingleNode(position,&pXMLNodeRef);
                            HR_CHECK(hr);
                            CHECK_IF_NULL(pXMLNodeRef);
                            varRef = pXMLNodeRef;
                            hr = pParentNode->insertBefore(pNewNode, varRef,&pOutAddNode);
                            varRef.Clear();
                            HR_CHECK(hr);
                        }

						//fire insert event.
						iter = m_eventHandlerMap.find((*it));
						if (iter != m_eventHandlerMap.end())
						{
							CHECK_IF_NULL(iter->second);
							iter1 = m_elementMap.find((*it));
							if (iter1 != m_elementMap.end())
							{
								_bstr_t xpath = bstrNodePath;
								xpath = L"/" + xpath;
								XmlEvent insertEvent(iter1->second,pOutAddNode,iter->second,xpath.GetBSTR(),XML_ET_INSERT);
								insertEvent.Fire();
							}
						}
                    }
                }
                else
                {
                    CComPtr<IXMLDOMNode> pNewNode = NULL;

                    //the node have no attribute
                    hr = pXML->createNode(_variant_t(long(1)),bstrNodeName,L"",&pNewNode);
                    HR_CHECK(hr);
                    CHECK_IF_NULL(pNewNode)
                    {
                        if(bstrNodeValue != NULL)
                        {
                            hr = pNewNode->put_text(bstrNodeValue);
                            HR_CHECK(hr);
                        }
                        if ( NULL == position )
                        {
                            hr = pParentNode->appendChild(pNewNode,&pOutAddNode);
                            HR_CHECK(hr);
                        }
                        else
                        {
                            CComVariant varRef;
                            CComPtr<IXMLDOMNode> pXMLNodeRef = NULL;
                            hr = pXML->selectSingleNode(position,&pXMLNodeRef);
                            HR_CHECK(hr);
                            CHECK_IF_NULL(pXMLNodeRef);
                            varRef = pXMLNodeRef;
                            CComPtr<IXMLDOMNode> pOutNewNode = NULL;
                            hr = pParentNode->insertBefore(pNewNode, varRef,&pOutAddNode);
                            varRef.Clear();
                            HR_CHECK(hr);
                        }

						//fire insert event.
						iter = m_eventHandlerMap.find((*it));
						if (iter != m_eventHandlerMap.end())
						{
							CHECK_IF_NULL(iter->second);
							iter1 = m_elementMap.find((*it));
							if (iter1 != m_elementMap.end())
							{
								_bstr_t xpath = bstrNodePath;
								xpath = L"/" + xpath;
								XmlEvent insertEvent(iter1->second,pOutAddNode,iter->second,xpath.GetBSTR(),XML_ET_INSERT);
								insertEvent.Fire();
							}
						}
                    }
                }
            }
        }
    }
exit:
    //::LeaveCriticalSection(&m_CriticalSection);
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::AddNode exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief Delete xml node.call like this:DeleteNode(L"TangClient/Service[@ID="2"]");
* @param [in] bstrNodePath the node path to be deleted.
* @return HRESULT  S_OK:successful;
                   S_FALSE: the node do not exit before;
                   otherwise return error code.
*/
HRESULT XmlWrapper::DeleteNode(
                               __in BSTR bstrNodePath
                               )
{
    HRESULT hr = S_OK;
    std::list<IUnknown *>::iterator it;
	std::map<IUnknown*,BSTR>::iterator iter;
	std::map<IUnknown*,IHTMLElement*>::iterator iter1;

    ARG_CHECK(bstrNodePath);
    RegulNodePath(bstrNodePath);
    {
        AutoLock lock(&m_CriticalSection); // 加上自动锁
        it = m_xmlDocLst.begin();
        for (;it!= m_xmlDocLst.end();it++)
        {
            CComPtr<IXMLDOMNode> pDeleteNode = NULL;

            CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
            CHECK_IF_NULL(pXML);
            hr = pXML->selectSingleNode(bstrNodePath,&pDeleteNode);
            HR_CHECK(hr);
			CHECK_IF_NULL(pDeleteNode)
			{
                //get the deleting parent node.
                CComPtr<IXMLDOMNode> pParentNode = NULL;
				hr = pDeleteNode->get_parentNode(&pParentNode);
				HR_CHECK(hr);
				CHECK_IF_NULL(pParentNode);

				//fire delete event.
				iter = m_eventHandlerMap.find((*it));
				if (iter != m_eventHandlerMap.end())
				{
					CHECK_IF_NULL(iter->second);
					iter1 = m_elementMap.find((*it));
					if (iter1 != m_elementMap.end())
					{
						_bstr_t xpath(bstrNodePath);
						xpath = L"/" + xpath;
						XmlEvent deleteEvent(iter1->second,pDeleteNode,iter->second,xpath.GetBSTR(),XML_ET_DELETE);
						deleteEvent.Fire();
					}
				}

				hr = pParentNode->removeChild(pDeleteNode,NULL);
				HR_CHECK(hr);
			}
        }
    }
  
exit:
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::DeleteNode exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief Edit xml node.call like this:EditNode(L"TangClient/Service[@ID='2']",L"newNodeValue",L"ID",L"new attribute value");
* @param [in] bstrNodePath the node path to be deleted.
* @param [in] bstrNodeValue specify the node value of bstrNodeName.Note:if the node have no value,this para should set NULL
* @param [in] bstrAttributeName specify the node's attribute name
* @param [in] bstrAttributeValue specify the value of node's attribute.
* @return   S_OK:successful;
*           S_FALSE: the node do not exit before;
*           otherwise return error code.
*/
HRESULT XmlWrapper::EditNode(
                             __in BSTR bstrNodePath,
                             __in BSTR bstrNodeValue,
                             __in BSTR bstrAttributeName,
                             __in BSTR bstrAttributeValue)
{
    HRESULT hr = S_OK;
    std::list<IUnknown *>::iterator it;
	std::map<IUnknown*,BSTR>::iterator iter;
	std::map<IUnknown*,IHTMLElement*>::iterator iter1;

    ARG_CHECK(bstrNodePath);
    ARG_CHECK(bstrNodeValue);
    {
        RegulNodePath(bstrNodePath);
        AutoLock lock(&m_CriticalSection); // 加上自动锁
        it = m_xmlDocLst.begin();
        for (;it!= m_xmlDocLst.end();it++)
        {
            //select the named node.
            CComPtr<IXMLDOMNode> pOutEditNode = NULL;
            CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
            CHECK_IF_NULL(pXML);
            hr = pXML->selectSingleNode(bstrNodePath,&pOutEditNode);
            HR_CHECK(hr);
            CHECK_IF_NULL(pOutEditNode);

            //edit node value.
            hr = pOutEditNode->put_text(bstrNodeValue);
            HR_CHECK(hr);

            //edit node attribute value.
            if( (bstrAttributeName != NULL) && (bstrAttributeValue != NULL) )
            {
                CComPtr<IXMLDOMNamedNodeMap> pDOMNamedNodeMap = NULL;
                //alter the attribute value
                hr = pOutEditNode->get_attributes(&pDOMNamedNodeMap);
                HR_CHECK(hr);
                if( NULL != pDOMNamedNodeMap )
                {
                    CComPtr<IXMLDOMNode> pNamedItemNode = NULL;
                    hr = pDOMNamedNodeMap->getNamedItem(bstrAttributeName,&pNamedItemNode);
                    HR_CHECK(hr);
                    if( NULL != pNamedItemNode )
                    {
                        CComVariant varAttributeValue(bstrAttributeValue);
                        hr = pNamedItemNode->put_nodeValue(varAttributeValue);
                        HR_CHECK(hr);
                    }
                }
            }

            //fire update event.
            iter = m_eventHandlerMap.find((*it));
            if (iter != m_eventHandlerMap.end())
            {
                CHECK_IF_NULL(iter->second);
                iter1 = m_elementMap.find((*it));
                if (iter1 != m_elementMap.end())
                {
                    _bstr_t xpath(bstrNodePath);
                    xpath = L"/" + xpath;
                    XmlEvent updateEvent(iter1->second,pOutEditNode,iter->second,xpath.GetBSTR(),XML_ET_UPDATE);
                    updateEvent.Fire();
                }
            }
        }
    }
exit:
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::EditNode exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief Query xml node text.call like this:QueryNode(L"TangClient/Service[@ID='2']/type",&bstr);
* @param [in] bstrNodePath specify the query node path.
* @param [out] bstrValue save the node text
* @return   S_OK:successful;
*           S_FALSE: the node do not exit before;
*           otherwise return error code.
*/
HRESULT XmlWrapper::QueryNode(
                              __in BSTR bstrNodePath,
                              __out _bstr_t &pbstrValue)
{
    HRESULT hr = S_OK;
    ARG_CHECK(bstrNodePath);
    RegulNodePath(bstrNodePath);
    //::EnterCriticalSection(&m_CriticalSection);
    {
        AutoLock lock(&m_CriticalSection); // 加上自动锁
        CHECK_IF(m_xmlDocLst.empty(),E_FAIL);
        std::list<IUnknown *>::iterator it = m_xmlDocLst.begin();
        CComPtr<IXMLDOMNode> pNode = NULL;
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
        CHECK_IF_NULL(pXML);
        hr = pXML->selectSingleNode(bstrNodePath,&pNode);
        HR_CHECK(hr);
        CHECK_IF_NULL(pNode);
        CComVariant varValue(VT_EMPTY);
        hr = pNode->get_nodeTypedValue(&varValue);
        HR_CHECK(hr);
        if(varValue.vt == VT_BSTR)
        {
            //::SysFreeString(pbstrValue);
            pbstrValue = varValue.bstrVal;
            ::SysFreeString(varValue.bstrVal);
             varValue.bstrVal = NULL;
        }
    }
exit:
    //::LeaveCriticalSection(&m_CriticalSection);
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::QueryNode exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief Check the node whether have in the xml document.call like this:QueryNode(L"TangClient/Service[@ID='2']/type",bool);
* @param [in] bstrNodePath:specify the query node path.
* @param [out] bIsExit:check the node whether have
* @return   S_OK:successful;
*           S_FALSE: the node do not exit before;
*           otherwise return error code.
*/
HRESULT XmlWrapper::CheckNodeIsExit(__in BSTR bstrNodePath,__out BOOL & bIsExit)
{
    HRESULT hr = S_OK;
    ARG_CHECK(bstrNodePath);
    RegulNodePath(bstrNodePath);
    //::EnterCriticalSection(&m_CriticalSection);
    {
        AutoLock lock(&m_CriticalSection); // 加上自动锁
        CHECK_IF(m_xmlDocLst.empty(),E_FAIL);
        std::list<IUnknown *>::iterator it = m_xmlDocLst.begin();
        CComPtr<IXMLDOMNode> pNode = NULL;
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
        CHECK_IF_NULL(pXML);
#ifdef _DEBUG
		BSTR bstrXMLStr;
		hr = pXML->get_xml(&bstrXMLStr);
		HR_CHECK(hr);
#endif
        hr = pXML->selectSingleNode(bstrNodePath,&pNode);
        HR_CHECK(hr);
        if ( NULL == pNode )
        {
            bIsExit = FALSE;
        }
        else
        {
            bIsExit = TRUE;
        }
    }
exit:
    //::LeaveCriticalSection(&m_CriticalSection);
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::CheckNode exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief create a new node for xml document
* @param [out] ppNewNode: the pointer ref to point the new node.
* @param [in] bstrNodeName: the name of the new node
* @param [in] bstrAttributeName: the name of the new node's attribute
* @param [in] pAttributeValue:the value of the attribute
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT XmlWrapper::CreateNode(
                                    __out IXMLDOMNode ** ppNewNode,
                                    __in BSTR bstrNodeName,
                                    __in BSTR bstrAttributeName,
                                    __in VARIANT * pAttributeValue)
{
    HRESULT hr = S_OK;
    //::EnterCriticalSection(&m_CriticalSection);
    {
        AutoLock lock(&m_CriticalSection); // 加上自动锁
        CHECK_IF(m_xmlDocLst.empty(),E_FAIL)
        
        std::list<IUnknown *>::iterator it = m_xmlDocLst.begin();
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
        CHECK_IF_NULL(pXML);
        if (NULL == bstrAttributeName)
        {
            hr = pXML->createNode(_variant_t(long(1)),bstrNodeName,L"",ppNewNode);
            HR_CHECK(hr);
        }
        else
        {
            ARG_CHECK(pAttributeValue);
            CComPtr<IXMLDOMElement> pNewElement = NULL;
            hr = pXML->createElement(bstrNodeName,&pNewElement);
            HR_CHECK(hr);
            hr = (pNewElement)->setAttribute(bstrAttributeName,*pAttributeValue);
            HR_CHECK(hr);
//             hr = pNewElement->cloneNode(VARIANT_TRUE,ppNewNode);
//             HR_CHECK(hr);
            hr = pNewElement->QueryInterface(IID_IXMLDOMNode,(LPVOID*)ppNewNode);
            HR_CHECK(hr);
            CHECK_IF_NULL(*ppNewNode);
        }
    }
exit:
    //::LeaveCriticalSection(&m_CriticalSection);
    if(FAILED(hr) )
    {
        //LOG_ERR(L"XmlWrapper::CreateNode exit with error 0x%08X", hr);
    }
    return hr;
}

/**
* @brief regulate the bstr delete the first and the last '/' if necessary.
* @param [in out] bstrPath.
* @return.
*/
void XmlWrapper::RegulNodePath(BSTR & bstrPath)
{
    BOOL bNeedRealloc = FALSE;
    _bstr_t bstrt(bstrPath);
    std::wstring strNodePath(bstrt);

	//the parameter is null or it's "", no need check the format.
	if (NULL == bstrPath || bstrt.length() == 0)
	{
		return;
	}

    if(L'/' == strNodePath.at(strNodePath.length() - 1) )
    {
        strNodePath = strNodePath.substr(0,strNodePath.length() - 1 );
        bNeedRealloc = TRUE;
    }

    if(L'/' == strNodePath.at(0) )
    {
        strNodePath = strNodePath.substr(1,strNodePath.length() - 1);
        bNeedRealloc = TRUE;
    }

    if(bNeedRealloc)
    {
        ::SysFreeString(bstrPath);
        bstrPath = ::SysAllocString(strNodePath.c_str() );
    }//if
}

/**
* @brief remove the node that is created for the complete structure of xml document 
* @param [in] bstrNULLNodePath: the path of the node.
* @return void
*/
void XmlWrapper::RemoveNULLNode(BSTR bstrNULLNodePath)
{
    if (m_xmlDocLst.empty())
    {
        return;
    }
    RegulNodePath(bstrNULLNodePath);
    std::list<IUnknown *>::iterator it = m_xmlDocLst.begin();
    for (;it!= m_xmlDocLst.end();it++)
    {
        CComPtr<IXMLDOMNode> pResultNode = NULL;
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
        if(pXML)
        {
            pXML->selectSingleNode(bstrNULLNodePath,&pResultNode);
            if (NULL == pResultNode)
            {
                return ;
            }
            CComPtr<IXMLDOMNode> pParentNode = NULL;
            pResultNode->get_parentNode(&pParentNode);
            if(NULL != pParentNode)
            {
                pParentNode->removeChild(pResultNode,NULL);
            } 
        }
    }
}

/**
* @brief clear the text value and the attribute value of the node 
* @param [in] pNode: the node to clear.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT XmlWrapper::ClearNodeText(
                                  __in IXMLDOMNode * pNode
                                  )
{
    HRESULT hr = S_OK;
    CComPtr<IXMLDOMNodeList> pChildLst = NULL;
    // set the value of ID attribute to empty
    CComPtr<IXMLDOMNamedNodeMap> pAttributeMap = NULL;
    hr = pNode->get_attributes(&pAttributeMap);
    HR_CHECK(hr);
    if ( pAttributeMap != NULL )
    {
        CComPtr<IXMLDOMNode> pNamedItem = NULL;
        hr = pAttributeMap->getNamedItem(L"ID",&pNamedItem);
        HR_CHECK(hr);
        if ( NULL != pNamedItem )
        {
            hr = pNamedItem->put_text(L"");
            HR_CHECK(hr);
        }
    }

    hr = pNode->get_childNodes(&pChildLst);
    HR_CHECK(hr);
    long len = 0;
    if (NULL != pChildLst )
    {
        hr = pChildLst->get_length(&len);
        HR_CHECK(hr);
    }
    // clear the text of all child node 
    for (long i = 0;i<len;i++)
    {
        CComPtr<IXMLDOMNode> pNextNode = NULL;
        hr = pChildLst->nextNode(&pNextNode);
        HR_CHECK(hr);
        if ( pNextNode != NULL)
        {
            hr = ClearNodeText(pNextNode);
            HR_CHECK(hr);
        }
    }
    if( 0 == len )
    {
        hr = pNode->put_text(L"");
        HR_CHECK(hr);
    }
exit:
    return hr;
}

/**
* @brief copy the node and it's child nodes to the new node
* @param [in] pSrcNode: the old node.
* @param [out] newNode: the new node
* @return HRESULT  S_OK:successful;E_FAIL: failed;
*/
HRESULT XmlWrapper::CopyNode(
                 __in IXMLDOMNode * pSrcNode,
                 __out IXMLDOMNode ** newNode)
{
    HRESULT hr = S_OK;
    CComPtr<IXMLDOMNodeList> pChildLst = NULL;
    hr = pSrcNode->cloneNode(TRUE,newNode);
    HR_CHECK(hr);
    hr = pSrcNode->get_childNodes(&pChildLst);
    HR_CHECK(hr);
    long len = 0;
    if ( NULL != pChildLst )
    {
        hr = pChildLst->get_length(&len);
        HR_CHECK(hr);
    }
    for (long i = 0;i<len;i++)
    {
        CComPtr<IXMLDOMNode> pNextNode = NULL;
        hr = pChildLst->nextNode(&pNextNode);
        HR_CHECK(hr);
        if ( pNextNode != NULL )
        {   
            CComPtr<IXMLDOMNode> pNewNextNode = NULL;
            hr =  CopyNode(pNextNode,&pNewNextNode);
            HR_CHECK(hr);
            CHECK_IF_NULL(pNewNextNode);
            hr = (*newNode)->appendChild(pNewNextNode,NULL);
            HR_CHECK(hr);
        }
    }
    
exit:
    return hr;    
}

/**
* @brief judge the node do have brother node
* @param [in] itemXMLDoc: the pointer to the xml document.
* @param [in] nodeFullPath: the path of the node with attribute.
* @param [out] isHave: FALSE means the node do not have brother node,TRUE means the node have brother node
* @return HRESULT  S_OK:successful;E_FAIL: failed;
*/
HRESULT XmlWrapper::JudgeIsHaveBrother(
                                       __in IXMLDOMDocument * itemXMLDoc,
                                       __in BSTR nodeFullPath,
                                       __out BOOL & isHave)
{
    HRESULT hr = S_OK;
    std::wstring nodePath(nodeFullPath);
    CComPtr<IXMLDOMNodeList> resultList = NULL;
    ARG_CHECK(itemXMLDoc);
    SIZE_T lastOf =  nodePath.find_last_of(L"[@");
    if ( lastOf != -1 )
    {
        nodePath = nodePath.substr(0,lastOf-1);
    }

    hr = itemXMLDoc->selectNodes(_bstr_t(nodePath.c_str()),&resultList);
    HR_CHECK(hr);
    long len = 0;
    if (NULL != resultList)
    {
        hr = resultList->get_length(&len);
        HR_CHECK(hr);
    }
    if (len > 1)
    {
        isHave = TRUE;
    }
    else
    {
        isHave = FALSE;
    }
exit:
    return hr;
}

/**
* @brief check the new node do exit in the xml document
* @param [out] isExit: the ref to the return value that mark the node does exit .
* @param [in] nodePath: the path of the new node
* @param [in] bstrAttributeName: the name of the new node's index attribute
* @param [in] pAttributeValue:the value of the attribute
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT XmlWrapper::CheckNewNodeIsExit(
                        __out BOOL &isExit,
                        __in BSTR nodePath,
                        __in BSTR indexAttibuteName,
                        __in BSTR indexAttibuteValue)
{
    HRESULT hr = S_OK;
    _bstr_t bstrNodePath;
    std::list<IUnknown *>::iterator it;
    ARG_CHECK(nodePath);
    if(NULL == indexAttibuteName )
    {
        isExit = FALSE;
        hr = S_OK;
        goto exit;
    }
    else
    {
        ARG_CHECK(indexAttibuteValue);
    }
    CHECK_IF(m_xmlDocLst.empty(),E_FAIL);
    bstrNodePath = _bstr_t(nodePath);
    bstrNodePath += _bstr_t(L"[@") +  _bstr_t(indexAttibuteName) +_bstr_t(L"='") + _bstr_t(indexAttibuteValue) + _bstr_t(L"']");
    
    it = m_xmlDocLst.begin();
    //test whether existed the same text node before
    IXMLDOMNode * pExistedNodeBefore = NULL;
    {
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
        CHECK_IF_NULL(pXML);
        hr = pXML->selectSingleNode(bstrNodePath,&pExistedNodeBefore);
        HR_CHECK(hr);
        if(pExistedNodeBefore != NULL)
        {
            pExistedNodeBefore->Release();
            isExit = TRUE;
        }
        else
        {
            isExit = FALSE;
        }
    }

exit:
    return hr;
}


/**
* @brief release all the xml document in the list 
* @return void
*/
void XmlWrapper::ReleaseAllXmlDocument()
{
    std::list<IUnknown *>::iterator it;
    AutoLock lock(&m_CriticalSection); // 加上自动锁
    it = m_xmlDocLst.begin();
    for (;it!= m_xmlDocLst.end();it++)
    {
        long lReadyState = 0;
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
        if(pXML)
        {
            HRESULT hr = pXML->get_readyState(&lReadyState);
            if (SUCCEEDED(hr) && lReadyState == 1)
            {
                pXML->abort();
            }
        }
        (*it)->Release();
    }
    m_xmlDocLst.clear();
}

/**
* @brief Load xml for list.
* @param none.
* @return.
*/
HRESULT XmlWrapper::ReLoadXml(BSTR & bstrXml)
{
    HRESULT  hr = S_OK;
    VARIANT_BOOL bSuccess = VARIANT_FALSE;
    CHECK_IF_NULL(bstrXml);
    for(std::list<IUnknown *>::iterator it = m_xmlDocLst.begin();it != m_xmlDocLst.end();it++)
    {
        CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pXML(*it);
        CHECK_IF_NULL(pXML);
        hr = pXML->loadXML(bstrXml,&bSuccess);
        HR_CHECK(hr);
        CHECK_IF(bSuccess == VARIANT_FALSE,E_FAIL);
    }

    //m_FirstDOMDocument should call this to insure UI refresh 
    //hr = m_FirstDOMDocument->loadXML(bstrXml,&bSuccess);

    //::SysFreeString(bstrXml);
    //bstrXml = NULL;
exit:
    return hr;
}

HRESULT XmlWrapper::GetXmlDocument( IUnknown* pElement,IUnknown** pXmlDocument )
{
	HRESULT hr = S_OK;
	BSTR propertyName = NULL;
	CComPtr<IDispatch> dispatch = NULL;
	DISPID dispid;
	UINT errArg;

	CHECK_IF_NULL(pElement);
	CHECK_IF_NULL(pXmlDocument);

	hr = pElement->QueryInterface(IID_IDispatch,(void **)&dispatch);
	CHECK_IF_NULL(dispatch);

	propertyName = ::SysAllocString(L"XMLDocument");
	hr = dispatch->GetIDsOfNames(IID_NULL, &propertyName, 1,
		LOCALE_SYSTEM_DEFAULT, &dispid);
	::SysFreeString(propertyName);
	if (SUCCEEDED(hr))
	{
		VARIANT pVarResult2;
		::VariantInit(&pVarResult2);
		DISPPARAMS dParams2 = { NULL, 0, 0, 0 };

		//invoke to query XMLDocument property.
		hr = dispatch->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
			DISPATCH_PROPERTYGET, &dParams2, &pVarResult2, 0, &errArg);
		if (SUCCEEDED(hr) && pVarResult2.vt == VT_DISPATCH)
		{
			CHECK_IF_NULL(pVarResult2.pdispVal);
			*pXmlDocument = pVarResult2.pdispVal;
			return S_OK;
		}
	}

exit:
	return hr;
}

XmlEventArg::XmlEventArg()
{
	m_xpath = NULL;
	m_pElement = NULL;
	m_pDomNode = NULL;
}

XmlEventArg::~XmlEventArg()
{

}

STDMETHODIMP XmlEventArg::get_xpath( BSTR* ppStrPath )
{
	HRESULT hr = S_OK;
	ARG_CHECK(ppStrPath);
	CHECK_IF_NULL ( m_xpath );
    *ppStrPath = ::SysAllocString(m_xpath);
exit:
	return hr;
}

STDMETHODIMP XmlEventArg::get_srcElement( IUnknown** pElement )
{
	HRESULT hr = S_OK;
	ARG_CHECK(pElement);
	*pElement = m_pElement;
exit:
	return hr;
}

STDMETHODIMP XmlEventArg::get_eventType( UINT32* uEventType )
{
	HRESULT hr = S_OK;
	ARG_CHECK(uEventType);
	*uEventType = m_enumType;
exit:
	return hr;
}

STDMETHODIMP XmlEventArg::QueryInterface( REFIID riid, void** ppv )
{
	*ppv=IsEqualIID(riid, IID_IDispatch)?this:0;
	return *ppv?S_OK:E_NOINTERFACE;
}

HRESULT XmlEventArg::put_xpath( BSTR xpath )
{
	HRESULT hr  = S_OK;
	ARG_CHECK(xpath);
	if (m_xpath != NULL)
	{
		::SysFreeString(m_xpath);
		m_xpath = NULL;
	}

	m_xpath = ::SysAllocString(xpath);
exit:
	return hr;
}

HRESULT XmlEventArg::put_srcElement( IUnknown* pElement )
{
	HRESULT hr  = S_OK;
	ARG_CHECK(pElement);
	m_pElement.Release();
	m_pElement = pElement;
exit:
	return hr;
}

HRESULT XmlEventArg::put_evetnType( XmlEventType eventType )
{
	m_enumType = eventType;
	return S_OK;
}

XmlEvent::XmlEvent( IHTMLElement* pElement,
				   IXMLDOMNode* pDomNode,
				   BSTR strJSFun,
				   BSTR strXpath,
				   XmlEventType eventType )
{
	if (strJSFun != NULL)
	{
		m_strEventName = ::SysAllocString(strJSFun);
	}
	
    if (pElement)
    {
        pElement->QueryInterface(IID_IHTMLElement,(void **)&m_pElement);
    }

    if (pDomNode)
    {
        pDomNode->QueryInterface(IID_IXMLDOMNode, (void **)&m_pDomNode);
    }

	if (strXpath)
	{
		m_strXpath = ::SysAllocString(strXpath);
	}
	
	m_eventType = eventType;
}

XmlEvent::~XmlEvent()
{
    SYSFREESTRING(m_strEventName);
    SYSFREESTRING(m_strXpath);
}

HRESULT XmlEvent::Fire()
{
	HRESULT hr = S_OK;
	CComPtr<IHTMLDocument2> pHTDoc = NULL;

	CHECK_IF_NULL(m_pElement);
	hr = m_pElement->get_document((IDispatch**) &pHTDoc);
	if(SUCCEEDED(hr) && pHTDoc)
	{
		UINT errArg = 0;
		DISPID dispid = 0;
		CComPtr<IHTMLWindow2> pHTWnd = NULL;
		hr = pHTDoc->get_parentWindow(&pHTWnd);	
		if (SUCCEEDED(hr) && pHTWnd)
		{
			CHECK_IF_NULL(m_strEventName);
			hr = pHTWnd->GetIDsOfNames(IID_NULL, &m_strEventName, 1,
				LOCALE_SYSTEM_DEFAULT, &dispid);
			if(SUCCEEDED(hr))
			{
				/*HR_CHECK(m_eventArg.put_evetnType(m_eventType));
				HR_CHECK(m_eventArg.put_xpath(m_strXpath));
				HR_CHECK(m_eventArg.put_srcElement(m_pElement));*/

#ifndef XML_EVENT_ARGS_NUM
                #define XML_EVENT_ARGS_NUM	(3)
#endif
                //alloc variant variable to avoid crash.
                VARIANT varParam[XML_EVENT_ARGS_NUM];
                //remove the forth param.
                //if (m_pElement)
                //{
                //    //第四个参数 source element
                //    varParam[0].vt = VT_UNKNOWN;
                //    hr = m_pElement->QueryInterface(IID_IUnknown,(void**)&(varParam[0].punkVal));
                //    if (FAILED(hr))
                //    {
                //        return hr;
                //    }
                //}

                if (m_pDomNode)
                {
                    //第三个参数 node
                    varParam[0].vt = VT_UNKNOWN;
                    hr = m_pDomNode->QueryInterface(IID_IUnknown,(void**)&(varParam[0].punkVal));
                    if (FAILED(hr))
                    {
                        return hr;
                    }
                }
                else
                {
                    varParam[0].vt = VT_NULL;
                }

                //第二个参数 xpath
                varParam[1].vt = VT_BSTR;
                varParam[1].bstrVal = SysAllocString(m_strXpath);

                //第一个参数 eventType
                varParam[2].vt = VT_UINT;
                varParam[2].uintVal = m_eventType;

				DISPPARAMS dParams = { varParam, 0, XML_EVENT_ARGS_NUM, 0 };
				hr = pHTWnd->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
					DISPATCH_METHOD, &dParams, 0, 0, &errArg);
				if(SUCCEEDED(hr))
				{
					//LOG_INFO(L"XmlWrapper: fire event:%d node:%s successfully.",m_eventType,m_strXpath);
				}
				else
				{
					LPVOID lpMsgBuf = NULL;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
						FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						HRESULT_FROM_WIN32(hr),
						MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
						(LPTSTR)&lpMsgBuf,
						0,
						NULL);
					//LOG_INFO(L"XmlWrapper: fire event:%d node:%s failed, errorMsg:%s.",m_eventType,m_strXpath,lpMsgBuf);
                    LocalFree(lpMsgBuf);
				}
			}
		}
	}

exit:
	return hr;
}
