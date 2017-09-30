#include "CefJSConversion.h"
#include <sstream>
#include <iomanip>
#include "common/StringConvert.h"
#include <math.h>
#include "LogManager.h"

namespace cefIntegration {

using namespace uc;

static bool CheckV8Value(CefRefPtr<CefV8Value> src) {
	if (src->IsDouble()) {
		if (::isnan<double>(src->GetDoubleValue())) {
			UC_LOG(ERROR) << "v8 value is nan!";
			return false;
		}
	}

	return true;
}

bool CefJSConversion::V8ArrayToList(CefRefPtr<CefV8Value> src, 
	CefRefPtr<CefListValue> dest) {
	if (!src->IsArray()) {
		return false;
	}
	
	int len = src->GetArrayLength();
	if (len == 0) {
	  	return true;
	}
    		
	dest->SetSize(len);

	bool res = true;
	for (int i = 0; i < len && res; ++i) {
		res = V8ValueToList(src->GetValue(i), i, dest);
	}	

	return res;
}

void CefJSConversion::ListToV8Array(CefRefPtr<CefListValue> src, 
	CefRefPtr<CefV8Value> dest) {
	if (!dest->IsArray()) {
		return;
	}

	int len = src->GetSize();
	if (len == 0) {
	  	return;
	}

	for (int i = 0; i < len; ++i) {
		CefRefPtr<CefV8Value> value;
		ListToV8Value(src, i, value);
		dest->SetValue(i, value);
	}
}

bool CefJSConversion::V8ValueToList(CefRefPtr<CefV8Value> src, int index, 
	CefRefPtr<CefListValue> dest) {
	if (!CheckV8Value(src)) {
		return false;
	}

	if (src->IsArray()) {
		CefRefPtr<CefListValue> newList = CefListValue::Create();
		if (V8ArrayToList(src, newList)) {
			dest->SetList(index, newList);
		}
		else {
			return false;
		}
	} else if (src->IsString() || src->IsDate()) {
		dest->SetString(index, src->GetStringValue());
	} else if (src->IsBool()) {
		dest->SetBool(index, src->GetBoolValue());
	} else if (src->IsInt()) {
		dest->SetInt(index, src->GetIntValue());
	} else if (src->IsDouble()) {
		dest->SetDouble(index, src->GetDoubleValue());
	}
	else if (src->IsNull()) {
        dest->SetNull(index);
	}
	else if (src->IsObject()) {
 		CefRefPtr<CefDictionaryValue> dictionary = CefDictionaryValue::Create();
		if (V8ValueToDictionary(src, dictionary)) {
			dest->SetDictionary(index, dictionary);
		}
		else {
			return false;
		}
		
	}

	return true;
}

void CefJSConversion::ListToV8Value(CefRefPtr<CefListValue> src, int index, 
	CefRefPtr<CefV8Value> &dest) {
	CefRefPtr<CefV8Value> newValue;
	CefValueType type = src->GetType(index);
	switch (type) {
		case VTYPE_LIST: {
				CefRefPtr<CefListValue> list = src->GetList(index);
				newValue = CefV8Value::CreateArray(list->GetSize());
				ListToV8Array(list, newValue);
			}
			break;
    	case VTYPE_BOOL:
			newValue = CefV8Value::CreateBool(src->GetBool(index));
			break;
    	case VTYPE_DOUBLE:
			newValue = CefV8Value::CreateDouble(src->GetDouble(index));
			break;
    	case VTYPE_INT:
			newValue = CefV8Value::CreateInt(src->GetInt(index));
			break;
    	case VTYPE_STRING:
			newValue = CefV8Value::CreateString(src->GetString(index));
			break;
		case VTYPE_NULL:
        	newValue = CefV8Value::CreateNull();
        	break;
		case VTYPE_DICTIONARY: {
            CefRefPtr<CefDictionaryValue> dictionary = src->GetDictionary(index);
            newValue = CefV8Value::CreateObject(NULL);
            DictionaryToV8Value(dictionary, newValue);
            break;
         }
    	default:
			newValue = CefV8Value::CreateNull();
			break;
	}

	dest = newValue;
}

bool CefJSConversion::V8ValueToDictionary(CefRefPtr<CefV8Value> src,
	CefRefPtr<CefDictionaryValue> dest) {
	if (!src->IsObject()) {
		return false;
	}

   std::vector<CefString> keys;
   src->GetKeys(keys);
   std::vector<CefString>::const_iterator begin = keys.begin();
   std::vector<CefString>::const_iterator end = keys.end();
   for (std::vector<CefString>::const_iterator iter = begin; iter != end; ++iter) {
		const CefString key = *iter;
		CefRefPtr<CefV8Value> value = src->GetValue(key);
		if (!CheckV8Value(value)) {
			UC_LOG(ERROR) << "key: " << key.ToWString() << " invalid!";
			return false;
		}

		if (value->IsBool()) {
			dest->SetBool(key,value->GetBoolValue());
		}
		else if (value->IsDouble()) {
        	dest->SetDouble(key,value->GetDoubleValue());
		}
		else if (value->IsInt()||value->IsUInt()) {
			dest->SetInt(key,value->GetIntValue());
		}
		else if (value->IsNull()) {
			dest->SetNull(key);
		}
		else if (value->IsString()||value->IsDate()) {
			dest->SetString(key, value->GetStringValue());
		}
		else if (value->IsArray()) {
			CefRefPtr<CefListValue> listValue = CefListValue::Create();
			if (V8ArrayToList(value, listValue)) {
				dest->SetList(key, listValue);
			}
			else {
				return false;
			}
		}
		else if (value->IsObject()) {
			CefRefPtr<CefDictionaryValue> dictionaryValue = CefDictionaryValue::Create();
			if (V8ValueToDictionary(value, dictionaryValue)) {
				dest->SetDictionary(key, dictionaryValue);
			}
			else {
				return false;
			}
      }
   }

   return true;
}

void CefJSConversion::DictionaryToV8Value(CefRefPtr<CefDictionaryValue> src, 
	CefRefPtr<CefV8Value> dest) {
	if (!dest->IsObject()) {
		return;
	}
   
	CefDictionaryValue::KeyList keys;
	src->GetKeys(keys);
	CefDictionaryValue::KeyList::const_iterator begin = keys.begin();
	CefDictionaryValue::KeyList::const_iterator end = keys.end();
   
   for (CefDictionaryValue::KeyList::const_iterator iter = begin; iter != end; ++iter)
   {
      CefRefPtr<CefV8Value> newValue;
      CefString key = *iter;
      CefValueType type = src->GetType(key);

      switch (type)
      {
      case VTYPE_BOOL:
         newValue = CefV8Value::CreateBool(src->GetBool(key));
         break;
      case VTYPE_DOUBLE:
         newValue = CefV8Value::CreateDouble(src->GetDouble(key));
         break;
      case VTYPE_INT:
         newValue = CefV8Value::CreateInt(src->GetInt(key));
         break;
      case VTYPE_STRING:
         newValue = CefV8Value::CreateString(src->GetString(key));
         break;
      case VTYPE_NULL:
         newValue = CefV8Value::CreateNull();
         break;
      case VTYPE_LIST:
         {
            CefRefPtr<CefListValue> list = src->GetList(key);
            newValue = CefV8Value::CreateArray(static_cast<int>(list->GetSize()));
            ListToV8Array(list, newValue);
         }
         break;
      case VTYPE_DICTIONARY:
         {
            CefRefPtr<CefDictionaryValue> dictionary = src->GetDictionary(key);
            newValue = CefV8Value::CreateObject(NULL);
            DictionaryToV8Value(dictionary, newValue);
         }
         break;
      default:
         break;
      }
      if(newValue.get())
      {
         dest->SetValue(key, newValue, V8_PROPERTY_ATTRIBUTE_NONE);
      }
      else
      {
         dest->SetValue(key, CefV8Value::CreateNull(), V8_PROPERTY_ATTRIBUTE_NONE);
      }
   }
}

double CefJSConversion::JSNumberToDouble(CefRefPtr<CefListValue> src, int index) {
	CefValueType type = src->GetType(index);
	if (type == VTYPE_INT) {
		return src->GetInt(index);
	}
	else if (type == VTYPE_DOUBLE) {
		return src->GetDouble(index);
	}

	return 0;
}

bool CefJSConversion::IsJsNumber(CefRefPtr<CefListValue> src, int index) {
	CefValueType type = src->GetType(index);
	return (type == VTYPE_INT || type == VTYPE_DOUBLE) ? true : false;
}

void CefJSConversion::ListToString(CefRefPtr<CefListValue> src,  std::wstring &json) {
	int len = src->GetSize();
	std::wstringstream ss;
	ss << L"[";

	for (int i = 0; i < len; ++i) {
		std::wstring elem;
		ListToString(src, i, elem);
		ss << elem << L",";
	}
	ss << L"]";

	json = ss.str();
}

void CefJSConversion::ListToString(CefRefPtr<CefListValue> src, int index,  std::wstring &json) {
	CefValueType type = src->GetType(index);
	std::wstringstream ss;

	switch (type) {
		case VTYPE_LIST: {
			ListToString( src->GetList(index), json);
			return;
		}
    	case VTYPE_BOOL:
			ss << src->GetBool(index);
			break;
    	case VTYPE_DOUBLE:
			ss << src->GetDouble(index);
			break;
    	case VTYPE_INT:
			ss << src->GetInt(index);
			break;
    	case VTYPE_STRING:
			ss << L"\"" << UTF2WJSON(src->GetString(index).ToString()) << L"\"";
			break;
	case VTYPE_DICTIONARY: {
			DictionaryToString(src->GetDictionary(index), json);
            return;
         }
    	default:
			break;
	}

	json = ss.str();
}

void CefJSConversion::DictionaryToString(CefRefPtr<CefDictionaryValue> src, std::wstring& json)  {
	CefDictionaryValue::KeyList keys;
	src->GetKeys(keys);
	CefDictionaryValue::KeyList::const_iterator begin = keys.begin();
	CefDictionaryValue::KeyList::const_iterator end = keys.end();
   
	std::wstringstream ss;

	ss << L"{";
	for (CefDictionaryValue::KeyList::const_iterator iter = begin; iter != end; ++iter) {
		CefRefPtr<CefV8Value> newValue;
		CefString key = *iter;
		CefValueType type = src->GetType(key);

		switch (type) {
			case VTYPE_BOOL:
				ss << L"\"" << key.ToWString() << L"\": " << src->GetBool(key) << L",";
				break;
			case VTYPE_DOUBLE:
				ss << L"\"" << key.ToWString() << L"\": " << std::setprecision(13) << src->GetDouble(key) << L",";
				break;
			case VTYPE_INT:
				ss << L"\"" << key.ToWString() << L"\": " << src->GetInt(key) << L",";
				break;
			case VTYPE_STRING:
				ss << L"\"" << key.ToWString() << L"\": \"" << UTF2WJSON(src->GetString(key).ToString()) << L"\",";
				break;
			case VTYPE_NULL:
				break;
			case VTYPE_LIST: {
				std::wstring elem;
				ListToString(src->GetList(key), elem);
				ss << L"\"" << key.ToWString() << L"\": " << elem << L",";
				break;
			}	
			case VTYPE_DICTIONARY:{
				std::wstring elem;
				DictionaryToString(src->GetDictionary(key), elem);
				ss << L"\"" << key.ToWString() << L"\": " << elem << L",";
				 break;
			 }
			default:
				break;
		}
	}

	ss << L"}";
	json = ss.str();
}

}