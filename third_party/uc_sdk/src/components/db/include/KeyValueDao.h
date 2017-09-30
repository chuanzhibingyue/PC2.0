

#if !defined(KEYVALUEDAO_H_)
#define KEYVALUEDAO_H_

#include "AppDao.h"

namespace uc {

	class KeyValueDao : public AppDao {
public:
	KeyValueDao();
	virtual ~KeyValueDao();

	int SaveValueByKey(const string& application, const string& key, const string& value);
	int GetValueByKey(const string& application, const string& key, string& value);
private:
	CppSQLite3DB* _db;

};

}
#endif // !defined(KEYVALUEDAO_H_)
