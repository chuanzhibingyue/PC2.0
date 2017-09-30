#if !defined(DBUPGRADE_H_)
#define DBUPGRADE_H_

#include "CppSQLite3.h"
#include <map>

/////////////////升级说明/////////////////
// 1.将当前数据表名改为临时表
// 2.创建新表
// 3.导入数据，新增字段填空
// 4.删除临时表
//////////////////////////////////////////

namespace uc {

using namespace std;

enum {
	DBV1 = 1,
	DBV2 = 2,
	DBV3 = 3,
};

typedef int (*UpgradePtr)(CppSQLite3DB* commonDb, CppSQLite3DB* userDb);

class DBUpgrade {
public:
	static DBUpgrade& GetInstance() {
		static DBUpgrade instance;
		return instance;
	}

	int Upgrade(const string& devDBPath, const string& userDBPath);

private:
	static int UpgradeV1(CppSQLite3DB* commonDb, CppSQLite3DB* userDb);
	static int UpgradeV2(CppSQLite3DB* commonDb, CppSQLite3DB* userDb);
	static int UpgradeV3(CppSQLite3DB* commonDb, CppSQLite3DB* userDb);

private:
	DBUpgrade();
	int GetDBVersion(CppSQLite3DB* userDb);
	int SetDBVersion(CppSQLite3DB* userDb, int version);
	UpgradePtr GetUpgradeFunc(int version);

private:
	map<int, UpgradePtr> _upgradeMap;
};

}

#endif //DBUPGRADE_H_