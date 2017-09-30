///////////////////////////////////////////////////////////
//  DBManagerDef.h
//  Implementation of the Class DBManagerDef
//  Created on:      22-五月-2015 10:34:22
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(DBMANAGERDEF_H_)
#define DBMANAGERDEF_H_

#include <string>

namespace uc {

using namespace std;

//客户端数据库版本,变更时依次递增(主线版本使用双数);
// 这个版本的调整会导致DB中若干表的重建，目前key-value和del_conversation这两张表不会重建，因此要修改这两张表结构，请更新sqlConfig.h中的drop_table中的内容
const int DB_NEW_VERSION = 34;

//帐号加密版本号
const int DB_ENCRYPTION_VERSION = 1;

//账号具有类型版本
const int DB_ACCOUNT_TYPE_VERSION = 2;

//DBManager指定错误码范围：1000~1500
class DBError
{
public:
	enum
	{
		DB_SUCCESS = 0,			//成功
		DB_INSERT_ERROR = 1000, //数据库插入失败
		DB_UPDATE_ERROR,		//数据库更新失败
		DB_DELETE_ERROR,		//数据库删除失败
		DB_QUERY_ERROR,			//数据库查询失败
		DB_QUERY_EMPTY,			//查询数据为空
		DB_INPUT_ERROR,			//输入DB的参数错误
		DB_UPGRADE_ERROR,       //数据库升级错误
		DB_OPEN_ERROR,			//数据库打开错误
		DB_CLOSE_ERROR,			//数据库关闭错误
		DB_CHECK_ERROR,			//数据库检测错误
		DB_CREATE_TABLE_ERROR,	//创建数据库表错误
	};
};

}
#endif // !defined(DBMANAGERDEF_H_)
