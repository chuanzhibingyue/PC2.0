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

#ifndef GNET_TANG_CLIENT_XML_WRAPPER_H
#define GNET_TANG_CLIENT_XML_WRAPPER_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <list>
#include <map>
#include <atlbase.h>
#include <atlcom.h>
#include <comdef.h>
#include <MsXml2.h>
#include <mshtml.h>

#include <Log.h>
#include "debug.h"
#include "Commonlib.h"

/*---------------------------------DEFINITION--------------------------------*/
//#ifndef __cplusplus
//#error "XmlWrapper.h" need c++ compiler
//#endif 

#define XMLHEADER				L"<?xml version=\"1.0\" encoding=\"utf-8\"?>"

enum XmlEventType
{
	XML_ET_INITED = 0,
	XML_ET_INSERT,
	XML_ET_DELETE,
	XML_ET_UPDATE
};

class XmlEventArg : public IDispatchImpl<ITangXmlEventArg>
{
public:
	XmlEventArg();
	~XmlEventArg();

	//STDMETHOD(get_xpath)(BSTR* ppStrPath);
	//STDMETHOD(get_srcElement)(IUnknown** pElement);
	//STDMETHOD(get_eventType)(UINT32* uEventType);

	/*ITangXmlEventObject interface*/
	STDMETHODIMP get_xpath(BSTR* ppStrPath);
	STDMETHODIMP get_srcElement(IUnknown** pElement);
	STDMETHODIMP get_eventType(UINT32* uEventType);

	HRESULT put_xpath(BSTR xpath);
	HRESULT put_srcElement(IUnknown* pElement);
	HRESULT put_evetnType(XmlEventType eventType);
	
	//IUnkown interface
	virtual inline STDMETHODIMP_(ULONG)	AddRef(){return 0;}
	virtual inline STDMETHODIMP_(ULONG)	Release(){return 0;}
	virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppv);

private:
	BSTR m_xpath;
	CComPtr<IXMLDOMNode> m_pDomNode;
	CComPtr<IUnknown> m_pElement;
	XmlEventType m_enumType;
};

class XmlEvent
{
public:
	XmlEvent(
		IHTMLElement* pElement,
		IXMLDOMNode* pDomNode,
		BSTR strJSFun,
		BSTR strXpath,
		XmlEventType eventType);
	~XmlEvent();
	HRESULT Fire();

private:
	BSTR m_strEventName;
	BSTR m_strXpath;
	CComPtr<IXMLDOMNode> m_pDomNode;
	CComPtr<IHTMLElement> m_pElement;
	XmlEventType m_eventType;
	//XmlEventArg m_eventArg;
};

/**
* @class XmlWrapper
* @brief This class is defined for xml operators such as add node,delete node,query node value,edit node value.It provides
* xml operator API.I suppose that all the data type in XML file is BSTR,If we want to support other data types,i should Modify the code.
* 
* @author Jianhua.Chen
* @version 1.0.0
*/
class XmlWrapper
{
public:
	XmlWrapper( BSTR  bstrDom);
public:
	~XmlWrapper(void);
private:
    XmlWrapper(){}
public:
	/************************************************************************/
	/*
	/*the sample xml as follow:
	/*<TangClient>
	<Service ID=1>
	<type>1</type>
	<privileges>10000001</privileges>
	<privileges>10000002</privileges>
	</Service>
	<Service ID=2>
	<type>1</type>
	<privileges>10000004</privileges>
	<privileges>10000005</privileges>
	</Service>
	</TangClient>*/
	/************************************************************************/

    /**
    * @brief Add type IXMLDOMDocument * to the date member. 
    * @param [in] pXmlDomDocument specify IXMLDOMDocument * which to be added.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    HRESULT AddXmlDocument(__in IUnknown *pElement);

    /**
    * @brief init the dom of the xml document. 
    * @param [in] bstrDom the complete struct of the xml document's dom
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
   // HRESULT InitXmlDom(__in BSTR  bstrDom);

    /**
    * @brief Query xml node text.
    * @param [in] pXmlDomDocument specify IXMLDOMDocument * which to be deleted.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    HRESULT DeleteXmlDocument(__in IUnknown *pElement);

	/**
	* @brief Add xml node.call like this: AddRootNode(L"TangClient");
	* @param [in] bstrRootNodeName the root node name.
	* @return HRESULT return S_OK if successful, otherwise return error code.
	*/
	HRESULT AddRootNode(
		__in BSTR bstrRootNodeName);

    /**
    * @brief Add xml node.call like this: AddNode(L"TangClient/Service[@ID="1"]",pNode *);
    * @param [in] bstrParentNodePath the parent node path.
    * @param [in] pNode the node name to be added.
    * @return   S_OK:successful;
    *           S_FALSE: the parent node do not exit;
    *           otherwise return error code.
    */
    HRESULT AddNode(
        __in BSTR bstrParentNodePath,
        __in IXMLDOMNode * pElement,
        __in BSTR position = NULL);

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
	HRESULT AddNode(
		__in BSTR bstrParentNodePath,
		__in BSTR bstrNodeName,
		__in BSTR bstrNodeValue,
		__in BSTR bstrAttributeName = NULL,
		__in BSTR bstrAttributeValue = NULL,
        __in BSTR position = NULL);

	

    /**
    * @brief Delete xml node.call like this:DeleteNode(L"TangClient/Service[@ID="2"]");
    * @param [in] bstrNodePath the node path to be deleted.
    * @return   S_OK:successful;
    *           S_FALSE: the node do not exit before;
    *           otherwise return error code.
    */
	HRESULT DeleteNode(
		__in BSTR bstrNodePath
		);

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
	HRESULT EditNode(
		__in BSTR bstrNodePath,
		__in BSTR bstrNodeValue,
		__in BSTR bstrAttributeName = NULL,
		__in BSTR bstrAttributeValue = NULL);

	/**
	* @brief Query xml node text.call like this:QueryNode(L"TangClient/Service[@ID='2']/type",&bstr);
	* @param [in] bstrNodePath specify the query node path.
	* @param [out] bstrValue save the node text
    * @return   S_OK:successful;
    *           S_FALSE: the node do not exit before;
    *           otherwise return error code.
    */
	HRESULT QueryNode(
		__in BSTR bstrNodePath,
		__out _bstr_t &pbstrValue);

    /**
    * @brief Check the node whether have in the xml document.call like this:QueryNode(L"TangClient/Service[@ID='2']/type",bool);
    * @param [in] bstrNodePath:specify the query node path.
    * @param [out] bIsExit:check the node whether have
    * @return   S_OK:successful;
    *           S_FALSE: the node do not exit before;
    *           otherwise return error code.
    */
    HRESULT CheckNodeIsExit(__in BSTR bstrNodePath,__out BOOL & bIsExit);

    
    /**
    * @brief create a new node for xml document
    * @param [out] ppNewNode: the pointer ref to point the new node.
    * @param [in] bstrNodeName: the name of the new node
    * @param [in] bstrAttributeName: the name of the new node's attribute
    * @param [in] pAttributeValue:the value of the attribute
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    HRESULT CreateNode(
        __out IXMLDOMNode ** ppNewNode,
        __in BSTR bstrNodeName,
        __in BSTR bstrAttributeName = NULL,
        __in VARIANT * pAttributeValue = NULL);

private:

    /**
    * @brief regulate the bstr delete the first and the last '/' if necessary.
    * @param [in out] bstrPath.
    * @return.
    */
    void RegulNodePath(BSTR & bstrPath);

    /**
    * @brief remove the node that is created for the complete structure of xml document 
    * @param [in] bstrNULLNodePath: the path of the node.
    * @return void
    */
    void RemoveNULLNode(BSTR bstrNULLNodePath);

    /**
    * @brief clear the text value and the attribute value of the node 
    * @param [in] pNode: the node to clear.
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    HRESULT ClearNodeText(
        __in IXMLDOMNode * pNode
        );

    /**
    * @brief copy the node and it's child nodes to the new node
    * @param [in] pSrcNode: the old node.
    * @param [out] newNode: the new node
    * @return HRESULT  S_OK:successful;E_FAIL: failed;
    */
    HRESULT CopyNode(
        __in IXMLDOMNode * pSrcNode,
        __out IXMLDOMNode ** newNode);

    /**
    * @brief judge the node do have brother node
    * @param [in] itemXMLDoc: the pointer to the xml document.
    * @param [in] nodeFullPath:the path of the node with attribute.
    * @param [out] isHave: FALSE means the node do not have brother node,TRUE means the node have brother node
    * @return HRESULT  S_OK:successful;E_FAIL: failed;
    */
    HRESULT JudgeIsHaveBrother(
        __in IXMLDOMDocument * itemXMLDoc,
        __in BSTR nodeFullPath,
        __out BOOL & isHave);

    /**
    * @brief check the new node do exit in the xml document
    * @param [out] isExit: the ref to the return value that mark the node does exit .
    * @param [in] nodePath: the path of the new node
    * @param [in] bstrAttributeName: the name of the new node's index attribute
    * @param [in] pAttributeValue:the value of the attribute
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
    HRESULT CheckNewNodeIsExit(
        __out BOOL &isExit,
        __in BSTR nodePath,
        __in BSTR indexAttibuteName,
        __in BSTR indexAttibuteValue);

    /**
    * @brief release all the xml document in the list 
    * @return void
    */
    void ReleaseAllXmlDocument(); 

    /**
    * @brief Load xml for list.
    * @param none.
    * @return.
    */
    HRESULT ReLoadXml(BSTR & bstrXml);

	HRESULT GetXmlDocument(IUnknown* pElement,IUnknown** pXmlDocument);

private:
	CRITICAL_SECTION m_CriticalSection;
	std::list<IUnknown *> m_xmlDocLst;
	std::map<IUnknown*,BSTR> m_eventHandlerMap;
	std::map<IUnknown*,IHTMLElement*> m_elementMap;
};

#endif //GNET_TANG_CLIENT_XML_WRAPPER_H