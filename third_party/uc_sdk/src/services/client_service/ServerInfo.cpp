/*
 * ServerInfo.cpp
 *
 *  Created on: 2015年5月7日
 *      Author: jianxue
 */

#include "ServerInfo.h"

namespace uc {

ServerInfo::ServerInfo(int16_t type, const string &domain, const string &cname, const string &token) {
	// TODO Auto-generated constructor stub
	_type = type;
	_domain = domain;
	_cname = cname;
	_token = token;
}

ServerInfo::~ServerInfo() {
	// TODO Auto-generated destructor stub
}

/**
 *
 */
int16_t ServerInfo::GetType() {
	return _type;
}

/**
 *
 */
string& ServerInfo::GetDomain() {
	return _domain;
}

/**
 *
 */
string& ServerInfo::GetCName() {
	return _cname;
}

/**
 *
 */
string& ServerInfo::GetToken() {
	return _token;
}

/**
 *
 */
void ServerInfo::Update(const string &domain, const string &cname, const string &token) {
	SetDomain(domain);
	SetCName(cname);
	SetToken(token);
}

/**
 *
 */
void ServerInfo::SetDomain(const string &domain) {
	_domain = domain;
}

/**
 *
 */
void ServerInfo::SetCName(const string &cname) {
	_cname = cname;
}

/**
 *
 */
void ServerInfo::SetToken(const string &token) {
	_token = token;
}

} /* namespace uc */
