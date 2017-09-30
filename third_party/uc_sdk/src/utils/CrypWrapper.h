#ifndef CRYPWRAPPER_H
#define CRYPWRAPPER_H

#include <string>

namespace uc {

class CrypWrapper {
public:
	CrypWrapper() {}
	~CrypWrapper() {}

	static std::string Encrypt(const std::string& src, const std::string& key = "");
	static std::string Decrypt(const std::string& src);

private:
	static std::string Sha256(const std::string& str);
};

}

#endif //CRYPWRAPPER_H
