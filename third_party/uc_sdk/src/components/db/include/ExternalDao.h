///////////////////////////////////////////////////////////
//  ExternalDao.h
//  Implementation of the Class ExternalDao
//  Created on:      Jan 28 2016 
//  Original author: yongshuai.cheng
///////////////////////////////////////////////////////////

#if !defined(EXTERNALDAO_H_)
#define EXTERNALDAO_H_

#include "AppDao.h"
#include "ExternalInfo.h"
#include <boost/thread/mutex.hpp>

#include <list>

namespace uc {

using namespace std;

class ExternalDao : public AppDao {
public:
	ExternalDao();
	virtual ~ExternalDao();

	int HaveData(string externalId);

	int GetMsgContentList(int32_t conversationType, string externalId, int64_t startTime, int64_t endTime, int count, list<ExternalMessage*>& message);
	
private:
	void SetMessage(CppSQLite3Query& query, ExternalMessage *&message);

private:
	CppSQLite3DB* _db;

};

}
#endif // !defined(EXTERNALDAO_H_)
