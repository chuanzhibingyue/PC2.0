#pragma once

#include "AppDao.h"
#include <list>
#include "AC_PbxCallRecord.h"

namespace uc {
#ifdef PBX_SERVICE
	class PbxDao : public AppDao
	{
	public:
		PbxDao();
		virtual ~PbxDao();

	public:
		int32_t GetCallRecordLst(int64_t timeStamp, int count, int selMask, PbxCallRecordList &lstData);

		int32_t SelectCallDetails(const AC_PbxCallRecord &cond, int64_t timeStamp,
			int count, int selMask, PbxCallRecordList &lstData);

		int32_t SearchCallRecord(const string& keyWord, int64_t timeStamp,
			int32_t count, PbxCallRecordList &lstData);

		int32_t InsetCallDetails(const AC_PbxCallRecord &obj);

		int32_t UpdateCallDetails(const AC_PbxCallRecord &cond, const AC_PbxCallRecord &data);

	private:
		void SetData(CppSQLite3Query &query, int selMask, AC_PbxCallRecord &data);
		// return index
		int MakerValue(const AC_PbxCallRecord &obj, CppSQLite3Statement &stmt, int index = 1);

		bool MakerInsertField(const AC_PbxCallRecord &insertData, std::string &strInsertField);

		bool MakerSelCols(unsigned int selectMask, std::string &strSelCols);

		bool MakerCondField(const AC_PbxCallRecord &obj, std::string &strDataLst, bool isCond, bool isValue);

		bool GetFieldNameByMask(int paramMask, std::string &fieldName);

		const char* GetTableName() {
			return "TAB_PBXCALLRECORD";
		}
		//////////////////////////////////////////////////////////////////////////
	private:
		CppSQLite3DB* _db;
	};

#endif // PBX_SERVICE
}

