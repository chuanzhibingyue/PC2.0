#include "ProxyInfoDao.h"
#include "LogManager.h"

namespace uc {

ProxyInfoDao::ProxyInfoDao()
{
	_db = SQLiteManager::GetInstance().GetCommonDB();
}

ProxyInfoDao::~ProxyInfoDao()
{
	SQLiteManager::GetInstance().ReleaseCommonDB(_db);
}

int ProxyInfoDao::SaveProxyAuthInfo(const ProxyInfo& proxyInfo)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_PROXY_AUTH_INFO"
		"(PROXY_URL, TYPE, USER_ACCOUNT, PASSWORD) "
		"VALUES(?, ?, ?, ?);");
	int i = 1;
	stmt.bind(i++, proxyInfo.GetProxyUrl().c_str());
	stmt.bind(i++, proxyInfo.GetAuthType());
	stmt.bind(i++, proxyInfo.GetProxyAccount().c_str());
	stmt.bind(i++, proxyInfo.GetProxyPassword().c_str());
	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) 
	{
		UC_LOG(ERROR) << "SaveProxyAuthInfo return error:" << ret
			<< "; user_account:" << proxyInfo.GetProxyAccount().c_str()
			<< ", password:" << proxyInfo.GetProxyPassword().c_str()
			<< ", proxy_url:" << proxyInfo.GetProxyUrl().c_str()
			<< ", type:" << proxyInfo.GetAuthType();
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ProxyInfoDao::GetAuthInfo(ProxyInfo& proxyInfo)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_PROXY_AUTH_INFO WHERE PROXY_URL = ?;");
	stmt.bind(1, proxyInfo.GetProxyUrl().c_str());
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0)
	{
		UC_LOG(ERROR) << "GetAuthInfo return error:" << ret << "; proxy_url:" << proxyInfo.GetProxyUrl().c_str();
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof())
	{
		int i = 1;
		proxyInfo.SetAuthType(atoi(query.fieldValue(i++)));
		proxyInfo.SetProxyAccount(query.fieldValue(i++));
		proxyInfo.SetProxyPassword(query.fieldValue(i++));
	}
	else {
		UC_LOG(ERROR) << "GetAuthInfo return db empty error!" << "proxy_url:" << proxyInfo.GetProxyUrl().c_str();
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int ProxyInfoDao::DeleteAuthInfo(const std::string& proxy_url)
{
	CppSQLite3Statement stmt = _db->compileStatement(
		"DELETE FROM TBL_PROXY_AUTH_INFO WHERE PROXY_URL = ?;");
	stmt.bind(1, proxy_url.c_str());
	int ret = ExecDML(stmt);
	if (ret != 0) 
	{
		UC_LOG(ERROR) << "DeleteAutoInfo return error:" << ret
			<< ", proxy_url:" << proxy_url.c_str();
		return DBError::DB_DELETE_ERROR;
	}

	return DBError::DB_SUCCESS;
}

}