#include "StringConvert.h"
#include <sstream>
#include <codecvt>

wstring StringConvert::ToUTF8Str(const string& data){

	if (data.empty()){
		return L"";
	}

	wstring dest;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	try{
		dest = conv.from_bytes(data);
	}
	catch (std::range_error&){
		dest = conv.from_bytes(data.substr(0, conv.converted()));
	}
	return dest;
}

string StringConvert::FromUTF8Str(const wstring& data){
	
	if (data.empty()){
		return "";
	}

	string dest;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	try{
		dest = conv.to_bytes(data);
	}
	catch (std::range_error&){
		dest = conv.to_bytes(data.substr(0, conv.converted()));
	}
	return dest;
}

int64_t StringConvert::Convert2Int64(const string& value){

	stringstream str;
	str << value;
	
	int64_t result;
	str >> result;
	
	return result;
}
string StringConvert::Convert2String(int64_t value){
	
	stringstream str;
	str << value;
	
	return str.str();
}

int32_t StringConvert::Convert2Int32(const string& value){
	stringstream str;
	str << value;

	int32_t result;
	str >> result;

	return result;
}

string StringConvert::Convert2String(int32_t value){
	stringstream str;
	str << value;

	return str.str();
}

struct PP_Var StringConvert::String2Var(PPB_Var* ppb_var_interface, const std::string& value){
	if (ppb_var_interface){
		return ppb_var_interface->VarFromUtf8(value.c_str(), value.length());
	}

	return PP_MakeUndefined();
}

string StringConvert::VarToString(PPB_Var* ppb_var_interface, struct PP_Var var){
	if (ppb_var_interface){
		uint32_t len = 0;
		return ppb_var_interface->VarToUtf8(var, &len);
	}

	return "";
}

struct PP_Var StringConvert::GetDictVar(PPB_VarDictionary* ppb_Dictionary_interface, struct PP_Var dict, struct PP_Var key){
	if (ppb_Dictionary_interface){
		return ppb_Dictionary_interface->Get(dict, key);
	}

	return PP_MakeUndefined();
}

