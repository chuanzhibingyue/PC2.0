#define _CRT_SECURE_NO_DEPRECATE
#define CRYPTOPP_DEFAULT_NO_DLL
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "dll.h"
#include "ripemd.h"
#include "rng.h"
#include "default.h"
#include "randpool.h"
#include "ida.h"
#include "base64.h"

#include "validate.h"
#include "bench.h"

#include <iostream>
#include <time.h>

#ifdef CRYPTOPP_WIN32_AVAILABLE
#include <windows.h>
#endif

#if defined(USE_BERKELEY_STYLE_SOCKETS) && !defined(macintosh)
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

#if (_MSC_VER >= 1000)
#include <crtdbg.h>		// for the debug heap
#endif

#if defined(__MWERKS__) && defined(macintosh)
#include <console.h>
#endif

#ifdef __BORLANDC__
#pragma comment(lib, "cryptlib_bds.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

#include "CrypWrapper.h"
#include "LogManager.h"

namespace uc {

	USING_NAMESPACE(CryptoPP)
	USING_NAMESPACE(std)

	//default key
	const std::string DEFAULT_KEY = "qffgggr8vrbvyrvxererrvwwwertwert";

	std::string CrypWrapper::Encrypt(const std::string& src, const std::string& key) {
		std::string dst;
		std::string enkey = DEFAULT_KEY;
		enkey = key.empty() ? DEFAULT_KEY : Sha256(key);
		CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryption((byte*)enkey.c_str(), enkey.length());
		CryptoPP::StringSource encryptor(src, true,
			new CryptoPP::StreamTransformationFilter(encryption,
			new CryptoPP::Base64Encoder(
			new CryptoPP::StringSink(dst),
			false
			)
			)
			);

		// 由于有psw的加密，不能在这里输出日志
// 		UC_LOG(INFO) << "Encrypt, src:" << src << ", dst:" << dst 
// 			<< ", key:" << key << ", enkey:" << enkey;
		return dst;
	}

	std::string CrypWrapper::Decrypt(const std::string& src) {
		std::string dst;
		CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decryption((byte*)DEFAULT_KEY.c_str(), DEFAULT_KEY.length());
		CryptoPP::StringSource decryptor(src, true,
			new CryptoPP::Base64Decoder(
			new CryptoPP::StreamTransformationFilter(decryption,
			new CryptoPP::StringSink(dst)
			)
			)
			);

		//UC_LOG(INFO) << "Decrypt, src:" << src << ", dst:" << dst;
		return dst;
 	}

	string CrypWrapper::Sha256(const string& str) {
		unsigned char hash[CryptoPP::SHA256::DIGESTSIZE];
		SHA256 sha;
		sha.Update((byte*)str.c_str(), str.size());
		sha.Final(hash);

		string result((char*)hash, CryptoPP::SHA256::DIGESTSIZE);
		return result;
	}

}
