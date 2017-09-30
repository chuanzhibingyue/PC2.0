#pragma once

namespace uc {

	class AC_PbxCallRecord
	{
	public:
		enum PARAM_MASK {
			PMNULL = 0,
			PMALL = 0xffffffff,

			PM_OPERATE_TIME = 1 << 0,
			PM_CALLID = 1 << 1,
			PM_NAME = 1 << 2,
			PM_NUMBER = 1 << 3,
			PM_STATE = 1 << 4,
			PM_USERID = 1 << 5,
			PM_TYPE = 1 << 6,
			PM_DRUATION = 1 << 7,
		};

		enum { PMCOUNT = 8 };

	public:
		AC_PbxCallRecord() {

			m_uiMarker = PMNULL;

			m_iOPERATETIME = 0;
			m_iCALLID = 0;
			m_iSTATE = 0;
			m_iUSERID = 0;
			m_iTYPE = 0;
			m_iDRUATION = 0;
		}

		AC_PbxCallRecord(const AC_PbxCallRecord &obj) {

			m_uiMarker = obj.m_uiMarker;

			m_iOPERATETIME = obj.m_iOPERATETIME;
			m_iCALLID = obj.m_iCALLID;
			m_strNAME.assign(obj.m_strNAME.c_str(), obj.m_strNAME.length());
			m_strNUMBER.assign(obj.m_strNUMBER.c_str(), obj.m_strNUMBER.length());
			m_iSTATE = obj.m_iSTATE;
			m_iUSERID = obj.m_iUSERID;
			m_iTYPE = obj.m_iTYPE;
			m_iDRUATION = obj.m_iDRUATION;
		}

		AC_PbxCallRecord & operator = (const AC_PbxCallRecord &obj) {

			m_uiMarker = obj.m_uiMarker;

			m_iOPERATETIME = obj.m_iOPERATETIME;
			m_iCALLID = obj.m_iCALLID;
			m_strNAME.assign(obj.m_strNAME.c_str(), obj.m_strNAME.length());
			m_strNUMBER.assign(obj.m_strNUMBER.c_str(), obj.m_strNUMBER.length());
			m_iSTATE = obj.m_iSTATE;
			m_iUSERID = obj.m_iUSERID;
			m_iTYPE = obj.m_iTYPE;
			m_iDRUATION = obj.m_iDRUATION;

			return *this;
		}

	public:
		inline bool SetOperatetime(const int64_t value) {
			m_iOPERATETIME = value;
			m_uiMarker |= PM_OPERATE_TIME;
			return true;
		}
		inline bool SetCallid(const int value) {
			m_iCALLID = value;
			m_uiMarker |= PM_CALLID;
			return true;
		}
		inline bool SetName(const std::string &value) {
			if (value.length() > 255) return false;
			m_strNAME = value;
			m_uiMarker |= PM_NAME;
			return true;
		}
		inline bool SetNumber(const std::string &value) {
			if (value.length() > 20) return false;
			m_strNUMBER = value;
			m_uiMarker |= PM_NUMBER;
			return true;
		}
		inline bool SetSTATE(const int value) {
			m_iSTATE = value;
			m_uiMarker |= PM_STATE;
			return true;
		}
		inline bool SetState(const int value, bool isIncoming) {
			m_iSTATE = value;
			if (isIncoming) {
				m_iSTATE |= 0x1000;
			}
			m_uiMarker |= PM_STATE;
			return true;
		}
		inline bool SetUserid(const int value) {
			m_iUSERID = value;
			m_uiMarker |= PM_USERID;
			return true;
		}
		inline bool SetType(const int value) {
			m_iTYPE = value;
			m_uiMarker |= PM_TYPE;
			return true;
		}
		inline bool SetDruation(const int value) {
			m_iDRUATION = value;
			m_uiMarker |= PM_DRUATION;
			return true;
		}

		inline int64_t GetOperatetime() const { return m_iOPERATETIME; }
		inline int GetCallid() const { return m_iCALLID; }
		inline const std::string &GetName() const { return m_strNAME; }
		inline const std::string &GetNumber() const { return m_strNUMBER; }
		inline int GetSTATE() const { return m_iSTATE; }
		inline int GetStateEx() const { return (m_iSTATE & 0x0FFF); }
		inline int IsIncoming() const {
			return (0 != (m_iSTATE & 0x1000)) ? 1 : 0;
		}

		inline int GetUserid() const { return m_iUSERID; }
		inline int GetType() const { return m_iTYPE; }
		inline int GetDruation() const { return m_iDRUATION; }

		void Update(AC_PbxCallRecord &data) {
			if (data.IsNotNull(AC_PbxCallRecord::PM_OPERATE_TIME))
				this->SetOperatetime(data.GetOperatetime());
			if (data.IsNotNull(AC_PbxCallRecord::PM_CALLID))
				this->SetCallid(data.GetCallid());
			if (data.IsNotNull(AC_PbxCallRecord::PM_NAME))
				this->SetName(data.GetName());
			if (data.IsNotNull(AC_PbxCallRecord::PM_NUMBER))
				this->SetNumber(data.GetNumber());
			if (data.IsNotNull(AC_PbxCallRecord::PM_STATE))
				this->SetState(data.GetSTATE(), false);
			if (data.IsNotNull(AC_PbxCallRecord::PM_USERID))
				this->SetUserid(data.GetUserid());
			if (data.IsNotNull(AC_PbxCallRecord::PM_TYPE))
				this->SetType(data.GetType());
			if (data.IsNotNull(AC_PbxCallRecord::PM_DRUATION))
				this->SetDruation(data.GetDruation());
		}

	private:
        int64_t m_iOPERATETIME;
		int m_iCALLID;
		std::string m_strNAME;
		std::string m_strNUMBER;
		int m_iSTATE;
		int m_iUSERID;
		int m_iTYPE;
		int m_iDRUATION;

	public:
		virtual ~AC_PbxCallRecord() {}
		// marker
		inline void SetMarker(unsigned int obj) { m_uiMarker = obj; }
		inline unsigned int GetMarker() const { return m_uiMarker; }
		// null
		inline void SetNull(unsigned int nMask) { m_uiMarker &= ~nMask; }
		inline bool IsNull(unsigned int nMask) const { return !(m_uiMarker &nMask); }
		inline bool IsNotNull(unsigned int nMask) const { return ((m_uiMarker &nMask) != 0); }

	private:
		unsigned int m_uiMarker;
	};

	typedef std::list<AC_PbxCallRecord> PbxCallRecordList;

// 	class PbxCallInfo : public AC_PbxCallRecord
// 	{
// 	public:
// 		PbxCallInfo() {}
// 		virtual ~PbxCallInfo() {}
// 
// 		bool isAlive;
// 		bool isIncoming;
// 		bool isHold;
// 		bool isMute;
// 
// 		int switchRecord;
// 		int64_t startTime;
// 
// 		std::string postValue;
// 		std::string callUUID;
// 	};
}
