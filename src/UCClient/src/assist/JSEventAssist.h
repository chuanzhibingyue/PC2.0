/*
 * CalendarServiceStrategy.h
 *
 *  Created on: Aug 19, 2015
 *      Author: hualong zhang
 */

#ifndef JS_EVENT_ASSIST_H_
#define JS_EVENT_ASSIST_H_

#include <string>
#include <map>
#include "CefClient.h"

namespace ucclient {

class JSEventAssist {
public:
	JSEventAssist(cefIntegration::CefClient *cefClient);
	virtual ~JSEventAssist();
	void MakeJSEventAndExecute(const std::string &jsEvent, const std::string &jsParameters);
protected:
	typedef std::map<std::string, std::string> KeyValue;
	typedef std::map<std::string, std::string>::const_iterator KeyValueIter;
	std::string _AddSingleQuoteSurroundString(const std::string &destValue);
	std::string _RetrieveParameters(const KeyValue &parameters);
	std::string _RetrieveParameter(const std::string &name, int value);
	std::string _RetrieveArrayParameter(const std::string &parameters);
	std::string _ConcatenateTwoJSParameters(const std::string &parameter1, const std::string &parameter2);
	std::string _MakeJSEvent(const std::string &eventName, const std::string &parameters);
private:
	std::string _AddCharSurroundString(const std::string &destValue, const std::string &surroundChar);
	cefIntegration::CefClient *_cefClient;
};

}

#endif //JS_EVENT_ASSIST_H_