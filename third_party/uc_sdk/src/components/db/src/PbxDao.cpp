#include "PbxDao.h"
#include "boost/regex.hpp"

namespace uc {
#ifdef PBX_SERVICE
	PbxDao::PbxDao() {
		_db = SQLiteManager::GetInstance().GetPbxDB();
	}

	PbxDao::~PbxDao() {
		SQLiteManager::GetInstance().ReleasePbxDB(_db);
	}

	int32_t PbxDao::GetCallRecordLst(int64_t timeStamp, int count, int selMask, PbxCallRecordList &lstData) {

		std::string strSelCols = "";
		if (!MakerSelCols(selMask, strSelCols)) {
			return 1020;
		}

		strSelCols += ", MAX(OPERATE_TIME) AS MAX_OPERATE_TIME";
		std::string strClause = " GROUP BY NUMBER, USERID ORDER BY MAX_OPERATE_TIME DESC";

		std::string strSql = "SELECT ";
		strSql += strSelCols;
		strSql += " FROM ";
		strSql += GetTableName();
		if (timeStamp > 0) {
			strSql += " WHERE OPERATE_TIME < ";
			std::stringstream ss;
			ss << timeStamp;
			strSql += ss.str();
		}
		strSql += strClause;
		if (count > 0) {
			strSql += " LIMIT 0, ";
			std::stringstream ss;
			ss << count;
			strSql += ss.str();
		}
		strSql += ";";

		CppSQLite3Query query;
		int ret = Query(strSql, _db, query);
		if (ret != 0) {
			return DBError::DB_QUERY_ERROR;
		}

		if (query.eof()) {
			return DBError::DB_QUERY_EMPTY;
		}

		AC_PbxCallRecord Temp;
		while (!query.eof()) {
			SetData(query, selMask, Temp);
			lstData.push_back(Temp);

			query.nextRow();
		}

		return DBError::DB_SUCCESS;
	}

	int32_t PbxDao::SelectCallDetails(const AC_PbxCallRecord &cond,
		int64_t timeStamp, int count, int selMask, PbxCallRecordList &lstData) {

		std::string strSelCols = "";
		if (!MakerSelCols(selMask, strSelCols)) {
			return 1020;
		}
		std::string strWhereCond = "";
		if (!MakerCondField(cond, strWhereCond, true, false)) {
			return 1021;
		}

		if (timeStamp > 0) {
			if (strWhereCond.length() > 0) {
				strWhereCond += " AND";
			}
			strWhereCond += " OPERATE_TIME < ?";
		}

		if (strWhereCond.length() > 0) {
			strWhereCond += " ORDER BY OPERATE_TIME DESC";
		}

		std::string strSql = "SELECT ";
		strSql += strSelCols;
		strSql += " FROM ";
		strSql += GetTableName();
		strSql += " WHERE ";
		strSql += strWhereCond;
		if (count > 0) {
			strSql += " LIMIT 0, ?";
		}
		strSql += ";";
		CppSQLite3Statement stmt = _db->compileStatement(strSql.c_str());
		int index = MakerValue(cond, stmt);
		if (timeStamp > 0) {
			stmt.bind(index++, timeStamp);
		}
		if (count > 0) {
			stmt.bind(index++, count);
		}

		CppSQLite3Query query;
		int ret = Query(stmt, query);
		if (ret != 0) {
			return DBError::DB_QUERY_ERROR;
		}

		if (query.eof()) {
			return DBError::DB_QUERY_EMPTY;
		}

		AC_PbxCallRecord Temp;
		while (!query.eof()) {
			SetData(query, selMask, Temp);
			lstData.push_back(Temp);

			query.nextRow();
		}

		return DBError::DB_SUCCESS;
	}

	int32_t PbxDao::SearchCallRecord(const string& keyWord, int64_t timeStamp,
		int32_t count, PbxCallRecordList &lstData) {
		bool isNumber = false;
		// 匹配任意个数字 
		std::string key = "^[0-9]*$";
		boost::regex reg(key);
		isNumber = boost::regex_match(keyWord, reg);
		int selMask = AC_PbxCallRecord::PM_NAME | AC_PbxCallRecord::PM_NUMBER
			| AC_PbxCallRecord::PM_STATE | AC_PbxCallRecord::PM_USERID | AC_PbxCallRecord::PM_TYPE;
		std::string strSelCols = "";
		if (!MakerSelCols(selMask, strSelCols)) {
			return 1020;
		}

		strSelCols += ", MAX(OPERATE_TIME) AS MAX_OPERATE_TIME";

		std::string strSql = "SELECT ";
		strSql += strSelCols;
		strSql += " FROM ";
		strSql += GetTableName();
		strSql += " WHERE";
		if (timeStamp > 0) {
			strSql += " OPERATE_TIME < ?";
			strSql += " AND";
		}
		strSql += " (NAME LIKE ?";
		if (isNumber) {
			strSql += " OR NUMBER LIKE ?";
		}
		strSql += ") GROUP BY NUMBER, USERID ORDER BY MAX_OPERATE_TIME DESC"; // ASC
		if (count > 0) {
			strSql += " LIMIT 0, ?";
		}
		strSql += ";";
		CppSQLite3Statement stmt = _db->compileStatement(strSql.c_str());

		string keyName = "%" + keyWord + "%";
		int i = 1;
		if (timeStamp > 0) {
			stmt.bind(i++, timeStamp);
		}
		stmt.bind(i++, keyName.c_str());
		if (isNumber) {
			stmt.bind(i++, keyName.c_str());
		}
		if (count > 0) {
			stmt.bind(i++, count);
		}

		CppSQLite3Query query;
		int ret = Query(stmt, query);
		if (ret != 0) {
			return DBError::DB_QUERY_ERROR;
		}

		if (query.eof()) {
			return DBError::DB_QUERY_EMPTY;
		}

		AC_PbxCallRecord Temp;
		while (!query.eof()) {
			SetData(query, selMask, Temp);
			lstData.push_back(Temp);

			query.nextRow();
		}

		return ret;
	}

	int32_t PbxDao::InsetCallDetails(const AC_PbxCallRecord &obj) {

		std::string strInsert = "";
		if (!MakerInsertField(obj, strInsert)) {
			return 1020;
		}

		std::string strSql = "INSERT OR REPLACE INTO ";
		strSql += GetTableName();
		strSql += " ";
		strSql += strInsert;
		strSql += ";";
		CppSQLite3Statement stmt = _db->compileStatement(strSql.c_str());
		MakerValue(obj, stmt);

		int ret = ExecDML(stmt);
		stmt.reset();
		if (ret != 0) {
			return DBError::DB_INSERT_ERROR;
		}

		return DBError::DB_SUCCESS;
	}

	int32_t PbxDao::UpdateCallDetails(const AC_PbxCallRecord &cond, const AC_PbxCallRecord &data) {

		std::string strUpdateValue = "";
		if (!MakerCondField(data, strUpdateValue, false, true)) {
			return 1020;
		}

		std::string strWhereCond = "";
		if (!MakerCondField(cond, strWhereCond, true, false)) {
			return 1021;
		}

		std::string strSql = "UPDATE ";
		strSql += GetTableName();
		strSql += " SET ";
		strSql += strUpdateValue;
		strSql += " WHERE ";
		strSql += strWhereCond;
		strSql += ";";
		CppSQLite3Statement stmt = _db->compileStatement(strSql.c_str());
		int index = MakerValue(data, stmt);
		MakerValue(cond, stmt, index);

		int ret = ExecDMLEx(stmt);
		stmt.reset();
		if (ret < 0) {
			return DBError::DB_DELETE_ERROR;
		}

		return DBError::DB_SUCCESS;
	}

	void PbxDao::SetData(CppSQLite3Query &query, int selMask, AC_PbxCallRecord &data) {

		if (AC_PbxCallRecord::PMNULL == selMask) {
			return;
		}

		int index = 0;
		unsigned int uiMarker = AC_PbxCallRecord::PMNULL;
		uiMarker = AC_PbxCallRecord::PM_OPERATE_TIME;
		if (0 != (selMask & uiMarker)) {
			data.SetOperatetime(StringConvert::Convert2Int64(query.fieldValue(index++)));
		}

		uiMarker = AC_PbxCallRecord::PM_CALLID;
		if (0 != (selMask & uiMarker)) {
			data.SetCallid(atoi(query.fieldValue(index++)));
		}

		uiMarker = AC_PbxCallRecord::PM_NAME;
		if (0 != (selMask & uiMarker)) {
			data.SetName(query.fieldValue(index++));
		}

		uiMarker = AC_PbxCallRecord::PM_NUMBER;
		if (0 != (selMask & uiMarker)) {
			data.SetNumber(query.fieldValue(index++));
		}

		uiMarker = AC_PbxCallRecord::PM_STATE;
		if (0 != (selMask & uiMarker)) {
			data.SetState(atoi(query.fieldValue(index++)), false);
		}

		uiMarker = AC_PbxCallRecord::PM_USERID;
		if (0 != (selMask & uiMarker)) {
			data.SetUserid(atoi(query.fieldValue(index++)));
		}

		uiMarker = AC_PbxCallRecord::PM_TYPE;
		if (0 != (selMask & uiMarker)) {
			data.SetType(atoi(query.fieldValue(index++)));
		}

		uiMarker = AC_PbxCallRecord::PM_DRUATION;
		if (0 != (selMask & uiMarker)) {
			data.SetDruation(atoi(query.fieldValue(index++)));
		}

		return;
	}

	int PbxDao::MakerValue(const AC_PbxCallRecord &data, CppSQLite3Statement &stmt, int index) {

		if (AC_PbxCallRecord::PMNULL == data.GetMarker()) {
			return index;
		}

		unsigned int uiMarker = AC_PbxCallRecord::PMNULL;
		uiMarker = AC_PbxCallRecord::PM_OPERATE_TIME;
		if (data.IsNotNull(uiMarker)) {
			stmt.bind(index++, data.GetOperatetime());
		}

		uiMarker = AC_PbxCallRecord::PM_CALLID;
		if (data.IsNotNull(uiMarker)) {
			stmt.bind(index++, data.GetCallid());
		}

		uiMarker = AC_PbxCallRecord::PM_NAME;
		if (data.IsNotNull(uiMarker)) {
			stmt.bind(index++, data.GetName().c_str());
		}

		uiMarker = AC_PbxCallRecord::PM_NUMBER;
		if (data.IsNotNull(uiMarker)) {
			stmt.bind(index++, data.GetNumber().c_str());
		}

		uiMarker = AC_PbxCallRecord::PM_STATE;
		if (data.IsNotNull(uiMarker)) {
			stmt.bind(index++, data.GetSTATE());
		}

		uiMarker = AC_PbxCallRecord::PM_USERID;
		if (data.IsNotNull(uiMarker)) {
			stmt.bind(index++, data.GetUserid());
		}

		uiMarker = AC_PbxCallRecord::PM_TYPE;
		if (data.IsNotNull(uiMarker)) {
			stmt.bind(index++, data.GetType());
		}

		uiMarker = AC_PbxCallRecord::PM_DRUATION;
		if (data.IsNotNull(uiMarker)) {
			stmt.bind(index++, data.GetDruation());
		}

		return index;
	}

	bool PbxDao::MakerInsertField(const AC_PbxCallRecord &insertData, std::string &strInsert) {

		if (AC_PbxCallRecord::PMNULL == insertData.GetMarker()) {
			return false;
		}

		std::string strValues = "";

		std::string strField = "";
		unsigned int uiMarker = AC_PbxCallRecord::PMNULL;
		for (int index = 0; index < AC_PbxCallRecord::PMCOUNT; ++index) {
			uiMarker = 1 << index;
			GetFieldNameByMask(uiMarker, strField);
			if (insertData.IsNotNull(uiMarker)) {
				if (strInsert.length() > 0){
					strInsert += ", ";
					strValues += ", ";
				}
				strInsert += strField;
				strValues += "?";
			}
		}

		if (strInsert.length() <= 0) {
			return false;
		}

		strInsert = "(" + strInsert;
		strInsert += ")VALUES(";
		strInsert += strValues;
		strInsert += ")";

		return true;
	}

	bool PbxDao::MakerSelCols(unsigned int selectMask, std::string &strSelCols) {

		if (selectMask == AC_PbxCallRecord::PMNULL) {
			selectMask = AC_PbxCallRecord::PMALL;
		}

		if (AC_PbxCallRecord::PMALL == selectMask) {
			strSelCols += "*";
			return true;
		}

		std::string strField;
		unsigned int uiMarker = AC_PbxCallRecord::PMNULL;
		for (int index = 0; index < AC_PbxCallRecord::PMCOUNT; ++index) {
			uiMarker = 1 << index;
			if ((selectMask &uiMarker) != 0) {
				if (strSelCols.length() > 0) {
					strSelCols += ", ";
				}

				GetFieldNameByMask(uiMarker, strField);
				strSelCols += strField;
			}
		}

		return (strSelCols.length() > 0);
	}

	bool PbxDao::MakerCondField(const AC_PbxCallRecord &obj, std::string &strDataLst, bool isCond, bool isValue) {
		//isCond = " and " isValue = " , ";
		if (obj.GetMarker() == AC_PbxCallRecord::PMNULL) {
			return false;
		}
		if (isCond == isValue) {
			return false;
		}

		const char *szConnCode = "";
		if (isCond) {
			szConnCode = " AND ";
		}
		else if (isValue) {
			szConnCode = " , ";
		}

		std::string strField = "";
		unsigned int uiMarker = AC_PbxCallRecord::PMNULL;
		for (int index = 0; index < AC_PbxCallRecord::PMCOUNT; ++index) {
			uiMarker = 1 << index;
			GetFieldNameByMask(uiMarker, strField);
			if (obj.IsNotNull(uiMarker)) {
				if (strDataLst.length() > 0) {
					strDataLst += szConnCode;
				}

				strDataLst += strField.c_str();
				strDataLst += " = ?";
			}
		}

		return (strDataLst.length() > 0);
	}

	bool PbxDao::GetFieldNameByMask(int paramMask, std::string &fieldName) {

		switch (paramMask)
		{
		case AC_PbxCallRecord::PM_OPERATE_TIME:
			fieldName = "OPERATE_TIME";
			break;
		case AC_PbxCallRecord::PM_CALLID:
			fieldName = "CALLID";
			break;
		case AC_PbxCallRecord::PM_NAME:
			fieldName = "NAME";
			break;
		case AC_PbxCallRecord::PM_NUMBER:
			fieldName = "NUMBER";
			break;
		case AC_PbxCallRecord::PM_STATE:
			fieldName = "STATE";
			break;
		case AC_PbxCallRecord::PM_USERID:
			fieldName = "USERID";
			break;
		case AC_PbxCallRecord::PM_TYPE:
			fieldName = "TYPE";
			break;
		case AC_PbxCallRecord::PM_DRUATION:
			fieldName = "DRUATION";
			break;
		default:
			return false;
		}

		return true;
	}

#endif // PBX_SERVICE
}