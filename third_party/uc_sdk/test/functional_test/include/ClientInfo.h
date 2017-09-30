#ifndef CLIENT_INFO_H_
#define CLIENT_INFO_H_
#include <string>
using namespace std;

const struct ClientInfo {	

	string account;
	string password;
	uint16_t clientType;
	string mac;
	string clientId;
	string pinNumber;
	string version;
	string storeType;
	string phoneNumber;
	string newPass;
	string confirmPass;
};

#endif