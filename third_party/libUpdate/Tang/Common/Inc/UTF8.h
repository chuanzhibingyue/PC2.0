#pragma once

/*
 * CW2UTF8EX<> Behaves similar to the ATL character conversion classes.
 */
template<int t_nBufferLength = 128>
class CW2UTF8EX
{
public:
	CW2UTF8EX(LPCWSTR psz) throw(...) :	m_psz(m_szBuffer)
	{
		Init(psz);
	}
	~CW2UTF8EX() throw()
	{
		if(m_psz != m_szBuffer)
		{
			delete m_psz;
		}
	}

	operator LPSTR() const throw()
	{
		return(m_psz);
	}

private:
	void Init(LPCWSTR psz) throw(...)
	{
		if(psz == NULL)
		{
			m_psz = NULL;
			return;
		}

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, psz, lstrlenW(psz), 0, 0, NULL, NULL);

		if(nLen+1 > t_nBufferLength)
		{
			m_psz = new CHAR[nLen+1];
		}

		m_psz[::WideCharToMultiByte(CP_UTF8, 0, psz, lstrlenW(psz), m_psz, nLen, NULL, NULL)] = 0;
	}

public:
	LPSTR m_psz;
	char m_szBuffer[t_nBufferLength];

private:
	CW2UTF8EX(const CW2UTF8EX&) throw();
	CW2UTF8EX& operator=(const CW2UTF8EX&) throw();
};
typedef CW2UTF8EX<> CW2UTF8;

/*
 * CUTF82WEX<> Behaves similar to the ATL character conversion classes.
 */
template<int t_nBufferLength = 128>
class CUTF82WEX
{
public:
	CUTF82WEX(const unsigned char *psz) throw(...) : m_psz(m_szBuffer)
	{
		Init((const char *) psz);
	}
	CUTF82WEX(LPCSTR psz) throw(...) : m_psz(m_szBuffer)
	{
		Init(psz);
	}
	~CUTF82WEX() throw()
	{
		if(m_psz != m_szBuffer)
		{
			delete m_psz;
		}
	}

	operator LPWSTR() const throw()
	{
		return(m_psz);
	}

private:
	void Init(LPCSTR psz) throw(...)
	{
		if(psz == NULL)
		{
			m_psz = NULL;
			return;
		}
		
		int nLen = ::MultiByteToWideChar(CP_UTF8, 0, psz, lstrlenA(psz), 0, 0);
		
		if(nLen+1 > t_nBufferLength)
		{
			m_psz = new wchar_t[nLen+1];
		}

		m_psz[::MultiByteToWideChar(CP_UTF8, 0, psz, lstrlenA(psz), m_psz, nLen)] = 0;
	}

public:
	LPWSTR m_psz;
	wchar_t m_szBuffer[t_nBufferLength];

private:
	CUTF82WEX(const CUTF82WEX&) throw();
	CUTF82WEX& operator=(const CUTF82WEX&) throw();
};
typedef CUTF82WEX<> CUTF82W;

