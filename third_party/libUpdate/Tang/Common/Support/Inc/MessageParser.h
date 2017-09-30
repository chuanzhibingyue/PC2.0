/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file MessageParser.h
* @brief DeSerialize the buffer to a ExternalMesasge in Tang Client.
* 
* @author Xiaogang Yu (xiaogang.yu@gnetis.com)
*
* @date Feb. 2 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/

#ifndef GNET_TANG_CLIENT_MESSAGEPARSER_H
#define GNET_TANG_CLIENT_MESSAGEPARSER_H

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include <Message\ExternalMessage.h>
#include <Message\CtrlMessage.h>
#include <Message\TransportMessage.h>
#include <Debug.h>
#include <string>

/*---------------------------------DEFINITION--------------------------------*/
#ifndef __cplusplus
#error "MessageParser.h" need c++ compiler
#endif 

#define MESSAGE_TYPE_CONTROL        (0)
#define MESSAGE_TYPE_TRANSPORT      (1)

namespace MessageParser
{

/**
* @brief This is a static function for DeSerialize buffer to ExternalMessage.
* @param [in][out] ppExternMsg: return the pointer of ExternalMessage.
* @param [in] pBuffer: the pointer of buffer.
* @param [in] cbBuffer: the size of buffer.
* @param [in] msgType: the type of message.
* @return return S_OK if successful, otherwise return error code.
*/
static
HRESULT
DeSerializeHdr(
               __inout ExternalMessage** ppExternMsg,
               __in LPBYTE pBuffer, 
               __in size_t cbBuffer, 
               __in UINT32 msgType
               )
{
    HRESULT             hr = S_OK;
    ExternalMessage*    pAbsMsg = NULL;

    ARG_CHECK(pBuffer);

    if(msgType == MESSAGE_TYPE_CONTROL)
    {
        // Control message.
        pAbsMsg = new CtrlMessage();
        ASSERT_CHECK_IF_NULL(pAbsMsg);

        *ppExternMsg = pAbsMsg;

        pAbsMsg->pBuffer = pBuffer;
        pAbsMsg->cbBuffer = cbBuffer;

        // Copy the ctrl message header from the buffer.
        UINT32 nLength = sizeof(CtrlMessage) - sizeof(ExternalMessage);
        memcpy((LPBYTE)pAbsMsg + sizeof(ExternalMessage), pBuffer, nLength);
    }
    else if (msgType == MESSAGE_TYPE_TRANSPORT)
    {
        // Transport message.
        pAbsMsg = new TransportMessage();
        ASSERT_CHECK_IF_NULL(pAbsMsg);

        *ppExternMsg = pAbsMsg;

        pAbsMsg->pBuffer = pBuffer;
        pAbsMsg->cbBuffer = cbBuffer;

        TransportMessage* pTransMsg = (TransportMessage*)pAbsMsg;
        UINT32 sizeRtpHdr = 0;
        UINT32 sizeExtendRtpHdr = 0;

        // DeSerialize the rtp header data.
        hr = pTransMsg->rtpHdr.DeSerialize(pAbsMsg->pBuffer, cbBuffer, sizeRtpHdr);
        HR_CHECK(hr);

        // DeSerialize the rtp extension header data.
        hr = pTransMsg->rtpExtendHdr.DeSerialize(pAbsMsg->pBuffer + sizeRtpHdr, cbBuffer - sizeRtpHdr, sizeExtendRtpHdr);
        HR_CHECK(hr);
    }

exit:
    if (FAILED(hr))
    {
        LOG_ERR(L"MessageParser::DeSerializeHdr exit with error 0x%08X", hr);
    }

    return hr;
}

/*
* 该套方法只是针对消息体长度可变的消息提出。
* 使用这套方法时，消息体定义中不需要标识出列表的长度，这样可以避免在使用过程中
* 的列表长度和列表具体内容长度的同步问题。使用时需要注意在GetBodyLength中添加
* 上列表的长度，几个列表就添加几个长度，每个长度对应sizeof(UINT32)个字节。
*/


template <class T>
HRESULT SerializeData(
                      LPBYTE __in pBufDest, // this buffer must been allocated outside.
                      size_t __in cbBufDest,
                      const LPBYTE __in pStableData, 
                      size_t __in cbStableData, 
                      std::vector<T> __in &vecElement,
                      size_t __inout &cbBufCost
                      )
{
    HRESULT hr = S_OK;

    ARG_CHECK(pBufDest);
    ARG_CHECK(pStableData);

    cbBufCost = 0;
    size_t sizeVector = vecElement.size() * sizeof(T);
    cbBufCost = cbStableData + sizeVector;
    if (cbBufDest < (cbBufCost + sizeof(UINT32)))
    {
        hr = E_FAIL;
    }
    else
    {
        memcpy(pBufDest, pStableData, cbStableData);
        pBufDest += cbStableData;
        *(UINT32*)pBufDest = (UINT32)(vecElement.size());
        cbBufCost += sizeof(UINT32);
        pBufDest += sizeof(UINT32);
        if (vecElement.size() > 0)
        {
            memcpy(pBufDest, (LPBYTE)&(vecElement.at(0)), sizeVector);
        }
    }

exit:
    if (FAILED(hr))
    {
        LOG_ERR(L"MessageParser::SerializeData exit with error 0x%08X", hr);
    }
    return hr;
}


template <class T>
HRESULT DeSerializeData(
                        LPBYTE __in pStableData, // the pointer of stable data in message.
                        size_t __in cbStableData, // the body length of stable data in message.
                        const __in LPBYTE pBufSrc, 
                        size_t __in sizeBufSrc, 
                        // the T struct cann't include vector,list, *pointer, and so on.
                        std::vector<T> __in &vecElement,
                        size_t __inout &sizeBufCost
                        )
{
    HRESULT hr = S_OK;

    ARG_CHECK(pStableData);
    ARG_CHECK(pBufSrc);

    LPBYTE pSrc = const_cast<LPBYTE>(pBufSrc);
    size_t sizeStableData = cbStableData + sizeof(UINT32);
    size_t sizeList = 0;
    sizeBufCost = 0;

    if ((sizeStableData + sizeList) > sizeBufSrc)
    {
        hr = E_FAIL;
    }
    else
    {
        memcpy(pStableData, pSrc, cbStableData);

        // count the size of the deserialize buffer in message,
        // this size will return.
        sizeBufCost += cbStableData;

        // skip the stable data length .
        pSrc += cbStableData;
        // get the length of vector.
        UINT32 uVecLength = *((UINT32*)pSrc);
        // skip the length of vector.
        pSrc += sizeof(UINT32);
        sizeBufCost += sizeof(UINT32);
        sizeBufCost += sizeof(T) * uVecLength;
        if (sizeBufCost > sizeBufSrc)
        {
            // check the buffer length before copy action.
            hr = E_FAIL;
            goto exit;
        }

        T *pVecElement = (T*)pSrc;
        for (UINT32 i = 0; i < uVecLength; i++)
        {
            vecElement.push_back(pVecElement[i]);
        }
    }

exit:
    if (FAILED(hr))
    {
        LOG_ERR(L"MessageParser::DeSerializeData exit with error 0x%08X", hr);
    }
    return hr;
}

static
HRESULT SerializeString(
                        LPBYTE __in pBufDest, // this buffer must been allocated outside.
                        size_t __in cbBufDest,
                        const LPBYTE __in pStableData, 
                        size_t __in cbStableData, 
                        std::string __in &strSrc,
                        size_t __inout &sizeBufCost
                        )
{
    HRESULT hr = S_OK;

    ARG_CHECK(pBufDest);
    ARG_CHECK(pStableData);

    sizeBufCost = 0;
    size_t sizeString = strSrc.length();
    sizeBufCost = cbStableData + sizeString;
    if (cbBufDest < (sizeBufCost + sizeof(UINT32)))
    {
        hr = E_FAIL;
    }
    else
    {
        memcpy(pBufDest, pStableData, cbStableData);
        pBufDest += cbStableData;
        *(UINT32*)pBufDest = (UINT32)(strSrc.length());
        sizeBufCost += sizeof(UINT32);
        pBufDest += sizeof(UINT32);
        if (strSrc.length() > 0)
        {
            memcpy(pBufDest, strSrc.c_str(), sizeString);
        }
    }

exit:
    if (FAILED(hr))
    {
        LOG_ERR(L"MessageParser::SerializeString exit with error 0x%08X", hr);
    }
    return hr;
}

static
HRESULT DeSerializeString(
                          LPBYTE __in pStableData, // the pointer of stable data in message class.
                          size_t __in cbStableData, // the body length of stable data in message.
                          const LPBYTE __in pBufSrc, 
                          size_t __in sizeBufSrc, 
                          std::string __in &strDest,
                          size_t __inout &sizeBufCost
                          )
{
    HRESULT hr = S_OK;

    ARG_CHECK(pStableData);
    ARG_CHECK(pBufSrc);

    LPBYTE pSrc = const_cast<LPBYTE>(pBufSrc);
    size_t sizeStableData = cbStableData + sizeof(UINT32);
    UINT32 sizeString = 0;
    sizeBufCost = 0;

    if ((sizeStableData + sizeString) > sizeBufSrc)
    {
        hr = E_FAIL;
    }
    else
    {
        memcpy(pStableData, pSrc, cbStableData);

        // count the size of the deserialize buffer in message,
        // this size will return.
        sizeBufCost += cbStableData;

        // skip the stable data length .
        pSrc += cbStableData;
        // get the length of vector.
        sizeString = *((UINT32*)pSrc);
        sizeBufCost += sizeString * sizeof(char);
        // skip the length of vector.
        pSrc += sizeof(UINT32);
		sizeBufCost += sizeof(UINT32);
        if (sizeBufCost > sizeBufSrc)
        {
            // check the buffer length before copy action.
            hr = E_FAIL;
            goto exit;
        }

        strDest.append((char*)pSrc, sizeString);
    }

exit:
    if (FAILED(hr))
    {
        LOG_ERR(L"MessageParser::DeSerializeString exit with error 0x%08X", hr);
    }
    return hr;
}


}//namespace MessageParser

#endif //GNET_TANG_CLIENT_MESSAGEPARSER_H
