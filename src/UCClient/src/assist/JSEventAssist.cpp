#include "JSEventAssist.h"
#include "LogManager.h"
#include "common/StringConvert.h"

namespace ucclient {
using namespace uc;

JSEventAssist::JSEventAssist(cefIntegration::CefClient *cefClient)
:_cefClient(cefClient) {

}

JSEventAssist::~JSEventAssist() {

}

void JSEventAssist::MakeJSEventAndExecute(const std::string &jsEvent, const std::string &jsParameters) {
	std::string jsCode = _MakeJSEvent(jsEvent, jsParameters);
	_cefClient->ExecuteJSForAllBrowsers(StringConvert::FromUTF8Str(jsCode));
	UC_LOG(INFO) << "dispatch event to JS: " << jsCode;
}

std::string JSEventAssist::_MakeJSEvent(const std::string &eventName, const std::string &jsParameters) {
	std::stringstream jsScript;
	jsScript << "var e = new CustomEvent(" << _AddSingleQuoteSurroundString(eventName) << ",{detail:{";
	jsScript << jsParameters;
	jsScript << "},});document.dispatchEvent(e);";

	return jsScript.str();
}

std::string JSEventAssist::_RetrieveParameters(const KeyValue &parameters) {
	std::stringstream jsParameters;
	for (KeyValueIter iter = parameters.begin(); iter != parameters.end(); iter++)
	{
		if (iter != parameters.begin())
		{
			jsParameters << ",";
		}
		jsParameters << iter->first << ":" << iter->second;
	}
	return jsParameters.str();
}

std::string JSEventAssist::_RetrieveParameter(const std::string &name, int value) {
	KeyValue parameters;
	parameters.insert(make_pair(name, StringConvert::Convert2String(value)));
	return _RetrieveParameters(parameters);
}

std::string JSEventAssist::_RetrieveArrayParameter(const std::string &parameters) {
	std::stringstream jsParameters;
	jsParameters << "[";
	jsParameters << parameters;
	jsParameters << "]";
	return jsParameters.str();
}

std::string JSEventAssist::_ConcatenateTwoJSParameters(const std::string &parameter1, const std::string &parameter2) {
	std::stringstream jsParameters;
	jsParameters << parameter1;
	jsParameters << ",";
	jsParameters << parameter2;
	return jsParameters.str();
}

std::string JSEventAssist::_AddCharSurroundString(const std::string &destValue, const std::string &surroundChar) {
	std::string retValue(destValue);
	retValue.insert(0, surroundChar);
	retValue.append(surroundChar);
	return retValue;
}

std::string JSEventAssist::_AddSingleQuoteSurroundString(const std::string &destValue) {
	return _AddCharSurroundString(destValue, "\"");
}

}