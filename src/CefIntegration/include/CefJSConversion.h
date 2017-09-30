/*
 * CefJSConversion.h
 *
 *  Created on: June 27, 2015
 *      Author: gang wang
 */

#ifndef CEF_JS_CONVERSION_H_
#define CEF_JS_CONVERSION_H_

#include "include/internal/cef_ptr.h"
#include "include/cef_v8.h"
#include "ClientDefs.h"

namespace cefIntegration {
	
class CefJSConversion {
public:
	/**
     * 转换v8数组为list
     * @param src v8数组
	 * @param dest list
     */
	static bool V8ArrayToList(CefRefPtr<CefV8Value> src, CefRefPtr<CefListValue> dest);
	
	/**
     * 转换list为v8数组
     * @param src list
	 * @param target v8数组
     */
	static void ListToV8Array(CefRefPtr<CefListValue> src, CefRefPtr<CefV8Value> dest) ;

	/**
     * 将v8数据插入到list[index]位置
     * @param source v8数据类型
	 * @param index list的索引
	 * @param dest list
     */
	static bool V8ValueToList(CefRefPtr<CefV8Value> src, int index,
			CefRefPtr<CefListValue> dest);

	/**
	 * 转换list[index]为v8数据类型
	 * @param src list
	 * @param index list的索引
	 * @param dest v8数据类型
	 */
	static void ListToV8Value(CefRefPtr<CefListValue> src, int index, CefRefPtr<CefV8Value> &dest);

	/**
	 * 转换v8数据类型为字典类型
	 * @param src v8数据类型
	 * @param dest 字典类型
	 */
	static bool V8ValueToDictionary(CefRefPtr<CefV8Value> src, CefRefPtr<CefDictionaryValue> dest);

	/**
	 * 转换字典类型为v8数据类型
	 * @param src 字典类型
	 * @param dest v8数据类型
	 */
	static void DictionaryToV8Value(CefRefPtr<CefDictionaryValue> src, CefRefPtr<CefV8Value> dest);

	/**
	 * 将js中number类型转换为double。
	 * number类型，小于int时C++映射成int;大于int时，则映射为double。
	 * 所以对于C++的int64整型需要该转换
	 * @param src list
	 * @param index list的索引
	 */
	CEF_CLIENT_API static double JSNumberToDouble(CefRefPtr<CefListValue> src, int index);

	/**
	 * 判断src[index]是否为number类型。
	 * @param src list
	 * @param index list的索引
	 */
	CEF_CLIENT_API static bool IsJsNumber(CefRefPtr<CefListValue> src, int index);

	/**
	 * 将字典类型转换为string类型。
	 * @param src 字典类型
	 * @param json json字符串
	 */
	static CEF_CLIENT_API void DictionaryToString(CefRefPtr<CefDictionaryValue> src, std::wstring& json);

	static CEF_CLIENT_API void ListToString(CefRefPtr<CefListValue> src, std::wstring &json);

	static void ListToString(CefRefPtr<CefListValue> src, int index, std::wstring &json);

};

}

#endif