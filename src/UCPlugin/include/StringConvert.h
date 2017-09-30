#ifndef STRING_CONVERT_H
#define STRING_CONVERT_H

#include "string"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppb_var_dictionary.h"

using namespace std;

typedef long long int64_t;

class StringConvert{

public:

	static wstring ToUTF8Str(const string& data);
	static string FromUTF8Str(const wstring& data);

	static int64_t Convert2Int64(const string& value);
	static string Convert2String(int64_t value);
	static int32_t Convert2Int32(const string& value);
	static string Convert2String(int32_t value);
	static struct PP_Var String2Var(PPB_Var* ppb_var_interface, const string& value);
	static string VarToString(PPB_Var* ppb_var_interface, struct PP_Var var);
	static struct PP_Var GetDictVar(PPB_VarDictionary* ppb_Dictionary_interface, struct PP_Var var, struct PP_Var key);
};


#endif //STRING_CONVERT_H