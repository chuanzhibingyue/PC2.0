#include "Configuration.h"

namespace ucclient {

Configuration::Configuration(const std::wstring& filePath) {
	_result = _doc.load_file(filePath.c_str());
}	

std::string Configuration::QueryStringValue(const std::string& xpath, const std::string& defaultValue) {
	if (pugi::status_ok == _result.status) {
		pugi::xml_node node = _doc.first_element_by_path(xpath.c_str());
		if (node.empty()) {
			return defaultValue;
		}

		std::string value = node.child_value();
		return value;
	}

	return defaultValue;
}

int Configuration::QueryIntValue(const std::string& xpath, int defaultValue) {
	if (pugi::status_ok == _result.status) {
		pugi::xml_node node = _doc.first_element_by_path(xpath.c_str());
		if (node.empty()) {
			return defaultValue;
		}

		int value = atoi(node.child_value());
		return value;
	}

	return defaultValue;
}

}