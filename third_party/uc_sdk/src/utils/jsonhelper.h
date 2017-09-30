#if !defined(JSON_HELPER_H_)
#define JSON_HELPER_H_

#include <string>
#include "rapidjson/document.h"
#include "common/StringConvert.h"

namespace jsonhelper {

int64_t inline getIntValue(const rapidjson::Value &data, const std::string& name, int default_value) {
	if (!data.HasMember(name.c_str()))
		return default_value;
	
	const rapidjson::Value &value = data[name.c_str()];

	if (value.IsNull()) {
		return default_value;
	}
	else if (value.IsString()) {
		return value.GetStringLength() == 0 ? default_value : uc::StringConvert::Convert2Int64(value.GetString());
	}
	else if (value.IsInt() || value.IsInt64()) {
		return value.GetInt64();
	}
	else if (value.IsUint() || value.IsUint64()) {
		return (int64_t)value.GetUint64();
	}
	else if (value.IsDouble()) {
		return (int64_t)value.GetDouble();
	}
	else {
		return default_value;
	}
}

std::string inline getStrValue(const rapidjson::Value &value, const std::string& default_value) {
	if (value.IsNull()) {
		return default_value;
	}
	else if (value.IsString()) {
		return value.GetString();
	}
	else if (value.IsInt() || value.IsInt64()) {
		return uc::StringConvert::Convert2String(value.GetInt64());
	}
	else if ( value.IsUint() || value.IsUint64()) {
		return uc::StringConvert::Convert2String((int64_t)value.GetUint64());
	}
	else if (value.IsDouble()) {
		return uc::StringConvert::Convert2String((int64_t)value.GetDouble());
	}
	else if (value.IsArray()) {
		return value.Size() ? getStrValue(value[0], default_value) : default_value;
	}
	else {
		return default_value;
	}
}

std::string inline getStrValue(const rapidjson::Value &data, const std::string& name, const std::string& default_value) {
	if (!data.HasMember(name.c_str()))
		return default_value;
	
	const rapidjson::Value &value = data[name.c_str()];
	return getStrValue(value, default_value);
}

}
#endif//UC_TIME_H_
