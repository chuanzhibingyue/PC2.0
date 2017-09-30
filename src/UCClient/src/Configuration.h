#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>

#include "pugixml.hpp"


namespace ucclient {

class Configuration {
public:
	Configuration(const std::wstring& filePath);

	std::string QueryStringValue(const std::string& xpath, const std::string& defaultValue = "");

	int QueryIntValue(const std::string& xpath, int defaultValue = 0);
private:
	pugi::xml_document _doc;
	pugi::xml_parse_result _result;
};

}

#endif